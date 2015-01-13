#include "CVSNWin32Thread.h"

CVSNWin32Thread::CVSNWin32Thread() {}
CVSNWin32Thread::~CVSNWin32Thread() {}

bool
CVSNWin32Thread::init()
{
	return true;
}
void *
CVSNWin32Thread::Create(int(*thread_func)(void * hThread, void * data), void * data)
{
	PF_THREAD * thread = new PF_THREAD;
	if (!thread) { return NULL; }

	thread->data = data;
	thread->thread_func = thread_func;
	thread->result = 0;

	thread->hWin32Thread = CreateThread(
		NULL, 0, CVSNWin32Thread::ThreadProc, thread, 0, &(thread->idWin32Thread));

	return thread;
}

void
CVSNWin32Thread::Exit(void * hThread, int status)
{
	PF_THREAD * pThread = (PF_THREAD *)hThread;
	longjmp(pThread->jmp, status);
}

bool
CVSNWin32Thread::Watch(void * hThread, int * status)
{
	PF_THREAD * pThread = (PF_THREAD *)hThread;
	DWORD dwParam;
	GetExitCodeThread(pThread->hWin32Thread, &dwParam);
	if (dwParam == STILL_ACTIVE) {
		return true;
	}
	*status = pThread->result;
	return false;
}

void
CVSNWin32Thread::Delete(void * hThread)
{
	PF_THREAD * pThread = (PF_THREAD *)hThread;
	CloseHandle(pThread->hWin32Thread);
	delete pThread;
}

void
CVSNWin32Thread::Break(void * hThread)
{
	PF_THREAD * pThread = (PF_THREAD *)hThread;
	TerminateThread(pThread->hWin32Thread, 0);
}

DWORD WINAPI
CVSNWin32Thread::ThreadProc(LPVOID lpParam)
{
	PF_THREAD * pThread = (PF_THREAD *)lpParam;
	if (!(pThread->result = setjmp(pThread->jmp))) {
		pThread->result = (pThread->thread_func)(pThread, pThread->data);
	}
	ExitThread(TRUE);
}
