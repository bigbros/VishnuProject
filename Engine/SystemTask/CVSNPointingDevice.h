#ifndef CVSNPointingDevice_h
#define CVSNPointingDevice_h

#include "CVSNTask.h"
#include "CVSNPointing.h"
#include "CVSNTouchPanel.h"

class CVSNPointingDevice : public CVSNTask
{
public:
	CVSNPointingDevice();
	virtual ~CVSNPointingDevice();

	bool init();

	void update(int deltaT);
	void die();

private:

	CVSNPointListener	m_listener;
};


#endif // CVSNPointingDevice_h
