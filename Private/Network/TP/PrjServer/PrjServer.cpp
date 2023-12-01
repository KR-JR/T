#define _CRT_SECURE_NO_WARNINGS
#include "Common.h"

#define SERVERPORT 9000
#define BUFSIZE    256
#define SIZE_TOT 256                    // 전송 패킷(헤더 + 데이터) 전체 크기
#define SIZE_DAT (SIZE_TOT-sizeof(int)) // 헤더를 제외한 데이터 부분만의 크기
#define TYPE_CHAT     1000              // 메시지 타입: 채팅
#define TYPE_DRAWLINE 1001              // 메시지 타입: 선 그리기
#define TYPE_ERASEPIC 1002              // 메시지 타입: 그림 지우기
#define TYPE_FILE	  1003				// 메시지 타입: 파일 보내기
#define TYPE_SHAPE    1004				// 메시지 타입: 도형 그리기

// 공통 메시지 형식
// sizeof(COMM_MSG) == 256
typedef struct _COMM_MSG
{
	int  type;
	char dummy[SIZE_DAT];
} COMM_MSG;

// 채팅 메시지 형식
// sizeof(CHAT_MSG) == 256
typedef struct _CHAT_MSG
{
	int  type;
	char clientID[20];
	char msg[SIZE_DAT - 20];
} CHAT_MSG;

// 파일 메시지 형식
typedef struct _FILEDATA_MSG
{
	int type;
	char filename[SIZE_DAT];
}FILEDATA_MSG;

// 선 그리기 메시지 형식
// sizeof(DRAWLINE_MSG) == 256
typedef struct _DRAWLINE_MSG
{
	int  type;
	int  color;
	int  x0, y0;
	int  x1, y1;
	char dummy[SIZE_TOT - 6 * sizeof(int)];
} DRAWLINE_MSG;

// 도형 그리기 메시지 형식
typedef struct _SHAPE_MSG
{
	int type;
	int shape;
	int color;
	int x0, y0;
	int x1, y1;
	char dummy[SIZE_TOT - 7 * sizeof(int)];
} SHAPE_MSG;

// 그림 지우기 메시지 형식
// sizeof(ERASEPIC_MSG) == 256
typedef struct _ERASEPIC_MSG
{
	int  type;
	char dummy[SIZE_DAT];
} ERASEPIC_MSG;

// UDP 메시지
typedef struct MessageNode {
	CHAT_MSG message;
	struct MessageNode* next;
} MessageNode;

MessageNode* head = NULL;
MessageNode* tail = NULL;

// 소켓 정보 저장을 위한 구조체와 변수
typedef struct _SOCKETINFO
{
	SOCKET sock;
	bool   isIPv6;
	bool   isUDP;
	char   buf[BUFSIZE + 1];
	int    recvbytes;
	COMM_MSG msg; // 추가: 공용 메시지 구조체
	DRAWLINE_MSG msg2;
	ERASEPIC_MSG msg3;
	FILEDATA_MSG msg4;
	SHAPE_MSG msg5;
} SOCKETINFO;
int nTotalSockets = 0;
SOCKETINFO* SocketInfoArray[FD_SETSIZE];

// 소켓 정보 관리 함수
bool AddSocketInfo(SOCKET sock, bool isIPv6, bool isUDP);
void RemoveSocketInfo(int nIndex);

void StoreMessage(const CHAT_MSG* msg);
void SendStoredMessages(SOCKET sock);

