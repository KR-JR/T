#include "..\..\Common.h"
#include "PrjClient.h"
#include "resource.h"
#include <windows.h>
#include <commdlg.h>
#include <wchar.h>

/* 윈도우 관련 전역 변수 */
static HINSTANCE     g_hInstance;     // 프로그램 인스턴스 핸들
static HWND          g_hBtnSendFile;  // [파일 전송] 버튼
static HWND          g_hBtnSendMsg;   // [메시지 전송] 버튼
static HWND          g_hEditStatus;   // 각종 메시지 출력 영역
static HWND          g_hBtnErasePic;  // [그림 지우기] 버튼
static HWND          g_hDrawWnd;      // 그림을 그릴 윈도우
static HWND			 g_hBtnChatID;	  // [채팅 ID 결정] 버튼

static HWND			 g_hBtnCircle;	  // 도형 그리기 [원] 버튼
static HWND			 g_hBtnRectangle;	  // 도형 그리기 [사각형] 버튼
static HWND			 g_hBtnTriangle;	  // 도형 그리기 [삼각형] 버튼

/* 통신 관련 전역 변수 */
static volatile bool g_isIPv6;        // IPv4 or IPv6 주소?
static char          g_ipaddr[64];    // 서버 IP 주소(문자열)
static int           g_port;          // 서버 포트 번호
static volatile bool g_isUDP;         // TCP or UDP 프로토콜?
static HANDLE        g_hClientThread; // 스레드 핸들
static volatile bool g_bCommStarted;  // 통신 시작 여부
static SOCKET        g_sock;          // 클라이언트 소켓
static HANDLE        g_hReadEvent;    // 이벤트 핸들(1)
static HANDLE        g_hWriteEvent;   // 이벤트 핸들(2)

/* 메시지 관련 전역 변수 */
static CHAT_MSG      g_chatmsg;       // 채팅 메시지
static DRAWLINE_MSG  g_drawlinemsg;   // 선 그리기 메시지
static int           g_drawcolor;     // 선 그리기 색상
static ERASEPIC_MSG  g_erasepicmsg;   // 그림 지우기 메시지
static FILEDATA_MSG  g_filedatamsg;   // 파일 전송 메시지
static SHAPE_MSG	 g_shapemsg;	  // 도형 그리기 메시지
static int			 g_shapecolor;	  // 도형 그리기 색상
static int			 g_shape;		  // 도형 유형

static boolean isShape;
static boolean g_drawing;

boolean isChkCircle = false;
boolean isChkRect = false;
boolean isChkTriangle = false;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 이벤트 생성(각각 신호, 비신호 상태)
	g_hReadEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	if (g_hReadEvent == NULL) return 1;
	g_hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (g_hWriteEvent == NULL) return 1;

	// 전역 변수 초기화(일부)
	g_chatmsg.type = TYPE_CHAT;
	g_drawlinemsg.type = TYPE_DRAWLINE;
	g_drawlinemsg.color = RGB(255, 0, 0);
	g_shapemsg.type = TYPE_SHAPE;
	g_shapemsg.shape = SHAPE_LINE;
	g_shapemsg.color = RGB(255, 0, 0);
	g_erasepicmsg.type = TYPE_ERASEPIC;
	g_filedatamsg.type = TYPE_FILE;

	// 대화상자 생성
	g_hInstance = hInstance;
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);

	// 이벤트 제거
	CloseHandle(g_hReadEvent);
	CloseHandle(g_hWriteEvent);

	// 윈속 종료
	WSACleanup();
	return 0;
}

