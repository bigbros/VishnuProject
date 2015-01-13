#include "C2DQueueItem.h"
#include "C2DTexOT.h"
#include "C2DSprite.h"

C2DQueueItem::QUEUELINK::QUEUELINK() : prev(0), next(0) {}

C2DQueueItem::C2DQueueItem(C2DTexOT * ot, int pri)
	: CGLBase()
	, m_link()
	, m_useItem(0)
	, m_ot(ot)
	, m_pri(pri)
{
	for (int i = 0; i < QUEUE_MAX; i++) {
		m_sprites[i] = 0;
		m_matrices[i].toScale(0.0f, 0.0f);
		m_rgba[i].set(0.0f, 0.0f, 0.0f, 0.0f);
		m_uv[i].set(0.0f, 0.0f, 0.0f, 0.0f);
	}

	m_link.prev = ot->m_qlist[pri].end;
	if (m_link.prev) {
		m_link.prev->m_link.next = this;
	}
	else {
		ot->m_qlist[pri].begin = this;
	}
	ot->m_qlist[pri].end = this;
}

C2DQueueItem::~C2DQueueItem() {
	if (m_link.prev) {
		m_link.prev->m_link.next = m_link.next;
	}
	else {
		m_ot->m_qlist[m_pri].begin = m_link.next;
	}
	if (m_link.next) {
		m_link.next->m_link.prev = m_link.prev;
	}
	else {
		m_ot->m_qlist[m_pri].end = m_link.prev;
	}
}

bool
C2DQueueItem::alloc(C2DSprite * spr)
{
	for (int i = 0; i < QUEUE_MAX; i++) {
		if (!m_sprites[i]) {
			m_sprites[i] = spr;
			spr->m_queue = this;
			spr->m_qidx = i;
			spr->m_drawmat = &m_matrices[i];
			spr->m_rgba = &m_rgba[i];
			spr->m_uv = &m_uv[i];
			m_useItem++;
			return true;
		}
	}
	return false;
}

int
C2DQueueItem::remove(C2DSprite * spr)
{
	if (spr->m_queue != this) return -1;
	m_sprites[spr->m_qidx] = 0;
	m_matrices[spr->m_qidx].toScale(0.0f, 0.0f);
	m_rgba[spr->m_qidx].set(0.0f, 0.0f, 0.0f, 0.0f);
	m_uv[spr->m_qidx].set(0.0f, 0.0f, 0.0f, 0.0f);
	m_useItem--;
	return m_useItem;
}

void
C2DQueueItem::kick(C2DDefaultShader * shader)
{
	// �ݒ肳�ꂽ����matrix��RGBA�l�AUV����uniform�ɓ]��

	// �`��̍ہA�S�̂̒��_�F�ɂ�����RGBA�l���ő�32���܂Ƃ߂ē]������
	glUniform4fv(shader->m_u_rgba, QUEUE_MAX, (GLfloat *)m_rgba);

	// �`��ɗp����e�N�X�`������UV�l���ő�32���܂Ƃ߂ē]������
	glUniform4fv(shader->m_u_uv, QUEUE_MAX, (GLfloat *)m_uv);

	// �`��̍ۂɗp����I�u�W�F�N�g�̃}�g���N�X(���T�C�Y/��]/�ړ�)���ő�32���܂Ƃ߂ē]������
	glUniformMatrix3fv(shader->m_u_mat, QUEUE_MAX, GL_FALSE, (const GLfloat *)m_matrices);

	// �]���ς݂̒��_�ƃC���f�b�N�X�A����у}�g���N�X�ARGBA�AUV�l�ŕ`�悷��
	glDrawElements(GL_TRIANGLE_STRIP, QUEUE_MAX * 5, GL_UNSIGNED_BYTE, 0);
}