int main(int argc, char* argv[])
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

	// 넌블로킹 소켓 옵션 추가
	u_long on = 1;
	ioctlsocket(listen_sock4, FIONBIO, &on);

	// bind()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock4, (struct sockaddr*)&serveraddr,
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
	
	// 넌블로킹 소켓 옵션 추가
	u_long on = 1;
	ioctlsocket(listen_sock6, FIONBIO, &on);

	// bind()
	struct sockaddr_in6 serveraddr6;
	memset(&serveraddr6, 0, sizeof(serveraddr6));
	serveraddr6.sin6_family = AF_INET6;
	serveraddr6.sin6_addr = in6addr_any;
	serveraddr6.sin6_port = htons(SERVERPORT);
	retval = bind(listen_sock6, (struct sockaddr*)&serveraddr6,
		sizeof(serveraddr6));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock6, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");
	/*----- TCP/IPv6 소켓 초기화 종료 -----*/

	/*----- UDP/IPv4 소켓 초기화 시작 -----*/
	SOCKET udp_sock4 = socket(AF_INET, SOCK_DGRAM, 0);
	if (udp_sock4 == INVALID_SOCKET) err_quit("socket()");

	// bind()
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(udp_sock4, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// 비동기 소켓 이벤트 생성 및 연결
	WSAEVENT udpEvent4 = WSACreateEvent();
	if (udpEvent4 == WSA_INVALID_EVENT) err_quit("WSACreateEvent()");
	retval = WSAEventSelect(udp_sock4, udpEvent4, FD_READ | FD_CLOSE);
	if (retval == SOCKET_ERROR) err_quit("WSAEventSelect()");

	// 소켓 정보 추가
	if (!AddSocketInfo(udp_sock4, false, true)) {
		closesocket(udp_sock4);
	}
	/*----- UDP/IPv4 소켓 초기화 종료 -----*/

	/*----- UDP/IPv6 소켓 초기화 시작 -----*/
	SOCKET udp_sock6 = socket(AF_INET6, SOCK_DGRAM, 0);
	if (udp_sock6 == INVALID_SOCKET) err_quit("socket()");

	// bind()
	memset(&serveraddr6, 0, sizeof(serveraddr6));
	serveraddr6.sin6_family = AF_INET6;
	serveraddr6.sin6_addr = in6addr_any;
	serveraddr6.sin6_port = htons(SERVERPORT);
	retval = bind(udp_sock6, (struct sockaddr*)&serveraddr6, sizeof(serveraddr6));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// 비동기 소켓 이벤트 생성 및 연결
	WSAEVENT udpEvent6 = WSACreateEvent();
	if (udpEvent6 == WSA_INVALID_EVENT) err_quit("WSACreateEvent()");
	retval = WSAEventSelect(udp_sock6, udpEvent6, FD_READ | FD_CLOSE);
	if (retval == SOCKET_ERROR) err_quit("WSAEventSelect()");

	// 소켓 정보 추가
	if (!AddSocketInfo(udp_sock6, true, true)) {
		closesocket(udp_sock6);
	}
	/*----- UDP/IPv6 소켓 초기화 종료 -----*/

	// 데이터 통신에 사용할 변수(공통)
	fd_set rset;
	SOCKET client_sock;
	int addrlen;
	// 데이터 통신에 사용할 변수(IPv4)
	struct sockaddr_in clientaddr4;
	// 데이터 통신에 사용할 변수(IPv6)
	struct sockaddr_in6 clientaddr6;

	while (1) {
		// 소켓 셋 초기화
		FD_ZERO(&rset);
		FD_SET(listen_sock4, &rset);
		FD_SET(listen_sock6, &rset);
		for (int i = 0; i < nTotalSockets; i++) {
			FD_SET(SocketInfoArray[i]->sock, &rset);
		}

		// select()
		retval = select(0, &rset, NULL, NULL, NULL);
		if (retval == SOCKET_ERROR) err_quit("select()");

		// 소켓 셋 검사(1): 클라이언트 접속 수용 
		// UDP/IPV4
		if (FD_ISSET(udp_sock4, &rset)) {
			// IPv4 UDP 소켓에서 데이터 수신
			struct sockaddr_in src_addr;
			int src_addr_len = sizeof(src_addr);
			char udp_buf[BUFSIZE];
			int recv_len = recvfrom(udp_sock4, udp_buf, BUFSIZE, 0, 
									(struct sockaddr *)&src_addr, &src_addr_len);
			if (recv_len > 0) {
				// 수신된 데이터 처리
				// 예: printf("Received UDP data: %s\n", udp_buf);
			}
		}

		// UDP/IPV6
		if (FD_ISSET(udp_sock6, &rset)) {
			// IPv4 UDP 소켓에서 데이터 수신
			struct sockaddr_in src_addr;
			int src_addr_len = sizeof(src_addr);
			char udp_buf[BUFSIZE];
			int recv_len = recvfrom(udp_sock6, udp_buf, BUFSIZE, 0, 
									(struct sockaddr *)&src_addr, &src_addr_len);
			if (recv_len > 0) {
				// 수신된 데이터 처리
				// 예: printf("Received UDP data: %s\n", udp_buf);
			}
		}		
		
		// TCP
		if (FD_ISSET(listen_sock4, &rset)) { // TCP/IPv4
			addrlen = sizeof(clientaddr4);
			client_sock = accept(listen_sock4,
				(struct sockaddr*)&clientaddr4, &addrlen);
			if (client_sock == INVALID_SOCKET) {
				err_display("accept()");
				break;
			}
			else {
				// 클라이언트 정보 출력
				char addr[INET_ADDRSTRLEN];
				inet_ntop(AF_INET, &clientaddr4.sin_addr, addr, sizeof(addr));
				printf("\n[TCP/IPv4 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
					addr, ntohs(clientaddr4.sin_port));
				// 소켓 정보 추가: 실패 시 소켓 닫음		
				if (!AddSocketInfo(client_sock, false, false)) {
					closesocket(client_sock);
				}
				else {
					SendStoredMessages(client_sock);
				}				
			}
		}
		if (FD_ISSET(listen_sock6, &rset)) { // TCP/IPv6
			addrlen = sizeof(clientaddr6);
			client_sock = accept(listen_sock6,
				(struct sockaddr*)&clientaddr6, &addrlen);
			if (client_sock == INVALID_SOCKET) {
				err_display("accept()");
				break;
			}
			else {
				// 클라이언트 정보 출력
				char addr[INET6_ADDRSTRLEN];
				inet_ntop(AF_INET6, &clientaddr6.sin6_addr, addr, sizeof(addr));
				printf("\n[TCP/IPv6 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
					addr, ntohs(clientaddr6.sin6_port));
				// 소켓 정보 추가: 실패 시 소켓 닫음
				if (!AddSocketInfo(client_sock, true, false)) {
					closesocket(client_sock);
				}
				else {
					SendStoredMessages(client_sock);
				}				
			}
		}

		// 소켓 셋 검사(2): 데이터 통신
		// 데이터 수신 부분의 수정
		for (int i = 0; i < nTotalSockets; i++) {
			SOCKETINFO* ptr = SocketInfoArray[i];
			if (FD_ISSET(ptr->sock, &rset)) {


				// 먼저 COMM_MSG의 헤더만큼만 받아 메시지 타입을 확인
				retval = recv(ptr->sock, (char*)&(ptr->msg), sizeof(int), 0);
				if (retval <= 0) {
					RemoveSocketInfo(i);
					continue;
				}

				// 메시지 타입에 따라 나머지 부분을 받음
				switch (ptr->msg.type) {
				case TYPE_CHAT: {
					retval = recv(ptr->sock, (char*)&(ptr->msg) + sizeof(int), sizeof(CHAT_MSG) - sizeof(int), 0);
					if (retval <= 0) {
						RemoveSocketInfo(i);
						continue;
					}
					StoreMessage((CHAT_MSG*)&(ptr->msg));
					CHAT_MSG* chatMsg = (CHAT_MSG*)&(ptr->msg);
					printf("[%s]: %s\n", chatMsg->clientID,chatMsg->msg);
					// 다른 클라이언트에게 CHAT_MSG 전달
					for (int j = 0; j < nTotalSockets; j++) {
						if (i != j) { // 메시지를 보낸 클라이언트 제외
							SOCKETINFO* ptr2 = SocketInfoArray[j];
							send(ptr2->sock, (char*)chatMsg, sizeof(CHAT_MSG), 0);
						}
					}

					// 자신에게도 메시지 전달
					send(ptr->sock, (char*)chatMsg, SIZE_TOT, 0);
					break;
				}
				case TYPE_DRAWLINE: {
					retval = recv(ptr->sock, (char*)&(ptr->msg) + sizeof(int), sizeof(DRAWLINE_MSG) - sizeof(int), 0);
					if (retval <= 0) {
						RemoveSocketInfo(i);
						continue;
					}
					DRAWLINE_MSG* drawLineMsg = (DRAWLINE_MSG*)&(ptr->msg);
					// DRAWLINE_MSG 처리 로직 (예: 로깅, 브로드캐스트 등)
					printf("[DRAWLINE 메시지] 시작점: (%d, %d), 끝점: (%d, %d), 색상: %d\n",
						drawLineMsg->x0, drawLineMsg->y0, drawLineMsg->x1, drawLineMsg->y1, drawLineMsg->color);
					// 다른 클라이언트에게 DRAWLINE_MSG 전달
					for (int j = 0; j < nTotalSockets; j++) {
						if (i != j) { // 메시지를 보낸 클라이언트 제외
							SOCKETINFO* ptr2 = SocketInfoArray[j];
							send(ptr2->sock, (char*)drawLineMsg, sizeof(DRAWLINE_MSG), 0);
						}
					}

					// 자신에게도 메시지 전달
					send(ptr->sock, (char*)drawLineMsg, SIZE_TOT, 0);
					break;
				}
				case TYPE_ERASEPIC: {
					retval = recv(ptr->sock, (char*)&(ptr->msg) + sizeof(int), sizeof(ERASEPIC_MSG) - sizeof(int), 0);
					if (retval <= 0) {
						RemoveSocketInfo(i);
						continue;
					}
					ERASEPIC_MSG* erasePicMsg = (ERASEPIC_MSG*)&(ptr->msg);
					// ERASEPIC_MSG 처리 로직 (예: 로깅, 브로드캐스트 등)
					printf("[ERASEPIC 메시지] 그림 지우기 요청\n");
					// 다른 클라이언트에게 ERASEPIC_MSG 전달
					for (int j = 0; j < nTotalSockets; j++) {
						if (i != j) { // 메시지를 보낸 클라이언트 제외
							SOCKETINFO* ptr2 = SocketInfoArray[j];
							send(ptr2->sock, (char*)erasePicMsg, sizeof(ERASEPIC_MSG), 0);
						}
					}

					// 자신에게도 메시지 전달
					send(ptr->sock, (char*)erasePicMsg, SIZE_TOT, 0);
					break;
				}
				case TYPE_FILE: {
					retval = recv(ptr->sock, (char*)&(ptr->msg4) + sizeof(int), sizeof(FILEDATA_MSG) - sizeof(int), 0);
					if (retval <= 0) {
						RemoveSocketInfo(i);
						continue;
					}
					printf("[파일 전송 메시지] 파일 전송 요청\n");
					printf("[전송된 파일] %s\n", ptr->msg4.filename);

					char buf[BUFSIZE] = { 0, };
					size_t bytesRead;

					// 파일을 쓰기 모드로 열기 (쓰기 가능한 경로를 사용하세요)
					FILE* fd = fopen("RecievedFile.txt", "wb");
					if (fd == NULL) {
						perror("파일 열기 실패");
						exit(EXIT_FAILURE);
					}

					// 데이터를 수신하고 파일에 쓰기
					while ((bytesRead = recv(ptr->sock, buf, sizeof(buf), 0)) > 0) {
						size_t bytesWritten = fwrite(buf, sizeof(char), sizeof(buf), fd);
						if (bytesWritten != bytesRead)	break;
					}

					// 파일과 소켓을 닫기
					fclose(fd);

					printf("파일 저장 완료\n");

					break;
				}
				case TYPE_SHAPE:
					retval = recv(ptr->sock, (char*)&(ptr->msg) + sizeof(int), sizeof(SHAPE_MSG) - sizeof(int), 0);
					if (retval <= 0) {
						RemoveSocketInfo(i);
						continue;
					}
					SHAPE_MSG* drawShapeMsg = (SHAPE_MSG*)&(ptr->msg);
					// DRAWSHAPE_MSG 처리 로직 (예: 로깅, 브로드캐스트 등)
					printf("[DRAWSHAPE 메시지] 시작점: (%d, %d), 끝점: (%d, %d), 색상: %d\n",
						drawShapeMsg->x0, drawShapeMsg->y0, drawShapeMsg->x1, drawShapeMsg->y1, drawShapeMsg->color);
					// 다른 클라이언트에게 DRAWLINE_MSG 전달
					for (int j = 0; j < nTotalSockets; j++) {
						if (i != j) { // 메시지를 보낸 클라이언트 제외
							SOCKETINFO* ptr2 = SocketInfoArray[j];
							send(ptr2->sock, (char*)drawShapeMsg, sizeof(SHAPE_MSG), 0);
						}
					}

					// 자신에게도 메시지 전달
					send(ptr->sock, (char*)drawShapeMsg, SIZE_TOT, 0);
					break;
				}
			}
		} /* end of for */

	} /* end of while (1) */

	// 소켓 닫기
	closesocket(listen_sock4);

	// 윈속 종료
	WSACleanup();

	void FreeMessageList();

	return 0;
}


