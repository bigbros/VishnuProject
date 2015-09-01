#ifndef CColBase_h
#define CColBase_h


#include <stdio.h>
#include <exception>

#include "CColAlloc.h"

class CColException : public std::exception
{
private:
	const char * m_msg;
public:
	CColException(const char * msg);
	const char * what() const throw();
};

class CColBase
{
public:
	CColBase();
	virtual ~CColBase();

	// �A���P�[�^�̓G���W�����\�[�X�̈�Ɋ��蓖�Ă���̂��g�������̂ŁA
	// �z�u new/delete ����������B
	void * operator new(size_t size);
	void operator delete(void * p);
	void * operator new[](size_t size);
	void operator delete[](void *p);
};


#endif // CColBase_h
