#ifndef CVSNTrackTest_h
#define CVSNTrackTest_h


#include "CVSNScriptTask.h"
#include "CVSNTracking.h"
#include "C3DDrawEnv.h"

class CVSNTrackTest : public CVSNScriptTask
{
public:
	CVSNTrackTest();
	virtual ~CVSNTrackTest();

	bool init(CVSNScriptArgs& args);
	void update(int deltaT);
	void die();

	void on_change();

private:
	CVSNTracking	*	m_track;
	C3DDrawEnv		*	m_env;
};

#endif // CVSNTrackTest_h
