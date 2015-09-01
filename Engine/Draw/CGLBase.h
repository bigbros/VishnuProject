#ifndef CGLBase_h
#define CGLBase_h

#include <stdio.h>
#include <exception>
#include <GLES2/gl2.h>

class CGLException : public std::exception
{
private:
	const char * m_msg;
public:
	CGLException(const char * msg);
	const char * what() const throw();
};

class CGLBase
{
public:
	CGLBase();
	virtual ~CGLBase();

	// アロケータはエンジンリソース領域に割り当てるものを使いたいので、
	// 配置 new/delete を持たせる。
	void * operator new(size_t size);
	void operator delete(void * p);
	void * operator new[](size_t size);
	void operator delete[](void *p);
};


#endif // CGLBase_h
