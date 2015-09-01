#include "C3DDrawEnv.h"
#include "C3DShader.h"

C3DShader::SHADER_LINK::SHADER_LINK() : prev(0), next(0) {}

C3DShader::C3DShader(int id, C3DDrawEnv * env, DRAWPHASE phase)
	: CGLShader()
	, m_sisters()
	, m_phase(phase)
	, m_env(env)
	, m_sid(id)
{
	env->setShader(phase, this);
}

C3DShader::~C3DShader()
{
	m_env->removeShader(this);
}

void
C3DShader::render()
{
	for (C3DDrawable * draw = m_begin; draw; draw = draw->m_sisters.next) {
		draw->Render();
	}
}

GLuint
C3DShader::uniformOffset()
{
	return -1;
}
