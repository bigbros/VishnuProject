#ifndef C3DTex_h
#define C3DTex_h

#include "c3dgles.h"
#include "CGLObj.h"
#include "C3DDrawEnv.h"

class CGLTex : public CGLObj
{
private:
	GLuint		m_texture;
	GLint		m_format;	// GL_RGB, GL_RGBA, etc.
	C3DDrawEnv*	m_env;

	const char * m_texName;
protected:
	int			m_width;		// �A�g���X�̕�
	int			m_height;		// �A�g���X�̍���
	int			m_imgWidth;		// �����s�N�Z���̈�̕�
	int			m_imgHeight;	// ���s�s�N�Z���̈�̍���
	int			m_stride;
	u8		*	m_bitmap;
public:
	CGLTex(const char * name = 0);
	virtual ~CGLTex();

	void set(GLenum target = GL_TEXTURE0);
	inline int getWidth() const { return m_imgWidth; }
	inline int getHeight() const { return m_imgHeight; }

	inline int getAtlasWidth() const { return m_width; }
	inline int getAtlasHeight() const { return m_height; }

protected:

	bool newImage(int width, int height, int depthbyte, GLint format);
	bool destruction();
	void setBuffer();

private:
	int nextpow2(int x);
};




#endif // C3DTex_h
