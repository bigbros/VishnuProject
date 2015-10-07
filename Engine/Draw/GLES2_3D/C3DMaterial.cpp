#include "C3DDrawEnv.h"
#include "C3DMaterial.h"
#include "CGLTex.h"

C3DMaterial::C3DMaterial()
	: CGLBase()
	, m_texture(0)
	, m_normal(0)
	, m_shininess(0)
	, m_switch(DIFFUSE)
{
}

C3DMaterial::~C3DMaterial()
{
	delete m_texture;
	delete m_normal;
}

void
C3DMaterial::setup(C3DDefaultShader * shader)
{
	if (m_switch & TEXTURE) {
		m_texture->set(GL_TEXTURE0);
		glUniform1i(shader->m_u_tex, 0);
		glUniform1i(shader->m_u_f_texmap, GL_TRUE);
	}
	else {
		glUniform1i(shader->m_u_f_texmap, GL_FALSE);
	}
	if (m_switch & NORMAL) {
		m_normal->set(GL_TEXTURE1);
		glUniform1i(shader->m_u_normmap, 1);
		glUniform1i(shader->m_u_f_normalmap, GL_TRUE);
	}
	else {
		glUniform1i(shader->m_u_f_normalmap, GL_FALSE);
	}
	glUniform1f(shader->m_u_shininess, m_shininess);
	glUniform1i(shader->m_u_switch, m_switch);
}