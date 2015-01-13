#ifndef CWin32GLES2_h
#define CWin32GLES2_h

#include <tchar.h>
#include <Windows.h>

#include <GLES2/gl2.h>
#include <EGL/egl.h>


class CWin32GLES2
{
private:
	int				m_width;
	int				m_height;
protected:
	HWND			m_hWnd = NULL;
	HDC				m_hDC = NULL;
	EGLDisplay		m_eglDisplay;
	EGLSurface		m_eglSurface;
	EGLContext		m_eglContext;

public:
	CWin32GLES2(HWND hWnd);
	virtual ~CWin32GLES2();

	void UpdateBySystem();
	void Flip();
	void Resize(int width, int height);
	void Pointing(int id, int mode, int x, int y, float force);

	inline int width() const { return m_width; }
	inline int height() const { return m_height; }

private:
	bool createWindow(HWND hWnd);
	void destroyWindow();
	void destroyEGL();
	bool createEGL();
};

#endif // win32child_h
