#ifndef C3DDefaultShader_h
#define C3DDefaultShader_h

#include "C3DShader.h"

class C3DDefaultShader : public C3DShader
{
public:
	enum {
		SID_DEFAULT = SID_SYSTEM | 0x00000001
	};
public:
	// uniforms
	GLint				m_u_light;
	GLint				m_u_offset;
	GLint				m_u_camera;
	GLint				m_u_projection;
	GLint				m_u_bone;
	GLint				m_u_bonepos;

	// textures
	GLint				m_u_maptexture;
	GLint				m_u_mapnormal;
	GLint				m_u_mapspecular;

	// enable flags
	GLint				m_u_f_normalmap;
	GLint				m_u_f_texmap;
	GLint				m_u_f_specularmap;

	// material
	GLint				m_u_rgba;
	GLint				m_u_modelcol;
	GLint				m_u_ambient;
	GLint				m_u_diffuse;
	GLint				m_u_emissive;
	GLint				m_u_specular;

	GLint				m_u_ambient_f;
	GLint				m_u_diffuse_f;
	GLint				m_u_emissive_f;
	GLint				m_u_shininess;

	// attribs
	GLint				m_a_vert;
	GLint				m_a_norm;
	GLint				m_a_tang;
	GLint				m_a_uv;
	GLint				m_a_rgba;
	GLint				m_a_bone;
	GLint				m_a_wght;

public:
	C3DDefaultShader(C3DDrawEnv * env);
	virtual ~C3DDefaultShader();

	void preConfig(C3DVec * lightVec, C3DVec * lightCol, C3DVec * ambientCol, C3DMat * projection, C3DMat * cameraInvert);
	void after();
	GLuint uniformOffset();
protected:
	void setShaderParams(GLuint program);
};

#endif // C3DDefaultShader_h
