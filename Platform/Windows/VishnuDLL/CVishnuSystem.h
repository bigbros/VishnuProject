#ifndef CVishnuSystem_h
#define CVishnuSystem_h

#include "CTestEnv.h"
#include "CVSNWin32FS.h"
#include "CVSNWin32Thread.h"
#include "CWin32GLES2.h"

class CVishnuSystem
{
	enum {
		VISHNU_MEMORY = 16
	};
public:
	CVishnuSystem(const char * path, int width, int height);
	virtual ~CVishnuSystem();

	void Update();
	void OnSizeChanged(int width, int height);
	void OnPointing(int id, int mode, int x, int y, float force);
	void OnPause();
	void OnResume();
	void OnDetach();
	void OnSurface();

private:
	CVSNPointing	*	m_Pointing;
	CVSNWin32FS		*	m_Storage;
	CVSNWin32Thread	*	m_Thread;
	CTestEnv		*	m_env;

	void			*	m_heapBuf;
	size_t				m_heapSize;

	DWORD				m_prevTime;
};


#endif // CVishnuGLES2_h
