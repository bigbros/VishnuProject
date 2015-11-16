#include "C3DDrawEnv.h"
#include "C3DDefaultShader.h"
#include "platform.h"

C3DDefaultShader::C3DDefaultShader(C3DDrawEnv * env) : C3DShader(SID_DEFAULT, env, C3DShader::DP_OBJECT) {
	LOG("[3D Default Shader]\n");
}
C3DDefaultShader::~C3DDefaultShader() {}

void
C3DDefaultShader::preConfig(C3DVec * lightVec, C3DVec * lightCol, C3DVec * ambientCol, C3DMat * projection, C3DMat * cameraInvert)
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// ���������A�����F�A�����F��ݒ肷��
	C3DVec offset(0.0f, 0.0f, 0.0f);
	glUniform4fv(m_u_light, 1, (GLfloat *)lightVec);			// �����x�N�g��
	glUniform4fv(m_u_rgba, 1, (GLfloat *)lightCol);		// �����F
	glUniform4fv(m_u_ambient, 1, (GLfloat *)ambientCol);	// ���F
	glUniform4fv(m_u_offset, 1, (GLfloat *)&offset);	// �f�t�H���g�̃I�t�Z�b�g

	// ���̃t���[���ɂ�����camera->view->projection�s�����ɂ܂Ƃ߂����̂��v���W�F�N�V�����p�ɑ���
	glUniformMatrix4fv(m_u_projection, 1, GL_FALSE, (const GLfloat *)projection);

	// �X�y�L���������̂��߃J�����t�s��𑗂��Ă����B
	glUniformMatrix4fv(m_u_camera, 1, GL_FALSE, (const GLfloat *)cameraInvert);
}

void
C3DDefaultShader::after()
{

}

GLuint
C3DDefaultShader::uniformOffset()
{
	return m_u_offset;
}

void
C3DDefaultShader::setShaderParams(GLuint program)
{
	LOG("setShaderParams\n");
	// shader�̊euniform�ɑ�������l���擾���Ă����B
	m_u_light = glGetUniformLocation(program, "u_light");
	m_u_offset = glGetUniformLocation(program, "u_offset");
	m_u_camera = glGetUniformLocation(program, "u_camera");
	m_u_projection = glGetUniformLocation(program, "u_projection");

	m_u_bone = glGetUniformLocation(program, "u_bone");
	m_u_bonepos = glGetUniformLocation(program, "u_bonepos");
	m_u_boneorg = glGetUniformLocation(program, "u_boneorg");
	m_u_matrix = glGetUniformLocation(program, "u_matrix");

	m_u_maptexture = glGetUniformLocation(program, "u_maptexture");
	m_u_mapnormal = glGetUniformLocation(program, "u_mapnormal");
	m_u_mapspecular = glGetUniformLocation(program, "u_mapspecular");

	m_u_f_normalmap = glGetUniformLocation(program, "u_f_normalmap");
	m_u_f_texmap = glGetUniformLocation(program, "u_f_texmap");
	m_u_f_specularmap = glGetUniformLocation(program, "u_f_specularmap");

	m_u_rgba = glGetUniformLocation(program, "u_rgba");
	m_u_modelcol = glGetUniformLocation(program, "u_modelcol");
	m_u_ambient = glGetUniformLocation(program, "u_ambient");
	m_u_diffuse = glGetUniformLocation(program, "u_diffuse");
	m_u_emissive = glGetUniformLocation(program, "u_emissive");
	m_u_specular = glGetUniformLocation(program, "u_specular");

	m_u_ambient_f = glGetUniformLocation(program, "u_ambient_f");
	m_u_diffuse_f = glGetUniformLocation(program, "u_diffuse_f");
	m_u_emissive_f = glGetUniformLocation(program, "u_emissive_f");
	m_u_shininess = glGetUniformLocation(program, "u_shininess");

	// shader�̊eattrib�ɑ�������l���擾���Ă����B
	m_a_vert = glGetAttribLocation(program, "a_vert");
	m_a_norm = glGetAttribLocation(program, "a_norm");
	m_a_tang = glGetAttribLocation(program, "a_tang");
	m_a_uv = glGetAttribLocation(program, "a_uv");
	m_a_rgba = glGetAttribLocation(program, "a_rgba");
	m_a_bone = glGetAttribLocation(program, "a_bone");
	m_a_wght = glGetAttribLocation(program, "a_wght");

	// attribute ��L���ɂ���
	glEnableVertexAttribArray(m_a_vert);
	glEnableVertexAttribArray(m_a_norm);
	glEnableVertexAttribArray(m_a_tang);
	glEnableVertexAttribArray(m_a_uv);
	glEnableVertexAttribArray(m_a_rgba);
	glEnableVertexAttribArray(m_a_bone);
	glEnableVertexAttribArray(m_a_wght);

	LOG("Finished.\n");
}
