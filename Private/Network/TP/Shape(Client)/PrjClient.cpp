#include "..\..\Common.h"
#include "PrjClient.h"
#include "resource.h"
#include <windows.h>
#include <commdlg.h>
#include <wchar.h>

/* ������ ���� ���� ���� */
static HINSTANCE     g_hInstance;     // ���α׷� �ν��Ͻ� �ڵ�
static HWND          g_hBtnSendFile;  // [���� ����] ��ư
static HWND          g_hBtnSendMsg;   // [�޽��� ����] ��ư
static HWND          g_hEditStatus;   // ���� �޽��� ��� ����
static HWND          g_hBtnErasePic;  // [�׸� �����] ��ư
static HWND          g_hDrawWnd;      // �׸��� �׸� ������
static HWND			 g_hBtnChatID;	  // [ä�� ID ����] ��ư

static HWND			 g_hBtnCircle;	  // ���� �׸��� [��] ��ư
static HWND			 g_hBtnRectangle;	  // ���� �׸��� [�簢��] ��ư
static HWND			 g_hBtnTriangle;	  // ���� �׸��� [�ﰢ��] ��ư

/* ��� ���� ���� ���� */
static volatile bool g_isIPv6;        // IPv4 or IPv6 �ּ�?
static char          g_ipaddr[64];    // ���� IP �ּ�(���ڿ�)
static int           g_port;          // ���� ��Ʈ ��ȣ
static volatile bool g_isUDP;         // TCP or UDP ��������?
static HANDLE        g_hClientThread; // ������ �ڵ�
static volatile bool g_bCommStarted;  // ��� ���� ����
static SOCKET        g_sock;          // Ŭ���̾�Ʈ ����
static HANDLE        g_hReadEvent;    // �̺�Ʈ �ڵ�(1)
static HANDLE        g_hWriteEvent;   // �̺�Ʈ �ڵ�(2)

/* �޽��� ���� ���� ���� */
static CHAT_MSG      g_chatmsg;       // ä�� �޽���
static DRAWLINE_MSG  g_drawlinemsg;   // �� �׸��� �޽���
static int           g_drawcolor;     // �� �׸��� ����
static ERASEPIC_MSG  g_erasepicmsg;   // �׸� ����� �޽���
static FILEDATA_MSG  g_filedatamsg;   // ���� ���� �޽���
static SHAPE_MSG	 g_shapemsg;	  // ���� �׸��� �޽���
static int			 g_shapecolor;	  // ���� �׸��� ����
static int			 g_shape;		  // ���� ����

static boolean isShape;
static boolean g_drawing;

boolean isChkCircle = false;
boolean isChkRect = false;
boolean isChkTriangle = false;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// �̺�Ʈ ����(���� ��ȣ, ���ȣ ����)
	g_hReadEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	if (g_hReadEvent == NULL) return 1;
	g_hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (g_hWriteEvent == NULL) return 1;

	// ���� ���� �ʱ�ȭ(�Ϻ�)
	g_chatmsg.type = TYPE_CHAT;
	g_drawlinemsg.type = TYPE_DRAWLINE;
	g_drawlinemsg.color = RGB(255, 0, 0);
	g_shapemsg.type = TYPE_SHAPE;
	g_shapemsg.shape = SHAPE_LINE;
	g_shapemsg.color = RGB(255, 0, 0);
	g_erasepicmsg.type = TYPE_ERASEPIC;
	g_filedatamsg.type = TYPE_FILE;

	// ��ȭ���� ����
	g_hInstance = hInstance;
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);

	// �̺�Ʈ ����
	CloseHandle(g_hReadEvent);
	CloseHandle(g_hWriteEvent);

	// ���� ����
	WSACleanup();
	return 0;
}

