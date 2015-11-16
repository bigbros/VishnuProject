#ifndef C3DObj_h
#define C3DObj_h

#include <GLES2/gl2.h>
#include "Types3D.h"
#include "dtypes.h"
#include "CGLBase.h"

class C3DObj : public CGLBase
{
	friend class C3DRootObj;
private:
	struct OBJLIST {
		OBJLIST();
		C3DObj	*	begin;
		C3DObj	*	end;
	};
	struct OBJLINK {
		OBJLINK();
		C3DObj	*	prev;
		C3DObj	*	next;
	};
	C3DObj	*	m_parent;			// 親オブジェクト
	int			m_parent_matidx;	// 親オブジェクトの持つ何番目のマトリクスに追従するか(通常は0)
	OBJLIST		m_childs;			// 子オブジェクト
	OBJLINK		m_sisters;			// 姉妹オブジェクト

	bool		m_visible;	// 表示状態
	bool		m_recalc;	// 移動/回転したのでmatrixの再計算が必要

protected:

	C3DMat		m_matrix;	// 位置/回転をあらわす matrix(一つ以上)

	C3DVec		m_position;	// ローカル位置
	C3DQuat		m_rotation;	// ローカル回転


public:
	C3DObj();
	virtual ~C3DObj();

	inline void setVisible(bool visible) throw() {
		m_recalc = visible; // && !m_visible;
		m_visible = visible;
	}

	inline const C3DObj * getParent() const throw() { return m_parent; }

	void connectParent(C3DObj * parent, int matidx = 0);
	inline void addChild(C3DObj * child) throw() { child->connectParent(this); }

	inline void setRotation(const C3DQuat& quat) { m_rotation = quat;  m_recalc = true; }
	inline void setPosition(const C3DVec& vec) { m_position = vec;  m_recalc = true; }

	inline const C3DQuat & getRotation() { return m_rotation; }
	inline const C3DVec  & getPosition() { return m_position; }

protected:

	// このオブジェクト以下に対し、
	// 指定されたマトリクスを上位マトリクスとして再計算を行う。
	void recalcMatrix(C3DMat& m, bool upper_recalc = false);

	// このオブジェクトのマトリクス計算後に行う処理を記述
	virtual bool calcProcedure(bool is_recalc) = 0;

private:
	void removeByParent();
};

#endif // C3DObj_h
