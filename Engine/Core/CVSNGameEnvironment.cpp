#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <exception>
#include "PlatformUtil.h"
#include "CVSNGameEnvironment.h"
#include "CVSNEngineHeap.h"
#include "CVSNTask.h"

#include "CGLEnv.h"
#include "C3DFBO.h"
#include "CVSN3DLightVecFunc.h"
#include "CGLAlloc.h"


CVSNGameEnvironment::CVSNGameEnvironment(void * pHeapBuffer, size_t sizeHeap, int width, int height)
: m_pHeapBuffer(pHeapBuffer)
, m_sizeHeap(sizeHeap)
, m_scriptSystem(0)
, m_firstScript(0)
, m_pathShaders(0)
, m_width(width)
, m_height(height)
{
	{
		GLint vertUniform, fragUniform, texNum;
		glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, &vertUniform);
		glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS, &fragUniform);
		glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &texNum);
		char buf[1024];
		sprintf(buf, "vertex shader max uniform %d vectors\n", vertUniform);
		LOG(buf);
		sprintf(buf, "fragment shader max uniform: %d vectors\n", fragUniform);
		LOG(buf);
		sprintf(buf, "fragment shader max textures: %d units\n", texNum);
		LOG(buf);
	}

	CVSNEngineHeap::getInstance().setHeapArea(m_pHeapBuffer, m_sizeHeap);
}

CVSNGameEnvironment::~CVSNGameEnvironment()
{
	CVSNTaskMgr::getInstance().onFinish();
	CGLEnv::getInstance().Clear();
	delete m_scriptSystem;
}

bool
CVSNGameEnvironment::initSystemTask()
{
	bool result = true;
	// pointing device.
	try {
		// 描画モジュール用のアロケータとして、VSNアロケータを用いる。
		CGLAlloc::setAllocator(vsnMemAlloc, vsnMemFree);

		result = result && (m_pointingDevice = new CVSNPointingDevice())->init();
		result = result && initDraw3D();
		result = result && initDraw2D();
		result = result && (m_script = new CVSNScriptDriver(m_scriptSystem, m_firstScript));

		CGLEnv::getInstance().setResolution(m_width, m_height);
	}
	catch (std::exception& ex) {
		LOG(ex.what());
		result = false;
	}
	return result;
}

void
CVSNGameEnvironment::setShadersPath(const char * pathShaders)
{
	try {
		char * buf = (char *)CVSNEngineHeap::getInstance().malloc(strlen(pathShaders) + 1);
		if (!buf) return;
		strcpy(buf, pathShaders);
		CVSNEngineHeap::getInstance().free((void *)m_pathShaders);
		m_pathShaders = buf;
	}
	catch (std::exception& ex) {
		LOG(ex.what());
	}
}

void
CVSNGameEnvironment::setScriptPath(const char * scriptPath, const char * firstScript)
{
	try {
		if (!m_scriptSystem) {
			m_scriptSystem = createScriptSystem();
			if (!m_scriptSystem) return;
		}
		m_scriptSystem->setScriptDirectoryPath(scriptPath);

		char * buf = (char *)CVSNEngineHeap::getInstance().malloc(strlen(firstScript) + 1);
		if (!buf) return;
		strcpy(buf, firstScript);
		CVSNEngineHeap::getInstance().free((void *)m_firstScript);
		m_firstScript = buf;
	}
	catch (std::exception& ex) {
		LOG(ex.what());
	}
}

bool
CVSNGameEnvironment::Startup(const char * shaderPath, const char * scriptPath, const char * firstScript)
{
	bool result = true;
	try {
		setShadersPath(shaderPath);
		setScriptPath(scriptPath, firstScript);
		result = result && initSystemTask();
		result = result && initUserTask();
		return result;
	}

	catch (std::exception& ex) {
		LOG(ex.what());
		return false;
	}
}


void
CVSNGameEnvironment::Update(int deltaT)
{
	try {
		CGLEnv::getInstance().screenClear();
		CVSNTaskMgr::getInstance().Update(deltaT);
		CGLEnv::getInstance().screenFinish();
	}
	catch (std::exception& ex) {
		LOG(ex.what());
	}
}

void
CVSNGameEnvironment::onPause()
{
	try {
		LOG("VSN: onPause()\n");
		CVSNTaskMgr::getInstance().onPause();
	}
	catch (std::exception& ex) {
		LOG(ex.what());
	}
}

void
CVSNGameEnvironment::onResume()
{
	try {
		LOG("VSN: onResume()\n");
		CVSNTaskMgr::getInstance().onResume();
	}
	catch (std::exception& ex) {
		LOG(ex.what());
	}
}

void
CVSNGameEnvironment::onChange(int width, int height)
{
	try {
		LOG("VSN: onChange()\n");
		CGLEnv::getInstance().setResolution(width, height);
		CVSNTaskMgr::getInstance().onChange();
	}
	catch (std::exception& ex) {
		LOG(ex.what());
	}
}

void
CVSNGameEnvironment::onSurface()
{
	try {
		LOG("VSN: onSurface()\n");
		CGLEnv::getInstance().recovery();
		CVSNTaskMgr::getInstance().onSurface();
	}
	catch (std::exception& ex) {
		LOG(ex.what());
	}

}

void
CVSNGameEnvironment::onDetach()
{
	try {
		LOG("VSN: onDetach()\n");
		CVSNTaskMgr::getInstance().onDetach();
		CGLEnv::getInstance().destruction();
	}
	catch (std::exception& ex) {
		LOG(ex.what());
	}
}

bool
CVSNGameEnvironment::initDraw2D()
{
	return (m_draw2d = new CVSNDraw2D())->init(m_pathShaders);
	return true;
}

bool
CVSNGameEnvironment::initDraw3D()
{
	// setup util func.
	static CVSN3DLightVecFunc lightvec;
	static CVSN3DLightColFunc lightcol;
	static CVSN3DAmbientFunc  ambient;
	CGLFBO * fbo = createFBO();
	return (m_draw3d = new CVSNDraw3D())->init(m_pathShaders, fbo);
}

CGLFBO *
CVSNGameEnvironment::createFBO()
{
	return new C3DFBO(m_pathShaders, m_width, m_height);
}