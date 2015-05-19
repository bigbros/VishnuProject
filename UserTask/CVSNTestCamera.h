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
	// �w�肳�ꂽ�Ίp������p���琅������p���v�Z����
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
};



#endif // CVSNTestCamera_h
