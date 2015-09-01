#include "CVSNUtil.h"
#include "platform.h"
#include "C3DVRFBO.h"

C3DVRFBOShader::C3DVRFBOShader() : CGLShader() {}
C3DVRFBOShader::~C3DVRFBOShader() {}

void
C3DVRFBOShader::setShaderParams(GLuint program)
{
	// shaderの各uniformに相当する値を取得しておく。
	m_u_tex_r = glGetUniformLocation(program, "u_tex_r");
	m_u_tex_l = glGetUniformLocation(program, "u_tex_l");
	m_u_angle = glGetUniformLocation(program, "u_angle");
	m_u_f = glGetUniformLocation(program, "u_f");
	m_u_w = glGetUniformLocation(program, "u_w");

	// shaderの各attribに相当する値を取得しておく。
	m_a_vert = glGetAttribLocation(program, "a_vert");
	m_a_uv = glGetAttribLocation(program, "a_uv");

	// attribute を有効にする
	glEnableVertexAttribArray(m_a_vert);
	glEnableVertexAttribArray(m_a_uv);
}

C3DVRFBO::C3DVRFBO(const char * shaderPath, int width, int height)
	: CGLFBO(2, 512, 512)
	, m_offsetR(EYE_DISTANCE / 2.0f, 0.0f, 0.0f)
	, m_offsetL(-EYE_DISTANCE / 2.0f, 0.0f, 0.0f)
	, m_angle(110.0f * F_PI / 180.0f)
{
	m_f = 1.0f / tanf(m_angle / 2.0f);
	m_w = 0.5f;
	createShader(shaderPath);
	createVertex();
	
}

C3DVRFBO::~C3DVRFBO()
{
	delete m_shader;
}

void
C3DVRFBO::setLens(int lensId, GLint offsetUniform)
{
	if (offsetUniform < 0) return;
	glUniform4fv(offsetUniform, 1, (float *)((lensId == 0) ? &m_offsetR : &m_offsetL));
}

void
C3DVRFBO::renderGlobal()
{
	glViewport(0, 0, m_globalWidth, m_globalHeight);
	m_shader->useProgram();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_fboTex[0]);
	glUniform1i(m_shader->m_u_tex_r, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_fboTex[1]);
	glUniform1i(m_shader->m_u_tex_l, 1);

	glUniform1f(m_shader->m_u_angle, m_angle);
	glUniform1f(m_shader->m_u_f, m_f);
	glUniform1f(m_shader->m_u_w, m_w);

	glBindBuffer(GL_ARRAY_BUFFER, m_idxVert);
	glVertexAttribPointer(m_shader->m_a_vert, 2, GL_FLOAT, GL_FALSE, sizeof(VEC), (const GLvoid *)(char *)offsetof(VEC, x));
	glVertexAttribPointer(m_shader->m_a_uv, 3, GL_FLOAT, GL_FALSE, sizeof(VEC), (const GLvoid *)(char *)offsetof(VEC, u));

	// インデックスバッファを有効にする
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_idxIndex);

	// 転送済みの頂点とインデックス、およびマトリクス、ボーン位置情報で描画する。
	glDrawElements(GL_TRIANGLE_STRIP, 9, GL_UNSIGNED_SHORT, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void
C3DVRFBO::onChangeGlobalResolution(int width, int height)
{
}

void
C3DVRFBO::createShader(const char * shaderPath)
{
	CVSNStorage * storage = CVSNPlatform::getInstance().Module<CVSNStorage>(PF_STORAGE, "DATA");
	const char * vpath = CVSNUtil::jointPath(shaderPath, "VRFBO.vsh");	// default vertex shader
	const char * fpath = CVSNUtil::jointPath(shaderPath, "VRFBO.fsh");	// default fragment shader
	size_t vsize, fsize;

	const char * srcVertex = (const char *)storage->readText(vpath, &vsize);
	const char * srcFragment = (const char *)storage->readText(fpath, &fsize);
	if (srcVertex && srcFragment) {
		m_shader = new C3DVRFBOShader();
		m_shader->init(srcVertex, srcFragment);
	}
	storage->closeData((void *)srcVertex);
	storage->closeData((void *)srcFragment);
	CVSNUtil::freePath(vpath);
	CVSNUtil::freePath(fpath);
}

void
C3DVRFBO::createVertex()
{
	m_vertices = new VEC[8];
	m_indices = new u16[9];

	for (int i = 0; i < 4; i++) {
		m_indices[i] = i;
		m_vertices[i].x = (i & 1) ? 0.0f : -1.0f;
		m_vertices[i].y = (i & 2) ? -1.0f : 1.0f;
		m_vertices[i].u = (float)(i & 1);
		m_vertices[i].v = (float)(1 - ((i & 2) >> 1));
		m_vertices[i].t = 1.0f;

		m_indices[i+5] = i + 4;
		m_vertices[i+4].x = (i & 1) ? 1.0f : 0.0f;
		m_vertices[i+4].y = (i & 2) ? -1.0f : 1.0f;
		m_vertices[i+4].u = (float)(i & 1);
		m_vertices[i+4].v = (float)(1 - ((i & 2) >> 1));
		m_vertices[i].t = 0.0f;
	}
	m_indices[4] = 4;

	GLuint bufIdx[2];
	glGenBuffers(2, bufIdx);

	m_idxVert = bufIdx[0];
	glBindBuffer(GL_ARRAY_BUFFER, m_idxVert);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VEC) * 8, m_vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	m_idxIndex = bufIdx[1];
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_idxIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u16) * 9, m_indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

float
C3DVRFBO::getAngle()
{
	return m_angle;
}
