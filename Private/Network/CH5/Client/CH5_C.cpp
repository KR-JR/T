#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE    50

// ���� �Լ� ���� ��� �� ����
void err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// ���� �Լ� ���� ���
void err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

int main(int argc, char *argv[])
{
	int retval;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if(retval == SOCKET_ERROR) err_quit("connect()");

	// ������ ��ſ� ����� ����
	char buf[BUFSIZE];
	char *testdata[] = {
		"�ȳ��ϼ���",
		"�ݰ�����",
		"���õ��� �� �̾߱Ⱑ ���� �� ���׿�",
		"���� �׷��׿�",
	};
	int len;

	// ������ ������ ���
    char *filename = "SAMPLE.exe"; // ���� �̸�(Ȯ�� ����)
    FILE *fp = fopen(filename, "rb");
    if(fp == NULL) {
        perror("File opening error");
        return 1;
    }

    // ������ ũ�⸦ ��´�.
    fseek(fp, 0, SEEK_END);
    int filesize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // ���� �̸��� ���̿� �̸��� ����
    int filename_len = strlen(filename);
    send(sock, (char *)&filename_len, sizeof(int), 0);
    send(sock, filename, filename_len, 0);

    // ���� ũ�� ����
    send(sock, (char *)&filesize, sizeof(int), 0);

    // ���� ���� ����
    char send_buffer[BUFSIZE];
    int bytes_read;
    while((bytes_read = fread(send_buffer, 1, BUFSIZE, fp)) > 0) {
        send(sock, send_buffer, bytes_read, 0);
    }
    fclose(fp);


	// closesocket()
	closesocket(sock);

	// ���� ����
	WSACleanup();
	return 0;
}
