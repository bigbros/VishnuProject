#include "CTestEnv.h"
#include "CVSNLuaScript.h"
#include "C3DVRFBO.h"

CTestEnv::CTestEnv(void * pHeapBuffer, size_t sizeHeap, int width, int height)
	: CVSNGameEnvironment(pHeapBuffer, sizeHeap, width, height)
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
	return new C3DVRFBO(m_pathShaders, m_width, m_height);
}