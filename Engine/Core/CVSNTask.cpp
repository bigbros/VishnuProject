#include "CVSNTask.h"

CVSNTask::CVSNTask(PHASE phase)
: CVSNObj()
, m_pParent(0)
, m_phase(phase)
, m_is_dead(false)
{
	m_childs.begin	= m_childs.end = 0;
	m_sisters.prev	= m_sisters.next = 0;
	m_execute.prev	= m_execute.next = 0;
	m_dead.prev		= m_dead.next = 0;
}

CVSNTask::~CVSNTask()
{
}

void
CVSNTask::die()
{
}

void CVSNTask::on_pause() {}
void CVSNTask::on_resume() {}
void CVSNTask::on_change() {}
void CVSNTask::on_surface() {}
void CVSNTask::on_detach() {}
void CVSNTask::on_finish() {}

void
CVSNTask::change_parent(CVSNTask * pParent)
{
	set_parent(pParent);
	link_parent();
}

void
CVSNTask::set_parent(CVSNTask * pParent)
{
	if (m_pParent) {
		if (m_sisters.prev) {
			m_sisters.prev->m_sisters.next = m_sisters.next;
		}
		else {
			if(m_pParent->m_childs.begin == this) m_pParent->m_childs.begin = m_sisters.next;
		}
		if (m_sisters.next) {
			m_sisters.next->m_sisters.prev = m_sisters.prev;
		}
		else {
			if(m_pParent->m_childs.end == this) m_pParent->m_childs.end = m_sisters.prev;
		}
		m_sisters.prev = 0;
		m_sisters.next = 0;
	}
	m_pParent = pParent;
}

void
CVSNTask::link_parent()
{
	if (m_pParent) {
		m_sisters.prev = m_pParent->m_childs.end;
		if (m_sisters.prev) {
			m_sisters.prev->m_sisters.next = this;
		}
		else {
			m_pParent->m_childs.begin = this;
		}
		m_pParent->m_childs.end = this;
	}
}

bool
CVSNTask::birth()
{
	link_parent();
	return CVSNTaskMgr::getInstance().registBirth(this);
}

void
CVSNTask::kill()
{
	if (m_is_dead) return;
	for (CVSNTask * pTask = m_childs.begin; pTask; pTask = pTask->m_sisters.next) {
		pTask->kill();
	}
	CVSNTaskMgr::getInstance().dieReserve(this);
	m_is_dead = true;
}


CVSNTaskMgr::CVSNTaskMgr()
{
	m_die.begin = 0;
	m_die.end = 0;

	for (int i = 0; i < CVSNTask::PHASE_NUMS; i++) {
		m_execute[i].begin = 0;
		m_execute[i].end = 0;
	}
}

CVSNTaskMgr::~CVSNTaskMgr()
{
}

CVSNTaskMgr&
CVSNTaskMgr::getInstance()
{
	static CVSNTaskMgr instance;
	return instance;
}

bool
CVSNTaskMgr::registBirth(CVSNTask * pTask)
{
	CVSNTask::PHASE phase = pTask->m_phase;
	if (phase == CVSNTask::NOT_REGIST) return false;

	pTask->m_execute.prev = m_execute[phase].end;
	if (pTask->m_execute.prev) {
		pTask->m_execute.prev->m_execute.next = pTask;
	}
	else {
		m_execute[phase].begin = pTask;
	}
m_execute[phase].end = pTask;
return true;
}

void
CVSNTaskMgr::dieReserve(CVSNTask * pTask)
{
	pTask->m_dead.prev = m_die.end;
	if (pTask->m_dead.prev) {
		pTask->m_dead.prev->m_dead.next = pTask;
	}
	else {
		m_die.begin = pTask;
	}
	m_die.end = pTask;
}

void
CVSNTaskMgr::removeTask()
{
	CVSNTask * pTask = m_die.begin;
	while (pTask) {
		CVSNTask * pNext = pTask->m_dead.next;

		pTask->die();

		if (pTask->m_execute.prev) {
			pTask->m_execute.prev->m_execute.next = pTask->m_execute.next;
		}
		else {
			m_execute[pTask->m_phase].begin = pTask->m_execute.next;
		}
		if (pTask->m_execute.next) {
			pTask->m_execute.next->m_execute.prev = pTask->m_execute.prev;
		}
		else {
			m_execute[pTask->m_phase].end = pTask->m_execute.prev;
		}

		delete pTask;

		pTask = pNext;
	}
	m_die.begin = m_die.end = 0;
}

void
CVSNTaskMgr::execTask(int deltaT)
{
	for (int i = 0; i < CVSNTask::PHASE_NUMS; i++) {
		for (CVSNTask * pTask = m_execute[i].begin; pTask; pTask = pTask->m_execute.next) {
			pTask->update(deltaT);
		}
	}
}

void
CVSNTaskMgr::Update(int deltaT)
{
	execTask(deltaT);
	removeTask();
}

void
CVSNTaskMgr::onPause()
{
	for (int i = 0; i < CVSNTask::PHASE_NUMS; i++) {
		for (CVSNTask * pTask = m_execute[i].begin; pTask; pTask = pTask->m_execute.next) {
			pTask->on_pause();
		}
	}
	removeTask();
}

void
CVSNTaskMgr::onResume()
{
	for (int i = 0; i < CVSNTask::PHASE_NUMS; i++) {
		for (CVSNTask * pTask = m_execute[i].begin; pTask; pTask = pTask->m_execute.next) {
			pTask->on_resume();
		}
	}
	removeTask();
}

void
CVSNTaskMgr::onChange()
{
	for (int i = 0; i < CVSNTask::PHASE_NUMS; i++) {
		for (CVSNTask * pTask = m_execute[i].begin; pTask; pTask = pTask->m_execute.next) {
			pTask->on_change();
		}
	}
	removeTask();
}

void
CVSNTaskMgr::onSurface()
{
	for (int i = 0; i < CVSNTask::PHASE_NUMS; i++) {
		for (CVSNTask * pTask = m_execute[i].begin; pTask; pTask = pTask->m_execute.next) {
			pTask->on_surface();
		}
	}
	removeTask();
}

void
CVSNTaskMgr::onDetach()
{
	for (int i = 0; i < CVSNTask::PHASE_NUMS; i++) {
		for (CVSNTask * pTask = m_execute[i].begin; pTask; pTask = pTask->m_execute.next) {
			pTask->on_detach();
		}
	}
	removeTask();
}
void
CVSNTaskMgr::onFinish()
{
	for (int i = 0; i < CVSNTask::PHASE_NUMS; i++) {
		for(CVSNTask * pTask = m_execute[i].begin; pTask; pTask = pTask->m_execute.next) {
			pTask->on_finish();
			pTask->kill();
		}
	}
	removeTask();
}
