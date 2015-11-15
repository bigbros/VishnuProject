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
	// 子がいたら再帰的に破棄
	C2DObj * child = m_childs.begin;
	while (child) {
		C2DObj * next = child->m_sisters.next;
		delete child;
		child = next;
	}
	// 親から切断
	removeByParent();
}

void
C2DObj::removeByParent()
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
	m_recalc = true;	// 繋ぎ直したので要再計算
}

void
C2DObj::recalcMatrix(C2DMat& m, bool upper_recalc)
{
	// そもそも表示されていなければ、以下の再計算は不要。
	if (!m_visible) return;

	// 上位で既に再計算が必要になったか、あるいは自身が要再計算であれば true になる。
	bool recalc = upper_recalc || m_recalc;

	if (recalc) {
		m_matrix = m_rotation;	// 回転を反映
		m_matrix += m_position;	// ローカルマトリクスになる
		m_matrix *= m;			// 貰った親のマトリクスをかけるとワールドマトリクスになる

		m_recalc = false;	// 再度変更されるまでは再計算不要(親が動けば別)
	}

	// 先頭以外のマトリクスは自分で計算させる。
	calcProcedure(recalc);

	// 今計算したマトリクスを子に渡して計算させる。
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