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
	
	std::auto_ptr<CVSNKVObj> camera_param(args.getTable(1));
	std::auto_ptr<CVSNKVObj> camera_pos(args.getTable(2));
	m_anime = false;
	if(args.argc() > 3) m_anime = args.getBool(3);

	C3DVec camera((*camera_pos)["x"], (*camera_pos)["y"], (*camera_pos)["z"], 1.0f);

	m_eyeAngle = F_PI / 2.0f;
	m_eyeNear = 10.0f;
	m_eyeFar = 300.0f;
	m_eyeAspect = 1.0f;
	if ((*camera_param).ContainsKey("angle")) m_eyeAngle = (*camera_param)["angle"];
	if ((*camera_param).ContainsKey("near")) m_eyeNear = (*camera_param)["near"];
	if ((*camera_param).ContainsKey("far")) m_eyeFar = (*camera_param)["far"];
	if ((*camera_param).ContainsKey("aspect")) m_eyeAspect = (*camera_param)["aspect"];


	C3DDrawEnv * env3d = CGLEnv::getInstance().DrawEnv<C3DDrawEnv>(CGLEnv::C3D);
	C3DObj * root = env3d->getRootObj();
	m_shaft = new C3DNullObj();
	m_shaft->connectParent(root);

	m_camera = new C3DCameraObj();
	m_camera->connectParent(m_shaft);

	float angle = m_camera->angleDiagonal2Horizontal(m_eyeAngle, (float)m_width, (float)m_height);
	m_camera->setView(angle, m_eyeAspect, m_eyeNear, m_eyeFar);
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

			C3DVec pole((float)deltaY, (float)deltaX, 0.0f);
			float theta = pole.len() * F_PI / (float)wide;
			C3DQuat rot(pole.unit(), theta);
			rot = m_preRot * rot;
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
	m_camera->setView(angle, m_eyeAspect, m_eyeNear, m_eyeFar);
}

float
CVSNTestCamera::calc_view(float angle)
{
	return angle;
}
