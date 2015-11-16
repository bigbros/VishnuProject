#include <exception>
#include "CGLAlloc.h"
#include "C3DObj.h"

C3DObj::OBJLIST::OBJLIST() : begin(0), end(0) {}
C3DObj::OBJLINK::OBJLINK() : prev(0), next(0) {}

C3DObj::C3DObj()
	: CGLBase()
	, m_parent(0)
	, m_parent_matidx(0)
	, m_visible(false)
	, m_recalc(true)
	, m_position()
	, m_rotation()
{
}

C3DObj::~C3DObj()
{
	// �q��������ċA�I�ɔj��
	C3DObj * child = m_childs.begin;
	while (child) {
		C3DObj * next = child->m_sisters.next;
		delete child;
		child = next;
	}
	// �e����ؒf
	removeByParent();
}


void
C3DObj::removeByParent()
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
C3DObj::connectParent(C3DObj * parent, int matidx)
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
	m_parent_matidx = matidx;
	m_recalc = true;	// �q���������̂ŗv�Čv�Z
}

void
C3DObj::recalcMatrix(C3DMat& m, bool upper_recalc)
{
	// ���������\������Ă��Ȃ���΁A�ȉ��̍Čv�Z�͕s�v�B
	if (!m_visible) return;

	// ��ʂŊ��ɍČv�Z���K�v�ɂȂ������A���邢�͎��g���v�Čv�Z�ł���� true �ɂȂ�B
	bool recalc = upper_recalc || m_recalc;

	if (recalc) {
		// �擪�̃}�g���N�X�����͊m���Ɍv�Z����B
		m_matrix = m_rotation;
		m_matrix += m_position;			// ���[�J���}�g���N�X�ɂȂ�
		m_matrix *= m;	// ������e�̃}�g���N�X��������ƃ��[���h�}�g���N�X�ɂȂ�
		m_recalc = false;	// �ēx�ύX�����܂ł͍Čv�Z�s�v(�e�������Ε�)
	}

	// ��]���Ȃǂ��v�Z������B
	calcProcedure(recalc);

	for (C3DObj * child = m_childs.begin; child; child = child->m_sisters.next) {
		child->recalcMatrix(m_matrix, recalc);
	}
}
