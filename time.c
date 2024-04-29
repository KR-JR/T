#include <windows.h>
#include <stdio.h>

// 글로벌 변수
HINSTANCE g_hInst;
HWND hEdit, hStartBtn, hStopBtn, hStatus;
UINT_PTR timerID;
unsigned int timerSeconds, originalTimerSeconds;

// 함수 선언
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// 타이머 콜백 함수
VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
    if (timerSeconds > 0) {
        timerSeconds--;
        char timeText[32];
        sprintf(timeText, "남은시간: %u 초", timerSeconds);
        SetWindowText(hStatus, timeText);
    } else {
        Beep(500, 500);  // 500 Hz의 주파수로 500ms 동안 비프음 발생
        timerSeconds = originalTimerSeconds;  // 초기 시간으로 타이머 재설정
        SetTimer(hwnd, 1, 1000, TimerProc);  // 타이머 재시작
    }
}

// WinMain 함수
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    g_hInst = hInstance;
    MSG msg;
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);  // 배경색을 하얀색으로 설정
    wc.lpszClassName = "TimerApp";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClass(&wc);

    HWND hwnd = CreateWindow(wc.lpszClassName, "타이머", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 300, 200, NULL, NULL, hInstance, NULL);
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}

// 메인 윈도우 프로시저
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
        hEdit = CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER, 50, 50, 200, 20, hwnd, NULL, g_hInst, NULL);
        hStartBtn = CreateWindow("BUTTON", "Start", WS_CHILD | WS_VISIBLE, 50, 80, 75, 30, hwnd, (HMENU)1, g_hInst, NULL);
        hStopBtn = CreateWindow("BUTTON", "Stop", WS_CHILD | WS_VISIBLE, 175, 80, 75, 30, hwnd, (HMENU)2, g_hInst, NULL);
        hStatus = CreateWindow("STATIC", "", WS_CHILD | WS_VISIBLE, 50, 120, 200, 20, hwnd, NULL, g_hInst, NULL);
        break;
    case WM_COMMAND:
        if (LOWORD(wParam) == 1) {  // Start 버튼 클릭
            char buffer[256];
            GetWindowText(hEdit, buffer, 256);
            originalTimerSeconds = atoi(buffer);
            timerSeconds = originalTimerSeconds;
            timerID = SetTimer(hwnd, 1, 1000, TimerProc);
        } else if (LOWORD(wParam) == 2) {  // Stop 버튼 클릭
            KillTimer(hwnd, timerID);
            SetWindowText(hStatus, "타이머 정지");
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