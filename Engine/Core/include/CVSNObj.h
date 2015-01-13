#ifndef CVSNObj_h
#define CVSNObj_h

#include <stddef.h>
#include <exception>

class CVSNException : public std::exception
{
private:
	const char *		m_msg;
public:
	CVSNException(const char * const msg);
	const char * what() const throw();
};

class CVSNObj
{
public:
	CVSNObj();
	virtual ~CVSNObj();

	static void * operator new(size_t size);
	static void * operator new[](size_t size);
	static void operator delete(void * p);
	static void operator delete[](void * p);
};

#endif // CVSNObj_h
