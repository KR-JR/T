#include "..\..\Common.h"

#define SERVERPORT 9000
#define BUFSIZE    256

// 소켓 정보 저장을 위한 구조체와 변수
typedef struct _SOCKETINFO
{
	SOCKET sock;
	char buf[BUFSIZE + 1];
	int recvbytes;
	int sendbytes;
} SOCKETINFO;
int nTotalSockets = 0;
SOCKETINFO *SocketInfoArray[FD_SETSIZE];
WSAEVENT EventArray[WSA_MAXIMUM_WAIT_EVENTS];

// 소켓 정보 관리 함수
bool AddSocketInfo(SOCKET sock, bool isIPv6, bool isUDP);
void RemoveSocketInfo(int nIndex);

int main(int argc, char *argv[])
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	/*----- TCP/IPv4 소켓 초기화 시작 -----*/
	// 소켓 생성
	SOCKET listen_sock4 = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock4 == INVALID_SOCKET) err_quit("socket()");

	// bind()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock4, (struct sockaddr *)&serveraddr,
		sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock4, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");
	/*----- TCP/IPv4 소켓 초기화 종료 -----*/

	/*----- TCP/IPv6 소켓 초기화 시작 -----*/
	// 소켓 생성
	SOCKET listen_sock6 = socket(AF_INET6, SOCK_STREAM, 0);
	if (listen_sock6 == INVALID_SOCKET) err_quit("socket()");

	// bind()
	struct sockaddr_in6 serveraddr6;
	memset(&serveraddr6, 0, sizeof(serveraddr6));
	serveraddr6.sin6_family = AF_INET6;
	serveraddr6.sin6_addr = in6addr_any;
	serveraddr6.sin6_port = htons(SERVERPORT);
	retval = bind(listen_sock6, (struct sockaddr *)&serveraddr6,
		sizeof(serveraddr6));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock6, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");
	/*----- TCP/IPv6 소켓 초기화 종료 -----*/

	/*----- UDP/IPv4 소켓 초기화 시작 -----*/
	// TODO: 소켓을 생성하고 초기화한다.
	/*----- UDP/IPv4 소켓 초기화 종료 -----*/

	/*----- UDP/IPv6 소켓 초기화 시작 -----*/
	// TODO: 소켓을 생성하고 초기화한다.
	/*----- UDP/IPv6 소켓 초기화 종료 -----*/

	// 데이터 통신에 사용할 변수(공통)
	fd_set rset;
	SOCKADDR_IN clientaddr;
	int i, addrlen;
	WSANETWORKEVENTS NetworkEvents;

	// 데이터 통신에 사용할 변수(IPv4)
	struct sockaddr_in clientaddr4;
	// 데이터 통신에 사용할 변수(IPv6)
	struct sockaddr_in6 clientaddr6;

	while (1) {
		// 이벤트 객체 관찰하기
		i = WSAWaitForMultipleEvents(nTotalSockets, EventArray,
			FALSE, WSA_INFINITE, FALSE);
		if (i == WSA_WAIT_FAILED) continue;
		i -= WSA_WAIT_EVENT_0;

		// 구체적인 네트워크 이벤트 알아내기
		retval = WSAEnumNetworkEvents(SocketInfoArray[i]->sock,
			EventArray[i], &NetworkEvents);
		if (retval == SOCKET_ERROR) continue;

		// FD_ACCEPT 이벤트 처리
		if (NetworkEvents.lNetworkEvents & FD_ACCEPT) {
			if (NetworkEvents.iErrorCode[FD_ACCEPT_BIT] == 0) {
				SOCKET client_sock;
				struct sockaddr_storage clientaddr;
				int addrlen;

				if (client_sock == listen_sock4) { // IPv4 소켓
					addrlen = sizeof(struct sockaddr_in);
					client_sock = accept(client_sock, (struct sockaddr*)&clientaddr, &addrlen);
					char addr4[INET_ADDRSTRLEN];
					inet_ntop(AF_INET, &((struct sockaddr_in*)&clientaddr)->sin_addr, addr4, sizeof(addr4));
					printf("\n[TCP/IPv4 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n", addr4, ntohs(((struct sockaddr_in*)&clientaddr)->sin_port));

				}
				else if (client_sock == listen_sock6) { // IPv6 소켓
					addrlen = sizeof(struct sockaddr_in6);
					client_sock = accept(client_sock, (struct sockaddr*)&clientaddr, &addrlen);
					char addr6[INET6_ADDRSTRLEN];
					inet_ntop(AF_INET6, &((struct sockaddr_in6*)&clientaddr)->sin6_addr, addr6, sizeof(addr6));
					printf("\n[TCP/IPv6 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n", addr6, ntohs(((struct sockaddr_in6*)&clientaddr)->sin6_port));
				}

				if (client_sock == INVALID_SOCKET) {
					err_display("accept()");
					continue;
				}

				// 소켓 정보 추가 & WSAEventSelect()
				AddSocketInfo(client_sock);
				retval = WSAEventSelect(client_sock, EventArray[nTotalSockets - 1], FD_READ | FD_WRITE | FD_CLOSE);
				if (retval == SOCKET_ERROR) {
					err_display("WSAEventSelect()");
					RemoveSocketInfo(nTotalSockets - 1); // 실패 시 소켓 정보 삭제 및 소켓 닫기
					continue;
				}
			}
		}



		// FD_READ & FD_WRITE 이벤트 처리
		if (NetworkEvents.lNetworkEvents & FD_READ
			|| NetworkEvents.lNetworkEvents & FD_WRITE)
		{
			if (NetworkEvents.lNetworkEvents & FD_READ
				&& NetworkEvents.iErrorCode[FD_READ_BIT] != 0)
			{
				err_display(NetworkEvents.iErrorCode[FD_READ_BIT]);
				continue;
			}
			if (NetworkEvents.lNetworkEvents & FD_WRITE
				&& NetworkEvents.iErrorCode[FD_WRITE_BIT] != 0)
			{
				err_display(NetworkEvents.iErrorCode[FD_WRITE_BIT]);
				continue;
			}

			SOCKETINFO* ptr = SocketInfoArray[i];

			if (ptr->recvbytes == 0) {
				// 데이터 받기
				retval = recv(ptr->sock, ptr->buf, BUFSIZE, 0);
				if (retval == SOCKET_ERROR) {
					if (WSAGetLastError() != WSAEWOULDBLOCK) {
						err_display("recv()");
						RemoveSocketInfo(i);
					}
					continue;
				}
				ptr->recvbytes = retval;
				// 받은 데이터 출력
				ptr->buf[retval] = '\0';
				addrlen = sizeof(clientaddr);
				getpeername(ptr->sock, (SOCKADDR*)&clientaddr, &addrlen);
				printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
					ntohs(clientaddr.sin_port), ptr->buf);
			}

			if (ptr->recvbytes > ptr->sendbytes) {
				// 데이터 보내기
				retval = send(ptr->sock, ptr->buf + ptr->sendbytes,
					ptr->recvbytes - ptr->sendbytes, 0);
				if (retval == SOCKET_ERROR) {
					if (WSAGetLastError() != WSAEWOULDBLOCK) {
						err_display("send()");
						RemoveSocketInfo(i);
					}
					continue;
				}
				ptr->sendbytes += retval;
				// 받은 데이터를 모두 보냈는지 체크
				if (ptr->recvbytes == ptr->sendbytes)
					ptr->recvbytes = ptr->sendbytes = 0;
			}
		}

		// FD_CLOSE 이벤트 처리
		if (NetworkEvents.lNetworkEvents & FD_CLOSE) {
			if (NetworkEvents.iErrorCode[FD_CLOSE_BIT] != 0)
				err_display(NetworkEvents.iErrorCode[FD_CLOSE_BIT]);
			RemoveSocketInfo(i);
		}
	}   /* end of while (1) */

	// 소켓 닫기
	closesocket(listen_sock4);

	// 윈속 종료
	WSACleanup();
	return 0;
}

