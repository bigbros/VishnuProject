#include "CVSNTrackTest.h"


static CVSNFactory<CVSNTrackTest> factory("vrcamera");

CVSNTrackTest::CVSNTrackTest() : CVSNScriptTask() {}
CVSNTrackTest::~CVSNTrackTest() {}

bool
CVSNTrackTest::init(CVSNScriptArgs& args)
{
	bool result = true;
	m_track = CVSNPlatform::getInstance().Module<CVSNTracking>(PF_SENSOR, "TRACKER");
	m_env = CGLEnv::getInstance().DrawEnv<C3DDrawEnv>(CGLEnv::C3D);
	return result;
}

void
CVSNTrackTest::update(int deltaT)
{
	C3DQuat rot;
	C3DVec trans;
	m_track->getStatus(rot, trans);
	C3DCameraObj * camera = m_env->getCamera();
	camera->setRotation(rot);
}

void
CVSNTrackTest::die()
{
}

void
CVSNTrackTest::on_change()
{
}
