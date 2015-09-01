#ifndef CCol3DGroup_h
#define CCol3DGroup_h

#include "Types3D.h"
#include "CColBase.h"

// BoundingBox
struct CCol3DBound {
	C3DVec	min;
	C3DVec	max;

	// ������
	inline void init(C3DVec& vec) {
		min = vec;
		max = vec;
	}

	// �v�f�ǉ�
	inline void update(C3DVec& vec) {
		min.x = (min.x < vec.x) ? min.x : vec.x;
		min.y = (min.y < vec.y) ? min.y : vec.y;
		min.z = (min.z < vec.z) ? min.z : vec.z;

		max.x = (max.x < vec.x) ? vec.x : max.x;
		max.y = (max.y < vec.y) ? vec.y : max.y;
		max.z = (max.z < vec.z) ? vec.z : max.z;
	}

	// ����BoundingBox�Ɨ̈悪��邩�̔���
	inline bool isOverlap(CCol3DBound& t) {
		return
			(min.x <= t.max.x && max.x >= t.min.x) &&
			(min.y <= t.max.y && max.y >= t.max.y) &&
			(min.z <= t.max.z && max.z >= t.max.z);
	}
};


class CCol3DGroup;

// Collider object �̊��
class CCol3D : public CColBase
{
	friend class CCol3DGroup;
public:
	enum TYPE {
		C_MESH,		// polygon mesh
		C_BOX,		// oriented box
		C_SPHERE,	// sphere
		C_LINE,		// line strip
		C_POINT		// point
	};
private:
	TYPE				m_type;
	CCol3DGroup		*	m_parent;
	CCol3D			*	m_prev;
	CCol3D			*	m_next;

	C3DMat			*	m_ref;		// �Q�ƍs��
	u32					m_bit;		// �X�V��Ԃ��Q�Ƃ���r�b�g�}�X�N
	u32				*	m_flag;		// �}�g���N�X�X�V��Ԃ����f�����u32�l

	bool				m_enable;	// ���̔����L���ɂ��邩�ۂ�

	// m_flag �Ŏw�������u32�l��m_bit��and���ʂ�0�łȂ���΍��W�̍Čv�Z���s����B

protected:
	CCol3DBound			m_bound;	// ���̔���̎�����BoundingBox
public:
	CCol3D(TYPE type, C3DMat * ref, u32 * flag, u32 bit);
	virtual ~CCol3D();

	inline void setEnabled(bool enable) { m_enable = enable; }
	void strip();

	inline void update() {
		// ���̔��肪���ݗL���ŁA�X�V�X�e�[�^�X�̃|�C���^���w�肳��Ă���A�}�X�N�r�b�g��0�łȂ����̂�����΍Čv�Z���s��
		if (m_enable && m_flag && *m_flag & m_bit) update(*m_ref);
	}
	virtual void update(C3DMat& m) = 0;
};


class CCol3DGroup : public CColBase
{
	friend class CCol3D;
	friend class CCol3DGroupMgr;
private:


	CCol3DGroup	*	m_prev;
	CCol3DGroup	*	m_next;

	CCol3D		*	m_begin;
	CCol3D		*	m_end;

	u32				m_gID;	// �O���[�v�ɂ�������ID

	int				m_count;	// �ێ����Ă���A�C�e����

public:
	CCol3DGroup(u32 gID);
	virtual ~CCol3DGroup();

	void add(CCol3D * collider);	// collider �ǉ�

	void strip();					// manager ���珜��

	void update();					// �o�^�S�I�u�W�F�N�g�ɂ��āA�K�v�Ȃ��͍̂Čv�Z

	// ���̃O���[�v�Ƃ̊ԂŔ�����s��
	void judge(CCol3DGroup * target);
};

class CCol3DGroupMgr
{
	friend class CCol3DGroup;
private:
	CCol3DGroup		*	m_begin;
	CCol3DGroup		*	m_end;
private:
	CCol3DGroupMgr();
	~CCol3DGroupMgr();
public:
	static CCol3DGroupMgr& getInstance();

	void add(CCol3DGroup * grp);
	CCol3DGroup * findGroup(u32 gID);
};

#endif // CCol3DGROUP_h
