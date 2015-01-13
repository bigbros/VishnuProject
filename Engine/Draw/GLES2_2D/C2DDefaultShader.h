#ifndef C2DDefaultShader_h
#define C2DDefaultShader_h

#include "CGLShader.h"

class C2DDefaultShader : public CGLShader
{
public:
	// uniforms
	GLint				m_u_projection;	// 2D�v���W�F�N�V�����}�g���N�X
	GLint				m_u_mat;		// ���T�C�Y/��]/���s�ړ��}�g���N�X(vec3)�̔z��
	GLint				m_u_uv;			// �Ίp�̐��K��UV(����/�E��)��vec4�`��(x,y)-(z,w)�ŗ^�������̂̔z��
	GLint				m_u_tex;		// �g�p����e�N�X�`��
	GLint				m_u_rgba;		// �`��RGBA�l

	// attribs
	GLint				m_a_vert;		// ���_vec2(��ӂ�1.0�̐����`��`���`�ŗ^����B
	GLint				m_a_params;		// ���_�Ɋւ���p�����[�^(vec2)�B���݂̂Ƃ���.x ���u�g�p����uniform�z���index�v�Ƃ����Ƃ��낾���B

public:
	C2DDefaultShader();
	virtual ~C2DDefaultShader();

protected:
	void setShaderParams(GLuint program);
};

#endif // C2DDefaultShader_h