// 소켓 정보 추가
bool AddSocketInfo(SOCKET sock, bool isIPv6, bool isUDP)
{
	if (nTotalSockets >= FD_SETSIZE) {
		printf("[오류] 소켓 정보를 추가할 수 없습니다!\n");
		return false;
	}
	SOCKETINFO* ptr = new SOCKETINFO;
	if (ptr == NULL) {
		printf("[오류] 메모리가 부족합니다!\n");
		return false;
	}
	ptr->sock = sock;
	ptr->isIPv6 = isIPv6;
	ptr->isUDP = isUDP;
	ptr->recvbytes = 0;
	SocketInfoArray[nTotalSockets++] = ptr;

	return true;
}



// 소켓 정보 삭제
void RemoveSocketInfo(int nIndex)
{
	SOCKETINFO* ptr = SocketInfoArray[nIndex];

	if (ptr->isIPv6 == false) {
		// 클라이언트 정보 얻기
		struct sockaddr_in clientaddr;
		int addrlen = sizeof(clientaddr);
		getpeername(ptr->sock, (struct sockaddr*)&clientaddr, &addrlen);
		// 클라이언트 정보 출력
		char addr[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
		printf("[TCP/IPv4 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
			addr, ntohs(clientaddr.sin_port));
	}
	else {
		// 클라이언트 정보 얻기
		struct sockaddr_in6 clientaddr;
		int addrlen = sizeof(clientaddr);
		getpeername(ptr->sock, (struct sockaddr*)&clientaddr, &addrlen);
		// 클라이언트 정보 출력
		char addr[INET6_ADDRSTRLEN];
		inet_ntop(AF_INET6, &clientaddr.sin6_addr, addr, sizeof(addr));
		printf("[TCP/IPv6 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
			addr, ntohs(clientaddr.sin6_port));
	}
	// 소켓 닫기
	closesocket(ptr->sock);
	delete ptr;

	if (nIndex != (nTotalSockets - 1))
		SocketInfoArray[nIndex] = SocketInfoArray[nTotalSockets - 1];
	--nTotalSockets;
}


void StoreMessage(const CHAT_MSG* msg) {
	MessageNode* newNode = (MessageNode*)malloc(sizeof(MessageNode));
	newNode->message = *msg;
	newNode->next = NULL;

	if (tail == NULL) {
		head = newNode;
		tail = newNode;
	}
	else {
		tail->next = newNode;
		tail = newNode;
	}
}

void SendStoredMessages(SOCKET sock) {
	MessageNode* current = head;
	while (current != NULL) {
		send(sock, (char*)&(current->message), sizeof(CHAT_MSG), 0);
		current = current->next;
	}
}

void FreeMessageList() {
	MessageNode* current = head;
	while (current != NULL) {
		MessageNode* temp = current;
		current = current->next;
		free(temp);
	}
	head = NULL;
	tail = NULL;
}