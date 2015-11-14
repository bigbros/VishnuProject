#include <stdlib.h>
#include <string.h>
#include <memory>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <io.h>

#include "CVSNWin32Tracking.h"

#include "platform.h"

CVSNWin32Tracking::CVSNWin32Tracking()
	: CVSNTracking()
	, m_hmd(0)
{

}

CVSNWin32Tracking::~CVSNWin32Tracking()
{
	if (m_hmd) ovr_Shutdown();
}


bool
CVSNWin32Tracking::init()
{
	if (ovr_Initialize(0)) {

		m_hmd = ovrHmd_Create(0);

		if (m_hmd) {
			m_resolution = m_hmd->Resolution;

			ovrBool result = ovrHmd_ConfigureTracking(m_hmd, VSN_OVR_SUPPORT_CAPS, VSN_OVR_REQUIRE_CAPS);
			ovrHmd_RecenterPose(m_hmd);
			return (result == ovrTrue);
		}
		return false;
	}
	return false;
}

bool
CVSNWin32Tracking::getStatus(C3DQuat& rot, C3DVec& trans)
{
	if (m_hmd) {
		// Query the HMD for the current tracking state.
		ovrTrackingState ts = ovrHmd_GetTrackingState(m_hmd, ovr_GetTimeInSeconds());

		if (ts.StatusFlags & (ovrStatus_OrientationTracked | ovrStatus_PositionTracked))
		{
			ovrPoseStatef pose = ts.HeadPose;
			ovrQuatf& rot_stat = pose.ThePose.Orientation;

			C3DQuat quat(rot_stat.x, rot_stat.y, rot_stat.z, rot_stat.w);
			rot = quat;
			return true;
		}
	}
	// 取得できない場合(更新がエラーの場合)は回転なし、移動なしの値を設定し、falseを返す。
	rot.set(0.0f, 0.0f, 0.0f, 1.0f);
	trans.set(0.0f, 0.0f, 0.0f, 1.0f);
	return false;
}

bool
CVSNWin32Tracking::reposition()
{
	return true;
}