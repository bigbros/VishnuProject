#include <tchar.h>
#include <Windows.h>
#include <iostream>
#include "esUtil.h"
#include "CTestEnv.h"
#include "CVSNLuaScript.h"
#include "platform.h"
#include "CVSNKVObj.h"
#include "CVSNWin32FS.h"
#include "CVSNWin32Thread.h"

void draw(ESContext * esContext);

struct UserData {
	GLuint						programObject;
	CVSNGameEnvironment		*	pEnv;
	DWORD						prevTime;
	CVSNPointing			*	pPointing;
	bool						touched;
};

LRESULT WINAPI myWinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT  lRet = 1;
	ESContext *esContext = (ESContext*)(LONG_PTR)GetWindowLongPtr(hWnd, GWL_USERDATA);
	UserData * ud = (UserData *)esContext->userData;
	switch (uMsg)
	{
	case WM_LBUTTONDOWN: {
		int x = lParam & 0xffff;
		int y = (lParam >> 16) & 0xffff;
		ud->pPointing->setEvent(0, IVSNPointListener::PT_TAP, x, y, 1.0f);
		ud->touched = true;
		break;
	}
	case WM_LBUTTONUP: {
		int x = lParam & 0xffff;
		int y = (lParam >> 16) & 0xffff;
		ud->pPointing->setEvent(0, IVSNPointListener::PT_RELEASE, x, y, 1.0f);
		ud->touched = false;
		break;
	}
	case WM_MOUSEMOVE: {
		if (ud->touched) {
			int x = lParam & 0xffff;
			int y = (lParam >> 16) & 0xffff;
			ud->pPointing->setEvent(0, IVSNPointListener::PT_DRAG, x, y, 1.0f);
		}
		break;
	}
	default:
		lRet = 0;
		break;
	}
	return lRet;
}

#define WIDTH 640
#define HEIGHT 960
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	ESContext esContext;
	esInitContext(&esContext);
	GLboolean status = esCreateWindow(&esContext, "Vishnu Project", WIDTH, HEIGHT,
		ES_WINDOW_RGB | ES_WINDOW_DEPTH | ES_WINDOW_ALPHA);


	// platform ‚Ì—pˆÓ
	CVSNPointing pointing;
	CVSNWin32FS storage(".");
	CVSNWin32Thread w32thread;
	CVSNPlatform::getInstance().registModule(&pointing);
	CVSNPlatform::getInstance().registModule(&storage);
	CVSNPlatform::getInstance().registModule(&w32thread);

	size_t heapSize = MEGA(4);
	void * heapBuf = malloc(heapSize);
	{
		CTestEnv envGame(heapBuf, heapSize, WIDTH, HEIGHT);

		UserData userData;
		userData.pEnv = &envGame;
		userData.prevTime = timeGetTime();
		userData.pPointing = &pointing;
		userData.touched = false;
		esContext.userData = &userData;
		esContext.eventProc = myWinProc;
		esRegisterDrawFunc(&esContext, draw);

		envGame.Startup("shaders", "LuaScripts", "startup");
		esMainLoop(&esContext);
	}
	free(heapBuf);

	return 0;
}

void draw(ESContext * esContext)
{
	UserData * pUser = (UserData *)esContext->userData;
	int deltaT;
	DWORD now;
	now = timeGetTime();
	deltaT = (int)(now - pUser->prevTime);
	pUser->prevTime = now;
	char buf[1024];
	if (deltaT > 0 && (1000/deltaT) < 60) { sprintf(buf, "%d fps\n", 1000 / deltaT); LOG(buf); }
	pUser->pEnv->Update(deltaT);
	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
}

