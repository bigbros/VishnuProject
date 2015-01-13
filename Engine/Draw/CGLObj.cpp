#include "CGLEnv.h"
#include "CGLObj.h"

CGLObj::CGLObj()
	: CGLBase()
	, m_prev(0)
	, m_next(0)
{
	CGLEnv& env = CGLEnv::getInstance();
	m_prev = env.m_end;
	if (m_prev) {
		m_prev->m_next = this;
	}
	else {
		env.m_begin = this;
	}
	env.m_end = this;
}

CGLObj::~CGLObj()
{
	CGLEnv& env = CGLEnv::getInstance();
	if (m_prev) {
		m_prev->m_next = m_next;
	}
	else {
		env.m_begin = m_next;
	}
	if (m_next) {
		m_next->m_prev = m_prev;
	}
	else {
		env.m_end = m_prev;
	}
}
