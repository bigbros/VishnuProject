#ifndef CVSNLeapMotion_h
#define CVSNLeapMotion_h


#include "CVSNPlatform.h"
#include "Types3D.h"
#include "Leap.h"


class CVSNLeapMotion;

// LeapMotion ����擾������̏�ԁB
// �����܂ł� LeapMotion ���猩���ʒu�ł��邱�Ƃɒ��ӁB
// LeapMotion ���ǂ̂悤�ɗp�����邩�ɂ���āA�l�̈������͈قȂ�B
// ��1) ����ɒu���ėp����ꍇ
// ��2) Oculus Rift �O�ʂɑ������ėp����ꍇ
class CVSNLeapHandStatus
{
	friend class CVSNLeapMotion;
public:
	struct FINGER {
		C3DVec	tip;		// ��[�̍��W

		bool extended;		// �L�΂��Ă��邩�ۂ�
		bool exist;		// ���o���ʂ��L�����ǂ���
	};

	inline const bool isExist() const { return m_exist; }
	inline const C3DVec& position() const { return m_position; }
	inline const FINGER& finger(int id) const { return m_fingers[id]; }

	inline const s32 id() const { return m_id; }

private:
	bool	m_exist;		// ���̎肪�L�����ۂ�
	s32		m_id;			// ���ID

	C3DVec	m_position;		// ��̈ʒu

	FINGER	m_fingers[5];		// ���ꂼ��̎w�̏��: [0]�e�w / [4]���w
};


class CVSNLeapListener : public Leap::Listener
{
public:
	CVSNLeapListener(CVSNLeapMotion * module);
	virtual ~CVSNLeapListener();

	void onFrame(const Leap::Controller& controller);
private:
	CVSNLeapMotion		*	m_leapModule;
};

class CVSNLeapConverter
{
public:
	CVSNLeapConverter();
	virtual ~CVSNLeapConverter();

	void update(Leap::Frame& frame);

	const C3DVec& convert(Leap::Vector& vec);

private:
	Leap::InteractionBox		m_iBox;

	float		m_width;
	float		m_height;
	float		m_depth;

	float		m_left;
	float		m_right;
	float		m_top;
	float		m_bottom;
	float		m_back;
	float		m_front;
};




class CVSNLeapMotion : public IVSNModule
{
	friend class CVSNLeapListener;
public:
	CVSNLeapMotion();
	virtual ~CVSNLeapMotion();

	bool init();

	const CVSNLeapHandStatus& watchHandStatus(int side);
private:
	void updateFrame(Leap::Frame& frame);
	const C3DVec& convert(Leap::InteractionBox& iBox, Leap::Vector& vec);

private:
	Leap::Controller		m_leapCtrl;
	CVSNLeapListener	*	m_listener;

	CVSNLeapHandStatus		m_handStatus[2];

	CVSNLeapConverter		m_conv;
};

#endif // CVSNLeapMotion_h
