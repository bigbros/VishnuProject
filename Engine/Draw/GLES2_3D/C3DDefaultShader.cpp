#include "C3DDefaultShader.h"

C3DDefaultShader::C3DDefaultShader() : CGLShader() {}
C3DDefaultShader::~C3DDefaultShader() {}

void
C3DDefaultShader::setShaderParams(GLuint program)
{
	// shaderの各uniformに相当する値を取得しておく。
	m_u_light = glGetUniformLocation(program, "u_light");
	m_u_rgba = glGetUniformLocation(program, "u_rgba");
	m_u_ambient = glGetUniformLocation(program, "u_ambient");
	m_u_camera = glGetUniformLocation(program, "u_camera");
	m_u_projection = glGetUniformLocation(program, "u_projection");
	m_u_bone = glGetUniformLocation(program, "u_bone");
	m_u_bonepos = glGetUniformLocation(program, "u_bonepos");
	m_u_tex = glGetUniformLocation(program, "u_tex");
	m_u_normmap = glGetUniformLocation(program, "u_normmap");
	m_u_modelcol = glGetUniformLocation(program, "u_modelcol");
	m_u_shininess = glGetUniformLocation(program, "u_shininess");
	m_u_switch = glGetUniformLocation(program, "u_switch");

	// shaderの各attribに相当する値を取得しておく。
	m_a_vert = glGetAttribLocation(program, "a_vert");
	m_a_norm = glGetAttribLocation(program, "a_norm");
	m_a_tang = glGetAttribLocation(program, "a_tang");
	m_a_uv = glGetAttribLocation(program, "a_uv");
	m_a_rgba = glGetAttribLocation(program, "a_rgba");
	m_a_bone = glGetAttribLocation(program, "a_bone");
	m_a_wght = glGetAttribLocation(program, "a_wght");

	// attribute を有効にする
	glEnableVertexAttribArray(m_a_vert);
	glEnableVertexAttribArray(m_a_norm);
	glEnableVertexAttribArray(m_a_tang);
	glEnableVertexAttribArray(m_a_uv);
	glEnableVertexAttribArray(m_a_rgba);
	glEnableVertexAttribArray(m_a_bone);
	glEnableVertexAttribArray(m_a_wght);
}
