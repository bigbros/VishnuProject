#include "CVSNWin32LeapMotion.h"

CVSNWin32LeapMotion::CVSNWin32LeapMotion()
	: CVSNLeapMotion()
	, m_leapCtrl()
{
}

CVSNWin32LeapMotion::~CVSNWin32LeapMotion()
{
}

bool
CVSNWin32LeapMotion::init()
{
	return true;
}