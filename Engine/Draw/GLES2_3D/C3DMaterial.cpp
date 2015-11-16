#include "C3DDrawEnv.h"
#include "C3DMaterial.h"
#include "CGLTex.h"

C3DMaterial::C3DMaterial()
	: CGLBase()
	, m_prev(0)
	, m_next(0)
	, m_texture(0)
	, m_normal(0)
	, m_specular(0)
	, m_switch(DIFFUSE)
{
}

C3DMaterial::~C3DMaterial()
{
	delete m_texture;
	delete m_normal;
}

void
C3DMaterial::setTexture(int mask, CGLTex * tex, GLenum target, int num, GLint shaderIdx, GLint flagIdx)
{
	if (m_switch & mask) {
		tex->set(target);
		glUniform1i(shaderIdx, num);
		glUniform1i(flagIdx, GL_TRUE);
	}
	else {
		glUniform1i(flagIdx, GL_FALSE);
	}
	GLenum errcode = glGetError();
	if (errcode == GL_INVALID_OPERATION) {
		LOG("GL_INVALID_OPERATION in C3DMaterial::setTexture().\n");
	}

}
void
C3DMaterial::setup(C3DDefaultShader * shader)
{
	setTexture(TEXTURE, m_texture,	GL_TEXTURE0, 0, shader->m_u_maptexture, shader->m_u_f_texmap);
	setTexture(NORMAL, m_normal,	GL_TEXTURE1, 1, shader->m_u_mapnormal, shader->m_u_f_normalmap);
	setTexture(SPECULAR, m_specular, GL_TEXTURE2, 2, shader->m_u_mapspecular, shader->m_u_f_specularmap);

	glUniform4fv(shader->m_u_ambient, 1, (GLfloat *)&params.ambient);
	glUniform4fv(shader->m_u_diffuse, 1, (GLfloat *)&params.diffuse);
	glUniform4fv(shader->m_u_emissive, 1, (GLfloat *)&params.emissive);
	glUniform4fv(shader->m_u_specular, 1, (GLfloat *)&params.specular);
	glUniform1f(shader->m_u_ambient_f, params.ambientFactor);
	glUniform1f(shader->m_u_diffuse_f, params.diffuseFactor);
	glUniform1f(shader->m_u_emissive_f, params.emissiveFactor);
	glUniform1f(shader->m_u_shininess, params.shininess);
}

