#ifndef C3DVRFBO_h
#define C3DVRFBO_h

#include "CGLFBO.h"
#include "CGLShader.h"
#include "dtypes.h"
#include "Types3D.h"

class C3DVRFBOShader : public CGLShader
{
public:
	// uniforms
	GLint				m_u_tex_r;		// �g�p����e�N�X�`��
	GLint				m_u_tex_l;

	GLint				m_u_angle;		// ����p[rad]
	GLint				m_u_f;			//
	GLint				m_u_w;			//
	GLint				m_u_vrate;

	// attribs
	GLint				m_a_vert;		// ���_vec2(��ӂ�1.0�̐����`��`���`�ŗ^����B
	GLint				m_a_uv;			// �e�N�X�`���I����uv��vec3
public:
	C3DVRFBOShader();
	virtual ~C3DVRFBOShader();

protected:
	void setShaderParams(GLuint program);
};

#define EYE_DISTANCE  0.064f

class C3DVRFBO : public CGLFBO
{
public:
	C3DVRFBO(const char * shaderPath, int width, int height);
	virtual ~C3DVRFBO();

	void setLens(int lensId, GLint offsetUniform);
	void renderGlobal();
	void onChangeGlobalResolution(int width, int height);
	float getAngle();			// �f�t�H���g�̎���p��Ԃ�

private:
	void createShader(const char * shaderPath);
	void createVertex();
	void destroyVertex();
private:
	C3DVRFBOShader	*	m_shader;

	struct VEC {
		float x;
		float y;

		float u;
		float v;
		float t;
	};
	VEC				*	m_vertices;
	u16				*	m_indices;
	GLuint			m_idxVert;		// ���_�o�b�t�@
	GLuint			m_idxIndex;		// �C���f�b�N�X�o�b�t�@

	C3DVec			m_offsetR;
	C3DVec			m_offsetL;

	float			m_angle;
	float			m_f;
	float			m_w;
	float			m_vrate;
};

#endif // C3DVRFBO_h
