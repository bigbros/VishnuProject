#ifndef CVSNCelestialSphere_h
#define CVSNCelestialSphere_h

#include "CVSNScriptTask.h"
#include "C3DCelestialSphere.h"

#include "CVSNPNGTex.h"

class CVSNCelestialSphere : public CVSNScriptTask
{
public:
	CVSNCelestialSphere();
	virtual ~CVSNCelestialSphere();

	bool init(CVSNScriptArgs& args);
	void update(int deltaT);
	void die();

private:
	C3DCelestialSphere	*	m_sphere;
	CVSNPNGTex			*	m_pTex;
};
#endif // CVSNCelestialSphere_h
