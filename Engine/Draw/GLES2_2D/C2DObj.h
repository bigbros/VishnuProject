#ifndef C2DObj_h
#define C2DObj_h

#include <GLES2/gl2.h>
#include "Types2D.h"
#include "dtypes.h"
#include "CGLBase.h"

class C2DObj : public CGLBase
{
	friend class C2DDrawEnv;
	friend class C2DRootObj;
private:
	struct OBJLIST {
		OBJLIST();
		C2DObj	*	begin;
		C2DObj	*	end;
	};
	struct OBJLINK {
		OBJLINK();
		C2DObj	*	prev;
		C2DObj	*	next;
	};
	C2DObj	*	m_parent;			// �e�I�u�W�F�N�g
	OBJLIST		m_childs;			// �q�I�u�W�F�N�g
	OBJLINK		m_sisters;			// �o���I�u�W�F�N�g

	bool		m_visible;	// �\�����

protected:
	bool			m_recalc;	// �ړ�/��]/���T�C�Y�����̂�matrix�̍Čv�Z���K�v
	C2DMat			m_matrix;	// ���g�̌v�Z�p�}�g���N�X

	C2DVec			m_position;	// ���[�J���ʒu
	float			m_rotation;	// ���[�J����]

	int				m_priority;	// �\���v���C�I���e�B
public:
	C2DObj();
	virtual ~C2DObj();

	inline void setVisible(bool visible) throw() {
		m_recalc = visible; // && !m_visible;
		m_visible = visible;
	}

	inline const C2DObj * getParent() const throw() { return m_parent; }

	void connectParent(C2DObj * parent, int matidx = 0);
	inline void addChild(C2DObj * child) throw() { child->connectParent(this); }

	inline void setRotation(const float rot) { m_rotation = rot;  m_recalc = true; }
	inline void setPosition(const C2DVec& vec) { m_position = vec;  m_recalc = true; }

	inline const float & getRotation() { return m_rotation; }
	inline const C2DVec  & getPosition() { return m_position; }

	bool setPriority(int pri);

protected:
	// ���̃I�u�W�F�N�g�ȉ��ɑ΂��A
	// �w�肳�ꂽ�}�g���N�X����ʃ}�g���N�X�Ƃ��čČv�Z���s���B
	void recalcMatrix(C2DMat& m, bool upper_recalc = false);

	// ���̃I�u�W�F�N�g�̃}�g���N�X�v�Z��ɍs���������L�q
	virtual bool calcProcedure(bool is_recalc) = 0;

	// �v���C�I���e�B���Đݒ肳�ꂽ�ۂɂ��ׂ�����
	virtual bool priorityProcedure(int priority);

private:
	void removeByParent();
};

#endif // C2DObj_h
