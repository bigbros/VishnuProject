#include <string.h>
#include <memory>
#include <exception>
#include "CGLEnv.h"
#include "CGLAlloc.h"

IGLDrawEnv::IGLDrawEnv() : CGLBase() {}
IGLDrawEnv::~IGLDrawEnv(){}


CGLEnv::CGLEnv()
	: CGLBase()
	, m_width(0)
	, m_height(0)
{
}

CGLEnv::~CGLEnv()
{
}

CGLEnv&
CGLEnv::getInstance()
{
	static CGLEnv instance;
	return instance;
}

void
CGLEnv::Clear()
{
	for (int i = 0; i < ENV_MAX; i++) if(m_drawEnv[i]) m_drawEnv[i]->Clear();

	CGLObj * obj = m_begin;
	while (obj) {
		CGLObj * next = obj->m_next;
		delete obj;
		obj = next;
	}
}

void
CGLEnv::setResolution(int width, int height)
{
	m_width = width;
	m_height = height;

	for (int i = 0; i < ENV_MAX; i++) if(m_drawEnv[i]) m_drawEnv[i]->setResolution(width, height);

	glViewport(0, 0, width, height);
}

void
CGLEnv::screenClear()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void
CGLEnv::screenFinish()
{
	glFlush();
}

bool
CGLEnv::destruction()
{
	bool result = true;
	for (CGLObj * pObj = m_begin; pObj; pObj = pObj->m_next) {
		result = result && pObj->destruction();
	}
	return  result;
}

bool
CGLEnv::recovery()
{
	bool result = true;
	for (CGLObj * pObj = m_begin; pObj; pObj = pObj->m_next) {
		result = result && pObj->recovery();
	}
	return result;
}

