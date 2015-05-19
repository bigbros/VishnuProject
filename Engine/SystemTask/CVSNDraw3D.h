#ifndef CVSNDraw3D_h
#define CVSNDraw3D_h

#include "CVSNTask.h"
#include "C3DDrawEnv.h"

class CVSNDraw3D : public CVSNTask
{
public:
	CVSNDraw3D();
	virtual ~CVSNDraw3D();

	bool init(const char * shaderPath);

	void update(int deltaT);
	void die();

	void on_pause();
	void on_resume();
	void on_change(int width, int height);

	void on_surface();
	void on_detach();
private:
	C3DDrawEnv*						m_env;
	C3DDefaultShader			*	m_shader;
	C3DCelestialSphereShader	*	m_celestial;
};

#endif // CVSNDraw3D_h
