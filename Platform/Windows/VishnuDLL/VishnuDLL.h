// 以下の ifdef ブロックは DLL からのエクスポートを容易にするマクロを作成するための 
// 一般的な方法です。この DLL 内のすべてのファイルは、コマンド ラインで定義された VISHNUDLL_EXPORTS
// シンボルを使用してコンパイルされます。このシンボルは、この DLL を使用するプロジェクトでは定義できません。
// ソースファイルがこのファイルを含んでいる他のプロジェクトは、 
// VISHNUDLL_API 関数を DLL からインポートされたと見なすのに対し、この DLL は、このマクロで定義された
// シンボルをエクスポートされたと見なします。
#ifdef VISHNUDLL_EXPORTS
#define VISHNUDLL_API __declspec(dllexport)
#else
#define VISHNUDLL_API __declspec(dllimport)
#endif

extern "C" {
	VISHNUDLL_API void * gles2CreateWindow(HWND parent_hWnd);
	VISHNUDLL_API void gles2DestroyWindow(void * ptr);
	VISHNUDLL_API void gles2Resize(void * ptr, int width, int height);
	VISHNUDLL_API void gles2Update(void * ptr);
	VISHNUDLL_API void gles2Flip(void * ptr);


	VISHNUDLL_API void * vsnCreate(const char * path, int width, int height);
	VISHNUDLL_API void vsnDestroy(void * ptr);
	VISHNUDLL_API void vsnUpdate(void * ptr);
	VISHNUDLL_API void vsnOnSizeChanged(void * ptr, int width, int height);
	VISHNUDLL_API void vsnOnPointing(void * ptr, int id, int mode, int x, int y, float force);
	VISHNUDLL_API void vsnOnPause(void * ptr);
	VISHNUDLL_API void vsnOnResume(void * ptr);
	VISHNUDLL_API void vsnOnDetach(void * ptr);
	VISHNUDLL_API void vsnOnSurface(void * ptr);
	VISHNUDLL_API void vsnSetLogFunc(void(*func)(const char *));
}
