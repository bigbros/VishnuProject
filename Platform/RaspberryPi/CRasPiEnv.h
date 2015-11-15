#ifndef CRasPiEnv_h
#define CRasPiEnv_h

#include "CVSNGameEnvironment.h"

class CRasPiEnv : public CVSNGameEnvironment
{
public:
	CRasPiEnv(void * pHeapBuffer, size_t sizeHeap, int width, int height, int is_vr);
	virtual ~CRasPiEnv();
	
protected:
	CVSNScript * createScriptSystem();
	bool initUserTask();
	bool initDraw3D();
	CGLFBO * createFBO();

	bool	m_is_vr;
};
#endif // CRasPiEnv_h