// 대화상자 프로시저
INT_PTR CALLBACK DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hChkIsIPv6;
	static HWND hEditIPaddr;
	static HWND hEditPort;
	static HWND hChkIsUDP;
	static HWND hBtnConnect;
	static HWND hBtnSendFile; // 전역 변수에도 저장
	static HWND hBtnSendMsg; // 전역 변수에도 저장
	static HWND hEditMsg;
	static HWND hEditStatus; // 전역 변수에도 저장
	static HWND hColorRed;
	static HWND hColorGreen;
	static HWND hColorBlue;
	static HWND hBtnErasePic; // 전역 변수에도 저장
	static HWND hStaticDummy;
	static HWND hChatID;	// 채팅 아이디
	static HWND hBtnChatID;	// 채팅 ID 확정
	static HWND hBtnCircle;	// 원 그리기 버튼
	static HWND hBtnRectangle; // 사각형 그리기 버튼
	static HWND hBtnTriangle; // 삼각형 그리기 버튼

	switch (uMsg) {
	case WM_INITDIALOG:
		// 컨트롤 핸들 얻기
		hChkIsIPv6 = GetDlgItem(hDlg, IDC_ISIPV6);
		hEditIPaddr = GetDlgItem(hDlg, IDC_IPADDR);
		hEditPort = GetDlgItem(hDlg, IDC_PORT);
		hChkIsUDP = GetDlgItem(hDlg, IDC_ISUDP);
		hBtnConnect = GetDlgItem(hDlg, IDC_CONNECT);
		hBtnSendFile = GetDlgItem(hDlg, IDC_SENDFILE);
		g_hBtnSendFile = hBtnSendFile; // 전역 변수에 저장
		hBtnSendMsg = GetDlgItem(hDlg, IDC_SENDMSG);
		g_hBtnSendMsg = hBtnSendMsg; // 전역 변수에 저장
		hEditMsg = GetDlgItem(hDlg, IDC_MSG);
		hEditStatus = GetDlgItem(hDlg, IDC_STATUS);
		g_hEditStatus = hEditStatus; // 전역 변수에 저장
		hColorRed = GetDlgItem(hDlg, IDC_COLORRED);
		hColorGreen = GetDlgItem(hDlg, IDC_COLORGREEN);
		hColorBlue = GetDlgItem(hDlg, IDC_COLORBLUE);
		hBtnErasePic = GetDlgItem(hDlg, IDC_ERASEPIC);
		g_hBtnErasePic = hBtnErasePic; // 전역 변수에 저장
		hStaticDummy = GetDlgItem(hDlg, IDC_DUMMY);

		hBtnChatID = GetDlgItem(hDlg, IDC_CHATIDBTN);
		g_hBtnChatID = hBtnChatID;
		hChatID = GetDlgItem(hDlg, IDC_CHATID);

		hBtnCircle = GetDlgItem(hDlg, IDC_CIRCLE);
		g_hBtnCircle = hBtnCircle;
		hBtnRectangle = GetDlgItem(hDlg, IDC_RECTANGLE);
		g_hBtnRectangle = hBtnRectangle;
		hBtnTriangle = GetDlgItem(hDlg, IDC_TRIANGLE);
		g_hBtnTriangle = hBtnTriangle;


		// 컨트롤 초기화
		SetDlgItemText(hDlg, IDC_IPADDR, SERVERIP4);
		SetDlgItemInt(hDlg, IDC_PORT, SERVERPORT, FALSE);
		EnableWindow(g_hBtnSendFile, FALSE);
		EnableWindow(g_hBtnSendMsg, FALSE);
		EnableWindow(g_hBtnChatID, FALSE);
		SendMessage(hEditMsg, EM_SETLIMITTEXT, SIZE_DAT / 2, 0);
		SendMessage(hColorRed, BM_SETCHECK, BST_CHECKED, 0);
		SendMessage(hColorGreen, BM_SETCHECK, BST_UNCHECKED, 0);
		SendMessage(hColorBlue, BM_SETCHECK, BST_UNCHECKED, 0);
		EnableWindow(g_hBtnErasePic, FALSE);

		// 윈도우 클래스 등록
		WNDCLASS wndclass;
		wndclass.style = CS_HREDRAW | CS_VREDRAW;
		wndclass.lpfnWndProc = ChildWndProc;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hInstance = g_hInstance;
		wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		wndclass.lpszMenuName = NULL;
		wndclass.lpszClassName = _T("MyWndClass");
		if (!RegisterClass(&wndclass)) exit(1);

		// 자식 윈도우 생성
		RECT rect; GetWindowRect(hStaticDummy, &rect);
		POINT pt; pt.x = rect.left; pt.y = rect.top;
		ScreenToClient(hDlg, &pt);
		g_hDrawWnd = CreateWindow(_T("MyWndClass"), _T(""), WS_CHILD,
			pt.x, pt.y, rect.right - rect.left, rect.bottom - rect.top,
			hDlg, (HMENU)NULL, g_hInstance, NULL);
		if (g_hDrawWnd == NULL) exit(1);
		ShowWindow(g_hDrawWnd, SW_SHOW);
		UpdateWindow(g_hDrawWnd);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_ISIPV6:
			g_isIPv6 = SendMessage(hChkIsIPv6, BM_GETCHECK, 0, 0);
			if (g_isIPv6 == false)
				SetDlgItemText(hDlg, IDC_IPADDR, SERVERIP4);
			else
				SetDlgItemText(hDlg, IDC_IPADDR, SERVERIP6);
			return TRUE;
		case IDC_CONNECT:
			// 컨트롤 상태 얻기
			GetDlgItemTextA(hDlg, IDC_IPADDR, g_ipaddr, sizeof(g_ipaddr));
			g_port = GetDlgItemInt(hDlg, IDC_PORT, NULL, TRUE);
			g_isIPv6 = SendMessage(hChkIsIPv6, BM_GETCHECK, 0, 0);
			g_isUDP = SendMessage(hChkIsUDP, BM_GETCHECK, 0, 0);
			// 소켓 통신 스레드 시작
			g_hClientThread = CreateThread(NULL, 0, ClientMain, NULL, 0, NULL);
			if (g_hClientThread == NULL) exit(0);
			// 서버 접속 성공 기다림
			while (g_bCommStarted == false);
			// 컨트롤 상태 변경
			EnableWindow(hChkIsIPv6, FALSE);
			EnableWindow(hEditIPaddr, FALSE);
			EnableWindow(hEditPort, FALSE);
			EnableWindow(hChkIsUDP, FALSE);
			EnableWindow(hBtnConnect, FALSE);
			EnableWindow(g_hBtnSendFile, FALSE);
			EnableWindow(g_hBtnSendMsg, FALSE);
			EnableWindow(g_hBtnChatID, TRUE);	// 채팅 아이디 버튼 활성화
			SetFocus(hChatID);
			EnableWindow(g_hBtnErasePic, TRUE);
			return TRUE;
		case IDC_SENDFILE:
			// 1. GetOpenFileName() 윈도우 API를 이용해서 파일 열기 대화상자를 열고
			// 전송할 파일을 선택한다. 2. 선택한 파일을 읽어서 서버에 전송한다.
		{
			send(g_sock, (const char*)g_filedatamsg.type, sizeof(int), 0);

			// 파일 선택 대화상자 열기
			OPENFILENAME ofn;
			char szFile[252] = { 0 };

			// 초기화
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = NULL;
			ofn.lpstrFile = (LPWSTR)szFile;
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = L"모든 파일\0*.*\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			// 파일 열기 대화상자 열기
			if (GetOpenFileName(&ofn) == TRUE) {
				// 선택된 파일의 경로 출력
				MessageBox(NULL, (LPCWSTR)szFile, L"선택된 파일", MB_OK);

				FILE* file = _wfopen((const wchar_t*)szFile, L"rb");
				if (!file) {
					MessageBox(NULL, L"파일 열기 실패", L"에러", MB_OK | MB_ICONERROR);
					return TRUE;
				}
				
				// ofn.lpstrFile 길이 계산
				size_t len = wcslen(ofn.lpstrFile);

				// ofn.lpstrFile 을 char * 형식으로 저장
				wcstombs(g_filedatamsg.filename, ofn.lpstrFile, len);

				send(g_sock, (char*)&g_filedatamsg, SIZE_TOT, 0);

				// 파일 내용을 읽어서 서버에 전송
				char buf[256];
				size_t bytesRead;
				while ((bytesRead = fread(buf, 1, sizeof(buf), file)) > 0) {
					send(g_sock, buf, bytesRead, 0);
				}

				fclose(file);
			}
			else {
				MessageBox(NULL, L"파일 선택이 취소되었습니다.", L"알림", MB_OK | MB_ICONWARNING);
			}

			return TRUE;
		}
		case IDC_SENDMSG:
			// 이전에 얻은 채팅 메시지 읽기 완료를 기다림
			WaitForSingleObject(g_hReadEvent, INFINITE);
			// 새로운 채팅 메시지를 얻고 쓰기 완료를 알림
			GetDlgItemTextA(hDlg, IDC_MSG, g_chatmsg.msg, SIZE_DAT);
			SetEvent(g_hWriteEvent);
			// 입력된 텍스트 전체를 선택 표시
			SendMessage(hEditMsg, EM_SETSEL, 0, -1);
			return TRUE;
		case IDC_COLORRED:
			g_drawlinemsg.color = RGB(255, 0, 0);
			g_shapemsg.color = RGB(255, 0, 0);
			return TRUE;
		case IDC_COLORGREEN:
			g_drawlinemsg.color = RGB(0, 255, 0);
			g_shapemsg.color = RGB(0, 255, 0);
			return TRUE;
		case IDC_COLORBLUE:
			g_drawlinemsg.color = RGB(0, 0, 255);
			g_shapemsg.color = RGB(0, 0, 255);
			return TRUE;
		case IDC_ERASEPIC:
			send(g_sock, (char *)&g_erasepicmsg, SIZE_TOT, 0);
			return TRUE;
		case IDCANCEL:
			closesocket(g_sock);
			EndDialog(hDlg, IDCANCEL);
			return TRUE;
		case IDC_CHATIDBTN:
			GetDlgItemTextA(hDlg, IDC_CHATID, g_chatmsg.clientID, 20);
			if (strlen(g_chatmsg.clientID) == 0 || strlen(g_chatmsg.clientID) > 20) {
				MessageBox(NULL, L"채팅아이디가 입력되지 않았거나 너무 깁니다.", L"알림", MB_OK | MB_ICONWARNING);
				return FALSE;
			}
			EnableWindow(g_hBtnSendFile, TRUE);
			EnableWindow(g_hBtnSendMsg, TRUE);
			EnableWindow(g_hBtnChatID, FALSE);
			EnableWindow(hChatID, FALSE);
			return TRUE;
		case IDC_CIRCLE:
			// 원이 클릭되지 않은 상태
			if (!isChkCircle) {
				g_shapemsg.shape = SHAPE_CIRCLE;
				isChkCircle = true;
				SendMessage(hBtnRectangle, BM_SETCHECK, BST_UNCHECKED, 0);
				SendMessage(hBtnTriangle, BM_SETCHECK, BST_UNCHECKED, 0);
				isChkRect = false;
				isChkTriangle = false;

				// 현재 상태 : 도형
				isShape = true;
			}
			// 원이 클릭되어져 있는 상태
			else if (isChkCircle) {
				g_shapemsg.shape = SHAPE_LINE;
				isChkCircle = false;

				// 현재 상태 : 선
				isShape = false;
			}

			return TRUE;
		case IDC_RECTANGLE:
			// 사각형이 클릭되지 않은 상태
			if (!isChkRect) {
				g_shapemsg.shape = SHAPE_RECTANGLE;
				isChkRect = true;
				SendMessage(hBtnCircle, BM_SETCHECK, BST_UNCHECKED, 0);
				SendMessage(hBtnTriangle, BM_SETCHECK, BST_UNCHECKED, 0);
				isChkCircle = false;
				isChkTriangle = false;

				// 현재 상태 : 도형
				isShape = true;
			}
			// 사각형이 클릭되어져 있는 상태
			else if (isChkRect) {
				g_shapemsg.shape = SHAPE_LINE;
				isChkRect = false;

				// 현재 상태 : 선
				isShape = false;
			}

			return TRUE;
		case IDC_TRIANGLE:
			// 삼각형이 클릭되지 않은 상태
			if (!isChkTriangle) {
				g_shapemsg.shape = SHAPE_TRIANGLE;
				isChkTriangle = true;
				SendMessage(hBtnRectangle, BM_SETCHECK, BST_UNCHECKED, 0);
				SendMessage(hBtnCircle, BM_SETCHECK, BST_UNCHECKED, 0);
				isChkRect = false;
				isChkCircle = false;

				// 현재 상태 : 도형
				isShape = true;
			}
			// 삼각형이 클릭되어져 있는 상태
			else if (isChkTriangle) {
				g_shapemsg.shape = SHAPE_LINE;
				isChkTriangle = false;

				// 현재 상태 : 선
				isShape = false;
			}

			return TRUE;
		}
	}
	return FALSE;
}

