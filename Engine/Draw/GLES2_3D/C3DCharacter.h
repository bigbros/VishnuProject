#ifndef C3DCharacter_h
#define C3DCharacter_h

#include "Types3D.h"
#include "CGLAlloc.h"
#include "C3DDrawable.h"
#include "C3DModel.h"

class C3DCharacter : public C3DDrawObj
{
protected:
	C3DQuat		*	m_motion;		// 現在の各関節の親に対する回転
	u8			*	m_recalcmotion;	// 各関節を再計算すべきか否かのフラグ
	u32				m_flagUpdate;	// 更新されたマトリクスをフラグで表す(shaderの都合上1オブジェクトあたりmatrixは最大32個)

	C3DVec			m_color;	// 表示カラー係数(RGBA):

	bool			m_ismotion;
public:
	C3DCharacter(C3DModel * model);
	virtual ~C3DCharacter();

	bool calcProcedure(bool recalc);

	inline const C3DQuat& getBoneRotation(int bone) { return m_motion[bone]; }
	void setBoneRotation(int bone, C3DQuat& rot);

	void render(C3DShader * pShader);
};



#endif // C3DCharacter_h
