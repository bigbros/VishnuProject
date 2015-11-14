#if defined(WIN32) || defined(WIN64)
#include "CVSNLeapTest.h"
#include "platform.h"

static CVSNFactory<CVSNLeapTest> factory("leapmotion");

CVSNLeapTest::CVSNLeapTest() : CVSNScriptTask() {}
CVSNLeapTest::~CVSNLeapTest() {}

bool
CVSNLeapTest::init(CVSNScriptArgs& args)
{
	bool result = true;

	m_leap = CVSNPlatform::getInstance().Module<CVSNLeapMotion>(PF_SENSOR, "LEAP");

	m_env = CGLEnv::getInstance().DrawEnv<C3DDrawEnv>(CGLEnv::C3D);

	C3DDefaultShader * shader = (C3DDefaultShader *)m_env->findShader(C3DDefaultShader::SID_DEFAULT);
	std::auto_ptr<CVSNKVObj> kvModel(args.getTable(1));
	m_model = new CVSN3DKVModel(shader, "point", kvModel.get());

	// 手首の位置を表すオブジェクトを作成し、カメラの子としてつける。
	m_right = new C3DCharacter(m_model);
	m_right->setPosition(C3DVec());
	m_right->setVisible(false);
	m_right->setColor(C3DVec(1.0f, 0.0f, 0.0f, 1.0f));

	m_left = new C3DCharacter(m_model);
	m_left->setPosition(C3DVec());
	m_left->setVisible(false);
	m_left->setColor(C3DVec(0.0f, 0.0f, 1.0f, 1.0f));

	return result;
}

void
CVSNLeapTest::update(int deltaT)
{
	// カメラオブジェクト取得
	C3DCameraObj * camera = m_env->getCamera();
	m_right->connectParent(camera);
	m_left->connectParent(camera);


	const CVSNLeapHandStatus& rightHand = m_leap->watchHandStatus(0);
	const CVSNLeapHandStatus& leftHand = m_leap->watchHandStatus(1);

	m_right->setVisible(rightHand.isExist());
	m_left->setVisible(leftHand.isExist());

	if (rightHand.isExist()) {
		const C3DVec& pos = rightHand.position();
		C3DVec v(-pos.x / 1000.0f, -pos.z / 1000.0f, -pos.y / 1000.0f - 0.1f);
		m_right->setPosition(v);
	}

	if (leftHand.isExist()) {
		const C3DVec& pos = leftHand.position();
		C3DVec v(-pos.x / 1000.0f, -pos.z / 1000.0f, -pos.y / 1000.0f - 0.1f);
		m_left->setPosition(v);
	}
}

void
CVSNLeapTest::die()
{
	delete m_right;
	delete m_left;
	delete m_model;
}

void
CVSNLeapTest::on_change()
{
}
#endif