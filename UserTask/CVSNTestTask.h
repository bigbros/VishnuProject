#ifndef CVSNTestTask_h
#define CVSNTestTask_h

#include "CVSNScriptTask.h"
#include "c3dgles.h"
#include "CVSN3DKVModel.h"
#include "CVSNTouchPanel.h"

class CVSNTestTask : public CVSNScriptTask
{
public:
	CVSNTestTask();
	virtual ~CVSNTestTask();

	bool init(CVSNScriptArgs& args);

	void update(int deltaT);
	bool command(CVSNScriptArgs& args, CVSNScriptArgs& ret);

	void die();

	void on_change();


private:
	CVSN3DKVModel	*	m_model;
	C3DCharacter	*	m_chara;

	bool				m_anime;

	int					m_width;
	int					m_height;

	int					m_preX;
	int					m_preY;
	C3DVec				m_preRot;
};


#endif // CVSNTestTask_h
