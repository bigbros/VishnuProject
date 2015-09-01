#ifndef C3DFBO_h
#define C3DFBO_h

#include "CGLFBO.h"
#include "CGLShader.h"
#include "dtypes.h"

class C3DFBOShader : public CGLShader
{
public:
	// uniforms
	GLint				m_u_tex;		// �g�p����e�N�X�`��

	// attribs
	GLint				m_a_vert;		// ���_vec2(��ӂ�1.0�̐����`��`���`�ŗ^����B
	GLint				m_a_uv;
public:
	C3DFBOShader();
	virtual ~C3DFBOShader();

protected:
	void setShaderParams(GLuint program);
};

class C3DFBO : public CGLFBO
{
public:
	C3DFBO(const char * shaderPath, int width, int height);
	virtual ~C3DFBO();

	void setLens(int lensId, GLint offsetUniform);
	void renderGlobal();
	void onChangeGlobalResolution(int width, int height);
	float getAngle();			// �f�t�H���g�̎���p��Ԃ�

private:
	void createShader(const char * shaderPath);
	void createVertex();
private:
	C3DFBOShader	*	m_shader;

	struct VEC {
		float x;
		float y;
		float u;
		float v;
	};
	VEC				*	m_vertices;
	u16				*	m_indices;
	GLuint			m_idxVert;		// ���_�o�b�t�@
	GLuint			m_idxIndex;		// �C���f�b�N�X�o�b�t�@
};


#endif // C3DFBO_h
