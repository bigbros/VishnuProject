#ifndef CVSN3DLightVecFunc_h
#define CVSN3DLightVecFunc_h

#include "IVSNScriptUtilFunc.h"


class CVSN3DLightVecFunc : public IVSNScriptUtilFunc
{
public:
	CVSN3DLightVecFunc();
	virtual ~CVSN3DLightVecFunc();

	bool utilFunc(CVSNScriptArgs& args, CVSNScriptArgs& ret);
};

class CVSN3DLightColFunc : public IVSNScriptUtilFunc
{
public:
	CVSN3DLightColFunc();
	virtual ~CVSN3DLightColFunc();

	bool utilFunc(CVSNScriptArgs& args, CVSNScriptArgs& ret);
};

class CVSN3DAmbientFunc : public IVSNScriptUtilFunc
{
public:
	CVSN3DAmbientFunc();
	virtual ~CVSN3DAmbientFunc();

	bool utilFunc(CVSNScriptArgs& args, CVSNScriptArgs& ret);
};


#endif // CVSN3DLightVecFunc_h
