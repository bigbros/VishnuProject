#ifndef CCol3DCollision_h
#define CCol3DCollision_h

#include"CCol3DTypes.h"

#define DEFAULT_THICK 1e-8f
#define DEFAULT_ZERO 1e-8f

class CCol3DCollision
{
public:
	// シンプルな3D図形同士の判定を行うユーティリティメソッド

	// ポリゴンメッシュvsいずれか(MESH系はオーダーが大きいため注意)
	static bool isHitMESHandMESH(CCol3DMESH * mesh1, CCol3DMESH * mesh2);
	static bool isHitMESHandBOX(CCol3DMESH * mesh, CCol3DBOX * box);
	static bool isHitMESHandSPHERE(C3DVec& hit, C3DVec&norm, CCol3DMESH * mesh, CCol3DSPHERE * sphere);
	static bool isHitMESHandLINE(C3DVec& hit, C3DVec& norm, CCol3DMESH * mesh, CCol3DLINE * line);
	static bool isHitMESHandPOINT(C3DVec& hit, C3DVec& norm, CCol3DMESH * mesh, CCol3DPOINT * point, float thick = DEFAULT_THICK);

	// BOXvs
	static bool isHitBOXandBOX(CCol3DBOX * box1, CCol3DBOX * box2);
	static bool isHitBOXandSPHERE(CCol3DBOX * box, CCol3DSPHERE * sphere);
	static bool isHitBOXandLINE(CCol3DBOX * box, CCol3DLINE * line);
	static bool isHitBOXandPOINT(CCol3DBOX * box, CCol3DPOINT * point, float thick = DEFAULT_THICK);

	// 球体vsいずれか
	static bool isHitSPHEREandSPHERE(CCol3DSPHERE * sphere1, CCol3DSPHERE * sphere2);
	static bool isHitSPHEREandLINE(C3DVec& hit, C3DVec& norm, CCol3DSPHERE * sphere, CCol3DLINE * line);
	static bool isHitSPHEREandPOINT(CCol3DSPHERE * sphere, CCol3DPOINT * point);

	// 線分vsいずれか
	// 線分vs球体は既にある
	static bool isHitLINEandLINE(CCol3DLINE * line1, CCol3DLINE * line2, float thick = DEFAULT_THICK);
	static bool isHitLINEandPOINT(CCol3DLINE * line, CCol3DPOINT * point, float thick = DEFAULT_THICK);

	// 点vs点
	// 他との組み合わせは既にある
	static bool isHitPOINTandPOINT(CCol3DPOINT * point1, CCol3DPOINT * point2, float thick = DEFAULT_THICK);

	// 引数 thick は点や線分の「太さ」として機能する。厳密に数値的な重なりが無くとも、
	// 両者間の距離がこの値以下になったときに衝突と判断する。
	// 直方体や球体など、少なくともどちらか一方が大きさを持った図形であれば不要だが、
	// 線分や点は大きさを持たないため、この値が無いと衝突することが極めて稀になる。

private:
	static bool distanceHit(C3DVec& v1, C3DVec& v2, float dist);

	// 判定を持つTriangleStripで表されたポリゴンメッシュの頂点バッファ、インデックスバッファを与え、線分との間で判定を行う。
	// 戻り値が true である場合線分はメッシュのいずれかのポリゴンを貫通しており、貫通しているポリゴンのうち
	// 最も線分の始点に近い場所の衝突点がhitに、衝突したポリゴンの法線ベクトルがnormに返される。
	// 戻り値が false の場合、線分はいずれのポリゴンも貫通していない。
	static bool isHitPOLYandLINE(C3DVec& hit, C3DVec& norm, C3DVec * vec, u16 * idx, int idxnum, C3DVec line[2]);

	static bool isHitPOLYandSPHERE(C3DVec& hit, C3DVec& norm, C3DVec * vec, u16 * idx, int idxnum, C3DVec pos, float r);

	static float distanceBOXandPOINT(C3DVec * vec, float * len, C3DVec& pos);

	static void getNormal(C3DVec& norm, C3DVec *vert[3]);

	static bool isHitTriangle(C3DVec * target[3], C3DVec& norm, C3DVec * test[3]);

	static float getLengthOnAxis(C3DVec * axis, C3DVec * vec);
	static float getLengthOnAxis2Triangle(C3DVec * axis, C3DVec ** vec);


	static float distanceLine(C3DVec& hit, C3DVec& begin, C3DVec& end, C3DVec& pos);

	static bool getR_point(C3DVec& hit, C3DVec& begin, C3DVec& end, C3DVec& pos, float r);
};


#endif // CCol3DCollision_h

