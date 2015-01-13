#ifndef CGLObj_h
#define CGLObj_h

#include "CGLBase.h"

class CGLObj : public CGLBase
{
	friend class CGLEnv;
public:
	CGLObj();
	virtual ~CGLObj();

	virtual bool recovery() = 0;
	virtual bool destruction() = 0;
private:
	CGLObj	*	m_prev;
	CGLObj	*	m_next;
};

#endif // C3DGLObj_h
