#include <stdio.h>
#include <memory>
#include "CVSNTestTask.h"
#include "CVSNJsonKVObj.h"
#include "platform.h"

// CVSNTestTask型タスクの起動をタスク名 "test" で登録する
static CVSNFactory<CVSNTestTask> factory("test");

// スクリプト側整数型定数の定義
static CVSNDefines::DEFINE_INT def_int[] = {
		{ "TEST_VALUE",  100 },
		{ "TEST_VALUE2", 200 },
		{ 0, 0 }
};

// スクリプト側文字列型定数の定義
static CVSNDefines::DEFINE_STR def_str[] = {
		{ "TEST_STR1", "define" },
		{ "TEST_STR2", "string" },
		{ 0, 0 }
};

// 定数の登録
static CVSNDefines define(def_int, def_str);




CVSNTestTask::CVSNTestTask() : CVSNScriptTask() {}
CVSNTestTask::~CVSNTestTask() {}

bool
CVSNTestTask::init(CVSNScriptArgs& args)
{
	bool result = true;
	
	LOG("Init: Model Test");

	m_width = CGLEnv::getInstance().width();
	m_height = CGLEnv::getInstance().height();

	std::auto_ptr<CVSNKVObj> kvModel(args.getTable(1));
	m_anime = false;
	if (args.argc() > 2) m_anime = args.getBool(2);

	m_model = new CVSN3DKVModel("test", kvModel.get());
	C3DObj * root = CGLEnv::getInstance().DrawEnv<C3DDrawEnv>(CGLEnv::C3D)->getRootObj();
	m_chara = new C3DCharacter(m_model);
	m_chara->connectParent(root);
	m_chara->setPosition(C3DVec(0.0f, 0.0f, 0.0f));
	m_chara->setVisible(true);

	return result;
}

void
CVSNTestTask::update(int deltaT)
{
	if (!m_anime) return;

	int wide = (m_width < m_height) ? m_width : m_height;

	CVSNTouchPanel& tpanel = CVSNTouchPanel::getInstance();
	tpanel.seekTop();
	const CVSNTouchPanel::ITEM * item;
	C3DQuat quat;
	C3DObj * pObj = m_chara;
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
			float theta = F_PI * pole.len() / (float)wide;
			C3DQuat rot(pole.unit(), theta);
			rot = rot * m_preRot;
			pObj->setRotation(rot);
		}
		case IVSNPointListener::PT_RELEASE: {
			break;
		}}
	}
}

void
CVSNTestTask::die()
{
	delete m_chara;
	delete m_model;
}

void
CVSNTestTask::on_change()
{
	m_width = CGLEnv::getInstance().width();
	m_height = CGLEnv::getInstance().height();
}

bool
CVSNTestTask::command(CVSNScriptArgs& args, CVSNScriptArgs& ret)
{
	CVSNKVObj * table = args.getTable(1);
	const char * json = CVSNJsonKVObj::ToJSON(table);
	LOG(json);
	vsnMemFree((void *)json);
	delete table;
	return true;
}