// ��ȭ���� ���ν���
INT_PTR CALLBACK DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hChkIsIPv6;
	static HWND hEditIPaddr;
	static HWND hEditPort;
	static HWND hChkIsUDP;
	static HWND hBtnConnect;
	static HWND hBtnSendFile; // ���� �������� ����
	static HWND hBtnSendMsg; // ���� �������� ����
	static HWND hEditMsg;
	static HWND hEditStatus; // ���� �������� ����
	static HWND hColorRed;
	static HWND hColorGreen;
	static HWND hColorBlue;
	static HWND hBtnErasePic; // ���� �������� ����
	static HWND hStaticDummy;
	static HWND hChatID;	// ä�� ���̵�
	static HWND hBtnChatID;	// ä�� ID Ȯ��
	static HWND hBtnCircle;	// �� �׸��� ��ư
	static HWND hBtnRectangle; // �簢�� �׸��� ��ư
	static HWND hBtnTriangle; // �ﰢ�� �׸��� ��ư

	switch (uMsg) {
	case WM_INITDIALOG:
		// ��Ʈ�� �ڵ� ���
		hChkIsIPv6 = GetDlgItem(hDlg, IDC_ISIPV6);
		hEditIPaddr = GetDlgItem(hDlg, IDC_IPADDR);
		hEditPort = GetDlgItem(hDlg, IDC_PORT);
		hChkIsUDP = GetDlgItem(hDlg, IDC_ISUDP);
		hBtnConnect = GetDlgItem(hDlg, IDC_CONNECT);
		hBtnSendFile = GetDlgItem(hDlg, IDC_SENDFILE);
		g_hBtnSendFile = hBtnSendFile; // ���� ������ ����
		hBtnSendMsg = GetDlgItem(hDlg, IDC_SENDMSG);
		g_hBtnSendMsg = hBtnSendMsg; // ���� ������ ����
		hEditMsg = GetDlgItem(hDlg, IDC_MSG);
		hEditStatus = GetDlgItem(hDlg, IDC_STATUS);
		g_hEditStatus = hEditStatus; // ���� ������ ����
		hColorRed = GetDlgItem(hDlg, IDC_COLORRED);
		hColorGreen = GetDlgItem(hDlg, IDC_COLORGREEN);
		hColorBlue = GetDlgItem(hDlg, IDC_COLORBLUE);
		hBtnErasePic = GetDlgItem(hDlg, IDC_ERASEPIC);
		g_hBtnErasePic = hBtnErasePic; // ���� ������ ����
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


		// ��Ʈ�� �ʱ�ȭ
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

		// ������ Ŭ���� ���
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

		// �ڽ� ������ ����
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
			// ��Ʈ�� ���� ���
			GetDlgItemTextA(hDlg, IDC_IPADDR, g_ipaddr, sizeof(g_ipaddr));
			g_port = GetDlgItemInt(hDlg, IDC_PORT, NULL, TRUE);
			g_isIPv6 = SendMessage(hChkIsIPv6, BM_GETCHECK, 0, 0);
			g_isUDP = SendMessage(hChkIsUDP, BM_GETCHECK, 0, 0);
			// ���� ��� ������ ����
			g_hClientThread = CreateThread(NULL, 0, ClientMain, NULL, 0, NULL);
			if (g_hClientThread == NULL) exit(0);
			// ���� ���� ���� ��ٸ�
			while (g_bCommStarted == false);
			// ��Ʈ�� ���� ����
			EnableWindow(hChkIsIPv6, FALSE);
			EnableWindow(hEditIPaddr, FALSE);
			EnableWindow(hEditPort, FALSE);
			EnableWindow(hChkIsUDP, FALSE);
			EnableWindow(hBtnConnect, FALSE);
			EnableWindow(g_hBtnSendFile, FALSE);
			EnableWindow(g_hBtnSendMsg, FALSE);
			EnableWindow(g_hBtnChatID, TRUE);	// ä�� ���̵� ��ư Ȱ��ȭ
			SetFocus(hChatID);
			EnableWindow(g_hBtnErasePic, TRUE);
			return TRUE;
		case IDC_SENDFILE:
			// 1. GetOpenFileName() ������ API�� �̿��ؼ� ���� ���� ��ȭ���ڸ� ����
			// ������ ������ �����Ѵ�. 2. ������ ������ �о ������ �����Ѵ�.
		{
			send(g_sock, (const char*)g_filedatamsg.type, sizeof(int), 0);

			// ���� ���� ��ȭ���� ����
			OPENFILENAME ofn;
			char szFile[252] = { 0 };

			// �ʱ�ȭ
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = NULL;
			ofn.lpstrFile = (LPWSTR)szFile;
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = L"��� ����\0*.*\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			// ���� ���� ��ȭ���� ����
			if (GetOpenFileName(&ofn) == TRUE) {
				// ���õ� ������ ��� ���
				MessageBox(NULL, (LPCWSTR)szFile, L"���õ� ����", MB_OK);

				FILE* file = _wfopen((const wchar_t*)szFile, L"rb");
				if (!file) {
					MessageBox(NULL, L"���� ���� ����", L"����", MB_OK | MB_ICONERROR);
					return TRUE;
				}
				
				// ofn.lpstrFile ���� ���
				size_t len = wcslen(ofn.lpstrFile);

				// ofn.lpstrFile �� char * �������� ����
				wcstombs(g_filedatamsg.filename, ofn.lpstrFile, len);

				send(g_sock, (char*)&g_filedatamsg, SIZE_TOT, 0);

				// ���� ������ �о ������ ����
				char buf[256];
				size_t bytesRead;
				while ((bytesRead = fread(buf, 1, sizeof(buf), file)) > 0) {
					send(g_sock, buf, bytesRead, 0);
				}

				fclose(file);
			}
			else {
				MessageBox(NULL, L"���� ������ ��ҵǾ����ϴ�.", L"�˸�", MB_OK | MB_ICONWARNING);
			}

			return TRUE;
		}
		case IDC_SENDMSG:
			// ������ ���� ä�� �޽��� �б� �ϷḦ ��ٸ�
			WaitForSingleObject(g_hReadEvent, INFINITE);
			// ���ο� ä�� �޽����� ��� ���� �ϷḦ �˸�
			GetDlgItemTextA(hDlg, IDC_MSG, g_chatmsg.msg, SIZE_DAT);
			SetEvent(g_hWriteEvent);
			// �Էµ� �ؽ�Ʈ ��ü�� ���� ǥ��
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
				MessageBox(NULL, L"ä�þ��̵� �Էµ��� �ʾҰų� �ʹ� ��ϴ�.", L"�˸�", MB_OK | MB_ICONWARNING);
				return FALSE;
			}
			EnableWindow(g_hBtnSendFile, TRUE);
			EnableWindow(g_hBtnSendMsg, TRUE);
			EnableWindow(g_hBtnChatID, FALSE);
			EnableWindow(hChatID, FALSE);
			return TRUE;
		case IDC_CIRCLE:
			// ���� Ŭ������ ���� ����
			if (!isChkCircle) {
				g_shapemsg.shape = SHAPE_CIRCLE;
				isChkCircle = true;
				SendMessage(hBtnRectangle, BM_SETCHECK, BST_UNCHECKED, 0);
				SendMessage(hBtnTriangle, BM_SETCHECK, BST_UNCHECKED, 0);
				isChkRect = false;
				isChkTriangle = false;

				// ���� ���� : ����
				isShape = true;
			}
			// ���� Ŭ���Ǿ��� �ִ� ����
			else if (isChkCircle) {
				g_shapemsg.shape = SHAPE_LINE;
				isChkCircle = false;

				// ���� ���� : ��
				isShape = false;
			}

			return TRUE;
		case IDC_RECTANGLE:
			// �簢���� Ŭ������ ���� ����
			if (!isChkRect) {
				g_shapemsg.shape = SHAPE_RECTANGLE;
				isChkRect = true;
				SendMessage(hBtnCircle, BM_SETCHECK, BST_UNCHECKED, 0);
				SendMessage(hBtnTriangle, BM_SETCHECK, BST_UNCHECKED, 0);
				isChkCircle = false;
				isChkTriangle = false;

				// ���� ���� : ����
				isShape = true;
			}
			// �簢���� Ŭ���Ǿ��� �ִ� ����
			else if (isChkRect) {
				g_shapemsg.shape = SHAPE_LINE;
				isChkRect = false;

				// ���� ���� : ��
				isShape = false;
			}

			return TRUE;
		case IDC_TRIANGLE:
			// �ﰢ���� Ŭ������ ���� ����
			if (!isChkTriangle) {
				g_shapemsg.shape = SHAPE_TRIANGLE;
				isChkTriangle = true;
				SendMessage(hBtnRectangle, BM_SETCHECK, BST_UNCHECKED, 0);
				SendMessage(hBtnCircle, BM_SETCHECK, BST_UNCHECKED, 0);
				isChkRect = false;
				isChkCircle = false;

				// ���� ���� : ����
				isShape = true;
			}
			// �ﰢ���� Ŭ���Ǿ��� �ִ� ����
			else if (isChkTriangle) {
				g_shapemsg.shape = SHAPE_LINE;
				isChkTriangle = false;

				// ���� ���� : ��
				isShape = false;
			}

			return TRUE;
		}
	}
	return FALSE;
}

