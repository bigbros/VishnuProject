#include "C2DDrawQueue.h"
#include "CGLAlloc.h"
#include "C2DSprite.h"
#include "C2DDefaultShader.h"
#include "C2DQueueItem.h"

C2DDrawQueue::C2DDrawQueue(C2DDefaultShader * shader)
	: CGLObj()
	, m_vertBuf(0)
	, m_idxBuf(0)
	, m_shader(shader)
{
	makeBuffer(shader);
}

C2DDrawQueue::~C2DDrawQueue()
{
	CGLAlloc::free(m_vertBuf);
	CGLAlloc::free(m_idxBuf);
	destruction();
}

void
C2DDrawQueue::bufferEnable(C2DDefaultShader * shader)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vert);
	glVertexAttribPointer(shader->m_a_vert, 2, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(VERTEX), (const GLvoid *)(char *)offsetof(VERTEX, vert));
	glVertexAttribPointer(shader->m_a_params, 2, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(VERTEX), (const GLvoid *)(char *)offsetof(VERTEX, idx));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_idx);

}

bool
C2DDrawQueue::recovery()
{
	// GL側への転送
	GLuint bufIdx[2];
	glGenBuffers(2, bufIdx);

	m_vert = bufIdx[0];
	glBindBuffer(GL_ARRAY_BUFFER, m_vert);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VERTEX) * 4 * MAX_DRAW_ITEMS, m_vertBuf, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	m_idx = bufIdx[1];
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_idx);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 5 * MAX_DRAW_ITEMS, m_idxBuf, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return true;
}

bool
C2DDrawQueue::destruction()
{
	GLuint bufIdx[2];
	bufIdx[0] = m_vert;
	bufIdx[1] = m_idx;
	glDeleteBuffers(2, bufIdx);
	return true;
}

// 頂点バッファの生成。このバッファはすべての矩形スプライト描画に流用される。
void
C2DDrawQueue::makeBuffer(C2DDefaultShader * shader)
{
	// バッファの作成
	m_vertBuf = (VERTEX *)CGLAlloc::malloc(sizeof(VERTEX) * 4 * MAX_DRAW_ITEMS);
	m_idxBuf = (u8 *)CGLAlloc::malloc(5 * MAX_DRAW_ITEMS);
	for (int i = 0; i < MAX_DRAW_ITEMS; i++) {
		for (int j = 0; j < 4; j++) {
			int idx = i * 4 + j;
			m_vertBuf[idx].idx = i;
			m_vertBuf[idx].pad = 0;
			m_vertBuf[idx].vert.x = 255 * (j & 1);
			m_vertBuf[idx].vert.y = 255 * ((j & 2) / 2);
		}
		for (int j = 0; j < 5; j++) {
			int idx = i * 5 + j;
			m_idxBuf[idx] = i * 4 + ((j < 4) ? j : 3);
		}
	}

	recovery();

}

void
C2DDrawQueue::drawInit()
{
	m_lastTex = 0;
	m_matCount = 0;
}

void
C2DDrawQueue::drawItem(C2DQueueItem * pItem)
{
	if (m_lastTex && pItem->m_ot != m_lastTex) {
		// 描画を終えたので、キューをリセットする
		drawInit();
	}
	if (!m_lastTex) {
		m_lastTex = pItem->m_ot;
		// テクスチャを切り替える
		m_lastTex->setTex(GL_TEXTURE0);
		glUniform1i(m_shader->m_u_tex, 0);
	}

	// C2DQueueItem は既に最大32個セットのキューであるため、そのまま転送して構わない。
	pItem->kick(m_shader);
}
