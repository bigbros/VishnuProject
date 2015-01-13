#ifndef CVSNTest2D_h
#define CVSNTest2D_h


#include "c2dgles.h"
#include "CVSNScriptTask.h"
#include "CVSNTouchPanel.h"
#include "CVSNPNGTex.h"

class CVSNTest2D : public CVSNScriptTask
{
public:
	CVSNTest2D();
	virtual ~CVSNTest2D();

	bool init(CVSNScriptArgs& args);

	void update(int deltaT);

	void die();

	void on_change();


private:
	C2DSprite		*	m_sprite[5];
	CVSNPNGTex		*	m_tex[2];

	C2DNullObj		*	m_ctrl;
	bool				m_anime;

	int					m_width;
	int					m_height;

	int					m_preX;
	int					m_preY;

	int					m_rot_deg;

	C2DVec				m_pos;
};

#endif // CVSNTest2D_h
