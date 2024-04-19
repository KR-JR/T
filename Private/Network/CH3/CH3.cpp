#pragma comment(lib, "ws2_32.dll")
#include <winsock2.h>
#include <stdio.h>


// ���� �Լ� ���� ���
void err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

// ������ �̸� -> IPv4 �ּ� �� ���� ���
BOOL PrintIPAddrAndAliases(char *name)
{
	HOSTENT *ptr = gethostbyname(name);
	if (ptr == NULL) {
		err_display("gethostbyname()");
		return FALSE;
	}
	
	if (ptr->h_addrtype != AF_INET)
		return FALSE;

    // ��� IP �ּ� ���
	for (int i = 0; ptr->h_addr_list[i]; i++) {
		IN_ADDR addr;
		memcpy(&addr, ptr->h_addr_list[i], ptr->h_length);
		printf("IP �ּ�: %s\n", inet_ntoa(addr));
	}

    // ��� ���� ���
    for (int i = 0; ptr->h_aliases[i]; i++) {
        printf("����: %s\n", ptr->h_aliases[i]);
    }

	return TRUE;
}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("����: %s <������ �̸�>\n", argv[0]);
		return 1;
	}

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	printf("������ �̸� = %s\n", argv[1]);

	// ������ �̸��� ���� IP �ּ� �� ���� ���
	PrintIPAddrAndAliases(argv[1]);

	WSACleanup();
	return 0;
}




