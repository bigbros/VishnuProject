#ifndef CVSNTracking_h
#define CVSNTracking_h


#include "CVSNPlatform.h"
#include "Types3D.h"

class CVSNTracking : public IVSNModule
{
public:
	CVSNTracking();
	virtual ~CVSNTracking();
	
	// 3���Z���T�[�ɂ���]�ƕ��s�ړ���Ԃ��B
	// ��]��quaternion�ɕϊ�����ĕԂ����B�Z���T�[�ɂ�鎲��]�̏��Ԃ��قȂ�\�������邽�߁B
	// ���s�ړ��̓x�N�g���ŁA�P�ʂ�[m]�ŕԂ��B
	virtual bool getStatus(C3DQuat& rot, C3DVec& trans) = 0;

	// 3���Z���T�[�̈ʒu/��]�������Z�b�g����B
	// ��̓I�ɉ����s���邩�̓f�o�C�X�ɂ��B
	virtual bool reposition() = 0;
};

#endif // CVSNTracking_h
