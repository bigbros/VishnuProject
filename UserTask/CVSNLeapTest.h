#ifndef CVSNLeapTest_h
#define CVSNLeapTest_h


#include "CVSNScriptTask.h"
#include "CVSNLeapMotion.h"
#include "C3DDrawEnv.h"
#include "c3dgles.h"
#include "CVSN3DKVModel.h"

class CVSNLeapTest : public CVSNScriptTask
{
public:
	CVSNLeapTest();
	virtual ~CVSNLeapTest();

	bool init(CVSNScriptArgs& args);
	void update(int deltaT);
	void die();

	void on_change();

private:
	CVSNLeapMotion	*	m_leap;

	CVSN3DKVModel	*	m_model;

	C3DCharacter	*	m_right;
	C3DCharacter	*	m_left;
	C3DDrawEnv		*	m_env;
};

#endif // CVSNLeapTest_h
