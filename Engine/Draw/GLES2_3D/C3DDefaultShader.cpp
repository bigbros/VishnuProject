#include "C3DDrawEnv.h"
#include "C3DDefaultShader.h"

C3DDefaultShader::C3DDefaultShader(C3DDrawEnv * env) : C3DShader(SID_DEFAULT, env, C3DShader::DP_OBJECT) {}
C3DDefaultShader::~C3DDefaultShader() {}

void
C3DDefaultShader::preConfig(C3DVec * lightVec, C3DVec * lightCol, C3DVec * ambientCol, C3DMat * projection, C3DMat * cameraInvert)
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// 光源方向、光源色、環境光色を設定する
	glUniform4fv(m_u_light, 1, (GLfloat *)lightVec);			// 光源ベクトル
	glUniform4fv(m_u_rgba, 1, (GLfloat *)lightCol);		// 光源色
	glUniform4fv(m_u_ambient, 1, (GLfloat *)ambientCol);	// 環境色

	// このフレームにおけるcamera->view->projection行列を一つにまとめたものをプロジェクション用に送る
	glUniformMatrix4fv(m_u_projection, 1, GL_FALSE, (const GLfloat *)projection);

	// スペキュラ処理のためカメラ逆行列を送っておく。
	glUniformMatrix4fv(m_u_camera, 1, GL_FALSE, (const GLfloat *)cameraInvert);
}


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
