#include "CVSNDraw3D.h"
#include "CVSNUtil.h"
#include "platform.h"

CVSNDraw3D::CVSNDraw3D() : CVSNTask(P_DRAW), m_env(0) {}
CVSNDraw3D::~CVSNDraw3D() {}


bool
CVSNDraw3D::init(const char * shaderPath, CGLFBO * fbo)
{
	bool result = false;

	// 3D描画環境を設定
	m_env = new C3DDrawEnv();
	m_env->init(fbo);
	CGLEnv::getInstance().setDrawEnv(CGLEnv::C3D, m_env);

	CVSNStorage * storage = CVSNPlatform::getInstance().Module<CVSNStorage>(PF_STORAGE, "DATA");
	const char * vpath = CVSNUtil::jointPath(shaderPath, "default3d.vsh");	// default vertex shader
	const char * fpath = CVSNUtil::jointPath(shaderPath, "default3d.fsh");	// default fragment shader
	size_t vsize, fsize;

	const char * srcVertex = (const char *)storage->readText(vpath, &vsize);
	const char * srcFragment = (const char *)storage->readText(fpath, &fsize);
	if (srcVertex && srcFragment) {
		m_shader = new C3DDefaultShader(m_env);
		m_shader->init(srcVertex, srcFragment);

		// 以下初期値
		C3DVec light(1.0f, 1.0f, 1.0f);
		m_env->setLightVec(light);	// 光源位置設定
		m_env->setLightCol(1.0f, 1.0f, 1.0f, 1.0f);		// 光源色設定
		m_env->setAmbient(0.05f, 0.05f, 0.05f, 1.0f);		// 環境光設定
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