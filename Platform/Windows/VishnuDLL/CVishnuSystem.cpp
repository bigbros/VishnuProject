#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "CVishnuSystem.h"

CVishnuSystem::CVishnuSystem(const char * path, int width, int height, int is_vr)
{
	m_heapSize = MEGA(VISHNU_MEMORY);
	m_heapBuf = malloc(m_heapSize);
	m_env = new CTestEnv(m_heapBuf, m_heapSize, width, height, is_vr);
	m_Pointing = new CVSNPointing();
	m_Storage = new CVSNWin32FS(path);
	m_Thread = new CVSNWin32Thread();
	m_Tracking = new CVSNWin32Tracking();
	m_LeapMotion = new CVSNLeapMotion();


	CVSNPlatform& platform = CVSNPlatform::getInstance();
	platform.registModule(m_Pointing);
	platform.registModule(m_Storage);
	platform.registModule(m_Thread);
	platform.registModule(m_Tracking);
	platform.registModule(m_LeapMotion);
	
	m_env->Startup("shaders", "LuaScripts", "startup");
}

CVishnuSystem::~CVishnuSystem() 
{
	delete m_env;
	delete m_LeapMotion;
	delete m_Tracking;
	delete m_Thread;
	delete m_Storage;
	delete m_Pointing;

	free(m_heapBuf);
}

void
CVishnuSystem::Update()
{
	DWORD now = timeGetTime();
	int deltaT = (int)(now - m_prevTime);
	m_prevTime = now;
	m_env->Update(deltaT);
}

void
CVishnuSystem::OnSizeChanged(int width, int height)
{
	m_env->onChange(width, height);
}

void
CVishnuSystem::OnPointing(int id, int mode, int x, int y, float force)
{
	m_Pointing->setEvent(id, (IVSNPointListener::EVENT)mode, x, y, force);
}

void
CVishnuSystem::OnPause()
{
	m_env->onPause();
}

void
CVishnuSystem::OnResume()
{
	m_env->onResume();
}

void
CVishnuSystem::OnDetach()
{
	m_env->onDetach();
}

void
CVishnuSystem::OnSurface()
{
	m_env->onSurface();
}
