#pragma once

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
