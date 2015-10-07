#include "CTestEnv.h"
#include "CVSNLuaScript.h"
#include "C3DFBO.h"
#include "C3DVRFBO.h"

CTestEnv::CTestEnv(void * pHeapBuffer, size_t sizeHeap, int width, int height, int is_vr)
	: CVSNGameEnvironment(pHeapBuffer, sizeHeap, width, height)
	, m_is_vr((is_vr))
{
}

CTestEnv::~CTestEnv() {}

CVSNScript *
CTestEnv::createScriptSystem()
{
	return new CVSNLuaScript();
}

bool
CTestEnv::initUserTask()
{
	return true;
}

CGLFBO *
CTestEnv::createFBO()
{
	if (m_is_vr) {
		return new C3DVRFBO(m_pathShaders, m_width, m_height);
	}
	else {
		return new C3DFBO(m_pathShaders, m_width, m_height);
	}
}
