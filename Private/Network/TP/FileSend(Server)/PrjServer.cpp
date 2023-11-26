#include "..\..\Common.h"

#define SERVERPORT 9000
#define BUFSIZE    256
#define SIZE_TOT 256                    // ���� ��Ŷ(��� + ������) ��ü ũ��
#define SIZE_DAT (SIZE_TOT-sizeof(int)) // ����� ������ ������ �κи��� ũ��
#define TYPE_CHAT     1000              // �޽��� Ÿ��: ä��
#define TYPE_DRAWLINE 1001              // �޽��� Ÿ��: �� �׸���
#define TYPE_ERASEPIC 1002              // �޽��� Ÿ��: �׸� �����
#define TYPE_FILE	  1003				// �޽��� Ÿ��: ���� ������

// ���� �޽��� ����
// sizeof(COMM_MSG) == 256
typedef struct _COMM_MSG
{
	int  type;
	char dummy[SIZE_DAT];
} COMM_MSG;

// ä�� �޽��� ����
// sizeof(CHAT_MSG) == 256
typedef struct _CHAT_MSG
{
	int  type;
	char msg[SIZE_DAT];
} CHAT_MSG;

// ���� �޽��� ����
typedef struct _FILEDATA_MSG
{
	int type;
	char filename[SIZE_DAT];
}FILEDATA_MSG;

// �� �׸��� �޽��� ����
// sizeof(DRAWLINE_MSG) == 256
typedef struct _DRAWLINE_MSG
{
	int  type;
	int  color;
	int  x0, y0;
	int  x1, y1;
	char dummy[SIZE_TOT - 6 * sizeof(int)];
} DRAWLINE_MSG;

// �׸� ����� �޽��� ����
// sizeof(ERASEPIC_MSG) == 256
typedef struct _ERASEPIC_MSG
{
	int  type;
	char dummy[SIZE_DAT];
} ERASEPIC_MSG;


// ���� ���� ������ ���� ����ü�� ����
typedef struct _SOCKETINFO
{
	SOCKET sock;
	bool   isIPv6;
	bool   isUDP;
	char   buf[BUFSIZE + 1];
	int    recvbytes;
	COMM_MSG msg; // �߰�: ���� �޽��� ����ü
	DRAWLINE_MSG msg2;
	ERASEPIC_MSG msg3;
	FILEDATA_MSG msg4;
} SOCKETINFO;
int nTotalSockets = 0;
SOCKETINFO* SocketInfoArray[FD_SETSIZE];

// ���� ���� ���� �Լ�
bool AddSocketInfo(SOCKET sock, bool isIPv6, bool isUDP);
void RemoveSocketInfo(int nIndex);

