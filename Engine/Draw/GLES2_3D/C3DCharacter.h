#ifndef C3DCharacter_h
#define C3DCharacter_h

#include "Types3D.h"
#include "CGLAlloc.h"
#include "C3DDrawable.h"
#include "C3DModel.h"

class C3DCharacter : public C3DDrawObj
{
protected:
	struct VEC3 {
		float	x;
		float	y;
		float	z;
	};

	int				m_bonenum;		// 関節の数
	C3DQuat		*	m_motion;		// 現在の各関節の親に対する回転

	C3DQuat		*	m_bonerot;		// 各関節のグローバル回転(uniform)
	VEC3		*	m_bonepos;		// モーション後の関節原点位置(uniform)

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

	inline void setColor(C3DVec& color) {
		m_color = color;
	}

	void render(C3DShader * pShader);
private:
	void makeBones(int bonenum);
};



#endif // C3DCharacter_h