// 소켓 정보 추가
BOOL AddSocketInfo(SOCKET sock)
{
	SOCKETINFO* ptr = new SOCKETINFO;
	if (ptr == NULL) {
		printf("[오류] 메모리가 부족합니다!\n");
		return FALSE;
	}

	WSAEVENT hEvent = WSACreateEvent();
	if (hEvent == WSA_INVALID_EVENT) {
		err_display("WSACreateEvent()");
		return FALSE;
	}

	ptr->sock = sock;
	ptr->recvbytes = 0;
	ptr->sendbytes = 0;
	SocketInfoArray[nTotalSockets] = ptr;
	EventArray[nTotalSockets] = hEvent;
	++nTotalSockets;

	return TRUE;
}

// 소켓 정보 삭제
void RemoveSocketInfo(int nIndex)
{
	SOCKETINFO* ptr = SocketInfoArray[nIndex];

	SOCKADDR_IN clientaddr;
	int addrlen = sizeof(clientaddr);
	getpeername(ptr->sock, (SOCKADDR*)&clientaddr, &addrlen);
	printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	closesocket(ptr->sock);
	delete ptr;
	WSACloseEvent(EventArray[nIndex]);

	if (nIndex != (nTotalSockets - 1)) {
		SocketInfoArray[nIndex] = SocketInfoArray[nTotalSockets - 1];
		EventArray[nIndex] = EventArray[nTotalSockets - 1];
	}
	--nTotalSockets;
}
