#ifndef CVSNWin32Tracking_h
#define CVSNWIn32Tracking_h

#include "OVR_CAPI.h"

#include "CVSNTracking.h"

// Win32�v���b�g�t�H�[�����̂ɂ͉����x�Z���T�[�ɂ��g���b�L���O�V�X�e���������Ȃ����̂��������A
// Oculus VR Rift �ł̎����ɂ��킹�Ă����B
class CVSNWin32Tracking : public CVSNTracking
{
public:
	CVSNWin32Tracking();
	virtual ~CVSNWin32Tracking();

	bool init();

	// 3���Z���T�[�ɂ���]�ƕ��s�ړ���Ԃ��B
	// ��]��quaternion�ɕϊ�����ĕԂ����B�Z���T�[�ɂ�鎲��]�̏��Ԃ��قȂ�\�������邽�߁B
	// ���s�ړ��̓x�N�g���ŁA�P�ʂ�[m]�ŕԂ��B
	virtual bool getStatus(C3DQuat& rot, C3DVec& trans);

	// 3���Z���T�[�̈ʒu/��]�������Z�b�g����B
	// ��̓I�ɉ����s���邩�̓f�o�C�X�ɂ��B
	virtual bool reposition();
private:

	ovrHmd			m_hmd;
	ovrSizei		m_resolution;
};

#define VSN_OVR_SUPPORT_CAPS (ovrTrackingCap_Orientation | ovrTrackingCap_MagYawCorrection | ovrTrackingCap_Position)
#define VSN_OVR_REQUIRE_CAPS 0


#endif // CVSNWin32Tracking_h
