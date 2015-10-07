#ifndef CVSNLeapMotion_h
#define CVSNLeapMotion_h


#include "CVSNPlatform.h"
#include "Types3D.h"
#include "Leap.h"


class CVSNLeapMotion;

// LeapMotion から取得した手の状態。
// あくまでも LeapMotion から見た位置であることに注意。
// LeapMotion がどのように用いられるかによって、値の扱い方は異なる。
// 例1) 机上に置いて用いる場合
// 例2) Oculus Rift 前面に装着して用いる場合
class CVSNLeapHandStatus
{
	friend class CVSNLeapMotion;
public:
	struct FINGER {
		C3DVec	tip;		// 先端の座標

		bool extended;		// 伸ばしているか否か
		bool exist;		// 検出結果が有効かどうか
	};

	inline const bool isExist() const { return m_exist; }
	inline const C3DVec& position() const { return m_position; }
	inline const FINGER& finger(int id) const { return m_fingers[id]; }

	inline const s32 id() const { return m_id; }

private:
	bool	m_exist;		// この手が有効か否か
	s32		m_id;			// 手のID

	C3DVec	m_position;		// 手の位置

	FINGER	m_fingers[5];		// それぞれの指の状態: [0]親指 / [4]小指
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
