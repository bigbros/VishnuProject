#include <iostream>
#include <memory>
#include <exception>
#include <string.h>
#include "CGLShader.h"
#include "CGLAlloc.h"

CGLShader::CGLShader()
	: CGLObj()
	, m_program(0)
	, m_vertex(-1)
	, m_fragment(-1)
{
}

CGLShader::~CGLShader()
{
	destruction();
	CGLAlloc::free((void *)m_srcVertex);
	CGLAlloc::free((void *)m_srcFragment);
}

bool
CGLShader::init(const char * vertex_src, const char * fragment_src)
{
	char * vsh = 0;
	char * fsh = 0;
	try {
		m_program = createProgram(vertex_src, fragment_src);
		if (!m_program) throw CGLException("failed to create shader program.");
		setShaderParams(m_program);

		vsh = (char *)CGLAlloc::malloc(strlen(vertex_src) + 1);
		fsh = (char *)CGLAlloc::malloc(strlen(fragment_src) + 1);
		strcpy(vsh, vertex_src);
		strcpy(fsh, fragment_src);
		m_srcVertex = vsh;
		m_srcFragment = fsh;

		return true;
	}
	catch (std::exception& ex) {
		CGLAlloc::free(vsh);
		CGLAlloc::free(fsh);
		LOG(ex.what());
		throw ex;
	}
}

bool
CGLShader::useProgram()
{
	if (!m_program) return false;
	glUseProgram(m_program);
	return true;
}

bool
CGLShader::recovery()
{
	CGLLog("Shader Recovery:\n");

	m_program = createProgram(m_srcVertex, m_srcFragment);
	if (!m_program) return false;
	setShaderParams(m_program);
	return true;
}

bool
CGLShader::destruction()
{
	if (m_program) glDeleteProgram(m_program);
	if (m_fragment >= 0) glDeleteShader(m_fragment);
	if (m_vertex >= 0) glDeleteShader(m_vertex);
	m_program = 0;
	m_fragment = -1;
	m_vertex = -1;
	return true;
}

GLint
CGLShader::loadShader(GLenum type, const char * buf)
{
	int shader = glCreateShader(type);
	GLint length = strlen(buf);
	GLint compiled;
	glShaderSource(shader, 1, &buf, &length);

	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	if (compiled == GL_FALSE) {
		length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		if (length > 1) {
			char * buf = (char *)CGLAlloc::malloc(length);
			glGetShaderInfoLog(shader, length, &length, buf);
			std::string exmsg = "Shader compile failed.\n";
			exmsg += buf;
			LOG(exmsg.c_str());
			CGLAlloc::free(buf);
			throw CGLException(exmsg.c_str());
		}
	}
	return (compiled == GL_FALSE) ? -1 : shader;
}

GLuint
CGLShader::createProgram(const char * srcVertex, const char * srcFragment)
{
	LOG("VertexShader\n");
	m_vertex = loadShader(GL_VERTEX_SHADER, srcVertex);
	LOG("FragmentShader\n");
	m_fragment = loadShader(GL_FRAGMENT_SHADER, srcFragment);

	if (m_vertex < 0 || m_fragment < 0) {
		if (m_vertex < 0) glDeleteShader(m_vertex);
		if (m_fragment < 0) glDeleteShader(m_fragment);
		m_vertex = -1;
		m_fragment = -1;
		throw CGLException("Shader compile failed.\n");
	}
	LOG("compile finished.\n");
	GLuint prog = glCreateProgram();

	LOG("attach vertex shader");
	glAttachShader(prog, m_vertex);
	LOG("attach fragment shader");
	glAttachShader(prog, m_fragment);
	LOG("Link shader\n");
	glLinkProgram(prog);
	GLint linked;
	glGetProgramiv(prog, GL_LINK_STATUS, &linked);

	if (linked == GL_FALSE) {
		GLint length = 0;
		glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &length);
		if (length > 1) {
			char * buf = (char *)CGLAlloc::malloc(length);
			glGetProgramInfoLog(prog, length, &length, buf);
			std::string msg = "Program attach failed: ";
			msg += buf;
			LOG(msg.c_str());
			CGLAlloc::free(buf);
			throw CGLException(msg.c_str());
		}
	}

	return (linked != GL_FALSE) ? prog : 0;
}

