#ifndef CGLShader_h
#define CGLShader_h

#include <GLES2/gl2.h>
#include "CGLObj.h"

class CGLShader : public CGLObj
{
private:
	GLuint			m_vertex;
	GLint			m_fragment;
	GLint			m_program;

	const char	*	m_srcVertex;
	const char	*	m_srcFragment;
public:
	CGLShader();
	virtual ~CGLShader();

	bool init(const char * vertex_src, const char * fragment_src);
	bool useProgram();

	bool recovery();
	bool destruction();

protected:
	virtual void setShaderParams(GLuint program) = 0;

private:
	GLuint createProgram(const char * srcVertex, const char * srcFragment);
	GLint loadShader(GLenum type, const char * shaderSource);
};


#endif // CGLShader_h
