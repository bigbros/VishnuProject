#ifndef C3DCameraObj_h
#define C3DCameraObj_h

#include "C3DObj.h"

class C3DDrawEnv;

class C3DCameraObj : public C3DObj
{
	friend class C3DDrawEnv;
private:
	bool		m_isActive;
	C3DMat		m_cameraInvert;
	C3DMat		m_view;

	C3DObj	*	m_lookAt;

public:
	C3DCameraObj();
	virtual ~C3DCameraObj();

	bool calcProcedure(bool is_recalc);
	void render();

	void setView(float angle, float aspect, float clip_near, float clip_far);
	void lookAt(C3DObj * pObj = 0) { m_lookAt = pObj; }

	void lookAtTarget();
};

#endif // C3DCameraObj_h
