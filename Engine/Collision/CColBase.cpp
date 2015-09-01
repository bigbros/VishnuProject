#include <stdio.h>
#include "CColBase.h"
#include "CColAlloc.h"

CColException::CColException(const char * const msg) : m_msg(msg) {}
const char *
CColException::what() const throw() { return m_msg; }

CColBase::CColBase() {}
CColBase::~CColBase() {}

void *	CColBase::operator new(size_t size)		{	return CColAlloc::malloc(size); }
void *	CColBase::operator new[](size_t size)	{	return CColAlloc::malloc(size);	}
void	CColBase::operator delete(void * p)		{	CColAlloc::free(p); }
void	CColBase::operator delete[](void *p)	{	CColAlloc::free(p);				}
