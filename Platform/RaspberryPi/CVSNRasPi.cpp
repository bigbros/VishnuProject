#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <sys/time.h>
#include "CVSNRasPi.h"

CVSNRasPi::CVSNRasPi(const char * path, int width, int height, int is_vr)
{
	m_heapSize = MEGA(VISHNU_MEMORY);
	m_heapBuf = malloc(m_heapSize);
	m_env = new CTestEnv(m_heapBuf, m_heapSize, width, height, is_vr);
	m_Pointing = new CVSNPointing();
	m_Storage = new CVSNUnixFS(path);
	m_Thread = new CVSNPthread();

	CVSNPlatform& platform = CVSNPlatform::getInstance();
	platform.registModule(m_Pointing);
	platform.registModule(m_Storage);
	platform.registModule(m_Thread);

	m_env->Startup("shaders", "LuaScripts", "startup");
}

CVSNRasPi::~CVSNRasPi()
{
	delete m_Thread;
	delete m_Storage;
	delete m_Pointing;
	delete m_env;
	free(m_heapBuf);
}

void
CVSNRasPi::Update()
{
	struct timeval tv;
	gettimeofday(&tv, 0);
	int64_t now = tv.tv_sec * 1000L + tv.tv_usec / 1000L;
	int deltaT = (int)(now - m_prevTime);
	m_prevTime = now;
	m_env->Update(deltaT);
}

void
CVSNRasPi::OnSizeChanged(int width, int height)
{
	m_env->onChange(width, height);
}

void
CVSNRasPi::OnPointing(int id, int mode, int x, int y, float force)
{
	m_Pointing->setEvent(id, (IVSNPointListener::EVENT)mode, x, y, force);
}

void
CVSNRasPi::OnPause()
{
	m_env->onPause();
}

void
CVSNRasPi::OnResume()
{
	m_env->onResume();
}

void
CVSNRasPi::OnDetach()
{
	m_env->onDetach();
}

void
CVSNRasPi::OnSurface()
{
	m_env->onSurface();
}
