#include <windows.h>
#include <stdio.h>

#define MAXCNT 1000000
int g_count = 0;

HANDLE hReadEvent;
HANDLE hWriteEvent;

DWORD WINAPI MyThread1(LPVOID arg)
{
	for(int i=0; i<MAXCNT; i++){
		WaitForSingleObject(hReadEvent, INFINITE);
		g_count += 2;
		SetEvent(hWriteEvent);
	}
	return 0;
}

DWORD WINAPI MyThread2(LPVOID arg)
{
	for(int i=0; i<MAXCNT; i++){
		WaitForSingleObject(hWriteEvent, INFINITE);
		g_count -= 2;  
		SetEvent(hReadEvent);
	}
	return 0;
}

int main(int argc, char *argv[])
{
	// 두 개의 자동 리셋 이벤트 생성(각각 비신호, 신호 상태)
	hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if(hWriteEvent == NULL) return 1;
	hReadEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	if(hReadEvent == NULL) return 1;
	
	// 두 개의 스레드 생성
	HANDLE hThread[2];
	hThread[0] = CreateThread(NULL, 0, MyThread1, NULL, 0, NULL);
	hThread[1] = CreateThread(NULL, 0, MyThread2, NULL, 0, NULL);
	
	// 두 개의 스레드 종료 대기
	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);
	
	// 이벤트 제거
	CloseHandle(hWriteEvent);
	CloseHandle(hReadEvent);

	// 결과 출력
	printf("g_count = %d\n", g_count);  // 결과 출력을 추가하였습니다.
	
	return 0;
}
