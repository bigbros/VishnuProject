#include "CVSNDraw2D.h"
#include "CVSNUtil.h"
#include "platform.h"

CVSNDraw2D::CVSNDraw2D() : CVSNTask(P_DRAW), m_env(0) {}
CVSNDraw2D::~CVSNDraw2D() {}


bool
CVSNDraw2D::init(const char * shaderPath)
{
	// 2D•`‰æŠÂ‹«‚ðÝ’è
	m_env = new C2DDrawEnv();
	CGLEnv::getInstance().setDrawEnv(CGLEnv::C2D, m_env);

	int width = CGLEnv::getInstance().width();
	int height = CGLEnv::getInstance().height();
	m_env->setResolution(width, height);

	CVSNStorage * storage = CVSNPlatform::getInstance().Module<CVSNStorage>(PF_STORAGE, "DATA");
	const char * vpath = CVSNUtil::jointPath(shaderPath, "default2d.vsh");
	const char * fpath = CVSNUtil::jointPath(shaderPath, "default2d.fsh");
	size_t vsize, fsize;
	const char * srcVertex = (const char *)storage->readText(vpath, &vsize);
	const char * srcFragment = (const char *)storage->readText(fpath, &fsize);

	m_shader = new C2DDefaultShader();
	m_shader->init(srcVertex, srcFragment);

	storage->closeData((void *)srcVertex);
	storage->closeData((void *)srcFragment);
	CVSNUtil::freePath(vpath);
	CVSNUtil::freePath(fpath);

	m_env->init(m_shader);

	return birth();
}

void
CVSNDraw2D::update(int deltaT)
{
	m_env->CalcWorld();
	m_env->Render();
}

void
CVSNDraw2D::die()
{
	CGLEnv::getInstance().setDrawEnv(CGLEnv::C2D, 0);
	delete m_shader;
	delete m_env;
}

void
CVSNDraw2D::on_pause()
{
}
void
CVSNDraw2D::on_resume()
{
}

void
CVSNDraw2D::on_surface()
{
}

void
CVSNDraw2D::on_detach()
{
}
void
CVSNDraw2D::on_change(int width, int height)
{
}