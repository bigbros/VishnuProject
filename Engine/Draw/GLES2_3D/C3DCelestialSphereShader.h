#ifndef C3DCelestialSphereShader_h
#define C3DCelestialSphereShader_h

#include "CGLShader.h"

class C3DCelestialSphereShader : public CGLShader
{
public:
	// uniforms
	GLint				m_u_rgba;
	GLint				m_u_camera;
	GLint				m_u_projection;
	GLint				m_u_tex;

	// attribs
	GLint				m_a_vert;
	GLint				m_a_uv;

public:
	C3DCelestialSphereShader();
	virtual ~C3DCelestialSphereShader();
protected:
	void setShaderParams(GLuint program);
};


#endif // C3DCelestialSphereShader_h
