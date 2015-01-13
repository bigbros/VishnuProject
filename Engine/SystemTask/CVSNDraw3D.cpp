#include "CVSNDraw3D.h"
#include "CVSNUtil.h"
#include "platform.h"

CVSNDraw3D::CVSNDraw3D() : CVSNTask(P_DRAW), m_env(0) {}
CVSNDraw3D::~CVSNDraw3D() {}


bool
CVSNDraw3D::init(const char * shaderPath)
{
	bool result = false;

	// 3Dï`âÊä¬ã´Çê›íË
	m_env = new C3DDrawEnv();
	CGLEnv::getInstance().setDrawEnv(CGLEnv::C3D, m_env);

	CVSNStorage * storage = CVSNPlatform::getInstance().Module<CVSNStorage>(PF_STORAGE, "DATA");
	const char * vpath = CVSNUtil::jointPath(shaderPath, "default3d.vsh");
	const char * fpath = CVSNUtil::jointPath(shaderPath, "default3d.fsh");
	size_t vsize, fsize;
	const char * srcVertex = (const char *)storage->readText(vpath, &vsize);
	const char * srcFragment = (const char *)storage->readText(fpath, &fsize);
	if (srcVertex && srcFragment) {
		m_shader = new C3DDefaultShader();
		m_shader->init(srcVertex, srcFragment);
		m_env->init(m_shader);

		// à»â∫èâä˙íl
		C3DVec light(1.0f, 1.0f, 1.0f);
		m_env->setLightVec(light);	// åıåπà íuê›íË
		m_env->setLightCol(1.0f, 1.0f, 1.0f, 1.0f);		// åıåπêFê›íË
		m_env->setAmbient(0.05f, 0.05f, 0.05f, 1.0f);		// ä¬ã´åıê›íË
		result = birth();
	}
	storage->closeData((void *)srcVertex);
	storage->closeData((void *)srcFragment);
	CVSNUtil::freePath(vpath);
	CVSNUtil::freePath(fpath);
	return result;
}

void
CVSNDraw3D::update(int deltaT)
{
	m_env->CalcWorld();
	m_env->Render();
}

void
CVSNDraw3D::die()
{
	CGLEnv::getInstance().setDrawEnv(CGLEnv::C3D, 0);
	delete m_shader;
	delete m_env;
}

void
CVSNDraw3D::on_pause()
{
}
void
CVSNDraw3D::on_resume()
{
}

void
CVSNDraw3D::on_surface()
{
}

void
CVSNDraw3D::on_detach()
{
}
void
CVSNDraw3D::on_change(int width, int height)
{
}