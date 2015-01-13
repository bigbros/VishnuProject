#ifndef C3DDefaultShader_h
#define C3DDefaultShader_h

#include "CGLShader.h"

class C3DDefaultShader : public CGLShader
{
public:
	// uniforms
	GLint				m_u_light;
	GLint				m_u_rgba;
	GLint				m_u_ambient;
	GLint				m_u_camera;
	GLint				m_u_projection;
	GLint				m_u_bone;
	GLint				m_u_bonepos;
	GLint				m_u_tex;
	GLint				m_u_normmap;
	GLint				m_u_modelcol;
	GLint				m_u_shininess;
	GLint				m_u_switch;

	// attribs
	GLint				m_a_vert;
	GLint				m_a_norm;
	GLint				m_a_tang;
	GLint				m_a_uv;
	GLint				m_a_rgba;
	GLint				m_a_bone;
	GLint				m_a_wght;

public:
	C3DDefaultShader();
	virtual ~C3DDefaultShader();

protected:
	void setShaderParams(GLuint program);
};

#endif // C3DDefaultShader_h
