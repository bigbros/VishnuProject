#ifndef CVSNGameEnvironment_h
#define CVSNGameEnvironment_h

#include <stdio.h>
#include "CVSNPlatform.h"
#include "CVSNScript.h"
#include "CVSNPointingDevice.h"
#include "CVSNScriptDriver.h"
#include "CVSNDraw3D.h"
#include "CVSNDraw2D.h"

#define MEGA(num) ((num) * 1024 * 1024)

class CVSNGameEnvironment
{
public:
	CVSNGameEnvironment(void * pHeapBuffer, size_t sizeHeap, int width, int height);
	virtual ~CVSNGameEnvironment();

	bool Startup(const char * shaderPath, const char * scriptPath, const char * firstScript);

	void Update(int deltaT);
	void onPause();
	void onResume();
	void onChange(int width, int height);
	void onSurface();
	void onDetach();

private:
	void setShadersPath(const char * pathShaders);
	void setScriptPath(const char * scriptPath, const char * firstScript);
	bool initSystemTask();

protected:
	virtual bool initDraw3D();
	virtual bool initDraw2D();
	virtual CGLFBO * createFBO();
	virtual CVSNScript * createScriptSystem() = 0;
	virtual bool initUserTask() = 0;

protected:
	void				*	m_pHeapBuffer;
	size_t					m_sizeHeap;

	CVSNScript			*	m_scriptSystem;
	CVSNPointingDevice	*	m_pointingDevice;
	CVSNDraw3D			*	m_draw3d;
	CVSNDraw2D			*	m_draw2d;
	CVSNScriptDriver	*	m_script;

	const char			*	m_firstScript;
	const char			*	m_pathShaders;

	int						m_width;
	int						m_height;
};


#endif // CVSNGameEnvironment_h
