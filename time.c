#include <windows.h>
#include <stdio.h>

// �۷ι� ����
HINSTANCE g_hInst;
HWND hEdit, hStartBtn, hStopBtn, hStatus;
UINT_PTR timerID;
unsigned int timerSeconds, originalTimerSeconds;

// �Լ� ����
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Ÿ�̸� �ݹ� �Լ�
VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
    if (timerSeconds > 0) {
        timerSeconds--;
        char timeText[32];
        sprintf(timeText, "�����ð�: %u ��", timerSeconds);
        SetWindowText(hStatus, timeText);
    } else {
        Beep(500, 500);  // 500 Hz�� ���ļ��� 500ms ���� ������ �߻�
        timerSeconds = originalTimerSeconds;  // �ʱ� �ð����� Ÿ�̸� �缳��
        SetTimer(hwnd, 1, 1000, TimerProc);  // Ÿ�̸� �����
    }
}

// WinMain �Լ�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    g_hInst = hInstance;
    MSG msg;
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);  // ������ �Ͼ������ ����
    wc.lpszClassName = "TimerApp";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClass(&wc);

    HWND hwnd = CreateWindow(wc.lpszClassName, "Ÿ�̸�", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 300, 200, NULL, NULL, hInstance, NULL);
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}

// ���� ������ ���ν���
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
        hEdit = CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER, 50, 50, 200, 20, hwnd, NULL, g_hInst, NULL);
        hStartBtn = CreateWindow("BUTTON", "Start", WS_CHILD | WS_VISIBLE, 50, 80, 75, 30, hwnd, (HMENU)1, g_hInst, NULL);
        hStopBtn = CreateWindow("BUTTON", "Stop", WS_CHILD | WS_VISIBLE, 175, 80, 75, 30, hwnd, (HMENU)2, g_hInst, NULL);
        hStatus = CreateWindow("STATIC", "", WS_CHILD | WS_VISIBLE, 50, 120, 200, 20, hwnd, NULL, g_hInst, NULL);
        break;
    case WM_COMMAND:
        if (LOWORD(wParam) == 1) {  // Start ��ư Ŭ��
            char buffer[256];
            GetWindowText(hEdit, buffer, 256);
            originalTimerSeconds = atoi(buffer);
            timerSeconds = originalTimerSeconds;
            timerID = SetTimer(hwnd, 1, 1000, TimerProc);
        } else if (LOWORD(wParam) == 2) {  // Stop ��ư Ŭ��
            KillTimer(hwnd, timerID);
            SetWindowText(hStatus, "Ÿ�̸� ����");
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}