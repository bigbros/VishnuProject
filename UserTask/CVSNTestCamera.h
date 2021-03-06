#ifndef CVSNTestCamera_h
#define CVSNTestCamera_h

#include "CVSNScriptTask.h"
#include "c3dgles.h"

class CVSNTestCamera : public CVSNScriptTask
{
public:
	CVSNTestCamera();
	virtual ~CVSNTestCamera();

	bool init(CVSNScriptArgs& args);
	void update(int deltaT);
	void die();

	void on_change();

private:
	// 指定された対角線視野角から水平視野角を計算する
	float calc_view(float angle);

	int					m_width;
	int					m_height;

	C3DCameraObj	*	m_camera;
	C3DObj			*	m_shaft;
	bool				m_anime;

	int					m_preX;
	int					m_preY;
	C3DVec				m_preRot;

	float				m_eyeAngle;
	float				m_eyeNear;
	float				m_eyeFar;
	float				m_eyeAspect;

};



#endif // CVSNTestCamera_h
