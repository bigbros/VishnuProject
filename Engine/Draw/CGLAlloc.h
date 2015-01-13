#ifndef CGLAlloc_h
#define CGLAlloc_h

#include <stdio.h>
#include <iostream>

class CGLAlloc
{
private:
	static void * (*m_malloc)(size_t size);
	static void   (*m_free)(void *p);
public:

	static void setAllocator(void * (*func_malloc)(size_t size), void(*func_free)(void *));

	static void	*	malloc(size_t size);
	static void		free(void * p);
};


#include "PlatformUtil.h"
#define CGLLog(msg)   LOG(msg)

#endif // CGLAlloc_h
