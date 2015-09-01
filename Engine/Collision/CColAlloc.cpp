#include <malloc.h>
#include <exception>
#include "CColAlloc.h"

void * (*CColAlloc::m_malloc)(size_t size) = malloc;
void(*CColAlloc::m_free)(void *p) = free;

void
CColAlloc::setAllocator(void * (*func_malloc)(size_t size), void(*func_free)(void *))
{
	m_malloc = func_malloc;
	m_free = func_free;
}

void *
CColAlloc::malloc(size_t size)
{
	void * p = m_malloc(size);
	if (!p) throw std::bad_alloc();
	return p;
}

void	CColAlloc::free(void * p)
{
	m_free(p);
}