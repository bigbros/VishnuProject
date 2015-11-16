#include "CRasPiEnv.h"
#include "CVSNLuaScript.h"
#include "C3DFBO.h"
#include "C3DVRFBO.h"

CRasPiEnv::CRasPiEnv(void * pHeapBuffer, size_t sizeHeap, int width, int height, int is_vr)
	: CVSNGameEnvironment(pHeapBuffer, sizeHeap, width, height)
	, m_is_vr((is_vr))
{
}

CRasPiEnv::~CRasPiEnv() {}

CVSNScript *
CRasPiEnv::createScriptSystem()
{
	return new CVSNLuaScript();
}

bool
CRasPiEnv::initUserTask()
{
	return true;
}
/*
bool
CRasPiEnv::initDraw3D()
{
	return true;
}
*/

CGLFBO *
CRasPiEnv::createFBO()
{
	if (m_is_vr) {
		return new C3DVRFBO(m_pathShaders, m_width, m_height);
	}
	else {
		return new C3DFBO(m_pathShaders, m_width, m_height);
	}
}
