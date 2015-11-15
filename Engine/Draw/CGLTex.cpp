#include <memory>
#include "CGLAlloc.h"
#include "CGLTex.h"

CGLTex::CGLTex(const char * name)
	: CGLObj()
	, m_texture(0)
	, m_format(0)
	, m_env(CGLEnv::getInstance().DrawEnv<C3DDrawEnv>(CGLEnv::C3D))
	, m_texName(0)
	, m_width(0)
	, m_height(0)
	, m_imgWidth(0)
	, m_imgHeight(0)
	, m_stride(0)
	, m_bitmap(0)

{
	char * buf = 0;
	try {
		int len = strlen(name);
		char * buf = (char *)CGLAlloc::malloc(len + 1);
		strcpy(buf, name);
		m_texName = buf;
	}
	catch (std::bad_alloc& ex) {
		CGLAlloc::free(buf);
		throw ex;
	}
}

CGLTex::~CGLTex()
{
	glDeleteTextures(1, &m_texture);
	CGLAlloc::free((void *)m_texName);
	if (m_bitmap) CGLAlloc::free(m_bitmap);
}

void
CGLTex::set(GLenum target)
{
	glActiveTexture(target);
	glBindTexture(GL_TEXTURE_2D, m_texture);
}

void
CGLTex::setBuffer()
{
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, m_format, m_width, m_height, 0, m_format, GL_UNSIGNED_BYTE, m_bitmap);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	CGLAlloc::free(m_bitmap);
	m_bitmap = 0;
}

bool
CGLTex::newImage(int width, int height, int depthbyte, GLint format)
{
	CGLAlloc::free(m_bitmap);
	m_width = nextpow2(width);
	m_height = nextpow2(height);

	// 初期状態では実行ピクセル領域サイズは要求されたサイズと一致する。必要ならば書き換えること。
	m_imgWidth = width;
	m_imgHeight = height;

	m_format = format;
	m_stride = depthbyte * m_width;
	size_t size = m_stride * m_height;
	m_bitmap = (u8 *)CGLAlloc::malloc(size);
	return true;
}

int
CGLTex::nextpow2(int n)
{
	int a = 1;
	while (a < n) a += a;
	return a;
}

bool
CGLTex::destruction()
{
	glDeleteTextures(1, &m_texture);
	return true;
}