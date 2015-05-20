#include "C3DDrawEnv.h"
#include "CVSNSkyDome.h"
#include "CVSNPNGTex.h"
#include "CGLEnv.h"
#include "CVSNStorage.h"

// CVSNSkyDome型タスクの起動をタスク名 "sysCelestialSphere" で登録する
static CVSNFactory<CVSNSkyDome> factory("sysSkyDome");

CVSNSkyDome::CVSNSkyDome() : CVSNScriptTask(), m_skydome(0), m_pTex(0), m_shader(0) {}
CVSNSkyDome::~CVSNSkyDome() {}

bool
CVSNSkyDome::init(CVSNScriptArgs& args)
{
	bool result = true;
	const char * tex_name = args.getString(1);
	float r = args.getFloat(2);

	try {

		C3DDrawEnv * env = CGLEnv::getInstance().DrawEnv<C3DDrawEnv>(CGLEnv::C3D);
		CVSNStorage * storage = CVSNPlatform::getInstance().Module<CVSNStorage>(PF_STORAGE, "DATA");
		const char * vpath = "shaders/skydome.vsh";	// vertex shader
		const char * fpath = "shaders/skydome.fsh";	// fragment shader
		size_t vsize, fsize;

		const char * srcVertex = (const char *)storage->readText(vpath, &vsize);
		const char * srcFragment = (const char *)storage->readText(fpath, &fsize);
		if (srcVertex && srcFragment) {
			m_shader = new C3DSkyDomeShader(env);
			m_shader->init(srcVertex, srcFragment);

			m_pTex = new CVSNPNGTex("tex", tex_name);
			m_skydome = new C3DSkyDome(m_shader, m_pTex, r);
		}
	}
	catch (std::bad_alloc& ex) {
		delete m_skydome;
		delete m_pTex;
		delete m_shader;
		result = false;
	}
	return result;
}

void
CVSNSkyDome::die()
{
//	CGLEnv::getInstance().DrawEnv<C3DDrawEnv>(CGLEnv::C3D)->setSkyDome(0);
	delete m_skydome;
	delete m_pTex;
	delete m_shader;
}

void
CVSNSkyDome::update(int deltaT) {}
