#include <stdio.h>
#include <memory.h>
#include "CVSNModelTest.h"
#include "platform.h"

// CVSNModelTest型タスクの起動をタスク名 "test2" で登録する
static CVSNFactory<CVSNModelTest> factory("model");

CVSNModelTest::CVSNModelTest() : CVSNScriptTask() {}
CVSNModelTest::~CVSNModelTest() {}

bool
CVSNModelTest::init(CVSNScriptArgs& args)
{
	bool result = true;

	LOG("Init: Model Test\n");

	m_width = CGLEnv::getInstance().width();
	m_height = CGLEnv::getInstance().height();

	m_anime = false;
	if (args.argc() > 3) m_anime = args.getBool(3);


	std::auto_ptr<CVSNKVObj> pos(args.getTable(1));
	C3DDrawEnv * env = CGLEnv::getInstance().DrawEnv<C3DDrawEnv>(CGLEnv::C3D);

	C3DDefaultShader * shader = (C3DDefaultShader *)env->findShader(C3DDefaultShader::SID_DEFAULT);

	const char * filename = args.getString(2);

	m_model = new CVSN3DVMDModel(shader, "test", filename);

	C3DObj * root = env->getRootObj();
	m_null = new C3DNullObj();
	m_null->connectParent(root);
	m_null->setPosition(C3DVec(0.0f, 0.0f, 0.0f));
	m_null->setVisible(true);

	m_chara = new C3DCharacter(m_model);
	m_chara->connectParent(m_null);
	m_chara->setPosition(C3DVec((*pos)["x"], (*pos)["y"], (*pos)["z"]));
	m_chara->setVisible(true);

	return result;
}

void
CVSNModelTest::update(int deltaT)
{
	if (!m_anime) return;

	int wide = (m_width < m_height) ? m_width : m_height;

	CVSNTouchPanel& tpanel = CVSNTouchPanel::getInstance();
	tpanel.seekTop();
	const CVSNTouchPanel::ITEM * item;
	C3DObj * pObj = m_null;

	while (0 != (item = tpanel.getItem())) {
		switch (item->ev) {
		case IVSNPointListener::PT_TAP:
			m_preX = item->x;
			m_preY = item->y;
			//m_preRot = pObj->getBoneRotation(2);
			m_preRot = pObj->getRotation();
			break;
		case IVSNPointListener::PT_DRAG: {
			int deltaX = item->x - m_preX;
			int deltaY = item->y - m_preY;
			if (!deltaX && !deltaY) break;

			C3DVec pole((float)deltaY, (float)deltaX, 0.0f);
			float theta = F_PI * pole.len() / (float)wide;
			C3DQuat rot(pole.unit(), theta);

			rot = rot * m_preRot;
			//pObj->setBoneRotation(2, rot);
			pObj->setRotation(rot);
		}
		case IVSNPointListener::PT_RELEASE: {
			break;
		}
		}
	}
}

void
CVSNModelTest::die()
{
	delete m_chara;
	delete m_model;
}

void
CVSNModelTest::on_change()
{
	m_width = CGLEnv::getInstance().width();
	m_height = CGLEnv::getInstance().height();
}
