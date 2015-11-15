#include <exception>
#include "CGLAlloc.h"
#include "C2DObj.h"
#include "C2DTexOT.h"

C2DObj::OBJLIST::OBJLIST() : begin(0), end(0) {}
C2DObj::OBJLINK::OBJLINK() : prev(0), next(0) {}

C2DObj::C2DObj()
	: CGLBase()
	, m_parent(0)
	, m_childs()
	, m_sisters()
	, m_visible(false)
	, m_recalc(true)
	, m_position(0.0f, 0.0f)
	, m_rotation(0.0f)
	, m_priority(0)
{
}

C2DObj::~C2DObj()
{
	// �q��������ċA�I�ɔj��
	C2DObj * child = m_childs.begin;
	while (child) {
		C2DObj * next = child->m_sisters.next;
		delete child;
		child = next;
	}
	// �e����ؒf
	removeByParent();
}

void
C2DObj::removeByParent()
{
	// ���g��e�Ǝo������ؒf
	if (m_parent) {
		if (m_sisters.prev) {
			m_sisters.prev->m_sisters.next = m_sisters.next;
		}
		else {
			m_parent->m_childs.begin = m_sisters.next;
		}
		if (m_sisters.next) {
			m_sisters.next->m_sisters.prev = m_sisters.prev;
		}
		else {
			m_parent->m_childs.end = m_sisters.prev;
		}
	}
	m_parent = 0;
	m_sisters.prev = 0;
	m_sisters.next = 0;
}

void
C2DObj::connectParent(C2DObj * parent, int matidx)
{
	removeByParent();
	m_parent = parent;
	if (!m_parent) return;
	m_sisters.prev = m_parent->m_childs.end;
	if (m_sisters.prev) {
		m_sisters.prev->m_sisters.next = this;
	}
	else {
		m_parent->m_childs.begin = this;
	}
	m_parent->m_childs.end = this;
	m_recalc = true;	// �q���������̂ŗv�Čv�Z
}

void
C2DObj::recalcMatrix(C2DMat& m, bool upper_recalc)
{
	// ���������\������Ă��Ȃ���΁A�ȉ��̍Čv�Z�͕s�v�B
	if (!m_visible) return;

	// ��ʂŊ��ɍČv�Z���K�v�ɂȂ������A���邢�͎��g���v�Čv�Z�ł���� true �ɂȂ�B
	bool recalc = upper_recalc || m_recalc;

	if (recalc) {
		m_matrix = m_rotation;	// ��]�𔽉f
		m_matrix += m_position;	// ���[�J���}�g���N�X�ɂȂ�
		m_matrix *= m;			// ������e�̃}�g���N�X��������ƃ��[���h�}�g���N�X�ɂȂ�

		m_recalc = false;	// �ēx�ύX�����܂ł͍Čv�Z�s�v(�e�������Ε�)
	}

	// �擪�ȊO�̃}�g���N�X�͎����Ōv�Z������B
	calcProcedure(recalc);

	// ���v�Z�����}�g���N�X���q�ɓn���Čv�Z������B
	for (C2DObj * child = m_childs.begin; child; child = child->m_sisters.next) {
		child->recalcMatrix(m_matrix, recalc);
	}
}

bool
C2DObj::setPriority(int pri)
{
	if (pri < 0 || pri >= C2DTexOT::C2D_MAX_PRIORITY) return false;
	bool result = priorityProcedure(pri);
	m_priority = pri;
	return result;
}

bool
C2DObj::priorityProcedure(int priority)
{
	return true;
}