int main(int argc, char* argv[])
{
	int retval;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	/*----- TCP/IPv4 ���� �ʱ�ȭ ���� -----*/
	// ���� ����
	SOCKET listen_sock4 = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock4 == INVALID_SOCKET) err_quit("socket()");

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
	/*----- TCP/IPv4 ���� �ʱ�ȭ ���� -----*/

	/*----- TCP/IPv6 ���� �ʱ�ȭ ���� -----*/
	// ���� ����
	SOCKET listen_sock6 = socket(AF_INET6, SOCK_STREAM, 0);
	if (listen_sock6 == INVALID_SOCKET) err_quit("socket()");

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
	/*----- TCP/IPv6 ���� �ʱ�ȭ ���� -----*/

	/*----- UDP/IPv4 ���� �ʱ�ȭ ���� -----*/
	// TODO: ������ �����ϰ� �ʱ�ȭ�Ѵ�.
	/*----- UDP/IPv4 ���� �ʱ�ȭ ���� -----*/

	/*----- UDP/IPv6 ���� �ʱ�ȭ ���� -----*/
	// TODO: ������ �����ϰ� �ʱ�ȭ�Ѵ�.
	/*----- UDP/IPv6 ���� �ʱ�ȭ ���� -----*/

	// ������ ��ſ� ����� ����(����)
	fd_set rset;
	SOCKET client_sock;
	int addrlen;
	// ������ ��ſ� ����� ����(IPv4)
	struct sockaddr_in clientaddr4;
	// ������ ��ſ� ����� ����(IPv6)
	struct sockaddr_in6 clientaddr6;

	while (1) {
		// ���� �� �ʱ�ȭ
		FD_ZERO(&rset);
		FD_SET(listen_sock4, &rset);
		FD_SET(listen_sock6, &rset);
		for (int i = 0; i < nTotalSockets; i++) {
			FD_SET(SocketInfoArray[i]->sock, &rset);
		}

		// select()
		retval = select(0, &rset, NULL, NULL, NULL);
		if (retval == SOCKET_ERROR) err_quit("select()");

		// ���� �� �˻�(1): Ŭ���̾�Ʈ ���� ����
		if (FD_ISSET(listen_sock4, &rset)) { // TCP/IPv4
			addrlen = sizeof(clientaddr4);
			client_sock = accept(listen_sock4,
				(struct sockaddr*)&clientaddr4, &addrlen);
			if (client_sock == INVALID_SOCKET) {
				err_display("accept()");
				break;
			}
			else {
				// Ŭ���̾�Ʈ ���� ���
				char addr[INET_ADDRSTRLEN];
				inet_ntop(AF_INET, &clientaddr4.sin_addr, addr, sizeof(addr));
				printf("\n[TCP/IPv4 ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
					addr, ntohs(clientaddr4.sin_port));
				// ���� ���� �߰�: ���� �� ���� ����		
				if (!AddSocketInfo(client_sock, false, false)) {
					closesocket(client_sock);
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
				// Ŭ���̾�Ʈ ���� ���
				char addr[INET6_ADDRSTRLEN];
				inet_ntop(AF_INET6, &clientaddr6.sin6_addr, addr, sizeof(addr));
				printf("\n[TCP/IPv6 ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
					addr, ntohs(clientaddr6.sin6_port));
				// ���� ���� �߰�: ���� �� ���� ����
				if (!AddSocketInfo(client_sock, true, false)) {
					closesocket(client_sock);
				}
			}
		}

		// ���� �� �˻�(2): ������ ���
		// ������ ���� �κ��� ����
		for (int i = 0; i < nTotalSockets; i++) {
			SOCKETINFO* ptr = SocketInfoArray[i];
			if (FD_ISSET(ptr->sock, &rset)) {
				// ���� COMM_MSG�� �����ŭ�� �޾� �޽��� Ÿ���� Ȯ��
				retval = recv(ptr->sock, (char*)&(ptr->msg), sizeof(int), 0);
				if (retval <= 0) {
					RemoveSocketInfo(i);
					continue;
				}

				// �޽��� Ÿ�Կ� ���� ������ �κ��� ����
				switch (ptr->msg.type) {
				case TYPE_CHAT: {
					retval = recv(ptr->sock, (char*)&(ptr->msg) + sizeof(int), sizeof(CHAT_MSG) - sizeof(int), 0);
					if (retval <= 0) {
						RemoveSocketInfo(i);
						continue;
					}
					CHAT_MSG* chatMsg = (CHAT_MSG*)&(ptr->msg);
					printf("[Ŭ���̾�Ʈ]: %s\n", chatMsg->msg);
					// �ٸ� Ŭ���̾�Ʈ���� CHAT_MSG ����
					for (int j = 0; j < nTotalSockets; j++) {
						if (i != j) { // �޽����� ���� Ŭ���̾�Ʈ ����
							SOCKETINFO* ptr2 = SocketInfoArray[j];
							send(ptr2->sock, (char*)chatMsg, sizeof(CHAT_MSG), 0);
						}
					}

					// �ڽſ��Ե� �޽��� ����
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
					// DRAWLINE_MSG ó�� ���� (��: �α�, ��ε�ĳ��Ʈ ��)
					printf("[DRAWLINE �޽���] ������: (%d, %d), ����: (%d, %d), ����: %d\n",
						drawLineMsg->x0, drawLineMsg->y0, drawLineMsg->x1, drawLineMsg->y1, drawLineMsg->color);
					// �ٸ� Ŭ���̾�Ʈ���� DRAWLINE_MSG ����
					for (int j = 0; j < nTotalSockets; j++) {
						if (i != j) { // �޽����� ���� Ŭ���̾�Ʈ ����
							SOCKETINFO* ptr2 = SocketInfoArray[j];
							send(ptr2->sock, (char*)drawLineMsg, sizeof(DRAWLINE_MSG), 0);
						}
					}

					// �ڽſ��Ե� �޽��� ����
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
					// ERASEPIC_MSG ó�� ���� (��: �α�, ��ε�ĳ��Ʈ ��)
					printf("[ERASEPIC �޽���] �׸� ����� ��û\n");
					// �ٸ� Ŭ���̾�Ʈ���� ERASEPIC_MSG ����
					for (int j = 0; j < nTotalSockets; j++) {
						if (i != j) { // �޽����� ���� Ŭ���̾�Ʈ ����
							SOCKETINFO* ptr2 = SocketInfoArray[j];
							send(ptr2->sock, (char*)erasePicMsg, sizeof(ERASEPIC_MSG), 0);
						}
					}

					// �ڽſ��Ե� �޽��� ����
					send(ptr->sock, (char*)erasePicMsg, SIZE_TOT, 0);
					break;
				}
				case TYPE_FILE: {
					retval = recv(ptr->sock, (char*)&(ptr->msg4) + sizeof(int), sizeof(FILEDATA_MSG) - sizeof(int), 0);
					if (retval <= 0) {
						RemoveSocketInfo(i);
						continue;
					}
					printf("[���� ���� �޽���] ���� ���� ��û\n");
					printf("[���۵� ����] %s\n", ptr->msg4.filename);

					char buf[BUFSIZE];
					size_t bytesRead;

					// ������ ���� ���� ���� (���� ������ ��θ� ����ϼ���)
					FILE* fd = _wfopen(L"\..\RecieveFile.txt", L"ab");
					if (fd == NULL) {
						perror("Error opening file for writing");
						exit(EXIT_FAILURE);
					}

					// �����͸� �����ϰ� ���Ͽ� ����
					while ((bytesRead = recv(ptr->sock, buf, sizeof(buf), 0)) > 0) {
						size_t bytesWritten = fwrite(buf, 1, bytesRead, fd);
						if (bytesWritten != bytesRead) {
							perror("Error writing to file");
							exit(EXIT_FAILURE);
						}
					}

					// ���ϰ� ������ �ݱ�
					fclose(fd);

				}

				}
			}
		} /* end of for */

	} /* end of while (1) */

	// ���� �ݱ�
	closesocket(listen_sock4);

	// ���� ����
	WSACleanup();
	return 0;
}


