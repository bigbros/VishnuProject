#ifndef CVSNScriptDriver_h
#define CVSNScriptDriver_h

#include "CVSNTask.h"
#include "CVSNScript.h"

class CVSNScriptDriver : public CVSNTask
{
public:
	CVSNScriptDriver(CVSNScript * scriptSystem, const char * startupScript);
	virtual ~CVSNScriptDriver();

	void update(int deltaT);
	void die();

	void on_pause();
	void on_resume();
private:
	bool init(CVSNScript * scriptSystem, const char * startupScript);
	CVSNScript	*	m_scriptSystem;
	bool			m_leaved;
};


#endif // CVSNScriptDriver_h
