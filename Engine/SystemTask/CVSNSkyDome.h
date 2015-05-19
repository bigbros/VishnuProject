#ifndef CVSNCelestialSphere_h
#define CVSNCelestialSphere_h

#include "CVSNScriptTask.h"
#include "C3DSkyDome.h"
#include "C3DSkyDomeShader.h"

#include "CVSNPNGTex.h"

class CVSNSkyDome : public CVSNScriptTask
{
public:
	CVSNSkyDome();
	virtual ~CVSNSkyDome();

	bool init(CVSNScriptArgs& args);
	void update(int deltaT);
	void die();

private:
	C3DSkyDomeShader	*	m_shader;
	C3DSkyDome	*	m_skydome;
	CVSNPNGTex	*	m_pTex;
};
#endif // CVSNCelestialSphere_h
