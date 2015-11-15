#include "C2DTexOT.h"
#include "C2DSprite.h"
#include "C2DDrawQueue.h"
#include "CGLEnv.h"
#include "C2DDrawEnv.h"
#include "C2DQueueItem.h"

C2DTexOT::TEXCHAIN::TEXCHAIN() : prev(0), next(0) {}
C2DTexOT::QUEUELIST::QUEUELIST() : begin(0), end(0) {}

C2DTexOT::C2DTexOT(CGLTex * pTex)
	: CGLBase()
	, m_tex(pTex)
	, m_chain()
{
	C2DDrawEnv * env = CGLEnv::getInstance().DrawEnv<C2DDrawEnv>(CGLEnv::C2D);
	m_chain.prev = env->m_texlist.end;
	if (m_chain.prev) {
		m_chain.prev->m_chain.next = this;
	}
	else {
		env->m_texlist.begin = this;
	}
	env->m_texlist.end = this;
}

C2DTexOT::~C2DTexOT()
{
	C2DDrawEnv * env = CGLEnv::getInstance().DrawEnv<C2DDrawEnv>(CGLEnv::C2D);
	if (m_chain.prev) {
		m_chain.prev->m_chain.next = m_chain.next;
	}
	else {
		env->m_texlist.begin = m_chain.next;
	}
	if (m_chain.next) {
		m_chain.next->m_chain.prev = m_chain.prev;
	}
	else {
		env->m_texlist.end = m_chain.prev;
	}
}

void
C2DTexOT::registSprite(C2DSprite * pSpr, int pri)
{
	pSpr->m_texOT = this;
	// 空きのあるキューアイテムを探す。
	for (C2DQueueItem * item = m_qlist[pri].begin; item; item++) {
		if (item->getBlank() > 0) {
			// 空きを見つけた
			item->alloc(pSpr);
			return;
		}
	}
	// 空きのあるキューアイテムは持ち合わせていなかった
	C2DQueueItem * item = new C2DQueueItem(this, pri);
	item->alloc(pSpr);
	m_useCount++;
}

int 
C2DTexOT::removeSprite(C2DSprite * pSpr)
{
	C2DQueueItem * item = pSpr->m_queue;
	if (item->remove(pSpr) == 0) {
		delete item;
		--m_useCount;
	}
	return m_useCount;
}

// 指定されたプライオリティを描画する
void
C2DTexOT::render(C2DDrawQueue * queue, int pri)
{
	for (C2DQueueItem * item = m_qlist[pri].begin; item; item = item->m_link.next) {
		queue->drawItem(item);
	}
}
