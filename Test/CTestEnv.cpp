#include "CTestEnv.h"
#include "CVSNLuaScript.h"

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
