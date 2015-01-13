#include <string.h>
#include "CVSNScript.h"
#include "CVSNScriptTask.h"
#include "CVSNEngineHeap.h"

CVSNScript::CVSNScript()
: CVSNObj()
, m_scriptPath(0)
, m_loadReserve(0)
{}
CVSNScript::~CVSNScript()
{
	if (m_scriptPath) CVSNEngineHeap::getInstance().free(m_scriptPath);
	if (m_loadReserve) CVSNEngineHeap::getInstance().free(m_loadReserve);
}

bool
CVSNScript::setScriptDirectoryPath(const char * dirPath)
{
	int length = strlen(dirPath);
	int noslash = 0;
	if (length > 0 && *(dirPath + length - 1) != '/') {
		noslash = 1;
	}
	char * buf = (char *)CVSNEngineHeap::getInstance().malloc(length + noslash + 1);
	if (!buf) return false;

	strcpy(buf, dirPath);
	if (noslash) strcpy(buf + length, "/");

	if (m_scriptPath) CVSNEngineHeap::getInstance().free(m_scriptPath);
	m_scriptPath = buf;
	return true;
}

const char *
CVSNScript::makeScriptPath(const char * scriptName)
{
	int length = strlen(m_scriptPath) + strlen(scriptName);
	char * buf = (char *)CVSNEngineHeap::getInstance().malloc(length + 1);
	strcpy(buf, m_scriptPath);
	strcat(buf, scriptName);
	return buf;
}

void
CVSNScript::destroyScriptPath(const char * scriptPath)
{
	char * buf = (char *)scriptPath;
	CVSNEngineHeap::getInstance().free(buf);
}

bool
CVSNScript::reserveLoadScript(const char * scriptName)
{
	if (m_loadReserve) CVSNEngineHeap::getInstance().free(m_loadReserve);
	int length = strlen(scriptName);
	m_loadReserve = (char *)CVSNEngineHeap::getInstance().malloc(length + 1);
	strcpy(m_loadReserve, scriptName);
	return true;
}

bool
CVSNScript::isLoadReserved()
{
	return (m_loadReserve != 0);
}

bool
CVSNScript::loadReservedScript()
{
	if (!isLoadReserved()) return true;

	bool result = loadScript(m_loadReserve);
	CVSNEngineHeap::getInstance().free(m_loadReserve);
	m_loadReserve = 0;

	return result;
}


bool
CVSNScript::loadScript(const char * scriptName)
{
	const char * scriptPath = makeScriptPath(scriptName);
	bool result = loadScriptFullPath(scriptPath, scriptName);
	destroyScriptPath(scriptPath);
	return result;
}

CVSNScriptArgs::CVSNScriptArgs() : CVSNObj() {}
CVSNScriptArgs::~CVSNScriptArgs() {}

CVSNScriptCallback::CVSNScriptCallback() : CVSNObj() {}
CVSNScriptCallback::~CVSNScriptCallback() {}
