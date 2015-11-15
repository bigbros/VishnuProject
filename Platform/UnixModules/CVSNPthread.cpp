#include <signal.h>
#include <pthread.h>
#include <setjmp.h>
#include <errno.h>
#include <exception>

#include "CVSNPthread.h"

CVSNPthread::CVSNPthread() : CVSNThread() {}
CVSNPthread::~CVSNPthread() {}


bool
CVSNPthread::init()
{
	return true;
}


void *
CVSNPthread::Create(int(*thread_func)(void * hThread, void * data), void * data)
{
	PF_THREAD * thread = new PF_THREAD;
	if(!thread) return 0;
	thread->data = data;
	thread->thread_func = thread_func;
	thread->result = 0;
	pthread_create(&(thread->id), 0, CVSNPthread::ThreadProc, thread);
	return thread;
}

void
CVSNPthread::Exit(void * hThread, int status)
{
	PF_THREAD * pThread = (PF_THREAD *)hThread;
	longjmp(pThread->jmp, status);
}

bool
CVSNPthread::Watch(void * hThread, int * status)
{
	PF_THREAD * pThread = (PF_THREAD *)hThread;
	if(pthread_kill(pThread->id, 0) != ESRCH) {
		return true;
	}
	*status = pThread->result;
	return false;
}

void
CVSNPthread::Delete(void * hThread)
{
	PF_THREAD * pThread = (PF_THREAD *)hThread;
	pthread_join(pThread->id, 0);
	delete pThread;
}

void
CVSNPthread::Break(void * hThread)
{
	PF_THREAD * pThread = (PF_THREAD *)hThread;
	pthread_kill(pThread->id, SIGKILL);
}

void *
CVSNPthread::ThreadProc(void * data)
{
	PF_THREAD * pThread = (PF_THREAD *)data;
	if(!(pThread->result = setjmp(pThread->jmp))) {
		pThread->result = (pThread->thread_func)(pThread, pThread->data);
	}
	return 0;
}
