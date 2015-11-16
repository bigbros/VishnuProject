#include "C3DDrawable.h"

C3DDrawObj::DRAWLINK::DRAWLINK() : prev(0), next(0) {}

C3DDrawObj::C3DDrawObj()
	: C3DObj()
	, m_drawable(0)
	, m_drawlink()
{
}

C3DDrawObj::~C3DDrawObj()
{
	useDrawable(0);
}

void
C3DDrawObj::useDrawable(C3DDrawable * drawable)
{
	// ���ɐݒ肳��Ă��� drawable ������Ȃ�A����drawable���玩�g���O��
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
		// �q�����f��
		m_drawlink.prev = 0;
		m_drawlink.next = 0;
	}

	// �w�肳�ꂽdrawable������Ȃ�A�����Ɏ��g��o�^
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

	// �w�肳�ꂽdrawable���L�^
	m_drawable = drawable;
}


C3DDrawable::USER::USER() : begin(0), end(0) {}
C3DDrawable::LINK::LINK() : prev(0), next(0) {}

C3DDrawable::C3DDrawable(C3DShader * shader)
	: CGLObj()
	, m_users()
	, m_sisters()
	, m_shader(shader)
{
	// �w�肳�ꂽshader�Ɏ��g��o�^
	m_sisters.prev = m_shader->m_end;
	if (m_sisters.prev) {
		m_sisters.prev->m_sisters.next = this;
	}
	else {
		m_shader->m_begin = this;
	}
	m_shader->m_end = this;
}

C3DDrawable::~C3DDrawable()
{
	// shader���̓o�^���폜
	if (m_sisters.prev) {
		m_sisters.prev->m_sisters.next = m_sisters.next;
	}
	else {
		m_shader->m_begin = m_sisters.next;
	}
	if (m_sisters.next) {
		m_sisters.next->m_sisters.prev = m_sisters.prev;
	}
	else {
		m_shader->m_end = m_sisters.prev;
	}

	// ���g�𗘗p���Ă����I�u�W�F�N�g����A���g�̗��p�ݒ����������
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
	// ����drawable��p����`��I�u�W�F�N�g������Ȃ���΁A�����ݒ�R�X�g�����ʂȂ̂ŏI���B
	C3DDrawObj * pObj = m_users.begin;
	if (!pObj) return;

	setup(m_shader);
	while (pObj) {
		pObj->render(m_shader);
		pObj = pObj->m_drawlink.next;
	}
	cleanup(m_shader);
}
