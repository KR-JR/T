#include <windows.h>
#include <thread>
#include <stdbool.h>
#include <chrono>

bool isRunning = false;
bool shouldStop = false;
int duration = 0;

void PerformActions() {
    while (!shouldStop) {
        if (isRunning) {
            int elapsed = 0;
            while (elapsed < duration * 1000 && !shouldStop) {
                if (elapsed >= (duration - 2) * 1000) {
                    break; // 남은 시간이 2초일 때 루프를 빠져나감
                }

                // Shift 키를 누름
                keybd_event(VK_SHIFT, 0, 0, 0);
                std::this_thread::sleep_for(std::chrono::milliseconds(50)); // 0.05초 간 누름
                keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
                std::this_thread::sleep_for(std::chrono::milliseconds(50)); // 나머지 0.45초 대기

                elapsed += 500; // 0.5초 추가
            }

            if (shouldStop) {
                break;
            }

            // 남은 시간이 1초일 때 Shift 누르는 동작을 멈추고 0.5초 대기
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));

            // 오른쪽 방향키를 누름
            keybd_event(VK_RIGHT, 0, 0, 0);
            std::this_thread::sleep_for(std::chrono::milliseconds(50)); // 0.05초 간 누름
            keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0);
            
            // 대기
            std::this_thread::sleep_for(std::chrono::milliseconds(200));

            // 왼쪽 방향키를 누름
            keybd_event(VK_LEFT, 0, 0, 0);
            std::this_thread::sleep_for(std::chrono::milliseconds(50)); // 0.05초 간 누름
            keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0);

            // 대기
            std::this_thread::sleep_for(std::chrono::milliseconds(200));

            // pagedown 누름
            keybd_event(VK_NEXT, 0, 0, 0);
            std::this_thread::sleep_for(std::chrono::milliseconds(50)); // 0.05초 간 누름
            keybd_event(VK_NEXT, 0, KEYEVENTF_KEYUP, 0);


        }

        // 주어진 시간이 끝난 후 잠시 대기한 후 반복
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static HWND hStartButton, hExitButton, hEditBox;

    switch (uMsg) {
    case WM_CREATE:
        // Start Button
        hStartButton = CreateWindow(
            "BUTTON",  
            "Start",     
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  
            50,        
            50,         
            100,        
            30,        
            hwnd,     
            (HMENU)1,       
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
            NULL);      

        // Exit Button
        hExitButton = CreateWindow(
            "BUTTON",  
            "Exit",     
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  
            200,        
            50,         
            100,        
            30,        
            hwnd,     
            (HMENU)2,       
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
            NULL);      

        // Edit Box
        hEditBox = CreateWindow(
            "EDIT",   
            "",       
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER,  
            50,        
            100,       
            250,       
            30,        
            hwnd,      
            (HMENU)3,  
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
            NULL);     
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == 1) {
            // Start button pressed
            if (!isRunning) {
                char buffer[256];
                GetWindowText(hEditBox, buffer, 256);
                duration = atoi(buffer);
                if (duration > 0) {
                    isRunning = true;
                    shouldStop = false;
                    std::thread actionThread(PerformActions);
                    actionThread.detach();
                }
            }
        } else if (LOWORD(wParam) == 2) {
            // Exit button pressed
            shouldStop = true;
            isRunning = false;
            PostQuitMessage(0);
        }
        break;

    case WM_DESTROY:
        shouldStop = true;
        isRunning = false;
        PostQuitMessage(0);
        return 0;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

int CALLBACK WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nCmdShow)
{
    const char CLASS_NAME[] = "Sample Window Class";

    WNDCLASS wc = {};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        WS_EX_TOPMOST,                   // Make window topmost
        CLASS_NAME,                     // Window class
        "Sample Window",                // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 370, 250, // Width 370px, Height 250px

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

