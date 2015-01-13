#ifndef CVSNDraw2D_h
#define CVSNDraw2D_h

#include "CVSNTask.h"
#include "C2DDrawEnv.h"

class CVSNDraw2D : public CVSNTask
{
public:
	CVSNDraw2D();
	virtual ~CVSNDraw2D();

	bool init(const char * shaderPath);

	void update(int deltaT);
	void die();

	void on_pause();
	void on_resume();
	void on_change(int width, int height);

	void on_surface();
	void on_detach();
private:
	C2DDrawEnv*				m_env;
	C2DDefaultShader	*	m_shader;
};

#endif // CVSNDraw2D_h
