#include "CVSNPointing.h"

IVSNPointListener::IVSNPointListener()
	: m_prev(0)
	, m_next(0)
	, m_parent(0)
{
}

IVSNPointListener::~IVSNPointListener()
{
	if (m_parent) {
		if (m_prev) {
			m_prev->m_next = m_next;
		}
		else {
			m_parent->m_begin = m_next;
		}
		if (m_next) {
			m_next->m_prev = m_prev;
		}
		else {
			m_parent->m_end = m_prev;
		}
	}
}


CVSNPointing::CVSNPointing()
	: IVSNModule(PF_POINTING, "POINT")
	, m_begin(0)
	, m_end(0)
{}

CVSNPointing::~CVSNPointing()
{
	IVSNPointListener * pListener = m_begin;
	while (pListener) {
		IVSNPointListener * pNext = pListener->m_next;
		delete pListener;
		pListener = pNext;
	}
}

void
CVSNPointing::registListener(IVSNPointListener * pListener)
{
	pListener->m_prev = m_end;
	if (pListener->m_prev) {
		pListener->m_prev->m_next = pListener;
	}
	else {
		m_begin = pListener;
	}
	m_end = pListener;
	pListener->m_parent = this;
}

void
CVSNPointing::setEvent(int touchId, IVSNPointListener::EVENT ev, int x, int y, float force)
{
	for (IVSNPointListener * pListener = m_begin; pListener; pListener = pListener->m_next) {
		pListener->listenEvent(touchId, ev, x, y, force);
	}
}

bool
CVSNPointing::init()
{
	return true;
}