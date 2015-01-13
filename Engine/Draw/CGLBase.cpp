#include <stdio.h>
#include "CGLBase.h"
#include "CGLAlloc.h"

CGLException::CGLException(const char * const msg) : m_msg(msg) {}
const char *
CGLException::what() const throw() { return m_msg; }

CGLBase::CGLBase() {}
CGLBase::~CGLBase() {}

void *	CGLBase::operator new(size_t size)		{	return CGLAlloc::malloc(size);	}
void *	CGLBase::operator new[](size_t size)	{	return CGLAlloc::malloc(size);	}
void	CGLBase::operator delete(void * p)		{	CGLAlloc::free(p);				}
void	CGLBase::operator delete[](void *p)		{	CGLAlloc::free(p);				}
