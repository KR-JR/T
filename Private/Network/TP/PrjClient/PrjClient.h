#pragma once
#include <ws2tcpip.h>

#define SERVERIP4  _T("127.0.0.1")
#define SERVERIP6  _T("::1")
#define SERVERPORT  9000

#define SIZE_TOT 256                    // ���� ��Ŷ(��� + ������) ��ü ũ��
#define SIZE_DAT (SIZE_TOT-sizeof(int)) // ����� ������ ������ �κи��� ũ��

#define TYPE_CHAT     1000              // �޽��� Ÿ��: ä��
#define TYPE_DRAWLINE 1001              // �޽��� Ÿ��: �� �׸���
#define TYPE_ERASEPIC 1002              // �޽��� Ÿ��: �׸� �����
#define TYPE_FILE	  1003				// �޽��� Ÿ��: ���� ����
#define TYPE_SHAPE	  1004				// �޽��� Ÿ��: ���� �׸���

#define WM_DRAWLINE (WM_USER+1)         // ����� ���� ������ �޽���(1)
#define WM_ERASEPIC (WM_USER+2)         // ����� ���� ������ �޽���(2)
#define WM_FILETRANSFER (WM_USER+3)     // ����� ���� ������ �޽���(3)
#define WM_DRAWSHAPE (WM_USER+4)		// ����� ���� ������ �޽���(4)

#define SHAPE_CIRCLE	 201			// ���� �׸��� : ��
#define SHAPE_RECTANGLE	 202			// ���� �׸��� : �簢��
#define SHAPE_TRIANGLE	 203			// ���� �׸��� : �ﰢ��
#define SHAPE_LINE		 204			// ���� �׸��� : ��

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
	char clientID[20];
	char msg[SIZE_DAT - 20];
} CHAT_MSG;

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

// ���� �׸��� �޽��� ����
typedef struct _SHAPE_MSG
{
	int type;
	int shape;
	int color;
	int x0, y0;
	int x1, y1;
	char dummy[SIZE_TOT - 7 * sizeof(int)];
} SHAPE_MSG;

// �׸� ����� �޽��� ����
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

// UDP ���� ���� ������ ���� ����ü
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



// Ŭ���̾�Ʈ���� UDP ���� �ʱ�ȭ �Լ�
void InitUDPSocket()
{
	// UDP ���� ����
	udpSocket.sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (udpSocket.sock == INVALID_SOCKET) {
		//DisplayText("UDP ���� ���� ����\n");
		return;
	}

	// ���� �ּ� ����
	memset(&udpSocket.serverAddr, 0, sizeof(udpSocket.serverAddr));
	udpSocket.serverAddr.sin_family = AF_INET;
	udpSocket.serverAddr.sin_port = htons(SERVERPORT); // ���� ��Ʈ ����
	InetPton(AF_INET, SERVERIP4, &(udpSocket.serverAddr.sin_addr)); // ���� IP ����
}

// Ŭ���̾�Ʈ���� UDP ������ ���� ������ �۽� �Լ�
void SendUDPMessage(const void* data, int dataSize)
{
	int sentBytes = sendto(udpSocket.sock, (const char*)data, dataSize, 0,
		(struct sockaddr*)&(udpSocket.serverAddr), sizeof(udpSocket.serverAddr));
	SendUDPMessage(data, sizeof(UDP_CHAT_MSG));
}

// �Լ��� ���� �� UDP ä�� �޽��� ���� �Լ� �߰�
void SendUDPChatMessage(const UDP_CHAT_MSG* data)
{
	SendUDPMessage(data, sizeof(UDP_CHAT_MSG));
}

// Ŭ���̾�Ʈ���� UDP ������ ���� ������ ���� �Լ�
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

// �Լ��� ���� �� UDP ä�� �޽��� ���� �Լ� �߰�
void ReceiveUDPChatMessage(UDP_CHAT_MSG* buffer)
{
	ReceiveUDPMessage(buffer, sizeof(UDP_CHAT_MSG));
}

// UDP ����� ���� ������ �Լ� �߰�
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

// ��ȭ���� ���ν���
INT_PTR CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
// �ڽ� ������ ���ν���
LRESULT CALLBACK ChildWndProc(HWND, UINT, WPARAM, LPARAM);
// ���� ��� ������ �Լ�
DWORD WINAPI ClientMain(LPVOID arg);
DWORD WINAPI ReadThread(LPVOID arg);
DWORD WINAPI WriteThread(LPVOID arg);
// ����Ʈ ��Ʈ�� ��� �Լ�
void DisplayText(const char *fmt, ...);
