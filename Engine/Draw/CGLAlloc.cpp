#include <malloc.h>
#include <exception>
#include "CGLAlloc.h"

void * (*CGLAlloc::m_malloc)(size_t size) = malloc;
void   (* CGLAlloc::m_free)(void *p) = free;

void
CGLAlloc::setAllocator(void * (*func_malloc)(size_t size), void(*func_free)(void *))
{
	m_malloc = func_malloc;
	m_free = func_free;
}

void *
CGLAlloc::malloc(size_t size)
{
	void * p = m_malloc(size);
	if (!p) throw std::bad_alloc();
	return p;
}

void	CGLAlloc::free(void * p)
{
	m_free(p);
}