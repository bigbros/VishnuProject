#ifndef CVSNBoneTest_h
#define CVSNBoneTest_h

#include "CVSNScriptTask.h"
#include "c3dgles.h"
#include "CVSN3DKVModel.h"
#include "CVSNTouchPanel.h"

class CVSNBoneTest : public CVSNScriptTask
{
public:
	CVSNBoneTest();
	virtual ~CVSNBoneTest();

	bool init(CVSNScriptArgs& args);
	void update(int deltaT);
	void die();

	void on_change();


private:
	static CVSN3DKVModel	*	m_model;
	static int					m_model_use;
	C3DCharacter	*	m_chara;
	C3DNullObj		*	m_null;

	bool				m_anime;

	int					m_width;
	int					m_height;

	int					m_preX;
	int					m_preY;
	C3DVec				m_preRot;
};


#endif // CVSNBoneTask_h
