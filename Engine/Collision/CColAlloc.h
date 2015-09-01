#ifndef CColAlloc_h
#define CColAlloc_h

#include <stdio.h>
#include <iostream>

class CColAlloc
{
private:
	static void * (*m_malloc)(size_t size);
	static void(*m_free)(void *p);
public:

	static void setAllocator(void * (*func_malloc)(size_t size), void(*func_free)(void *));

	static void	*	malloc(size_t size);
	static void		free(void * p);
};


#include "PlatformUtil.h"
#define CColLog(msg)   LOG(msg)

#endif // CColAlloc_h
