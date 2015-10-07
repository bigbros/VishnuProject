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

	// �܂��͗��肪�Ȃ����Ƃɂ���
	for (int i = 0; i < 2; i++) m_handStatus[i].m_exist = false;

	// ���o�ł�����𓾂�B
	for (auto hand : frame.hands()) {
		// ���E�̔���
		int index = (hand.isRight()) ? 0 : 1;


		// �X�e�[�^�X�̊i�[��
		CVSNLeapHandStatus& stat = m_handStatus[index];

		if (!hand.isValid()) {
			stat.m_exist = false;
			continue;
		}

		stat.m_id = hand.id();

		// ��̈ʒu���擾���i�[
		Leap::Vector pos = hand.palmPosition();
		stat.m_position = m_conv.convert(pos);

		// �Ƃ肠�������ׂĂ̎w�̌��o���ʂ𖳌��ł��������Ƃɂ���
		for (int i = 0; i < 5; i++) stat.m_fingers[i].exist = false;

		// �w�����o����
		for (auto finger : hand.fingers()) {

			// �w�̎�ނ𓾂�
			Leap::Finger::Type type = finger.type();
			int fid = (int)type;			// �w��ID�́A�e�w��0�Ƃ��ď��w�Ɍ�������1����������
			CVSNLeapHandStatus::FINGER& fstat = stat.m_fingers[fid];

			// ��[�̈ʒu
			Leap::Vector& vec = finger.tipPosition();
			fstat.tip = m_conv.convert(vec);

			// �L�΂��Ă��邩�ۂ�
			fstat.extended = finger.isExtended();

			// ���̎w�̌��o���ʂ�L���ɂ���
			fstat.exist = true;
		}

		// ������̑��̎�͂��邱�Ƃɂ���B
		stat.m_exist = true;
	}
}

const CVSNLeapHandStatus&
CVSNLeapMotion::watchHandStatus(int side) {
	Leap::Frame& frame = m_leapCtrl.frame();
	updateFrame(frame);
	return m_handStatus[side];
}
