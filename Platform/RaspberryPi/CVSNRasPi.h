#ifndef CVSNRasPi_h
#define CVSNRasPi_h

#include <stdint.h>
#include "CRasPiEnv.h"
#include "CVSNUnixFS.h"
#include "CVSNPthread.h"

class CVSNRasPi
{
	enum {
		VISHNU_MEMORY = 16
	};
public:
	CVSNRasPi(const char * path, int width, int height, int is_vr);
	virtual ~CVSNRasPi();

	void Update();
	void OnSizeChanged(int width, int height);
	void OnPointing(int id, int mode, int x, int y, float force);
	void OnPause();
	void OnResume();
	void OnDetach();
	void OnSurface();

private:
	CVSNPointing		*	m_Pointing;
	CVSNUnixFS			*	m_Storage;
	CVSNPthread		*	m_Thread;

	CVSNGameEnvironment	*	m_env;

	void				*	m_heapBuf;
	size_t					m_heapSize;

	int64_t					m_prevTime;
};


#endif // CVSNRasPi_h
