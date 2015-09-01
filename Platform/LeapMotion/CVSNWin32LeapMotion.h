#ifndef CVSNWin32LeapMotion_h
#define CVSNWin32LeapMotion_h

#include "CVSNLeapMotion.h"

#include "Leap.h"

class CVSNWin32LeapMotion : public CVSNLeapMotion
{
public:
	CVSNWin32LeapMotion();
	virtual ~CVSNWin32LeapMotion();

	bool init();

private:
	Leap::Controller	m_leapCtrl;
};

#endif // CVSNWin32LeapMotion_h
