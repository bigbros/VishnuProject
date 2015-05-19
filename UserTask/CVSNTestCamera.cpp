#include <stdio.h>
#include <memory>
#include "CVSNTestCamera.h"
#include "platform.h"

// CVSNTestTask型タスクの起動をタスク名 "test" で登録する
static CVSNFactory<CVSNTestCamera> factory("camera");



CVSNTestCamera::CVSNTestCamera() : CVSNScriptTask() {}
CVSNTestCamera::~CVSNTestCamera() {}

bool
CVSNTestCamera::init(CVSNScriptArgs& args)
{
	bool result = true;

	LOG("Init: Camera\n");

	m_width = CGLEnv::getInstance().width();
	m_height = CGLEnv::getInstance().height();

	std::auto_ptr<CVSNKVObj> camera_pos(args.getTable(1));
	m_anime = false;
	if(args.argc() > 2) m_anime = args.getBool(2);

	C3DVec camera((*camera_pos)["x"], (*camera_pos)["y"], (*camera_pos)["z"], 1.0f);

	C3DDrawEnv * env3d = CGLEnv::getInstance().DrawEnv<C3DDrawEnv>(CGLEnv::C3D);
	C3DObj * root = env3d->getRootObj();
	m_shaft = new C3DNullObj();
	m_shaft->connectParent(root);

	m_camera = new C3DCameraObj();
	m_camera->connectParent(m_shaft);

	m_eyeAngle = F_PI / 2.0f;
	float angle = m_camera->angleDiagonal2Horizontal(m_eyeAngle, (float)m_width, (float)m_height);
	m_camera->setView(angle, 1.0f, 10.0f, 300.0f);
	m_camera->setPosition(camera);
	m_shaft->setPosition(C3DVec(0.0f, 0.0f, 0.0f));

	env3d->setCamera(m_camera);
	m_camera->setVisible(true);
	m_shaft->setVisible(true);

	return result;
}

void
CVSNTestCamera::update(int deltaT)
{
	if (!m_anime) return;
	int wide = (m_width < m_height) ? m_width : m_height;

	CVSNTouchPanel& tpanel = CVSNTouchPanel::getInstance();
	tpanel.seekTop();
	const CVSNTouchPanel::ITEM * item;
	C3DQuat quat;
	C3DObj * pObj = m_shaft;
	while (0 != (item = tpanel.getItem())) {
		switch (item->ev) {
		case IVSNPointListener::PT_TAP:
			m_preX = item->x;
			m_preY = item->y;
			m_preRot = pObj->getRotation();
			break;
		case IVSNPointListener::PT_DRAG: {
			int deltaX = item->x - m_preX;
			int deltaY = item->y - m_preY;
			if (!deltaX && !deltaY) break;

			C3DVec pole((float)-deltaY, (float)-deltaX, 0.0f);
			float theta = F_PI * pole.len() / (float)wide;
			C3DQuat rot(pole.unit(), theta);
			rot = rot * m_preRot;
			pObj->setRotation(rot);
		}
		case IVSNPointListener::PT_RELEASE: {
			break;
		}
		}
	}

}

void
CVSNTestCamera::die()
{
	delete m_camera;
	delete m_shaft;
}

void
CVSNTestCamera::on_change()
{
	m_width = CGLEnv::getInstance().width();
	m_height = CGLEnv::getInstance().height();

	float angle = m_camera->angleDiagonal2Horizontal(m_eyeAngle, (float)m_width, (float)m_height);
	m_camera->setView(angle, 1.0f, 10.0f, 300.0f);
}

float
CVSNTestCamera::calc_view(float angle)
{
	return angle;
}
