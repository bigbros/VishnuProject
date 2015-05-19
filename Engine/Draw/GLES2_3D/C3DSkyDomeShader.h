#ifndef C3DSkyDomeShader_h
#define C3DSkyDomeShader_h

#include "C3DShader.h"

class C3DSkyDomeShader : public C3DShader
{
public:
	enum {
		SID_SKYDOME = SID_SYSTEM | 0x00001000
	};
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
	C3DSkyDomeShader(C3DDrawEnv * env);
	virtual ~C3DSkyDomeShader();

	void preConfig(C3DVec * lightVec, C3DVec * lightCol, C3DVec * ambientCol, C3DMat * projection, C3DMat * cameraInvert);

protected:
	void setShaderParams(GLuint program);
};


#endif // C3DSkyDomeShader_h
