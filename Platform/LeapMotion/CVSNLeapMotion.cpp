#include "CVSNLeapMotion.h"


CVSNLeapListener::CVSNLeapListener(CVSNLeapMotion * module)
	: Leap::Listener()
	, m_leapModule(module)
{
}

CVSNLeapListener::~CVSNLeapListener()
{
}

void
CVSNLeapListener::onFrame(const Leap::Controller& controller)
{
	Leap::Frame frame = controller.frame();
	m_leapModule->updateFrame(frame);
}



CVSNLeapConverter::CVSNLeapConverter() {}
CVSNLeapConverter::~CVSNLeapConverter() {}

void
CVSNLeapConverter::update(Leap::Frame& frame)
{
	m_iBox = frame.interactionBox();

	Leap::Vector& center = m_iBox.center();

	m_width = m_iBox.width();
	m_height = m_iBox.height();
	m_depth = m_iBox.depth();

	float w = m_width / 2.0f;
	float h = m_height / 2.0f;
	float d = m_depth / 2.0f;

	m_left = center.x - w;
	m_right = center.x + w;

	m_top = center.y - h;
	m_bottom = center.y + h;

	m_back = center.z - d;
	m_front = center.z + d;
}

const C3DVec&
CVSNLeapConverter::convert(Leap::Vector& vec)
{
	return C3DVec(vec.x, vec.y, vec.z);
	/*
	Leap::Vector normalized = m_iBox.normalizePoint(vec);
	float x = normalized.x * m_width + m_left;
	float y = normalized.y * m_height + m_bottom;
	float z = normalized.z * m_depth + m_back;
	return C3DVec(x, y, z);
	*/
}



CVSNLeapMotion::CVSNLeapMotion()
	: IVSNModule(PF_SENSOR, "LEAP")
	, m_leapCtrl()
	, m_listener(nullptr)
	, m_conv()
{
}

CVSNLeapMotion::~CVSNLeapMotion()
{
	if(m_listener) m_leapCtrl.removeListener(*m_listener);
	delete m_listener;
}


bool
CVSNLeapMotion::init()
{
//	m_listener = new CVSNLeapListener(this);
//	m_leapCtrl.addListener(*m_listener);
	for (int i = 0; i < 2; i++) m_handStatus[i].m_exist = false;

	return true;
}


void
CVSNLeapMotion::updateFrame(Leap::Frame& frame)
{
	// m_conv.update(frame);

	// まずは両手がないことにする
	for (int i = 0; i < 2; i++) m_handStatus[i].m_exist = false;

	// 検出できた手を得る。
	for (auto hand : frame.hands()) {
		// 左右の判別
		int index = (hand.isRight()) ? 0 : 1;


		// ステータスの格納先
		CVSNLeapHandStatus& stat = m_handStatus[index];

		if (!hand.isValid()) {
			stat.m_exist = false;
			continue;
		}

		stat.m_id = hand.id();

		// 手の位置を取得し格納
		Leap::Vector pos = hand.palmPosition();
		stat.m_position = m_conv.convert(pos);

		// とりあえずすべての指の検出結果を無効であったことにする
		for (int i = 0; i < 5; i++) stat.m_fingers[i].exist = false;

		// 指を検出する
		for (auto finger : hand.fingers()) {

			// 指の種類を得る
			Leap::Finger::Type type = finger.type();
			int fid = (int)type;			// 指のIDは、親指を0として小指に向かって1ずつ増加する
			CVSNLeapHandStatus::FINGER& fstat = stat.m_fingers[fid];

			// 先端の位置
			Leap::Vector& vec = finger.tipPosition();
			fstat.tip = m_conv.convert(vec);

			// 伸ばしているか否か
			fstat.extended = finger.isExtended();

			// その指の検出結果を有効にする
			fstat.exist = true;
		}

		// そちらの側の手はあることにする。
		stat.m_exist = true;
	}
}

const CVSNLeapHandStatus&
CVSNLeapMotion::watchHandStatus(int side) {
	Leap::Frame& frame = m_leapCtrl.frame();
	updateFrame(frame);
	return m_handStatus[side];
}
