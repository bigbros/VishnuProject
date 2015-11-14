#include <stdio.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>

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

static ScreenConfig  g_sc;

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
  VC_DISPMANX_ALPHA_T alpha = {DISPMANX_FLAGS_ALPHA_FIXED_ALL_PIXELS, 255, 0};

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
    EGL_RED_SIZE,       8,
    EGL_GREEN_SIZE,     8,
    EGL_BLUE_SIZE,      8,
    EGL_ALPHA_SIZE,     8,
    EGL_DEPTH_SIZE,     24,
    EGL_NONE
  };
  EGLint context[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
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

int main ( int argc, char *argv[] )
{
  unsigned int frames = 0;
  int res;

  bcm_host_init();
  res = WinCreate(&g_sc);
  if (!res) return 0;
  res = SurfaceCreate(&g_sc);
  if (!res) return 0;

  glClearColor(0.0f, 0.3f, 0.0f, 0.5f);

  /* 600frame / 60fps = 10sec */ 
  while(frames < 600) {
    glViewport(0, 0, g_sc.width, g_sc.height);
    glClear(GL_COLOR_BUFFER_BIT);
 
/*   Draw(); */

    eglSwapBuffers(g_sc.display, g_sc.surface);
    frames++;
  }
  return 0;
}