#include "C2DSprite.h"
#include "C2DDrawEnv.h"
#include "CGLTex.h"
#include "platform.h"

C2DSprite::TEXLINK::TEXLINK() : prev(0), next(0) {}

C2DSprite::C2DSprite()
	: C2DObj()
	, m_texlink()
	, m_texOT(0)
	, m_rgba(0)
	, m_drawmat(0)
	, m_uv(0)
	, m_luv()
	, m_lrgba()
{
}

C2DSprite::~C2DSprite()
{
	if (m_texOT) {
		// texOT ‚©‚çŽ©g‚Ì‹L˜^‚ðíœ: Žc‚è‚ª0‚É‚È‚Á‚½‚çíœ
		if (m_texOT->removeSprite(this) == 0) delete m_texOT;
	}
}

void
C2DSprite::setTex(CGLTex * pTex)
{
	C2DDrawEnv * env = CGLEnv::getInstance().DrawEnv<C2DDrawEnv>(CGLEnv::C2D);
	m_tex = pTex;
	if (m_texOT && !m_texOT->isTex(pTex)) {
		if (m_texOT->removeSprite(this) == 0) delete m_texOT;
	}
	if (pTex) {
		m_texOT = env->getTexOT(pTex);
		m_texOT->registSprite(this, m_priority);
	}
}

void
C2DSprite::setSize(const float width, const float height)
{
	m_width = width;
	m_height = height;
	m_recalc = true;
}

void
C2DSprite::setRGBA(C3DVec& rgba)
{
	m_lrgba = rgba;
	if(m_rgba) *m_rgba = m_lrgba;
}

void
C2DSprite::setUVnorm(C2DVec& uv0, C2DVec& uv1) 
{
	m_luv.set(uv0.x, 1.0f - uv0.y, uv1.x, 1.0f - uv1.w);
	if (m_uv) *m_uv = m_luv;
}

void
C2DSprite::setUVpx(C2DVec& uv0, C2DVec& uv1)
{
	float height = (float)m_tex->getHeight();
	float a_width = (float)m_tex->getAtlasWidth();
	float a_height = (float)m_tex->getAtlasHeight();
	m_luv.set(
		uv0.x / a_width,
		(height - uv0.y) / a_height,
		uv1.x / a_width,
		(height - uv1.y) / a_height);
	if (m_uv) *m_uv = m_luv;
}

bool
C2DSprite::calcProcedure(bool recalc)
{
	if (!recalc || !m_drawmat) return true;
	m_drawmat->toScale(m_width, m_height);
	*m_drawmat *= m_matrix;
	return true;
}

bool
C2DSprite::priorityProcedure(int pri)
{
	m_texOT->removeSprite(this);
	m_texOT->registSprite(this, pri);
	*m_rgba = m_lrgba;
	*m_uv = m_luv;
	return true;
}