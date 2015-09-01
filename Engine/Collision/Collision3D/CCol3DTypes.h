#ifndef CCol3DTypes_h
#define CCol3DTypes_h

#include"CCol3DGROUP.h"

// ポリゴンメッシュ
class CCol3DMESH : public CCol3D
{
public:
	CCol3DMESH(C3DMat * ref, u32 * flag, u32 bit, C3DVec * vert, int vnum, u16 * idx, int inum);
	virtual ~CCol3DMESH();
	C3DVec	*	vertorg;	// 未回転状態の頂点バッファ
	u16		*	indices;	// インデックスバッファ
	int			vert_num;	// 頂点数
	int			idx_num;	// 

	C3DVec	*	vertices;	// 回転後の頂点バッファ

	void update(C3DMat& mat);
};

// 回転可能直方体
class CCol3DBOX : public CCol3D
{
public:
	CCol3DBOX(C3DMat * ref, u32 * flag, u32 bit, C3DVec& size, C3DMat& m);
	virtual ~CCol3DBOX();
	C3DVec		org[4];	// ローカル座標上で回転された状態の各方向ベクトル
	float		len[4];	// 各方向ベクトルの長さ(sizeの半分)
	C3DVec		size;	// 回転していない状態における直方体の各軸方向サイズ(辺の長さ)

	C3DVec		vec[4];	// [0]平行移動済み中心位置 / [1]～[3] 回転済みの各軸方向ベクトル(サイズの半分長)

	// 現在のマトリクス値で vec[] の内容を再計算する
	void update(C3DMat& m);

	float getLengthOnAxis(C3DVec& axis);

private:
	static C3DVec	axis_x;
	static C3DVec	axis_y;
	static C3DVec	axis_z;
};

// 球体
class CCol3DSPHERE : public CCol3D
{
public:
	CCol3DSPHERE(C3DMat * ref, u32 * flag, u32 bit, C3DVec& pos, float r);
	virtual ~CCol3DSPHERE();
	C3DVec	org;	// ローカル座標位置
	float	r;		// 半径

	C3DVec	vec;	// マトリクスで移動された中心

	void update(C3DMat& m);
};

// 線分
class CCol3DLINE : public CCol3D
{
public:
	CCol3DLINE(C3DMat * ref, u32 * flag, u32 bit, C3DVec * pos);
	virtual ~CCol3DLINE();
	C3DVec	org[2];
	C3DVec	vec[2];	// [0]始点 / [1]終点

	void update(C3DMat& m);
};

// 点
class CCol3DPOINT : public CCol3D
{
public:
	CCol3DPOINT(C3DMat * ref, u32 * flag, u32 bit, C3DVec& vec);
	virtual ~CCol3DPOINT();
	C3DVec	org;
	C3DVec	vec;	// 位置
	void update(C3DMat& m);
};

#endif // CCOl3DTypes_h
