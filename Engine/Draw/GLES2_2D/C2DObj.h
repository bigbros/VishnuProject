#ifndef C2DObj_h
#define C2DObj_h

#include <GLES2/gl2.h>
#include "Types2D.h"
#include "dtypes.h"
#include "CGLBase.h"

class C2DObj : public CGLBase
{
	friend class C2DDrawEnv;
	friend class C2DRootObj;
private:
	struct OBJLIST {
		OBJLIST();
		C2DObj	*	begin;
		C2DObj	*	end;
	};
	struct OBJLINK {
		OBJLINK();
		C2DObj	*	prev;
		C2DObj	*	next;
	};
	C2DObj	*	m_parent;			// 親オブジェクト
	OBJLIST		m_childs;			// 子オブジェクト
	OBJLINK		m_sisters;			// 姉妹オブジェクト

	bool		m_visible;	// 表示状態

protected:
	bool			m_recalc;	// 移動/回転/リサイズしたのでmatrixの再計算が必要
	C2DMat			m_matrix;	// 自身の計算用マトリクス

	C2DVec			m_position;	// ローカル位置
	float			m_rotation;	// ローカル回転

	int				m_priority;	// 表示プライオリティ
public:
	C2DObj();
	virtual ~C2DObj();

	inline void setVisible(bool visible) throw() {
		m_recalc = visible; // && !m_visible;
		m_visible = visible;
	}

	inline const C2DObj * getParent() const throw() { return m_parent; }

	void connectParent(C2DObj * parent, int matidx = 0);
	inline void addChild(C2DObj * child) throw() { child->connectParent(this); }

	inline void setRotation(const float rot) { m_rotation = rot;  m_recalc = true; }
	inline void setPosition(const C2DVec& vec) { m_position = vec;  m_recalc = true; }

	inline const float & getRotation() { return m_rotation; }
	inline const C2DVec  & getPosition() { return m_position; }

	bool setPriority(int pri);

protected:
	// このオブジェクト以下に対し、
	// 指定されたマトリクスを上位マトリクスとして再計算を行う。
	void recalcMatrix(C2DMat& m, bool upper_recalc = false);

	// このオブジェクトのマトリクス計算後に行う処理を記述
	virtual bool calcProcedure(bool is_recalc) = 0;

	// プライオリティが再設定された際にやるべき処理
	virtual bool priorityProcedure(int priority);

private:
	void removeByParent();
};

#endif // C2DObj_h
