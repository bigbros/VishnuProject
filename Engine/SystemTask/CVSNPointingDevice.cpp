#include <stdio.h>
#include <stdlib.h>
#include "CVSNPointingDevice.h"
#include "PlatformUtil.h"
CVSNPointListener::CVSNPointListener() : IVSNPointListener() {}
CVSNPointListener::~CVSNPointListener() {}

// Listener は、すべてのタスクから共有される singleton のキューに新たなイベントを登録する。
void
CVSNPointListener::listenEvent(int touchId, IVSNPointListener::EVENT ev, int x, int y, float force)
{
	CVSNTouchPanel::getInstance().addItem(touchId, ev, x, y, force);
}

// singleton からは、直前のフレームに記録されたすべてのポインティングデバイスイベントを
// 同じフレーム内であれば何度でもキューから読みだすことができる。
CVSNTouchPanel::CVSNTouchPanel()
	: m_ptReadTop(0)
	, m_ptNextTop(0)
	, m_ptRec(0)
	, m_ptRead(0)
{
}
CVSNTouchPanel::~CVSNTouchPanel() {}

CVSNTouchPanel&
CVSNTouchPanel::getInstance()
{
	static CVSNTouchPanel instance;
	return instance;
}


const CVSNTouchPanel::ITEM *
CVSNTouchPanel::getItem()
{
	if (m_ptRead == m_ptNextTop) return 0;
	ITEM * ret = &m_queue[m_ptRead];
	m_ptRead = (m_ptRead + 1) % QUEUE_SIZE;
	return ret;
}

void
CVSNTouchPanel::flip()
{
	int next = m_ptNextTop;
	m_ptNextTop = m_ptRec;
	m_ptReadTop = next;
}

bool
CVSNTouchPanel::addItem(int touchId, IVSNPointListener::EVENT ev, int x, int y, float force)
{
	if (m_ptReadTop != m_ptNextTop && 
		m_ptRec == m_ptReadTop) return false;

	m_queue[m_ptRec].touchId = touchId;
	m_queue[m_ptRec].ev = ev;
	m_queue[m_ptRec].x = x;
	m_queue[m_ptRec].y = y;
	m_queue[m_ptRec].force = force;
	m_ptRec = (m_ptRec + 1) % QUEUE_SIZE;

	return true;
}


CVSNPointingDevice::CVSNPointingDevice()
	: CVSNTask(CVSNTask::P_INPUT)
//	, m_listener()
{
}

CVSNPointingDevice::~CVSNPointingDevice(){}

bool
CVSNPointingDevice::init()
{
	// 自身のもつListenerをポインティングデバイスモジュールに登録する。
	CVSNPlatform::getInstance().Module<CVSNPointing>(PF_POINTING, "POINT")->registListener(&m_listener);
	set_parent(0);
	return birth();
}

void
CVSNPointingDevice::update(int deltaT)
{
	CVSNTouchPanel::getInstance().flip();
}

void
CVSNPointingDevice::die()
{
}

