#include <Windows.h>
#include "SampleApplication.h"

#include "CVSNGameEnvironment.h"
#include "CVSNLuaScript.h"
#include "platform.h"
#include "CVSNKVObj.h"

class GLESTest : public SampleApplication
{
public:
	GLESTest::GLESTest() : SampleApplication("GLESTest", 640, 480), m_luaScript(), m_heapBuf(0), m_envGame(0), m_touched(false) {
		LOG("Constructor.");
	}

	virtual bool initialize()
	{
		CVSNPlatform::getInstance().registModule(&m_pointing);
		size_t heapSize = MEGA(4);
		m_heapBuf = malloc(heapSize);
		m_envGame = new CVSNGameEnvironment(&m_luaScript, m_heapBuf, heapSize);

		m_envGame->Startup("./LuaScripts", "startup");

		return true;
	}

	virtual void destroy()
	{
		delete m_envGame;
		free(m_heapBuf);
	}

	virtual void draw()
	{
		glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		int deltaT;
		DWORD now;
		now = timeGetTime();
		deltaT = (int)(now - m_prevTime);
		m_prevTime = now;
		m_envGame->Update(deltaT);
	}
private:
	CVSNPointing			m_pointing;
	void				*	m_heapBuf;
	CVSNLuaScript			m_luaScript;
	CVSNGameEnvironment	*	m_envGame;

	bool					m_touched;
	DWORD					m_prevTime;
};


int main(int argc,char **argv)
{
	GLESTest app;
	return app.run();
}
