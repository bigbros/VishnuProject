#include "C3DDrawEnv.h"
#include "C3DSkyDomeShader.h"


C3DSkyDomeShader::C3DSkyDomeShader(C3DDrawEnv * env) : C3DShader(SID_SKYDOME, env, C3DShader::DP_SKYDOME) {}
C3DSkyDomeShader::~C3DSkyDomeShader() {}

void
C3DSkyDomeShader::preConfig(C3DVec * lightVec, C3DVec * lightCol, C3DVec * ambientCol, C3DMat * projection, C3DMat * cameraInvert)
{
	// z�o�b�t�@�𖳌��ɂ���
	glDisable(GL_DEPTH_TEST);

	glUniform4fv(m_u_rgba, 1, (GLfloat *)lightCol);		// �����F

	// ���̃t���[���ɂ�����camera->view->projection�s�����ɂ܂Ƃ߂����̂��v���W�F�N�V�����p�ɑ���
	glUniformMatrix4fv(m_u_projection, 1, GL_FALSE, (const GLfloat *)projection);

	// �X�y�L���������̂��߃J�����t�s��𑗂��Ă����B
	glUniformMatrix4fv(m_u_camera, 1, GL_FALSE, (const GLfloat *)cameraInvert);
}

void
C3DSkyDomeShader::setShaderParams(GLuint program)
{
	// shader�̊euniform�ɑ�������l���擾���Ă����B
	m_u_rgba = glGetUniformLocation(program, "u_rgba");
	m_u_camera = glGetUniformLocation(program, "u_camera");
	m_u_projection = glGetUniformLocation(program, "u_projection");
	m_u_tex = glGetUniformLocation(program, "u_tex");

	// shader�̊eattrib�ɑ�������l���擾���Ă����B
	m_a_vert = glGetAttribLocation(program, "a_vert");
	m_a_uv = glGetAttribLocation(program, "a_uv");

	// attribute ��L���ɂ���
	glEnableVertexAttribArray(m_a_vert);
	glEnableVertexAttribArray(m_a_uv);
}

