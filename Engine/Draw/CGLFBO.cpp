#include <iostream>
#include <exception>
#include "CGLFBO.h"
#include "platform.h"

CGLFBO::CGLFBO(int num, int width, int height)
	: CGLObj()
	, m_fboNum(0)
	, m_fboIdx(0)
	, m_fboDepth(0)
	, m_fboTex(0)
	, m_width(width)
	, m_height(height)
{
	try {
		m_fboIdx = new GLuint[num];
		m_fboDepth = new GLuint[num];
		m_fboTex = new GLuint[num];
		m_fboNum = num;

		if (!makeFrameBuffer()) throw std::bad_alloc();

		m_projection.v[0][0] = (width > height) ? 1.0f : ((float)width / (float)height);
		m_projection.v[1][1] = (width < height) ? 1.0f : ((float)height / (float)width);
	}
	catch (std::bad_alloc& ex) {
		delete[] m_fboIdx;
		delete[] m_fboDepth;
		delete[] m_fboTex;
		throw ex;
	}
}
CGLFBO::~CGLFBO()
{
	destroyFrameBuffer();
	delete[] m_fboIdx;
	delete[] m_fboDepth;
	delete[] m_fboTex;
}

bool
CGLFBO::destruction()
{
	destroyFrameBuffer();
	destroyVertex();
	return true;
}

bool
CGLFBO::recovery()
{
	bool result = makeFrameBuffer();
	createVertex();
	return result;
}

void
CGLFBO::destroyFrameBuffer()
{
	glDeleteRenderbuffers(m_fboNum, m_fboDepth);
	glDeleteTextures(m_fboNum, m_fboTex);
	glDeleteFramebuffers(m_fboNum, m_fboIdx);
}

bool
CGLFBO::makeFrameBuffer()
{
	bool result = true;

	glGenFramebuffers(m_fboNum, m_fboIdx);
	glGenTextures(m_fboNum, m_fboTex);
	glGenRenderbuffers(m_fboNum, m_fboDepth);
	for (int i = 0; i < m_fboNum; i++) {
		glBindFramebuffer(GL_FRAMEBUFFER, m_fboIdx[i]);

		glBindTexture(GL_TEXTURE_2D, m_fboTex[i]);
		glTexImage2D(GL_TEXTURE_2D,
			0,
			GL_RGBA,
			m_width, m_height,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fboTex[i], 0);
		/*GL_DRAW_FRAMEBUFFER_APPLE*/
		glBindRenderbuffer(GL_RENDERBUFFER, m_fboDepth[i]);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, m_width, m_height);
		/*GL_DEPTH_COMPONENT24_OES*/
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_fboDepth[i]);
		/*GL_DRAW_FRAMEBUFFER_APPLE*/

		// FBO status check
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		switch (status) {
		case GL_FRAMEBUFFER_COMPLETE:
			LOG("fbo complete");
			break;

		case GL_FRAMEBUFFER_UNSUPPORTED:
			result = false;
			LOG("fbo unsupported");
			break;

		default:
			/* programming error; will fail on all hardware */
			result = false;
			LOG("Framebuffer Error");
			break;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	if (!result) {
		destroyFrameBuffer();
	}
	return result;
}

void
CGLFBO::SwitchFBO(int lensId)
{
	GLenum errcode;

	LOG("err-1\n");
	while (errcode = glGetError()) {
		if (errcode == GL_INVALID_OPERATION) LOG("GL_INVALID_OPERATION in C3DMaterial::setTexture().\n");
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	LOG("err-2\n");
	while (errcode = glGetError()) {
		if (errcode == GL_INVALID_OPERATION) LOG("GL_INVALID_OPERATION in C3DMaterial::setTexture().\n");
	}

	glEnable(GL_TEXTURE_2D);
	LOG("err-3\n");
	while (errcode = glGetError()) {
		if (errcode == GL_INVALID_OPERATION) LOG("GL_INVALID_OPERATION in C3DMaterial::setTexture().\n");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, m_fboIdx[lensId]);
	LOG("err-4\n");
	while (errcode = glGetError()) {
		if (errcode == GL_INVALID_OPERATION) LOG("GL_INVALID_OPERATION in C3DMaterial::setTexture().\n");
	}

	glViewport(0, 0, m_width, m_height);
	LOG("err-5\n");
	while (errcode = glGetError()) {
		if (errcode == GL_INVALID_OPERATION) LOG("GL_INVALID_OPERATION in C3DMaterial::setTexture().\n");
	}

}

void
CGLFBO::setGlobalResolution(int width, int height)
{
	m_globalWidth = width;
	m_globalHeight = height;
	onChangeGlobalResolution(width, height);
}

void
CGLFBO::onChangeGlobalResolution(int width, int height)
{
}

float
CGLFBO::getAspect()
{
	return 1.0f;
}

C3DMat&
CGLFBO::getProjection()
{
	return m_projection;
}