// 자식 윈도우 프로시저
LRESULT CALLBACK ChildWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;
	HPEN hPen, hOldPen;
	PAINTSTRUCT ps;
	static int cx, cy;
	static HBITMAP hBitmap;
	static HDC hDCMem;
	static int x0, y0;
	static int x1, y1;

	switch (uMsg) {
	case WM_SIZE:
		// 화면 출력용 DC 핸들 얻기
		hDC = GetDC(hWnd);
		// 배경 비트맵과 메모리 DC 생성
		cx = LOWORD(lParam);
		cy = HIWORD(lParam);
		hBitmap = CreateCompatibleBitmap(hDC, cx, cy);
		hDCMem = CreateCompatibleDC(hDC);
		SelectObject(hDCMem, hBitmap);
		// 배경 비트맵 흰색으로 채움
		SelectObject(hDCMem, GetStockObject(WHITE_BRUSH));
		SelectObject(hDCMem, GetStockObject(WHITE_PEN));
		Rectangle(hDCMem, 0, 0, cx, cy);
		// 화면 출력용 DC 핸들 해제
		ReleaseDC(hWnd, hDC);
		return 0;
	case WM_PAINT:
		// 화면 출력용 DC 핸들 얻기
		hDC = BeginPaint(hWnd, &ps);
		// 배경 비트맵을 화면에 전송
		BitBlt(hDC, 0, 0, cx, cy, hDCMem, 0, 0, SRCCOPY);
		// 화면 출력용 DC 핸들 해제
		EndPaint(hWnd, &ps);
		return 0;
	case WM_LBUTTONDOWN:
		// 마우스 클릭 좌표 얻기
		x0 = LOWORD(lParam);
		y0 = HIWORD(lParam);

		g_drawing = true;
		if (isChkCircle || isChkRect || isChkTriangle) {
			g_drawing = false;
			isShape = true;
		}
		return 0;
	case WM_MOUSEMOVE:
		if (g_drawing && !isShape && g_bCommStarted) {
			// 마우스 클릭 좌표 얻기
			x1 = LOWORD(lParam);
			y1 = HIWORD(lParam);
			// 선 그리기 메시지 보내기
			g_drawlinemsg.x0 = x0;
			g_drawlinemsg.y0 = y0;
			g_drawlinemsg.x1 = x1;
			g_drawlinemsg.y1 = y1;
			send(g_sock, (char*)&g_drawlinemsg, SIZE_TOT, 0);
			// 마우스 클릭 좌표 갱신
			x0 = x1;
			y0 = y1;
		}

		return 0;
	case WM_LBUTTONUP:
		// 마우스 클릭 좌표 얻기
		x1 = LOWORD(lParam);
		y1 = HIWORD(lParam);
		// 좌표 설정
		g_shapemsg.x0 = x0;
		g_shapemsg.y0 = y0;
		g_shapemsg.x1 = x1;
		g_shapemsg.y1 = y1;

		// 도형 그리기 : 원
		if (isChkCircle) {
			g_shapemsg.shape = SHAPE_CIRCLE;
			send(g_sock, (char*)&g_shapemsg, sizeof(SHAPE_MSG), 0);

			// 마우스 클릭 좌표 갱신
			x0 = x1;
			y0 = y1;
		}
		// 도형 그리기 : 사각형
		else if (isChkRect) {
			send(g_sock, (char*)&g_shapemsg, sizeof(SHAPE_MSG), 0);

			// 마우스 클릭 좌표 갱신
			x0 = x1;
			y0 = y1;
		}
		// 도형 그리기 : 삼각형
		else if (isChkTriangle) {
			send(g_sock, (char*)&g_shapemsg, sizeof(SHAPE_MSG), 0);

			// 마우스 클릭 좌표 갱신
			x0 = x1;
			y0 = y1;
		}

		g_drawing = false;
		isShape = false;

		SendMessage(g_hBtnCircle, BM_SETCHECK, BST_UNCHECKED, 0);
		SendMessage(g_hBtnRectangle, BM_SETCHECK, BST_UNCHECKED, 0);
		SendMessage(g_hBtnTriangle, BM_SETCHECK, BST_UNCHECKED, 0);

		isChkCircle = false;
		isChkRect = false;
		isChkTriangle = false;

		return 0;
	case WM_DRAWLINE:
		// 화면 출력용 DC와 Pen 핸들 얻기
		hDC = GetDC(hWnd);
		hPen = CreatePen(PS_SOLID, 3, g_drawcolor);
		// 윈도우 화면에 1차로 그리기
		hOldPen = (HPEN)SelectObject(hDC, hPen);
		MoveToEx(hDC, LOWORD(wParam), HIWORD(wParam), NULL);
		LineTo(hDC, LOWORD(lParam), HIWORD(lParam));
		SelectObject(hDC, hOldPen);
		// 배경 비트맵에 2차로 그리기
		hOldPen = (HPEN)SelectObject(hDCMem, hPen);
		MoveToEx(hDCMem, LOWORD(wParam), HIWORD(wParam), NULL);
		LineTo(hDCMem, LOWORD(lParam), HIWORD(lParam));
		SelectObject(hDCMem, hOldPen);
		// 화면 출력용 DC와 Pen 핸들 해제
		DeleteObject(hPen);
		ReleaseDC(hWnd, hDC);
		return 0;
	case WM_DRAWSHAPE:

		// 화면 출력용 DC와 Pen 핸들 얻기
		hDC = GetDC(hWnd);
		hPen = CreatePen(PS_SOLID, 3, g_shapecolor);
		hOldPen = (HPEN)SelectObject(hDC, hPen);
		if (g_shape == SHAPE_CIRCLE) {
			// 원 그리기
			DisplayText("[도형 그리기] 원\r\n");
			Ellipse(hDC, g_shapemsg.x0, g_shapemsg.y0, g_shapemsg.x1, g_shapemsg.y1);
			// 배경 비트맵에 2차로 그리기
			hOldPen = (HPEN)SelectObject(hDCMem, hPen);
			Ellipse(hDCMem, g_shapemsg.x0, g_shapemsg.y0, g_shapemsg.x1, g_shapemsg.y1);
			SelectObject(hDCMem, hOldPen);

			SendMessage(g_hBtnCircle, BM_SETCHECK, BST_UNCHECKED, 0);
		}
		else if (g_shape == SHAPE_RECTANGLE) {
			// 사각형 그리기
			Rectangle(hDC, g_shapemsg.x0, g_shapemsg.y0, g_shapemsg.x1, g_shapemsg.y1);
			DisplayText("[도형 그리기] 사각형\r\n");

			SendMessage(g_hBtnRectangle, BM_SETCHECK, BST_UNCHECKED, 0);
		}
		else if (g_shape == SHAPE_TRIANGLE) {
			// 삼각형 그리기
			POINT points[3]; // 삼각형을 이루는 세 점의 좌표를 저장하는 배열

			// 세 점의 좌표 설정
			points[0].x = g_shapemsg.x0;
			points[0].y = g_shapemsg.y0;
			points[1].x = (g_shapemsg.x0 + g_shapemsg.x1) / 2; // 중간 지점
			points[1].y = g_shapemsg.y1;
			points[2].x = g_shapemsg.x1;
			points[2].y = g_shapemsg.y0;

			Polygon(hDC, points, 3);

			DisplayText("[도형 그리기] 삼각형\r\n");
		}
		DeleteObject(hPen);
		ReleaseDC(hWnd, hDC);

		return 0;
	case WM_ERASEPIC:
		// 배경 비트맵 흰색으로 채움
		SelectObject(hDCMem, GetStockObject(WHITE_BRUSH));
		SelectObject(hDCMem, GetStockObject(WHITE_PEN));
		Rectangle(hDCMem, 0, 0, cx, cy);
		// WM_PAINT 메시지 강제 생성
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;
	case WM_DESTROY:
		DeleteDC(hDCMem);
		DeleteObject(hBitmap);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// 소켓 통신 스레드 함수(1) - 메인
DWORD WINAPI ClientMain(LPVOID arg)
{
	int retval;

	if (g_isIPv6 == false && g_isUDP == false) { // TCP/IPv4 서버
		// socket()
		g_sock = socket(AF_INET, SOCK_STREAM, 0);
		if (g_sock == INVALID_SOCKET) err_quit("socket()");

		// connect()
		struct sockaddr_in serveraddr;
		memset(&serveraddr, 0, sizeof(serveraddr));
		serveraddr.sin_family = AF_INET;
		inet_pton(AF_INET, g_ipaddr, &serveraddr.sin_addr);
		serveraddr.sin_port = htons(g_port);
		retval = connect(g_sock, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
		if (retval == SOCKET_ERROR) err_quit("connect()");
	}
	else if (g_isIPv6 == true && g_isUDP == false) { // TCP/IPv6 서버
		// socket()
		g_sock = socket(AF_INET6, SOCK_STREAM, 0);
		if (g_sock == INVALID_SOCKET) err_quit("socket()");

		// connect()
		struct sockaddr_in6 serveraddr;
		memset(&serveraddr, 0, sizeof(serveraddr));
		serveraddr.sin6_family = AF_INET6;
		inet_pton(AF_INET6, g_ipaddr, &serveraddr.sin6_addr);
		serveraddr.sin6_port = htons(g_port);
		retval = connect(g_sock, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
		if (retval == SOCKET_ERROR) err_quit("connect()");
	}
	else if (g_isIPv6 == false && g_isUDP == true) { // UDP/IPv4 서버
		MessageBox(NULL, _T("아직 구현하지 않았습니다."), _T("알림"), MB_ICONERROR);
		exit(1);
	}
	else if (g_isIPv6 == true && g_isUDP == true) { // UDP/IPv6 서버
		MessageBox(NULL, _T("아직 구현하지 않았습니다."), _T("알림"), MB_ICONERROR);
		exit(1);
	}
	MessageBox(NULL, _T("서버에 접속했습니다."), _T("알림"), MB_ICONINFORMATION);

	// 읽기 & 쓰기 스레드 생성
	HANDLE hThread[2];
	hThread[0] = CreateThread(NULL, 0, ReadThread, NULL, 0, NULL);
	hThread[1] = CreateThread(NULL, 0, WriteThread, NULL, 0, NULL);
	if (hThread[0] == NULL || hThread[1] == NULL) exit(1);
	g_bCommStarted = true;

	// 스레드 종료 대기 (둘 중 하나라도 종료할 때까지)
	retval = WaitForMultipleObjects(2, hThread, FALSE, INFINITE);
	retval -= WAIT_OBJECT_0;
	if (retval == 0)
		TerminateThread(hThread[1], 1);
	else
		TerminateThread(hThread[0], 1);
	CloseHandle(hThread[0]);
	CloseHandle(hThread[1]);

	MessageBox(NULL, _T("연결이 끊겼습니다."), _T("알림"), MB_ICONERROR);
	EnableWindow(g_hBtnSendFile, FALSE);
	EnableWindow(g_hBtnSendMsg, FALSE);
	EnableWindow(g_hBtnErasePic, FALSE);
	g_bCommStarted = false;
	closesocket(g_sock);
	return 0;
}

// 소켓 통신 스레드 함수(2) - 데이터 수신
DWORD WINAPI ReadThread(LPVOID arg)
{
	int retval;
	COMM_MSG comm_msg;
	CHAT_MSG *chat_msg;
	DRAWLINE_MSG *drawline_msg;
	ERASEPIC_MSG *erasepic_msg;
	SHAPE_MSG* shape_msg;

	while (1) {
		retval = recv(g_sock, (char *)&comm_msg, SIZE_TOT, MSG_WAITALL);
		if (retval == 0 || retval == SOCKET_ERROR) {
			break;
		}
		if (comm_msg.type == TYPE_CHAT) {
			chat_msg = (CHAT_MSG *)&comm_msg;
			DisplayText("[%s] %s\r\n", chat_msg->clientID, chat_msg->msg);
		}
		else if (comm_msg.type == TYPE_DRAWLINE) {
			drawline_msg = (DRAWLINE_MSG *)&comm_msg;
			g_drawcolor = drawline_msg->color;
			SendMessage(g_hDrawWnd, WM_DRAWLINE,
				MAKEWPARAM(drawline_msg->x0, drawline_msg->y0),
				MAKELPARAM(drawline_msg->x1, drawline_msg->y1));
		}
		else if (comm_msg.type == TYPE_ERASEPIC) {
			erasepic_msg = (ERASEPIC_MSG *)&comm_msg;
			SendMessage(g_hDrawWnd, WM_ERASEPIC, 0, 0);
		}
		else if (comm_msg.type == TYPE_FILE) {
			
		}
		else if (comm_msg.type == TYPE_SHAPE) {
			shape_msg = (SHAPE_MSG*)&comm_msg;
			g_shapecolor = shape_msg->color;
			g_shape = shape_msg->shape;
			
			SendMessage(g_hDrawWnd, WM_DRAWSHAPE,
				MAKEWPARAM(shape_msg->x0, shape_msg->y0),
				MAKELPARAM(shape_msg->x1, shape_msg->y1));
		}
	}
	return 0;
}

// 소켓 통신 스레드 함수(3) - 데이터 송신
DWORD WINAPI WriteThread(LPVOID arg)
{
	int retval;

	// 서버와 데이터 통신
	while (1) {
		// 쓰기 완료 기다리기
		WaitForSingleObject(g_hWriteEvent, INFINITE);
		// 문자열 길이가 0이면 보내지 않음
		if (strlen(g_chatmsg.msg) == 0) {
			// [메시지 전송] 버튼 활성화
			EnableWindow(g_hBtnSendMsg, TRUE);
			// 읽기 완료 알리기
			SetEvent(g_hReadEvent);
			continue;
		}
		// 데이터 보내기
		retval = send(g_sock, (char *)&g_chatmsg, SIZE_TOT, 0);
		if (retval == SOCKET_ERROR) break;
		// [메시지 전송] 버튼 활성화
		EnableWindow(g_hBtnSendMsg, TRUE);
		// 읽기 완료 알리기
		SetEvent(g_hReadEvent);
	}
	return 0;
}

// 에디트 컨트롤 출력 함수
void DisplayText(const char *fmt, ...)
{
	va_list arg;
	va_start(arg, fmt);
	char cbuf[1024];
	vsprintf(cbuf, fmt, arg);
	va_end(arg);

	int nLength = GetWindowTextLength(g_hEditStatus);
	SendMessage(g_hEditStatus, EM_SETSEL, nLength, nLength);
	SendMessageA(g_hEditStatus, EM_REPLACESEL, FALSE, (LPARAM)cbuf);
}
