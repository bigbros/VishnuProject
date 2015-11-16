#ifndef C3DObj_h
#define C3DObj_h

#include <GLES2/gl2.h>
#include "Types3D.h"
#include "dtypes.h"
#include "CGLBase.h"

class C3DObj : public CGLBase
{
	friend class C3DRootObj;
private:
	struct OBJLIST {
		OBJLIST();
		C3DObj	*	begin;
		C3DObj	*	end;
	};
	struct OBJLINK {
		OBJLINK();
		C3DObj	*	prev;
		C3DObj	*	next;
	};
	C3DObj	*	m_parent;			// �e�I�u�W�F�N�g
	int			m_parent_matidx;	// �e�I�u�W�F�N�g�̎����Ԗڂ̃}�g���N�X�ɒǏ]���邩(�ʏ��0)
	OBJLIST		m_childs;			// �q�I�u�W�F�N�g
	OBJLINK		m_sisters;			// �o���I�u�W�F�N�g

	bool		m_visible;	// �\�����
	bool		m_recalc;	// �ړ�/��]�����̂�matrix�̍Čv�Z���K�v

protected:

	C3DMat		m_matrix;	// �ʒu/��]������킷 matrix(��ȏ�)

	C3DVec		m_position;	// ���[�J���ʒu
	C3DQuat		m_rotation;	// ���[�J����]


public:
	C3DObj();
	virtual ~C3DObj();

	inline void setVisible(bool visible) throw() {
		m_recalc = visible; // && !m_visible;
		m_visible = visible;
	}

	inline const C3DObj * getParent() const throw() { return m_parent; }

	void connectParent(C3DObj * parent, int matidx = 0);
	inline void addChild(C3DObj * child) throw() { child->connectParent(this); }

	inline void setRotation(const C3DQuat& quat) { m_rotation = quat;  m_recalc = true; }
	inline void setPosition(const C3DVec& vec) { m_position = vec;  m_recalc = true; }

	inline const C3DQuat & getRotation() { return m_rotation; }
	inline const C3DVec  & getPosition() { return m_position; }

protected:

	// ���̃I�u�W�F�N�g�ȉ��ɑ΂��A
	// �w�肳�ꂽ�}�g���N�X����ʃ}�g���N�X�Ƃ��čČv�Z���s���B
	void recalcMatrix(C3DMat& m, bool upper_recalc = false);

	// ���̃I�u�W�F�N�g�̃}�g���N�X�v�Z��ɍs���������L�q
	virtual bool calcProcedure(bool is_recalc) = 0;

private:
	void removeByParent();
};

#endif // C3DObj_h
