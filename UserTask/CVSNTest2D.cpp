#include <stdio.h>
#include <memory>
#include "CVSNTest2D.h"
#include "CVSNKVObj.h"
#include "platform.h"
#include "c2dgles.h"

// CVSNTest2D型タスクの起動をタスク名 "test" で登録する
static CVSNFactory<CVSNTest2D> factory("test2d");

// スクリプト側整数型定数の定義
static CVSNDefines::DEFINE_INT def_int[] = {
		{ "TEST_VALUE", 100 },
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




CVSNTest2D::CVSNTest2D() : CVSNScriptTask() {}
CVSNTest2D::~CVSNTest2D() {}

bool
CVSNTest2D::init(CVSNScriptArgs& args)
{
	bool result = true;

	LOG("Init: 2D Test");

	m_width = CGLEnv::getInstance().width();
	m_height = CGLEnv::getInstance().height();

	std::auto_ptr<CVSNKVObj> kvParam(args.getTable(1));
	std::auto_ptr<CVSNKVObj> kvTex(args.getTable(2));
	m_anime = false;
	if (args.argc() > 3) m_anime = args.getBool(3);

	C2DObj * root = CGLEnv::getInstance().DrawEnv<C2DDrawEnv>(CGLEnv::C2D)->getRootObj();
	m_tex[0] = new CVSNPNGTex("texture", (*kvTex)[0]);
	m_tex[1] = new CVSNPNGTex("texture", (*kvTex)[1]);
	char buf[1024];
	sprintf(buf, "tex[0]:%p tex[1]:%p\n", m_tex[0], m_tex[1]);
	LOG(buf);

	m_ctrl = new C2DNullObj();
	m_ctrl->connectParent(root);
	m_ctrl->setVisible(true);
	m_ctrl->setPosition(C2DVec(-(float)(*kvParam)["x"], -(float)(*kvParam)["y"]));

	for (int i = 0; i < 5; i++) {
		m_sprite[i] = new C2DSprite();
		m_sprite[i]->connectParent(m_ctrl);
		m_sprite[i]->setSize((*kvParam)["width"], (*kvParam)["height"]);
		m_sprite[i]->setPosition(C2DVec(
			(float)(*kvParam)["x"] + i*(float)(*kvParam)["width"]/2.0f,
			(float)(*kvParam)["y"] + i*(float)(*kvParam)["height"]/2.0f));
		int idx = (i == 2) ? 1 : 0;
		m_sprite[i]->setTex(m_tex[idx]);
		C3DVec rgba(
			(*kvParam)["r"],
			(*kvParam)["g"],
			(*kvParam)["b"],
			(*kvParam)["a"]);
		C2DVec lt(0.0f, 0.0f);
		C2DVec rb(256, 128.0f);
		m_sprite[i]->setRGBA(rgba);
		m_sprite[i]->setUVpx(lt , rb);
		m_sprite[i]->setVisible(true);
		m_sprite[i]->setPriority(i);
	}
	m_rot_deg = 0;
	return result;
}

void
CVSNTest2D::update(int deltaT)
{
	if (!m_anime) return;

	m_rot_deg += 3;
	m_rot_deg %= 360;
	float rot = (float)m_rot_deg * F_PI / 180.0f;
	m_ctrl->setRotation(rot);

//	int wide = (m_width < m_height) ? m_width : m_height;

	CVSNTouchPanel& tpanel = CVSNTouchPanel::getInstance();
	tpanel.seekTop();
	const CVSNTouchPanel::ITEM * item;
	while (0 != (item = tpanel.getItem())) {
		switch (item->ev) {
		case IVSNPointListener::PT_TAP:
			m_preX = item->x;
			m_preY = item->y;
			m_pos = m_ctrl->getPosition();
			break;
		case IVSNPointListener::PT_DRAG: {
		}
		case IVSNPointListener::PT_RELEASE: {
			int deltaX = item->x - m_preX;
			int deltaY = item->y - m_preY;
			C2DVec vec(m_pos.x + deltaX, m_pos.y + deltaY);
			m_ctrl->setPosition(vec);

			break;
		}
		}
	}
}

void
CVSNTest2D::die()
{
	for (int i = 0; i < 5; i++) {
		delete m_sprite[i];
	}
	delete m_tex[0];
	delete m_tex[1];
}

void
CVSNTest2D::on_change()
{
	m_width = CGLEnv::getInstance().width();
	m_height = CGLEnv::getInstance().height();
}

