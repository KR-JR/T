#pragma once
#include <ws2tcpip.h>

#define SERVERIP4  _T("127.0.0.1")
#define SERVERIP6  _T("::1")
#define SERVERPORT  9000

#define SIZE_TOT 256                    // 전송 패킷(헤더 + 데이터) 전체 크기
#define SIZE_DAT (SIZE_TOT-sizeof(int)) // 헤더를 제외한 데이터 부분만의 크기

#define TYPE_CHAT     1000              // 메시지 타입: 채팅
#define TYPE_DRAWLINE 1001              // 메시지 타입: 선 그리기
#define TYPE_ERASEPIC 1002              // 메시지 타입: 그림 지우기
#define TYPE_FILE	  1003				// 메시지 타입: 파일 전송
#define TYPE_SHAPE	  1004				// 메시지 타입: 도형 그리기

#define WM_DRAWLINE (WM_USER+1)         // 사용자 정의 윈도우 메시지(1)
#define WM_ERASEPIC (WM_USER+2)         // 사용자 정의 윈도우 메시지(2)
#define WM_FILETRANSFER (WM_USER+3)     // 사용자 정의 윈도우 메시지(3)
#define WM_DRAWSHAPE (WM_USER+4)		// 사용자 정의 윈도우 메시지(4)

#define SHAPE_CIRCLE	 201			// 도형 그리기 : 원
#define SHAPE_RECTANGLE	 202			// 도형 그리기 : 사각형
#define SHAPE_TRIANGLE	 203			// 도형 그리기 : 삼각형
#define SHAPE_LINE		 204			// 도형 그리기 : 선

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

typedef struct _FILEDATA_MSG
{
	int type;
	char filename[SIZE_DAT];
}FILEDATA_MSG;

typedef struct MessageNode {
	CHAT_MSG message;
	struct MessageNode* next;
} MessageNode;

MessageNode* head = NULL;
MessageNode* tail = NULL;

// UDP 소켓 정보 저장을 위한 구조체
typedef struct _UDP_SOCKETINFO
{
	SOCKET sock;
	SOCKADDR_IN serverAddr;
} UDP_SOCKETINFO;

typedef struct _UDP_CHAT_MSG
{
	int  type;
	char clientID[20];
	char msg[SIZE_DAT - 20];
} UDP_CHAT_MSG;


UDP_SOCKETINFO udpSocket;



// 클라이언트에서 UDP 소켓 초기화 함수
void InitUDPSocket()
{
	// UDP 소켓 생성
	udpSocket.sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (udpSocket.sock == INVALID_SOCKET) {
		//DisplayText("UDP 소켓 생성 실패\n");
		return;
	}

	// 서버 주소 설정
	memset(&udpSocket.serverAddr, 0, sizeof(udpSocket.serverAddr));
	udpSocket.serverAddr.sin_family = AF_INET;
	udpSocket.serverAddr.sin_port = htons(SERVERPORT); // 서버 포트 설정
	InetPton(AF_INET, SERVERIP4, &(udpSocket.serverAddr.sin_addr)); // 서버 IP 설정
}

// 클라이언트에서 UDP 소켓을 통한 데이터 송신 함수
void SendUDPMessage(const void* data, int dataSize)
{
	int sentBytes = sendto(udpSocket.sock, (const char*)data, dataSize, 0,
		(struct sockaddr*)&(udpSocket.serverAddr), sizeof(udpSocket.serverAddr));
	SendUDPMessage(data, sizeof(UDP_CHAT_MSG));
}

// 함수명 변경 및 UDP 채팅 메시지 전송 함수 추가
void SendUDPChatMessage(const UDP_CHAT_MSG* data)
{
	SendUDPMessage(data, sizeof(UDP_CHAT_MSG));
}

// 클라이언트에서 UDP 소켓을 통한 데이터 수신 함수
void ReceiveUDPMessage(void* buffer, int bufferSize)
{
	SOCKADDR_IN senderAddr;
	int senderAddrSize = sizeof(senderAddr);

	int receivedBytes = recvfrom(udpSocket.sock, (char*)buffer, bufferSize, 0,
		(struct sockaddr*)&senderAddr, &senderAddrSize);
	char senderIP[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(senderAddr.sin_addr), senderIP, sizeof(senderIP));
	int senderPort = ntohs(senderAddr.sin_port);

}

// 함수명 변경 및 UDP 채팅 메시지 수신 함수 추가
void ReceiveUDPChatMessage(UDP_CHAT_MSG* buffer)
{
	ReceiveUDPMessage(buffer, sizeof(UDP_CHAT_MSG));
}

// UDP 통신을 위한 쓰레드 함수 추가
DWORD WINAPI UDPReadThread(LPVOID arg)
{
	while (true) {
		UDP_CHAT_MSG udpChatMsg;
		ReceiveUDPChatMessage(&udpChatMsg);
	}
	return 0;
}

DWORD WINAPI UDPWriteThread(LPVOID arg)
{
	UDP_CHAT_MSG udpChatMsg;

	while (1) {
		SendUDPChatMessage(&udpChatMsg);
	}

	return 0;
}

// 대화상자 프로시저
INT_PTR CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
// 자식 윈도우 프로시저
LRESULT CALLBACK ChildWndProc(HWND, UINT, WPARAM, LPARAM);
// 소켓 통신 스레드 함수
DWORD WINAPI ClientMain(LPVOID arg);
DWORD WINAPI ReadThread(LPVOID arg);
DWORD WINAPI WriteThread(LPVOID arg);
// 에디트 컨트롤 출력 함수
void DisplayText(const char *fmt, ...);
