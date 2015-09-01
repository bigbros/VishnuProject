#ifndef CCol3DGroup_h
#define CCol3DGroup_h

#include "Types3D.h"
#include "CColBase.h"

// BoundingBox
struct CCol3DBound {
	C3DVec	min;
	C3DVec	max;

	// 初期化
	inline void init(C3DVec& vec) {
		min = vec;
		max = vec;
	}

	// 要素追加
	inline void update(C3DVec& vec) {
		min.x = (min.x < vec.x) ? min.x : vec.x;
		min.y = (min.y < vec.y) ? min.y : vec.y;
		min.z = (min.z < vec.z) ? min.z : vec.z;

		max.x = (max.x < vec.x) ? vec.x : max.x;
		max.y = (max.y < vec.y) ? vec.y : max.y;
		max.z = (max.z < vec.z) ? vec.z : max.z;
	}

	// 他のBoundingBoxと領域が被るかの判定
	inline bool isOverlap(CCol3DBound& t) {
		return
			(min.x <= t.max.x && max.x >= t.min.x) &&
			(min.y <= t.max.y && max.y >= t.max.y) &&
			(min.z <= t.max.z && max.z >= t.max.z);
	}
};


class CCol3DGroup;

// Collider object の基底
class CCol3D : public CColBase
{
	friend class CCol3DGroup;
public:
	enum TYPE {
		C_MESH,		// polygon mesh
		C_BOX,		// oriented box
		C_SPHERE,	// sphere
		C_LINE,		// line strip
		C_POINT		// point
	};
private:
	TYPE				m_type;
	CCol3DGroup		*	m_parent;
	CCol3D			*	m_prev;
	CCol3D			*	m_next;

	C3DMat			*	m_ref;		// 参照行列
	u32					m_bit;		// 更新状態を参照するビットマスク
	u32				*	m_flag;		// マトリクス更新状態が反映されるu32値

	bool				m_enable;	// この判定を有効にするか否か

	// m_flag で指示されるu32値とm_bitのand結果が0でなければ座標の再計算が行われる。

protected:
	CCol3DBound			m_bound;	// この判定の軸方向BoundingBox
public:
	CCol3D(TYPE type, C3DMat * ref, u32 * flag, u32 bit);
	virtual ~CCol3D();

	inline void setEnabled(bool enable) { m_enable = enable; }
	void strip();

	inline void update() {
		// この判定が現在有効で、更新ステータスのポインタが指定されており、マスクビットが0でないものがあれば再計算を行う
		if (m_enable && m_flag && *m_flag & m_bit) update(*m_ref);
	}
	virtual void update(C3DMat& m) = 0;
};


class CCol3DGroup : public CColBase
{
	friend class CCol3D;
	friend class CCol3DGroupMgr;
private:


	CCol3DGroup	*	m_prev;
	CCol3DGroup	*	m_next;

	CCol3D		*	m_begin;
	CCol3D		*	m_end;

	u32				m_gID;	// グループにあたえたID

	int				m_count;	// 保持しているアイテム数

public:
	CCol3DGroup(u32 gID);
	virtual ~CCol3DGroup();

	void add(CCol3D * collider);	// collider 追加

	void strip();					// manager から除去

	void update();					// 登録全オブジェクトについて、必要なものは再計算

	// 他のグループとの間で判定を行う
	void judge(CCol3DGroup * target);
};

class CCol3DGroupMgr
{
	friend class CCol3DGroup;
private:
	CCol3DGroup		*	m_begin;
	CCol3DGroup		*	m_end;
private:
	CCol3DGroupMgr();
	~CCol3DGroupMgr();
public:
	static CCol3DGroupMgr& getInstance();

	void add(CCol3DGroup * grp);
	CCol3DGroup * findGroup(u32 gID);
};

#endif // CCol3DGROUP_h
