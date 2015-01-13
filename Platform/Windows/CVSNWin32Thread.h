#ifndef CVSNWin32Thread_h
#define CVSNWin32Thread_h

#include <Windows.h>
#include <setjmp.h>
#include "CVSNThread.h"

class CVSNWin32Thread : public CVSNThread
{
public:
	CVSNWin32Thread();
	virtual ~CVSNWin32Thread();

	bool init();

	void	*	Create(int(*thread_func)(void * hThread, void * data), void * data);
	void		Exit(void * hThread, int status);
	bool		Watch(void * hThread, int * status);
	void		Delete(void * hThread);
	void		Break(void * hThread);

private:
	struct PF_THREAD {
		jmp_buf jmp;
		HANDLE	hWin32Thread;
		DWORD	idWin32Thread;
		int (*thread_func)(void *, void *);
		void * data;
		int result;
	};

	static DWORD WINAPI ThreadProc(LPVOID lpParam);
};

#endif // CVSNWin32Thread_h
