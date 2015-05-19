#include "CVSNCelestialSphere.h"
#include "CVSNPNGTex.h"
#include "CGLEnv.h"

// CVSNCelestialSphere型タスクの起動をタスク名 "sysCelestialSphere" で登録する
static CVSNFactory<CVSNCelestialSphere> factory("sysCelestialSphere");

CVSNCelestialSphere::CVSNCelestialSphere() : CVSNScriptTask(), m_sphere(0), m_pTex(0) {}
CVSNCelestialSphere::~CVSNCelestialSphere() {}

bool
CVSNCelestialSphere::init(CVSNScriptArgs& args)
{
	bool result = true;
	const char * tex_name = args.getString(1);

	try {
		C3DDrawEnv * env = CGLEnv::getInstance().DrawEnv<C3DDrawEnv>(CGLEnv::C3D);
		m_pTex = new CVSNPNGTex("tex", tex_name);
		m_sphere = new C3DCelestialSphere(m_pTex);
		env->setCelestialSphere(m_sphere);
	}
	catch (std::bad_alloc& ex) {
		delete m_sphere;
		delete m_pTex;
		result = false;
	}
	return result;
}

void
CVSNCelestialSphere::die()
{
	CGLEnv::getInstance().DrawEnv<C3DDrawEnv>(CGLEnv::C3D)->setCelestialSphere(0);
	delete m_sphere;
	delete m_pTex;
}

void
CVSNCelestialSphere::update(int deltaT) {}
