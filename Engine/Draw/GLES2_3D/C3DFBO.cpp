#include "CVSNUtil.h"
#include "platform.h"
#include "C3DFBO.h"

C3DFBOShader::C3DFBOShader() : CGLShader() {
	LOG("[FBO Shader]\n");
}
C3DFBOShader::~C3DFBOShader() {}

void
C3DFBOShader::setShaderParams(GLuint program)
{
	GLenum errcode;

	LOG("err-1\n");
	while (errcode = glGetError()) {
		if (errcode == GL_INVALID_OPERATION) LOG("GL_INVALID_OPERATION in C3DMaterial::setTexture().\n");
	}

	// shaderの各uniformに相当する値を取得しておく。
	m_u_tex = glGetUniformLocation(program, "u_tex");

	LOG("err-2\n");
	while (errcode = glGetError()) {
		if (errcode == GL_INVALID_OPERATION) LOG("GL_INVALID_OPERATION in C3DMaterial::setTexture().\n");
	}


	// shaderの各attribに相当する値を取得しておく。
	m_a_vert = glGetAttribLocation(program, "a_vert");
	m_a_uv = glGetAttribLocation(program, "a_uv");

	LOG("err-3\n");
	while (errcode = glGetError()) {
		if (errcode == GL_INVALID_OPERATION) LOG("GL_INVALID_OPERATION in C3DMaterial::setTexture().\n");
	}


	// attribute を有効にする
	glEnableVertexAttribArray(m_a_vert);
	glEnableVertexAttribArray(m_a_uv);

	LOG("err-4\n");
	while (errcode = glGetError()) {
		if (errcode == GL_INVALID_OPERATION) LOG("GL_INVALID_OPERATION in C3DMaterial::setTexture().\n");
	}

}

C3DFBO::C3DFBO(const char * shaderPath, int width, int height)
	: CGLFBO(1, width, height)
	, m_offset(0.0f, 0.0f, 0.0f)
{
	createShader(shaderPath);
	createVertex();
}

C3DFBO::~C3DFBO()
{
	destroyVertex();
	delete m_shader;
}

void
C3DFBO::setLens(int lensId, GLint offsetUniform)
{
	if (offsetUniform < 0) return;
	glUniform4fv(offsetUniform, 1, (GLfloat *)&m_offset);
}

void
C3DFBO::renderGlobal()
{
	m_shader->useProgram();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_fboTex[0]);
	glUniform1i(m_shader->m_u_tex, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_idxVert);
	glVertexAttribPointer(m_shader->m_a_vert, 2, GL_FLOAT, GL_FALSE, sizeof(VEC), (const GLvoid *)(char *)offsetof(VEC, x));
	glVertexAttribPointer(m_shader->m_a_uv, 2, GL_FLOAT, GL_FALSE, sizeof(VEC), (const GLvoid *)(char *)offsetof(VEC, u));

	// インデックスバッファを有効にする
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_idxIndex);

	// 転送済みの頂点とインデックス、およびマトリクス、ボーン位置情報で描画する。
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void
C3DFBO::onChangeGlobalResolution(int width, int height)
{
	destroyFrameBuffer();
	m_width = width;
	m_height = height;
	makeFrameBuffer();
	m_projection.v[0][0] = (width > height) ? 1.0f : ((float)height / (float)width);
	m_projection.v[1][1] = (width < height) ? 1.0f : ((float)width / (float)height);
}

void
C3DFBO::createShader(const char * shaderPath)
{
	CVSNStorage * storage = CVSNPlatform::getInstance().Module<CVSNStorage>(PF_STORAGE, "DATA");
	const char * vpath = CVSNUtil::jointPath(shaderPath, "defaultFBO.vsh");	// default vertex shader
	const char * fpath = CVSNUtil::jointPath(shaderPath, "defaultFBO.fsh");	// default fragment shader
	size_t vsize, fsize;

	const char * srcVertex = (const char *)storage->readText(vpath, &vsize);
	const char * srcFragment = (const char *)storage->readText(fpath, &fsize);
	if (srcVertex && srcFragment) {
		m_shader = new C3DFBOShader();
		m_shader->init(srcVertex, srcFragment);
	}
	storage->closeData((void *)srcVertex);
	storage->closeData((void *)srcFragment);
	CVSNUtil::freePath(vpath);
	CVSNUtil::freePath(fpath);
}

void
C3DFBO::createVertex()
{
	m_vertices = new VEC[4];
	m_indices = new u16 [4];

	for (int i = 0; i < 4; i++) {
		m_indices[i] = i;
		m_vertices[i].x = (i & 1) ? 1.0f : -1.0f;
		m_vertices[i].y = (i & 2) ? -1.0f : 1.0f;
		m_vertices[i].u = (float)(i & 1);
		m_vertices[i].v = (float)(1 - ((i & 2) >> 1));
	}

	GLuint bufIdx[2];
	glGenBuffers(2, bufIdx);

	m_idxVert = bufIdx[0];
	glBindBuffer(GL_ARRAY_BUFFER, m_idxVert);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VEC) * 4, m_vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	m_idxIndex = bufIdx[1];
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_idxIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u16) * 4, m_indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void
C3DFBO::destroyVertex()
{
	GLuint bufIdx[2];
	bufIdx[0] = m_idxVert;
	bufIdx[1] = m_idxIndex;
	glDeleteBuffers(2, bufIdx);
}

float
C3DFBO::getAngle()
{
	return F_PI / 2.0f;
}

float
C3DFBO::getAspect()
{
	return (float)m_height / (float)m_width;
}