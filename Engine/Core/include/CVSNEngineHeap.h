#ifndef CVSNEngineHeap_h
#define CVSNEngineHeap_h

#include <stddef.h>

class CVSNEngineHeap
{
private:
	size_t			m_heapSize;
	char		*	m_heapBuffer;

private:
	CVSNEngineHeap();
	~CVSNEngineHeap();	// singleton. could not extend.

public:
	static CVSNEngineHeap& getInstance();

	void		setHeapArea(void * pArea, size_t size);
	void	*	malloc(size_t size);
	void	*	realloc(void * ptr, size_t size);
	void		free(void * ptr);
private:
	struct MEMINFO {
		MEMINFO		*	prev;
		MEMINFO		*	next;
		size_t			size;
	};

	MEMINFO		*	m_pFirstBlock;

	enum {
		INFOSIZE = 16 * ((sizeof(MEMINFO)+15) / 16)
	};
};

extern "C" {
	void * vsnMemAlloc(size_t size);
	void * vsnMemReAlloc(void * p, size_t size);
	void   vsnMemFree(void * p);
}

#endif // CVSNEngineHeap_h
