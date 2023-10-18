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
	// �� ���� �ڵ� ���� �̺�Ʈ ����(���� ���ȣ, ��ȣ ����)
	hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if(hWriteEvent == NULL) return 1;
	hReadEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	if(hReadEvent == NULL) return 1;
	
	// �� ���� ������ ����
	HANDLE hThread[2];
	hThread[0] = CreateThread(NULL, 0, MyThread1, NULL, 0, NULL);
	hThread[1] = CreateThread(NULL, 0, MyThread2, NULL, 0, NULL);
	
	// �� ���� ������ ���� ���
	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);
	
	// �̺�Ʈ ����
	CloseHandle(hWriteEvent);
	CloseHandle(hReadEvent);

	// ��� ���
	printf("g_count = %d\n", g_count);  // ��� ����� �߰��Ͽ����ϴ�.
	
	return 0;
}
