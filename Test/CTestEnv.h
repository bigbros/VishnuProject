#ifndef CTestEnv_h
#define CTestEnv_h

#include "CVSNGameEnvironment.h"

class CTestEnv : public CVSNGameEnvironment
{
public:
	CTestEnv(void * pHeapBuffer, size_t sizeHeap, int width, int height, int is_vr);
	virtual ~CTestEnv();
	
protected:
	CVSNScript * createScriptSystem();
	bool initUserTask();
	CGLFBO * createFBO();

	bool	m_is_vr;
};
#endif // CTestEnv_h
