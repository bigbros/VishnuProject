#ifndef CVSNWin32Tracking_h
#define CVSNWIn32Tracking_h

#include "OVR_CAPI.h"

#include "CVSNTracking.h"

// Win32プラットフォーム自体には加速度センサーによるトラッキングシステムを持たないものが多いが、
// Oculus VR Rift での実装にあわせておく。
class CVSNWin32Tracking : public CVSNTracking
{
public:
	CVSNWin32Tracking();
	virtual ~CVSNWin32Tracking();

	bool init();

	// 3軸センサーによる回転と並行移動を返す。
	// 回転はquaternionに変換されて返される。センサーによる軸回転の順番が異なる可能性があるため。
	// 平行移動はベクトルで、単位は[m]で返す。
	virtual bool getStatus(C3DQuat& rot, C3DVec& trans);

	// 3軸センサーの位置/回転等をリセットする。
	// 具体的に何が行われるかはデバイスによる。
	virtual bool reposition();
private:

	ovrHmd			m_hmd;
	ovrSizei		m_resolution;
};

#define VSN_OVR_SUPPORT_CAPS (ovrTrackingCap_Orientation | ovrTrackingCap_MagYawCorrection | ovrTrackingCap_Position)
#define VSN_OVR_REQUIRE_CAPS 0


#endif // CVSNWin32Tracking_h