// �ڽ� ������ ���ν���
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
		// ȭ�� ��¿� DC �ڵ� ���
		hDC = GetDC(hWnd);
		// ��� ��Ʈ�ʰ� �޸� DC ����
		cx = LOWORD(lParam);
		cy = HIWORD(lParam);
		hBitmap = CreateCompatibleBitmap(hDC, cx, cy);
		hDCMem = CreateCompatibleDC(hDC);
		SelectObject(hDCMem, hBitmap);
		// ��� ��Ʈ�� ������� ä��
		SelectObject(hDCMem, GetStockObject(WHITE_BRUSH));
		SelectObject(hDCMem, GetStockObject(WHITE_PEN));
		Rectangle(hDCMem, 0, 0, cx, cy);
		// ȭ�� ��¿� DC �ڵ� ����
		ReleaseDC(hWnd, hDC);
		return 0;
	case WM_PAINT:
		// ȭ�� ��¿� DC �ڵ� ���
		hDC = BeginPaint(hWnd, &ps);
		// ��� ��Ʈ���� ȭ�鿡 ����
		BitBlt(hDC, 0, 0, cx, cy, hDCMem, 0, 0, SRCCOPY);
		// ȭ�� ��¿� DC �ڵ� ����
		EndPaint(hWnd, &ps);
		return 0;
	case WM_LBUTTONDOWN:
		// ���콺 Ŭ�� ��ǥ ���
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
			// ���콺 Ŭ�� ��ǥ ���
			x1 = LOWORD(lParam);
			y1 = HIWORD(lParam);
			// �� �׸��� �޽��� ������
			g_drawlinemsg.x0 = x0;
			g_drawlinemsg.y0 = y0;
			g_drawlinemsg.x1 = x1;
			g_drawlinemsg.y1 = y1;
			send(g_sock, (char*)&g_drawlinemsg, SIZE_TOT, 0);
			// ���콺 Ŭ�� ��ǥ ����
			x0 = x1;
			y0 = y1;
		}

		return 0;
	case WM_LBUTTONUP:
		// ���콺 Ŭ�� ��ǥ ���
		x1 = LOWORD(lParam);
		y1 = HIWORD(lParam);
		// ��ǥ ����
		g_shapemsg.x0 = x0;
		g_shapemsg.y0 = y0;
		g_shapemsg.x1 = x1;
		g_shapemsg.y1 = y1;

		// ���� �׸��� : ��
		if (isChkCircle) {
			g_shapemsg.shape = SHAPE_CIRCLE;
			send(g_sock, (char*)&g_shapemsg, sizeof(SHAPE_MSG), 0);

			// ���콺 Ŭ�� ��ǥ ����
			x0 = x1;
			y0 = y1;
		}
		// ���� �׸��� : �簢��
		else if (isChkRect) {
			send(g_sock, (char*)&g_shapemsg, sizeof(SHAPE_MSG), 0);

			// ���콺 Ŭ�� ��ǥ ����
			x0 = x1;
			y0 = y1;
		}
		// ���� �׸��� : �ﰢ��
		else if (isChkTriangle) {
			send(g_sock, (char*)&g_shapemsg, sizeof(SHAPE_MSG), 0);

			// ���콺 Ŭ�� ��ǥ ����
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
		// ȭ�� ��¿� DC�� Pen �ڵ� ���
		hDC = GetDC(hWnd);
		hPen = CreatePen(PS_SOLID, 3, g_drawcolor);
		// ������ ȭ�鿡 1���� �׸���
		hOldPen = (HPEN)SelectObject(hDC, hPen);
		MoveToEx(hDC, LOWORD(wParam), HIWORD(wParam), NULL);
		LineTo(hDC, LOWORD(lParam), HIWORD(lParam));
		SelectObject(hDC, hOldPen);
		// ��� ��Ʈ�ʿ� 2���� �׸���
		hOldPen = (HPEN)SelectObject(hDCMem, hPen);
		MoveToEx(hDCMem, LOWORD(wParam), HIWORD(wParam), NULL);
		LineTo(hDCMem, LOWORD(lParam), HIWORD(lParam));
		SelectObject(hDCMem, hOldPen);
		// ȭ�� ��¿� DC�� Pen �ڵ� ����
		DeleteObject(hPen);
		ReleaseDC(hWnd, hDC);
		return 0;
	case WM_DRAWSHAPE:

		// ȭ�� ��¿� DC�� Pen �ڵ� ���
		hDC = GetDC(hWnd);
		hPen = CreatePen(PS_SOLID, 3, g_shapecolor);
		hOldPen = (HPEN)SelectObject(hDC, hPen);
		if (g_shape == SHAPE_CIRCLE) {
			// �� �׸���
			DisplayText("[���� �׸���] ��\r\n");
			Ellipse(hDC, g_shapemsg.x0, g_shapemsg.y0, g_shapemsg.x1, g_shapemsg.y1);
			// ��� ��Ʈ�ʿ� 2���� �׸���
			hOldPen = (HPEN)SelectObject(hDCMem, hPen);
			Ellipse(hDCMem, g_shapemsg.x0, g_shapemsg.y0, g_shapemsg.x1, g_shapemsg.y1);
			SelectObject(hDCMem, hOldPen);

			SendMessage(g_hBtnCircle, BM_SETCHECK, BST_UNCHECKED, 0);
		}
		else if (g_shape == SHAPE_RECTANGLE) {
			// �簢�� �׸���
			Rectangle(hDC, g_shapemsg.x0, g_shapemsg.y0, g_shapemsg.x1, g_shapemsg.y1);
			DisplayText("[���� �׸���] �簢��\r\n");

			SendMessage(g_hBtnRectangle, BM_SETCHECK, BST_UNCHECKED, 0);
		}
		else if (g_shape == SHAPE_TRIANGLE) {
			// �ﰢ�� �׸���
			POINT points[3]; // �ﰢ���� �̷�� �� ���� ��ǥ�� �����ϴ� �迭

			// �� ���� ��ǥ ����
			points[0].x = g_shapemsg.x0;
			points[0].y = g_shapemsg.y0;
			points[1].x = (g_shapemsg.x0 + g_shapemsg.x1) / 2; // �߰� ����
			points[1].y = g_shapemsg.y1;
			points[2].x = g_shapemsg.x1;
			points[2].y = g_shapemsg.y0;

			Polygon(hDC, points, 3);

			DisplayText("[���� �׸���] �ﰢ��\r\n");
		}
		DeleteObject(hPen);
		ReleaseDC(hWnd, hDC);

		return 0;
	case WM_ERASEPIC:
		// ��� ��Ʈ�� ������� ä��
		SelectObject(hDCMem, GetStockObject(WHITE_BRUSH));
		SelectObject(hDCMem, GetStockObject(WHITE_PEN));
		Rectangle(hDCMem, 0, 0, cx, cy);
		// WM_PAINT �޽��� ���� ����
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

// ���� ��� ������ �Լ�(1) - ����
DWORD WINAPI ClientMain(LPVOID arg)
{
	int retval;

	if (g_isIPv6 == false && g_isUDP == false) { // TCP/IPv4 ����
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
	else if (g_isIPv6 == true && g_isUDP == false) { // TCP/IPv6 ����
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
	else if (g_isIPv6 == false && g_isUDP == true) { // UDP/IPv4 ����
		MessageBox(NULL, _T("���� �������� �ʾҽ��ϴ�."), _T("�˸�"), MB_ICONERROR);
		exit(1);
	}
	else if (g_isIPv6 == true && g_isUDP == true) { // UDP/IPv6 ����
		MessageBox(NULL, _T("���� �������� �ʾҽ��ϴ�."), _T("�˸�"), MB_ICONERROR);
		exit(1);
	}
	MessageBox(NULL, _T("������ �����߽��ϴ�."), _T("�˸�"), MB_ICONINFORMATION);

	// �б� & ���� ������ ����
	HANDLE hThread[2];
	hThread[0] = CreateThread(NULL, 0, ReadThread, NULL, 0, NULL);
	hThread[1] = CreateThread(NULL, 0, WriteThread, NULL, 0, NULL);
	if (hThread[0] == NULL || hThread[1] == NULL) exit(1);
	g_bCommStarted = true;

	// ������ ���� ��� (�� �� �ϳ��� ������ ������)
	retval = WaitForMultipleObjects(2, hThread, FALSE, INFINITE);
	retval -= WAIT_OBJECT_0;
	if (retval == 0)
		TerminateThread(hThread[1], 1);
	else
		TerminateThread(hThread[0], 1);
	CloseHandle(hThread[0]);
	CloseHandle(hThread[1]);

	MessageBox(NULL, _T("������ ������ϴ�."), _T("�˸�"), MB_ICONERROR);
	EnableWindow(g_hBtnSendFile, FALSE);
	EnableWindow(g_hBtnSendMsg, FALSE);
	EnableWindow(g_hBtnErasePic, FALSE);
	g_bCommStarted = false;
	closesocket(g_sock);
	return 0;
}

// ���� ��� ������ �Լ�(2) - ������ ����
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

// ���� ��� ������ �Լ�(3) - ������ �۽�
DWORD WINAPI WriteThread(LPVOID arg)
{
	int retval;

	// ������ ������ ���
	while (1) {
		// ���� �Ϸ� ��ٸ���
		WaitForSingleObject(g_hWriteEvent, INFINITE);
		// ���ڿ� ���̰� 0�̸� ������ ����
		if (strlen(g_chatmsg.msg) == 0) {
			// [�޽��� ����] ��ư Ȱ��ȭ
			EnableWindow(g_hBtnSendMsg, TRUE);
			// �б� �Ϸ� �˸���
			SetEvent(g_hReadEvent);
			continue;
		}
		// ������ ������
		retval = send(g_sock, (char *)&g_chatmsg, SIZE_TOT, 0);
		if (retval == SOCKET_ERROR) break;
		// [�޽��� ����] ��ư Ȱ��ȭ
		EnableWindow(g_hBtnSendMsg, TRUE);
		// �б� �Ϸ� �˸���
		SetEvent(g_hReadEvent);
	}
	return 0;
}

// ����Ʈ ��Ʈ�� ��� �Լ�
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