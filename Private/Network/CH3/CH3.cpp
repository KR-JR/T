#pragma comment(lib, "ws2_32.dll")
#include <winsock2.h>
#include <stdio.h>


// 소켓 함수 오류 출력
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

// 도메인 이름 -> IPv4 주소 및 별명 출력
BOOL PrintIPAddrAndAliases(char *name)
{
	HOSTENT *ptr = gethostbyname(name);
	if (ptr == NULL) {
		err_display("gethostbyname()");
		return FALSE;
	}
	
	if (ptr->h_addrtype != AF_INET)
		return FALSE;

    // 모든 IP 주소 출력
	for (int i = 0; ptr->h_addr_list[i]; i++) {
		IN_ADDR addr;
		memcpy(&addr, ptr->h_addr_list[i], ptr->h_length);
		printf("IP 주소: %s\n", inet_ntoa(addr));
	}

    // 모든 별명 출력
    for (int i = 0; ptr->h_aliases[i]; i++) {
        printf("별명: %s\n", ptr->h_aliases[i]);
    }

	return TRUE;
}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("사용법: %s <도메인 이름>\n", argv[0]);
		return 1;
	}

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	printf("도메인 이름 = %s\n", argv[1]);

	// 도메인 이름에 대한 IP 주소 및 별명 출력
	PrintIPAddrAndAliases(argv[1]);

	WSACleanup();
	return 0;
}