// ���� ���� �߰�
bool AddSocketInfo(SOCKET sock, bool isIPv6, bool isUDP)
{
	if (nTotalSockets >= FD_SETSIZE) {
		printf("[����] ���� ������ �߰��� �� �����ϴ�!\n");
		return false;
	}
	SOCKETINFO* ptr = new SOCKETINFO;
	if (ptr == NULL) {
		printf("[����] �޸𸮰� �����մϴ�!\n");
		return false;
	}
	ptr->sock = sock;
	ptr->isIPv6 = isIPv6;
	ptr->isUDP = isUDP;
	ptr->recvbytes = 0;
	SocketInfoArray[nTotalSockets++] = ptr;

	return true;
}



// ���� ���� ����
void RemoveSocketInfo(int nIndex)
{
	SOCKETINFO* ptr = SocketInfoArray[nIndex];

	if (ptr->isIPv6 == false) {
		// Ŭ���̾�Ʈ ���� ���
		struct sockaddr_in clientaddr;
		int addrlen = sizeof(clientaddr);
		getpeername(ptr->sock, (struct sockaddr*)&clientaddr, &addrlen);
		// Ŭ���̾�Ʈ ���� ���
		char addr[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
		printf("[TCP/IPv4 ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
			addr, ntohs(clientaddr.sin_port));
	}
	else {
		// Ŭ���̾�Ʈ ���� ���
		struct sockaddr_in6 clientaddr;
		int addrlen = sizeof(clientaddr);
		getpeername(ptr->sock, (struct sockaddr*)&clientaddr, &addrlen);
		// Ŭ���̾�Ʈ ���� ���
		char addr[INET6_ADDRSTRLEN];
		inet_ntop(AF_INET6, &clientaddr.sin6_addr, addr, sizeof(addr));
		printf("[TCP/IPv6 ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
			addr, ntohs(clientaddr.sin6_port));
	}
	// ���� �ݱ�
	closesocket(ptr->sock);
	delete ptr;

	if (nIndex != (nTotalSockets - 1))
		SocketInfoArray[nIndex] = SocketInfoArray[nTotalSockets - 1];
	--nTotalSockets;
}