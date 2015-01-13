#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "CVSNEngineHeap.h"

#define BLOCKSIZE(size) (16 * (((size) + 15) / 16))

extern "C" {
	void * vsnMemAlloc(size_t size)
	{
		return CVSNEngineHeap::getInstance().malloc(size);
	}

	void * vsnMemReAlloc(void * p, size_t size)
	{
		return CVSNEngineHeap::getInstance().realloc(p, size);
	}

	void vsnMemFree(void * p)
	{
		return CVSNEngineHeap::getInstance().free(p);
	}
}

CVSNEngineHeap::CVSNEngineHeap()
: m_heapSize(0)
, m_heapBuffer(0)
, m_pFirstBlock(0)
{
}

CVSNEngineHeap::~CVSNEngineHeap() {}

CVSNEngineHeap&
CVSNEngineHeap::getInstance()
{
	static CVSNEngineHeap instance;
	return instance;
}

void
CVSNEngineHeap::setHeapArea(void * pArea, size_t size)
{
	m_heapBuffer = (char *)pArea;
	m_heapSize = 16 * (size / 16);

	m_pFirstBlock = 0;
}

void *
CVSNEngineHeap::malloc(size_t size)
{
	size_t allocSize = BLOCKSIZE(size);

	if (!allocSize) {
		return 0;
	}
	MEMINFO * info = 0;

	do {
		MEMINFO * next = (info) ? info->next : m_pFirstBlock;
		char * infoTail = (info) ? ((char *)info + (INFOSIZE + info->size)) : m_heapBuffer;
		char * nextInfo = (next) ? (char *)next : (m_heapBuffer + m_heapSize);
		size_t gapSize = nextInfo - infoTail;
		if (gapSize >= (allocSize + INFOSIZE)) {
			MEMINFO * newInfo = (MEMINFO *)infoTail;
			newInfo->prev = info;
			newInfo->next = (info) ? info->next : 0;
			newInfo->size = allocSize;
			if (info) {
				if (info->next) info->next->prev = newInfo;
				info->next = newInfo;
			}
			else {
				m_pFirstBlock = newInfo;
			}
			void * allocPointer = (void *)((char *)newInfo + INFOSIZE);

			return allocPointer;	// allocated.

		}
		info = (info) ? info->next: m_pFirstBlock;
	} while (info);

	return 0;
}

void *
CVSNEngineHeap::realloc(void * ptr, size_t size)
{
	if (!ptr) return this->malloc(size);

	size_t allocSize = BLOCKSIZE(size);
		
	MEMINFO * info = (MEMINFO *)((char *)ptr - INFOSIZE);
	char * nextInfo = (info->next) ? (char *)info->next : (m_heapBuffer + m_heapSize);
	size_t maxSize = nextInfo - (char *)ptr;

	if (maxSize < allocSize) {
		void * newPtr = this->malloc(size);
		if (newPtr) {
			memcpy(newPtr, ptr, info->size);
			this->free(ptr);
		}
		return newPtr;
	}

	info->size = allocSize;
	return ptr;
}

void
CVSNEngineHeap::free(void * ptr)
{
	if (!ptr) return;

	MEMINFO * info = (MEMINFO *)((char *)ptr - INFOSIZE);
	if (info->prev) {
		info->prev->next = info->next;
	}
	else {
		m_pFirstBlock = info->next;
	}
	if (info->next) {
		info->next->prev = info->prev;
	}
}
