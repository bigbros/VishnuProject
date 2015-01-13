#ifndef CTestEnv_h
#define CTestEnv_h

#include "CVSNGameEnvironment.h"

class CTestEnv : public CVSNGameEnvironment
{
public:
	CTestEnv(void * pHeapBuffer, size_t sizeHeap, int width, int height);
	virtual ~CTestEnv();
	
protected:
	CVSNScript * createScriptSystem();
	bool initUserTask();
};

#endif // CTestEnv_h
