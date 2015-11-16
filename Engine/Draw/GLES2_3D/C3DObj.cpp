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
	// 子がいたら再帰的に破棄
	C3DObj * child = m_childs.begin;
	while (child) {
		C3DObj * next = child->m_sisters.next;
		delete child;
		child = next;
	}
	// 親から切断
	removeByParent();
}


void
C3DObj::removeByParent()
{
	// 自身を親と姉妹から切断
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
	m_recalc = true;	// 繋ぎ直したので要再計算
}

void
C3DObj::recalcMatrix(C3DMat& m, bool upper_recalc)
{
	// そもそも表示されていなければ、以下の再計算は不要。
	if (!m_visible) return;

	// 上位で既に再計算が必要になったか、あるいは自身が要再計算であれば true になる。
	bool recalc = upper_recalc || m_recalc;

	if (recalc) {
		// 先頭のマトリクスだけは確実に計算する。
		m_matrix = m_rotation;
		m_matrix += m_position;			// ローカルマトリクスになる
		m_matrix *= m;	// 貰った親のマトリクスをかけるとワールドマトリクスになる
		m_recalc = false;	// 再度変更されるまでは再計算不要(親が動けば別)
	}

	// 回転情報などを計算させる。
	calcProcedure(recalc);

	for (C3DObj * child = m_childs.begin; child; child = child->m_sisters.next) {
		child->recalcMatrix(m_matrix, recalc);
	}
}
