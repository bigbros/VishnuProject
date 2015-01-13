#include <iostream>
#include "CVSNObj.h"
#include "CVSNEngineHeap.h"

CVSNException::CVSNException(const char * const msg) : m_msg(msg) {}
const char *
CVSNException::what() const throw() { return m_msg; }

CVSNObj::CVSNObj() {}
CVSNObj::~CVSNObj() {}

void *
CVSNObj::operator new(std::size_t size)
{
	void * p = CVSNEngineHeap::getInstance().malloc(size);
	if (!p) throw std::bad_alloc();
	return p;
}

void
CVSNObj::operator delete(void * p)
{
	CVSNEngineHeap::getInstance().free(p);
}

void *
CVSNObj::operator new[](std::size_t size)
{
	void * p = CVSNEngineHeap::getInstance().malloc(size);
	if (!p) throw std::bad_alloc();
	return p;
}

void
CVSNObj::operator delete[](void * p)
{
	CVSNEngineHeap::getInstance().free(p);
}
