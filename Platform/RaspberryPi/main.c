#include <stdio.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include "raspi.h"

typedef struct {
	EGLNativeWindowType  nativeWin;
	EGLDisplay  display;
	EGLContext  context;
	EGLSurface  surface;
	EGLint      majorVersion;
	EGLint      minorVersion;
	int         width;
	int         height;
} ScreenConfig;

EGLBoolean WinCreate(ScreenConfig *sc)
{
	uint32_t success = 0;
	uint32_t width;
	uint32_t height;
	VC_RECT_T dst_rect;
	VC_RECT_T src_rect;
	DISPMANX_ELEMENT_HANDLE_T dispman_element;
	DISPMANX_DISPLAY_HANDLE_T dispman_display;
	DISPMANX_UPDATE_HANDLE_T dispman_update;
	static EGL_DISPMANX_WINDOW_T nativewindow;
	VC_DISPMANX_ALPHA_T alpha = { DISPMANX_FLAGS_ALPHA_FIXED_ALL_PIXELS, 255, 0 };

	success = graphics_get_display_size(0, &width, &height);
	if (success < 0) return EGL_FALSE;

	sc->width = width;
	sc->height = height;

	vc_dispmanx_rect_set(&dst_rect, 0, 0, sc->width, sc->height);
	vc_dispmanx_rect_set(&src_rect, 0, 0, sc->width << 16, sc->height << 16);

	dispman_display = vc_dispmanx_display_open(0);
	dispman_update = vc_dispmanx_update_start(0);
	dispman_element = vc_dispmanx_element_add(dispman_update, dispman_display,
		0, &dst_rect, 0, &src_rect, DISPMANX_PROTECTION_NONE, &alpha, 0, 0);
	vc_dispmanx_update_submit_sync(dispman_update);
	nativewindow.element = dispman_element;
	nativewindow.width = width;
	nativewindow.height = height;
	sc->nativeWin = &nativewindow;
	return EGL_TRUE;
}

EGLBoolean SurfaceCreate(ScreenConfig *sc)
{
	EGLint attrib[] = {
		EGL_RED_SIZE, 5,
		EGL_GREEN_SIZE, 6,
		EGL_BLUE_SIZE, 5,
		EGL_ALPHA_SIZE, 8,
		EGL_DEPTH_SIZE, 8,
		EGL_STENCIL_SIZE, EGL_DONT_CARE,
		EGL_SAMPLE_BUFFERS, 0,
		EGL_NONE
	};
	EGLint context[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
	EGLint numConfigs;
	EGLConfig config;

	sc->display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	if (sc->display == EGL_NO_DISPLAY) return EGL_FALSE;
	if (!eglInitialize(sc->display, &sc->majorVersion, &sc->minorVersion))
		return EGL_FALSE;
	if (!eglChooseConfig(sc->display, attrib, &config, 1, &numConfigs))
		return EGL_FALSE;
	sc->surface = eglCreateWindowSurface(sc->display, config, sc->nativeWin, NULL);
	if (sc->surface == EGL_NO_SURFACE) return EGL_FALSE;
	sc->context = eglCreateContext(sc->display, config, EGL_NO_CONTEXT, context);
	if (sc->context == EGL_NO_CONTEXT) return EGL_FALSE;
	if (!eglMakeCurrent(sc->display, sc->surface, sc->surface, sc->context))
		return EGL_FALSE;
	return EGL_TRUE;
}

void Destroy(ScreenConfig * sc)
{
	if (sc->context != EGL_NO_CONTEXT) {
		eglMakeCurrent(sc->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		eglDestroyContext(sc->display, sc->context);
		sc->context = EGL_NO_CONTEXT;
	}

	if (sc->surface != EGL_NO_SURFACE) {
		eglDestroySurface(sc->display, sc->surface);
		sc->surface = EGL_NO_SURFACE;
	}

	if (sc->display != EGL_NO_DISPLAY) {
		eglTerminate(sc->display);
		sc->display = EGL_NO_DISPLAY;
	}
}

int main(int argc, char *argv[])
{
	ScreenConfig  sc;
	int res;
	void * env = NULL;

	bcm_host_init();
	res = WinCreate(&sc);
	if (!res) return 0;
	res = SurfaceCreate(&sc);
	if (!res) return 0;

	env = vsnCreate(argv[1], sc.width, sc.height, 0);

	int is_continue = 1;
	while (is_continue) {
		/* draw */
		vsnUpdate(env);
		eglSwapBuffers(sc.display, sc.surface);
	}

	vsnDestroy(env);
	return 0;
}
