#include "C3DCelestialSphereShader.h"


C3DCelestialSphereShader::C3DCelestialSphereShader() : CGLShader() {}
C3DCelestialSphereShader::~C3DCelestialSphereShader() {}

void
C3DCelestialSphereShader::setShaderParams(GLuint program)
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

