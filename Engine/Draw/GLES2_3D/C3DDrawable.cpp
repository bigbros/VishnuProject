#include "C3DDrawable.h"

C3DDrawObj::DRAWLINK::DRAWLINK() : prev(0), next(0) {}

C3DDrawObj::C3DDrawObj(int matnum)
	: C3DObj(matnum)
	, m_drawlink()
	, m_drawable(0)
{
}

C3DDrawObj::~C3DDrawObj()
{
	useDrawable(0);
}

void
C3DDrawObj::useDrawable(C3DDrawable * drawable)
{
	// 既に設定されている drawable があるなら、そのdrawableから自身を外す
	if (m_drawable) {
		if (m_drawlink.prev) {
			m_drawlink.prev->m_drawlink.next = m_drawlink.next;
		}
		else {
			m_drawable->m_users.begin = m_drawlink.next;
		}
		if (m_drawlink.next) {
			m_drawlink.next->m_drawlink.prev = m_drawlink.prev;
		}
		else {
			m_drawable->m_users.end = m_drawlink.prev;
		}
		// 繋がりを断つ
		m_drawlink.prev = 0;
		m_drawlink.next = 0;
	}

	// 指定されたdrawableがあるなら、そこに自身を登録
	if (drawable) {
		m_drawlink.prev = drawable->m_users.end;
		if (m_drawlink.prev) {
			m_drawlink.prev->m_drawlink.next = this;
		}
		else {
			drawable->m_users.begin = this;
		}
		drawable->m_users.end = this;
	}

	// 指定されたdrawableを記録
	m_drawable = drawable;
}


C3DDrawable::USER::USER() : begin(0), end(0) {}
C3DDrawable::LINK::LINK() : prev(0), next(0) {}

C3DDrawable::C3DDrawable()
	: CGLObj()
	, m_users()
	, m_sisters()
	, m_env(CGLEnv::getInstance().DrawEnv<C3DDrawEnv>(CGLEnv::C3D))
{
	// 描画環境に自身を登録
	m_sisters.prev = m_env->m_end;
	if (m_sisters.prev) {
		m_sisters.prev->m_sisters.next = this;
	}
	else {
		m_env->m_begin = this;
	}
	m_env->m_end = this;
}

C3DDrawable::~C3DDrawable()
{
	// 描画環境側の登録を削除
	if (m_sisters.prev) {
		m_sisters.prev->m_sisters.next = m_sisters.next;
	}
	else {
		m_env->m_begin = m_sisters.next;
	}
	if (m_sisters.next) {
		m_sisters.next->m_sisters.prev = m_sisters.prev;
	}
	else {
		m_env->m_end = m_sisters.prev;
	}

	// 自身を利用していたオブジェクトから、自身の利用設定を解除する
	C3DDrawObj * pObj = m_users.begin;
	while(pObj) {
		C3DDrawObj * nxt = pObj->m_drawlink.next;
		pObj->useDrawable(0);
		pObj = nxt;
	}
}

void
C3DDrawable::registUser(C3DDrawObj * pObj)
{
	pObj->useDrawable(this);
}

void
C3DDrawable::Render()
{
	// このdrawableを用いる描画オブジェクトが一つもなければ、初期設定コストが無駄なので終了。
	C3DDrawObj * pObj = m_users.begin;
	if (!pObj) return;

	setup(m_env);
	while (pObj) {
		pObj->render(m_env);
		pObj = pObj->m_drawlink.next;
	}
	cleanup(m_env);
}