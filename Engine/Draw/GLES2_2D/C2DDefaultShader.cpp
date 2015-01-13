#include "C2DDefaultShader.h"

C2DDefaultShader::C2DDefaultShader() : CGLShader() {}
C2DDefaultShader::~C2DDefaultShader() {}

void
C2DDefaultShader::setShaderParams(GLuint program)
{
	// shaderの各uniformに相当する値を取得しておく。
	m_u_projection = glGetUniformLocation(program, "u_projection");
	m_u_mat = glGetUniformLocation(program, "u_mat");
	m_u_uv = glGetUniformLocation(program, "u_uv");
	m_u_tex = glGetUniformLocation(program, "u_tex");
	m_u_rgba = glGetUniformLocation(program, "u_rgba");

	// shaderの各attribに相当する値を取得しておく。
	m_a_vert = glGetAttribLocation(program, "a_vert");
	m_a_params = glGetAttribLocation(program, "a_params");

	// attribute を有効にする
	glEnableVertexAttribArray(m_a_vert);
	glEnableVertexAttribArray(m_a_params);
}
