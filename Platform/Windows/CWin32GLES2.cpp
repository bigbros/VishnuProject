#include "CWin32GLES2.h"
#include <memory>

#define CLASS_NAME "GLES2Window"


CWin32GLES2::CWin32GLES2(HWND hWnd)
	: m_eglDisplay(EGL_NO_DISPLAY)
	, m_eglSurface(EGL_NO_SURFACE)
	, m_eglContext(EGL_NO_CONTEXT)
	, m_hWnd(0)
	, m_hDC(0)
{
	if (!createWindow(hWnd)) {
		throw std::bad_alloc();
	}

	if (!createEGL()) {
		destroyWindow();
		throw std::bad_alloc();
	}
}

CWin32GLES2::~CWin32GLES2()
{
	destroyEGL();
	destroyWindow();

}

void
CWin32GLES2::destroyWindow()
{
	if (m_hDC) {
		ReleaseDC(m_hWnd, m_hDC);
		m_hDC = NULL;
	}
	if (m_hWnd) {
		m_hWnd = NULL;
	}
}

bool
CWin32GLES2::createWindow(HWND hWnd)
{
	RECT parentRect;
	GetWindowRect(hWnd, &parentRect);
	m_width = parentRect.right - parentRect.left;
	m_height = parentRect.bottom - parentRect.top;
	m_hWnd = hWnd;

	m_hDC = GetDC(m_hWnd);
	if (!m_hDC) {
		destroyWindow();
		return false;
	}

	return true;
}


void
CWin32GLES2::destroyEGL()
{
	if (m_eglContext != EGL_NO_CONTEXT) {
		eglMakeCurrent(m_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		eglDestroyContext(m_eglDisplay, m_eglContext);
		m_eglContext = EGL_NO_CONTEXT;
	}

	if (m_eglSurface != EGL_NO_SURFACE) {
		eglDestroySurface(m_eglDisplay, m_eglSurface);
		m_eglSurface = EGL_NO_SURFACE;
	}

	if (m_eglDisplay != EGL_NO_DISPLAY) {
		eglTerminate(m_eglDisplay);
		m_eglDisplay = EGL_NO_DISPLAY;
	}
 }

bool
CWin32GLES2::createEGL()
{
	m_eglDisplay = eglGetDisplay(m_hDC);
	if (m_eglDisplay == EGL_NO_DISPLAY) {
		m_eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
		if (m_eglDisplay == EGL_NO_DISPLAY) return false;
	}

	if (!eglInitialize(m_eglDisplay, 0, 0)) {
		m_eglDisplay = EGL_NO_DISPLAY;
		return false;
	}

	const EGLint configs[] = {
		EGL_RED_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_BLUE_SIZE, 8,
		EGL_ALPHA_SIZE, 8,
		EGL_DEPTH_SIZE, 24,
		EGL_STENCIL_SIZE, EGL_DONT_CARE,
		EGL_SAMPLE_BUFFERS, 0,
		EGL_NONE
	};

	EGLConfig configuration;
	EGLint numFoundConfigurations = 0;
	if (!eglChooseConfig(m_eglDisplay, configs, &configuration, 1, &numFoundConfigurations)) {
		destroyEGL();
		return false;
	}

	if (numFoundConfigurations < 1) {
		destroyEGL();
		return false;
	}

	m_eglSurface = eglCreateWindowSurface(m_eglDisplay, configuration, static_cast<EGLNativeWindowType>(m_hWnd), 0);
	if (!m_eglSurface) {
		destroyEGL();
		return false;
	}

	eglBindAPI(EGL_OPENGL_ES_API);

	EGLint attribList[] = {
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};

	m_eglContext = eglCreateContext(m_eglDisplay, configuration, EGL_NO_CONTEXT, attribList);
	if (m_eglContext == EGL_NO_CONTEXT) {
		destroyEGL();
		return false;
	}

	if (!eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext)) {
		destroyEGL();
		return false;
	}

	return true;
}

void
CWin32GLES2::UpdateBySystem()
{
	eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext);
}

void
CWin32GLES2::Flip()
{
	eglSwapBuffers(m_eglDisplay, m_eglSurface);
}

void
CWin32GLES2::Resize(int width, int height)
{
	m_width = width;
	m_height = height;
}
