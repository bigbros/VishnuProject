#ifndef CVSNThread_h
#define CVSNThread_h

#include "CVSNPlatform.h"

class CVSNThread : public IVSNModule
{
public:
	CVSNThread();
	virtual ~CVSNThread();

	virtual void	*	Create(int(*thread_func)(void * hThread, void * data), void * data) = 0;
	virtual void		Exit(void * hThread, int status) = 0;
	virtual bool		Watch(void * hThread, int * status) = 0;
	virtual void		Delete(void * hThread) = 0;
	virtual void		Break(void * hThread) = 0;
};

#endif // CVSNThread_h
