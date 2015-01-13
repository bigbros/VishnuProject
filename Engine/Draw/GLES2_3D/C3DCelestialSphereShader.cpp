#include "C3DCelestialSphereShader.h"


C3DCelestialSphereShader::C3DCelestialSphereShader() : CGLShader() {}
C3DCelestialSphereShader::~C3DCelestialSphereShader() {}

void
C3DCelestialSphereShader::setShaderParams(GLuint program)
{
	// shaderの各uniformに相当する値を取得しておく。
	m_u_rgba = glGetUniformLocation(program, "u_rgba");
	m_u_camera = glGetUniformLocation(program, "u_camera");
	m_u_projection = glGetUniformLocation(program, "u_projection");
	m_u_tex = glGetUniformLocation(program, "u_tex");

	// shaderの各attribに相当する値を取得しておく。
	m_a_vert = glGetAttribLocation(program, "a_vert");
	m_a_uv = glGetAttribLocation(program, "a_uv");

	// attribute を有効にする
	glEnableVertexAttribArray(m_a_vert);
	glEnableVertexAttribArray(m_a_uv);
}

