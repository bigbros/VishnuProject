#ifndef CVSNModelTest_h
#define CVSNModelTest_h

#include "CVSNScriptTask.h"
#include "c3dgles.h"
#include "CVSN3DVMDModel.h"
#include "CVSNTouchPanel.h"

class CVSNModelTest : public CVSNScriptTask
{
public:
	CVSNModelTest();
	virtual ~CVSNModelTest();

	bool init(CVSNScriptArgs& args);
	void update(int deltaT);
	void die();

	void on_change();


private:
	CVSN3DVMDModel	*	m_model;
	C3DCharacter	*	m_chara;
	C3DNullObj		*	m_null;

	bool				m_anime;

	int					m_width;
	int					m_height;

	int					m_preX;
	int					m_preY;
	C3DVec				m_preRot;

};


#endif // CVSNModelTest_h
