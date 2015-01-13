#ifndef CVSNPthread_h
#define CVSNPthread_h

#include <pthread.h>
#include <setjmp.h>
#include "CVSNThread.h"

class CVSNPthread : public CVSNThread
{
public:
	CVSNPthread();
	virtual ~CVSNPthread();

	bool init();

	void	*	Create(int(*thread_func)(void * hThread, void * data), void * data);
	void		Exit(void * hThread, int status);
	bool		Watch(void * hThread, int * status);
	void		Delete(void * hThread);
	void		Break(void * hThread);

private:
	struct PF_THREAD {
		jmp_buf		jmp;
		pthread_t	id;
		int (*thread_func)(void *, void *);
		void * data;
		int result;
	};

	static void * ThreadProc(void * data);
};


#endif // CVSNPthread_h
