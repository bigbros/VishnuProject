#include <string.h>
#include <memory>
#include "CVSNScript.h"
#include "CVSNScriptTask.h"
#include "CVSNUtil.h"

unsigned int CVSNScriptTask::ms_idSeed = 0;

CVSNScriptTask::CVSNScriptTask(CVSNTask::PHASE phase)
: CVSNTask(phase)
, m_taskId(++ms_idSeed)
, m_scriptData(0)
{}
CVSNScriptTask::~CVSNScriptTask()
{
	if (m_scriptData) {
		m_scriptData->pTask = 0;
		if (m_scriptData->remove) {
			m_scriptData->remove(m_scriptData);
			m_scriptData->remove = 0;
		}
	}
}

bool
CVSNScriptTask::create(CVSNScriptArgs& args)
{
	bool result = init(args);
	result = result && birth();
	return result;
}

bool
CVSNScriptTask::command(CVSNScriptArgs& args, CVSNScriptArgs& ret)
{
	return false;
}

IFactory	*	IFactory::ms_begin = 0;
IFactory	*	IFactory::ms_end = 0;

IFactory::IFactory(const char * taskName)
: m_prev(0)
, m_next(0)
, m_taskName(taskName)
{
	m_prev = ms_end;
	if (m_prev) {
		m_prev->m_next = this;
	}
	else {
		ms_begin = this;
	}
	ms_end = this;
	m_taskNameHash = CVSNUtil::strHash(taskName);
}

IFactory::~IFactory(){}

CVSNScriptTask *
IFactory::createTask(const char * taskName, CVSNScriptArgs& args)
{
	int hash = CVSNUtil::strHash(taskName);
	for (IFactory * pItem = ms_begin; pItem; pItem = pItem->m_next) {
		if (pItem->m_taskNameHash == hash && !strcmp(pItem->m_taskName, taskName)) {
			return pItem->factory(args);
		}
	}
	return 0;
}



CVSNDefines * CVSNDefines::ms_begin = 0;
CVSNDefines * CVSNDefines::ms_end = 0;

CVSNDefines::CVSNDefines(DEFINE_INT * defInt, DEFINE_STR * defStr)
: m_defInt(defInt)
, m_defStr(defStr)
{
	m_prev = ms_end;
	if (m_prev) {
		m_prev->m_next = this;
	}
	else {
		ms_begin = this;
	}
	ms_end = this;
}
CVSNDefines::~CVSNDefines() {}


bool
CVSNDefines::defineConst(CVSNScript * scriptSystem)
{
	bool result = true;
	for (DEFINE_INT * pDefInt = m_defInt; pDefInt->label; pDefInt++) {
		result = result && scriptSystem->defineConstInt(pDefInt->label, pDefInt->value);
	}

	for (DEFINE_STR * pDefStr = m_defStr; pDefStr->label; pDefStr++) {
		result = result && scriptSystem->defineConstString(pDefStr->label, pDefStr->value);
	}
	return result;
}

bool
CVSNDefines::defineAllConst(CVSNScript * scriptSystem)
{
	bool result = true;
	for (CVSNDefines * pItem = ms_begin; pItem; pItem = pItem->m_next) {
		result = result && pItem->defineConst(scriptSystem);
	}
	return result;
}

