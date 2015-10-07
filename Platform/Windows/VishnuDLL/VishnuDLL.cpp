// VishnuDLL.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"
#include "VishnuDLL.h"
#include "CWin32GLES2.h"
#include "CVishnuSystem.h"
#include <OVR_CAPI.h>

extern "C" {

	static void(*funcLogDisp)(const char * msg) = 0;

	void * gles2CreateWindow(HWND hWnd)
	{
		CWin32GLES2 * gles2 = new CWin32GLES2(hWnd);
		return (void *)gles2;
	}

	void gles2DestroyWindow(void * ptr)
	{
		CWin32GLES2 * gles2 = static_cast<CWin32GLES2 *>(ptr);
		delete gles2;
	}

	void gles2Update(void * ptr)
	{
		CWin32GLES2 * gles2 = static_cast<CWin32GLES2 *>(ptr);
		gles2->UpdateBySystem();
	}

	void gles2Flip(void * ptr)
	{
		CWin32GLES2 * gles2 = static_cast<CWin32GLES2 *>(ptr);
		gles2->Flip();
	}

	void gles2Resize(void * ptr, int width, int height)
	{
		CWin32GLES2 * gles2 = static_cast<CWin32GLES2 *>(ptr);
		gles2->Resize(width, height);
	}

	void * vsnCreate(const char * path, int width, int height, int is_vr) {
		CVishnuSystem * vsn = new CVishnuSystem(path, width, height, is_vr);
		return vsn;
	}

	void vsnDestroy(void * ptr)
	{
		CVishnuSystem * vsn = static_cast<CVishnuSystem *>(ptr);
		delete vsn;
	}

	void vsnUpdate(void * ptr)
	{
		CVishnuSystem * vsn = static_cast<CVishnuSystem *>(ptr);
		vsn->Update();
	}

	void vsnOnSizeChanged(void * ptr, int width, int height)
	{
		CVishnuSystem * vsn = static_cast<CVishnuSystem *>(ptr);
		vsn->OnSizeChanged(width, height);
	}

	void vsnOnPointing(void * ptr, int id, int mode, int x, int y, float force)
	{
		CVishnuSystem * vsn = static_cast<CVishnuSystem *>(ptr);
		vsn->OnPointing(id, mode, x, y, force);
	}

	void vsnOnPause(void * ptr)
	{
		CVishnuSystem * vsn = static_cast<CVishnuSystem *>(ptr);
		vsn->OnPause();
	}

	void vsnOnResume(void * ptr)
	{
		CVishnuSystem * vsn = static_cast<CVishnuSystem *>(ptr);
		vsn->OnResume();
	}

	void vsnOnDetach(void * ptr)
	{
		CVishnuSystem * vsn = static_cast<CVishnuSystem *>(ptr);
		vsn->OnDetach();
	}

	void vsnOnSurface(void * ptr)
	{
		CVishnuSystem * vsn = static_cast<CVishnuSystem *>(ptr);
		vsn->OnSurface();
	}

	void vsnSetLogFunc(void(*func)(const char *))
	{
		funcLogDisp = func;
	}

	void LogFunc(const char * msg) {
		if (funcLogDisp) funcLogDisp(msg);
	}
}
