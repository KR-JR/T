#pragma once

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
