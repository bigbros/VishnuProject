#include "C3DDrawEnv.h"
#include "C3DSkyDomeShader.h"


C3DSkyDomeShader::C3DSkyDomeShader(C3DDrawEnv * env) : C3DShader(SID_SKYDOME, env, C3DShader::DP_SKYDOME) {}
C3DSkyDomeShader::~C3DSkyDomeShader() {}

void
C3DSkyDomeShader::preConfig(C3DVec * lightVec, C3DVec * lightCol, C3DVec * ambientCol, C3DMat * projection, C3DMat * cameraInvert)
{
	// zバッファを無効にする
	glDisable(GL_DEPTH_TEST);

	glUniform4fv(m_u_rgba, 1, (GLfloat *)lightCol);		// 光源色

	// このフレームにおけるcamera->view->projection行列を一つにまとめたものをプロジェクション用に送る
	glUniformMatrix4fv(m_u_projection, 1, GL_FALSE, (const GLfloat *)projection);

	// スペキュラ処理のためカメラ逆行列を送っておく。
	glUniformMatrix4fv(m_u_camera, 1, GL_FALSE, (const GLfloat *)cameraInvert);
}

void
C3DSkyDomeShader::setShaderParams(GLuint program)
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

