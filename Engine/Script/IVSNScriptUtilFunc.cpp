#include <string.h>
#include "IVSNScriptUtilFunc.h"
#include "CVSNUtil.h"

IVSNScriptUtilFunc	*	IVSNScriptUtilFunc::m_begin = 0;
IVSNScriptUtilFunc	*	IVSNScriptUtilFunc::m_end = 0;

IVSNScriptUtilFunc::IVSNScriptUtilFunc(const char * name)
	: m_prev(0)
	, m_next(0)
	, m_name(name)
{
	m_prev = m_end;
	if (m_prev) {
		m_prev->m_next = this;
	}
	else {
		m_begin = this;
	}
	m_end = this;
	m_hash = CVSNUtil::strHash(name);
}

IVSNScriptUtilFunc::~IVSNScriptUtilFunc()
{
	if (m_prev) {
		m_prev->m_next = m_next;
	}
	else {
		m_begin = m_next;
	}
	if (m_next) {
		m_next->m_prev = m_prev;
	}
	else {
		m_end = m_prev;
	}
}

bool
IVSNScriptUtilFunc::execUtilFunc(const char * name, CVSNScriptArgs& args, CVSNScriptArgs& ret)
{
	int hash = CVSNUtil::strHash(name);
	for (IVSNScriptUtilFunc * item = m_begin; item; item = item->m_next) {
		if (hash == item->m_hash && !strcmp(name, item->m_name)) {
			return item->utilFunc(args, ret);
		}
	}
	return false;
}
