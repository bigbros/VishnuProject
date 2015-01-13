#ifndef C3DCharacter_h
#define C3DCharacter_h

#include "Types3D.h"
#include "CGLAlloc.h"
#include "C3DDrawable.h"
#include "C3DModel.h"

class C3DCharacter : public C3DDrawObj
{
protected:
	C3DQuat		*	m_motion;		// ���݂̊e�֐߂̐e�ɑ΂����]
	u8			*	m_recalcmotion;	// �e�֐߂��Čv�Z���ׂ����ۂ��̃t���O

	C3DVec			m_color;	// �\���J���[�W��(RGBA):

	bool			m_ismotion;
public:
	C3DCharacter(C3DModel * model);
	virtual ~C3DCharacter();

	bool calcProcedure(bool recalc);

	inline const C3DQuat& getBoneRotation(int bone) { return m_motion[bone]; }
	void setBoneRotation(int bone, C3DQuat& rot);

	void render(C3DDrawEnv * env);
};



#endif // C3DCharacter_h
