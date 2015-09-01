#include <iostream>
#include "CCol3DCollision.h"

// 三角形の2辺をベクトルとみなし、その外積を求めることで
// 三角形の法線を求める。
// データとして法線を持ち頂点同様にマトリクスで回転させるよりも、
// その場で外積を用いて求めたほうが計算量が少ない。
void
CCol3DCollision::getNormal(C3DVec& norm, C3DVec *vert[3])
{
	C3DVec v0 = *vert[1] - *vert[0];
	C3DVec v1 = *vert[2] - *vert[1];
	C3DVec vc = v0 / v1;
	norm = vc.unit();
}

// ある三角形を、別の三角形の辺が一つ以上通過しているかを判定する。
bool
CCol3DCollision::isHitTriangle(C3DVec * target[3], C3DVec& norm, C3DVec * test[3])
{
	float dist[3];
	// test側各頂点について、targetの面からの距離を求める
	for (int i = 0; i < 3; i++) {
		C3DVec v = *test[i] - *target[0];
		dist[i] = v % norm;	// 法線を表す単位ベクトルとの内積が、そのまま面からの距離
	}

	// test の各辺が target 内部を通過しているか調べる
	for (int i = 0; i < 3; i++) {
		int nxt = (i + 1) % 3;
		// 双方の符号が同じであればどちらも面の同じ側にあるので、辺は絶対に面を通過していない。
		if ((dist[i] * dist[nxt]) > 0.0f) continue;

		C3DVec v = *test[nxt] - *test[i];

		// 中分点の比を求める
		float rate = dist[i] / (dist[i] - dist[nxt]);
		C3DVec pos = *test[i] + v.rate(rate);		// 通過点

		// 面の通過点が三角形内部にあればこの辺は三角形targetを通過している
		bool status = true;
		for (int j = 0; status && j < 3; j++) {
			int n = (j + 1) % 3;
			C3DVec v0 = *target[n] - *target[j];
			C3DVec v1 = pos - *target[n];
			C3DVec vc = v0 / v1;
			float dot = vc % norm;	// 得られた外積と法線の内積

			// 法線と外積が同じ側を向いていれば、少なくともその辺にとっては三角形の内部側に通過点がある。
			status = status && (dot >= 0.0f);
		}
		if (status) return true;	// 三角形内部を通過している辺を見つけたので、衝突していると報告
	}
	// 少なくとも test の辺は target 内部を通過していない。
	// ただし、関係を逆にした場合には通過しているかもしれない。
	return false;
}
/*
	ポリゴンメッシュ同士の判定は、利用目的が限られる。
	あくまで「当たったか否か」の情報以外は返さない。
*/
bool
CCol3DCollision::isHitMESHandMESH(CCol3DMESH * mesh_target, CCol3DMESH * mesh_offence)
{
	CCol3DMESH * A = mesh_target;
	CCol3DMESH * B = mesh_offence;

	// どちらかがそもそも3つ以上頂点を持たなければ判定しない。
	if (A->idx_num < 3 || B->idx_num < 3) return false;

	C3DVec * vec_a[3];
	C3DVec * vec_b[3];

	CCol3DBound bound_a;
	CCol3DBound bound_b;

	C3DVec norm_a;
	C3DVec norm_b;

	for (int a = 2; a < A->idx_num; ++a) {

		vec_a[0] = &(A->vertices[ A->indices[a - 2] ]);
		vec_a[1] = &(A->vertices[ A->indices[a - 1] ]);
		vec_a[2] = &(A->vertices[ A->indices[a] ]);

		getNormal(norm_a, vec_a);

		bound_a.init(*vec_a[0]);
		bound_a.update(*vec_a[1]);
		bound_a.update(*vec_a[2]);

		for (int b = 0; b < B->idx_num; ++b) {
			vec_b[0] = &(B->vertices[ B->indices[b - 2] ]);
			vec_b[1] = &(B->vertices[ B->indices[b - 1] ]);
			vec_b[2] = &(B->vertices[ B->indices[b] ]);

			getNormal(norm_b, vec_b);

			// 法線同士の外積が長さ0であれば平行なので判定不要
			C3DVec cross = norm_a / norm_b;
			if (cross.sqlen() <= DEFAULT_ZERO) continue;

			bound_b.init(*vec_b[0]);
			bound_b.update(*vec_b[1]);
			bound_b.update(*vec_b[1]);

			// BoundingBox が重なっていなければ空間的に断絶された位置にあるので判定不要
			if (!bound_b.isOverlap(bound_a)) continue;

			// 当たったものを見つけたら、即その場でステータスを返して終了
			if (isHitTriangle(vec_a, norm_a, vec_b)) return true;	// Bの辺の中にAの面を通過するものがあるか
			if (isHitTriangle(vec_b, norm_b, vec_a)) return true;	// Aの辺の中にBの面を通過するものがあるか
		}
	}
	// 衝突している三角形が一つも見つけられなければ衝突していないと判定する
	return false;
}

bool
CCol3DCollision::isHitMESHandBOX(CCol3DMESH * mesh, CCol3DBOX * box)
{
	CCol3DMESH * M = mesh;
	CCol3DBOX * B = box;

	// どちらかがそもそも3つ以上頂点を持たなければ判定しない。
	if (M->idx_num < 3) return false;

	C3DVec * vert_a[3];
	C3DVec side[3];
	C3DVec tbox[3];
	C3DVec norm_a;
	C3DVec vu;
	float r1, r2, L;

	for (int a = 2; a < M->idx_num; ++a) {

		vert_a[0] = &(M->vertices[M->indices[a - 2]]);
		vert_a[1] = &(M->vertices[M->indices[a - 1]]);
		vert_a[2] = &(M->vertices[M->indices[a]]);

		getNormal(norm_a, vert_a);

		// 各頂点からBOX中心へ向かうベクトルを求めておく。
		for (int i = 0; i < 3; i++) {
			tbox[i] = B->vec[0] - *vert_a[i];
		}

		// 対象の三角形の法線を分離軸として判定
		C3DVec& vab = tbox[2];
		vu = norm_a.unit();
		r1 = 0;	// 法線方向を分離軸とするため、三角形側の厚みは0
		r2 = getLengthOnAxis(&vu, B->vec);
		L = fabsf(vab % vu);	// 分離軸方向の投影距離
		if (L > r1 + r2) continue;	// この三角形には明らかに衝突していないため次の三角形の検査に移って良い。

		// 各辺を分離軸とした判定
		bool status = false;
		for (int i = 0; i < 3; i++) {
			int nxt = (i + 1) % 3;

			// 三角形の辺を分離軸として判定
			vu = *vert_a[nxt] - *vert_a[i];
			vu = vu.unit();

			r1 = getLengthOnAxis2Triangle(&vu, vert_a);
			r2 = getLengthOnAxis(&vu, B->vec);
			L = 0.0f;
			for (int j = 0; j < 3; j++) {
				float len = fabsf(tbox[j] % vu);
				L = (len > L) ? len : L;
			}
			status = (L > r1 + r2);
			if (status) break;

			// BOXの各方向ベクトルを軸として判定
			vu = B->vec[i + 1].unit();
			r1 = getLengthOnAxis2Triangle(&vu, vert_a);
			r2 = getLengthOnAxis(&vu, B->vec);
			L = 0.0f;
			for (int j = 0; j < 3; j++) {
				float len = fabsf(tbox[j] % vu);
				L = (len > L) ? len : L;
			}
			status = (L > r1 + r2);
			if (status) break;
		}
		// 3辺のいずれかを分離軸とした際に、分離状態であることが確認された
		if (status) continue;

		// この三角形の各辺と、BOXの各方向ベクトルの外積を分離軸として判定を行う。

		// 法線と3軸の外積
		for (int i = 1; i < 4; i++) {
			vu = norm_a / B->vec[i];
			if (vu.sqlen() < DEFAULT_ZERO) continue;	// 外積の長さが0の場合は向きが同じ
			vu = vu.unit();
			r1 = getLengthOnAxis2Triangle(&vu, vert_a);
			r2 = getLengthOnAxis(&vu, B->vec);
			L = 0.0f;
			for (int j = 0; j < 3; j++) {
				float len = fabsf(tbox[j] % vu);
				L = (len > L) ? len : L;
			}
			status = (L > r1 + r2);
			if (status) break;
		}
		if (status) continue;

		// 各辺と3軸の外積
		status = false;
		for (int i = 0; !status && i < 3; i++) {
			int nxt = (i + 1) % 3;

			// 三角形の辺を取得
			C3DVec side = *vert_a[nxt] - *vert_a[i];
			for (int j = 0; j < 3; j++) {
				vu = side / B->vec[j];
				if (vu.sqlen() < DEFAULT_ZERO) continue;	// 外積の長さが0の場合は向きが同じ
				vu = vu.unit();
				r1 = getLengthOnAxis2Triangle(&vu, vert_a);
				r2 = getLengthOnAxis(&vu, B->vec);
				L = 0.0f;
				for (int j = 0; j < 3; j++) {
					float len = fabsf(tbox[j] % vu);
					L = (len > L) ? len : L;
				}
				status = (L > r1 + r2);
				if (status) break;	// 分離している証拠を見つけた
			}
		}
		// ここまで、三角形とBOXが衝突していない証拠を一つも見つけられていなければ衝突していると見なす。
		if (!status) return true;
	}
	// 衝突している三角形が一つも見つけられなければ衝突していないと判定する
	return false;
}

bool
CCol3DCollision::isHitMESHandSPHERE(C3DVec& hit, C3DVec&norm, CCol3DMESH * mesh, CCol3DSPHERE * sphere)
{
	return isHitPOLYandSPHERE(hit, norm, mesh->vertices, mesh->indices, mesh->idx_num, sphere->vec, sphere->r);
}

bool
CCol3DCollision::isHitMESHandLINE(C3DVec& hit, C3DVec& norm, CCol3DMESH * mesh, CCol3DLINE * line)
{
	return isHitPOLYandLINE(hit, norm, mesh->vertices, mesh->indices, mesh->idx_num, line->vec);
}

bool
CCol3DCollision::isHitMESHandPOINT(C3DVec& hit, C3DVec& norm, CCol3DMESH * mesh, CCol3DPOINT * point, float thick)
{
	return isHitPOLYandSPHERE(hit, norm, mesh->vertices, mesh->indices, mesh->idx_num, point->vec, thick);
}

float
CCol3DCollision::getLengthOnAxis(C3DVec * axis, C3DVec * vec)
{
	float len = 0.0f;
	for (int i = 1; i < 4; i++) {
		len += fabsf(vec[i] % *axis);
	}
	return len;
}

float
CCol3DCollision::getLengthOnAxis2Triangle(C3DVec * axis, C3DVec ** vec)
{
	// 三角形の分離軸投影は、各辺をベクトルと見なし、軸との内積絶対値が最大のものを採用する。
	float len = 0.0f;
	for (int i = 1; i < 4; i++) {
		int nxt = (i + 1) % 3;
		C3DVec v = *vec[nxt] - *vec[i];
		float dot = fabsf(*axis % v);
		len = (dot > len) ? dot : len;
	}
	return len;
}

bool
CCol3DCollision::isHitBOXandBOX(CCol3DBOX * box1, CCol3DBOX * box2)
{
	CCol3DBOX * A = box1;
	CCol3DBOX * B = box2;


	C3DVec vab = B->vec[0] - A->vec[0];
	
	// A, Bの各軸を分離軸とした判定(6本分)
	float r1, r2, L;
	C3DVec vu;
	for (int i = 1; i < 4; i++) {
		// A側の軸による判定
		vu = A->vec[i].unit();
		r1 = A->len[i];
		r2 = getLengthOnAxis(&vu, B->vec);
		L = fabsf(vu % vab);
		if (L > r1 + r2) return false;

		// B側の軸による判定
		vu = B->vec[i].unit();
		r1 = B->len[i];
		r2 = getLengthOnAxis(&vu, A->vec);
		L = fabsf(vu % vab);
		if (L > r1 + r2) return false;
	}

	// A,Bの各軸間で垂直なベクトル(外積)を軸とした判定(9本分)
	for (int a = 1; a < 4; a++) {
		for (int b = 1; b < 4; b++) {
			C3DVec vc = A->vec[a] / B->vec[b];	// 外積を求める

			// 外積の長さが0である場合二つの方向ベクトルは同じ方向もしくは正反対を向いているため、
			// いずれかの方向ベクトルを分離軸とした判定で既に判定が終わっている。
			// また、単位ベクトル化する際にゼロ除算が発生するため、このケースでは処理をスキップする。
			if (vc.sqlen() < DEFAULT_ZERO) continue;

			vu = vc.unit();	// 単位ベクトル化したものを分離軸として判定する
			r1 = getLengthOnAxis(&vu, A->vec);
			r2 = getLengthOnAxis(&vu, B->vec);
			L = fabsf(vu % vab);
			if (L > r1 + r2) return false;
		}
	}

	// 15本の分離軸全てにおいて衝突していない証拠を一つも見つけられなかった。
	// この場合衝突とみなす。
	return true;
}

bool
CCol3DCollision::isHitBOXandSPHERE(CCol3DBOX * box, CCol3DSPHERE * sphere)
{
	float distance = distanceBOXandPOINT(box->vec, box->len, sphere->vec);
	return (distance < sphere->r);
}

bool
CCol3DCollision::isHitBOXandLINE(CCol3DBOX * box, CCol3DLINE * line)
{
	CCol3DBOX * A = box;
	C3DVec vl[2];

	// 線分はBOXの中心から遠い側を起点とする。
	for (int i = 0; i < 2; i++) {
		vl[i] = line->vec[i] - box->vec[0];
	}
	C3DVec lv = vl[1] - vl[0];

	// 線分はBOXの中心から遠い側を起点とする。

	float r1, r2, L, L1, L2;
	C3DVec vu, v1, v2;
	for (int i = 1; i < 4; i++) {
		// BOXの各軸を分離軸として判定
		vu = A->vec[i].unit();
		r1 = A->len[i];
		r2 = fabsf(lv % vu);	// 線分の分離軸上の投影長
		L1 = fabsf(vu % vl[0]);
		L2 = fabsf(vu % vl[1]);
		L = (L1 > L2) ? L1 : L2;
		if (L > r1 + r2) return false;	// 接触していない証拠を得た。

		// BOXの各軸と、線分ベクトルそのものの外積を分離軸として判定
		C3DVec cross = A->vec[i] / lv;
		vu = cross.unit();
		r1 = getLengthOnAxis(&vu, A->vec);
		r2 = fabsf(lv % vu);
		L1 = fabsf(vu % vl[0]);
		L2 = fabsf(vu % vl[1]);
		L = (L1 > L2) ? L1 : L2;
		if (L > r1 + r2) return false;	// 接触していない証拠を得た。
	}

	return true;
}

bool
CCol3DCollision::isHitBOXandPOINT(CCol3DBOX * box, CCol3DPOINT * point, float thick)
{
	float distance = distanceBOXandPOINT(box->vec, box->len, point->vec);
	return (distance < thick);
}

bool
CCol3DCollision::isHitSPHEREandSPHERE(CCol3DSPHERE * sphere1, CCol3DSPHERE * sphere2)
{
	return distanceHit(sphere1->vec, sphere2->vec, sphere1->r + sphere2->r);
}

bool
CCol3DCollision::isHitSPHEREandLINE(C3DVec& hit, C3DVec& norm, CCol3DSPHERE * sphere, CCol3DLINE * line)
{
	// 線分上で球面と接触する、始点に近い側の点を得る。この点こそが衝突点となる。
	// 注意:線分が球面と交差/接触している場合のみ衝突と判定される。
	// 線分の始点/終点共に完全に球体内部に含まれる場合は衝突と見なされない。
	bool result = getR_point(hit, line->vec[0], line->vec[1], sphere->vec, sphere->r);
	C3DVec n = hit - sphere->vec;	// 当たった点から中心点を引く
	norm = n.unit();	// 衝突点が存在する面のベクトルは、球体なので、中心から見たその座標のベクトルを正規化したもの。
	return result;
}

bool
CCol3DCollision::isHitSPHEREandPOINT(CCol3DSPHERE * sphere, CCol3DPOINT * point)
{
	// 点と球であれば衝突点は自明(点の側の座標)
	return distanceHit(sphere->vec, point->vec, sphere->r);
}

bool
CCol3DCollision::isHitLINEandLINE(CCol3DLINE * line1, CCol3DLINE * line2, float thick)
{
	C3DVec v1 = line1->vec[1] - line1->vec[0];
	C3DVec v2 = line2->vec[1] - line2->vec[0];
	C3DVec v3 = line2->vec[0] - line1->vec[0];
	C3DVec vc1 = v1 / v2;
	C3DVec vc2 = v1 / v3;
	C3DVec cross = vc1 / vc2;
	return (cross.sqlen() <= thick * thick);
}

bool
CCol3DCollision::isHitLINEandPOINT(CCol3DLINE * line, CCol3DPOINT * point, float thick)
{
	C3DVec v1 = line->vec[1] - line->vec[0];	// 始点から終点に向かうベクトル
	C3DVec v2 = point->vec - line->vec[0];		// 始点からみた点の位置
	C3DVec vu = v1.unit();
	C3DVec cross = vu / v2;
	float dot = vu % v2;
	
	// 外積の長さが thick 以下で、dot の値が 0以上|v1|以下であればヒット。
	return (dot >= 0.0f && dot * dot < v1.sqlen() && cross.sqlen() <= thick * thick);
}

bool
CCol3DCollision::isHitPOINTandPOINT(CCol3DPOINT * point1, CCol3DPOINT * point2, float thick)
{
	return distanceHit(point1->vec, point2->vec, thick);
}





bool
CCol3DCollision::distanceHit(C3DVec& v1, C3DVec& v2, float dist)
{
	C3DVec vec = v1 - v2;
	float sqdist = vec.sqlen();
	return (sqdist <= dist * dist);
}

bool
CCol3DCollision::isHitPOLYandLINE(C3DVec& hit, C3DVec& norm, C3DVec * vec, u16 * idx, int idxnum, C3DVec line[2])
{
	short idx0, idx1, idx2, n;
	C3DVec v[2];
	C3DVec vl = line[1] - line[0];	// 線分のベクトル

	// 3頂点以上なければ三角形にはならないため、そもそも衝突しない。
	if (idxnum < 3) return false;

	// 線分のBoundingBoxを作る
	CCol3DBound bound_line;
	CCol3DBound bound_poly;

	// 線分は不変なので先に作っておく。
	bound_line.init(line[0]);
	bound_line.update(line[1]);

	float min_rate = 1.0f;
	bool result = false;

	idx0 = idx[0];
	idx1 = idx[1];
	n = 0;
	v[n] = vec[idx1] - vec[idx0];

	for (int i = 2; i < idxnum; ++i, idx0 = idx1, idx1 = idx2, n = 1 - n) {
		idx2 = idx[i];

		// 三角形の2辺をベクトルとする。
		v[1-n] = vec[idx2] - vec[idx1];

		// 法線ベクトルを単位ベクトルとして求める
		C3DVec vc = v[n] / v[1 - n];		// 外積

		// 外積の長さが0であればどちらか一方の辺は長さが0であり、
		// この三角形?の面積は0なので判定の必要がない。
		if (vc.sqlen() < DEFAULT_ZERO) continue;

		// 長さが0ではないので外積を単位ベクトル化した上で判定を行う。
		C3DVec vn = vc.unit();


		// 線分がこの法線を持つ面と平行であれば、少なくともこの面にヒットすることはない。
		if (fabsf(vn % vl) < DEFAULT_ZERO) continue;	// 内積値が0とみなせる場合、法線との角度が直角なので、面に対し平行である。

		// 線分のBoundingBoxとポリゴンのBoundingBoxが接触していない場合、
		// そもそも断絶された位置にあるので判定を行う必要がない。
		bound_poly.init(vec[idx0]);
		bound_poly.update(vec[idx1]);
		bound_poly.update(vec[idx2]);
		if (!bound_poly.isOverlap(bound_line)) continue;


		// 線分の始点、終点それぞれと面との距離を出す。法線の単位ベクトルとの内積値が面との距離。
		C3DVec vs = line[0] - vec[i];
		C3DVec ve = line[1] - vec[i];
		float start = vs % vn;
		float end = ve % vn;

		// 両方の符号が同じであれば、線分の両端ともが面の一方にあるため、線分は面を貫通していない。
		// 面の法線側にあれば距離値の符号は正,法線の逆側であれば負となるため、片側に寄っていれば両者の積は正となる。
		// 両端が面を挟むように存在していれば両者の積が負となる。
		if (start * end > 0.0f) continue;

		// 線分の両端が面を挟むように存在するので、線分は面を貫通している。
		// 線分が面を通過している座標を得る。
		float rate = start / (start - end);	// 貫通点がどのぐらい線分の始点に近いかのレート

		// それ以前に求められていた値より大きければ無視する。
		// 線分の中で最も始点に近いものを求めるため。
		if (rate > min_rate) continue;

		C3DVec pos = line[0] + vl.rate(rate);	// 貫通点の座標

		// 少なくともこの時点で、対象のポリゴンが含まれる面を線分が貫通していることは確定している。
		// ただし、貫通点が三角形の中に入っていなければ、結局そのポリゴンには当たっていない。
		// 各辺のベクトルと辺の終点から貫通点へのベクトルの外積と、法線との内積が全て正である場合、
		// 貫通点は三角形の中にある。
		float status = 1.0f;
		for (int j = 0; j < 3; j++) {
			int b = i - 2 + j;
			int e = i - 2 + (j + 1) % 3;
			C3DVec v1 = vec[e] - vec[b];
			C3DVec v2 = pos - vec[e];
			C3DVec vpn = v1 / v2;	// 辺ベクトルと、辺ベクトル先端から衝突点へのベクトルの外積

			// 求めた外積と法線の内積が負であれば衝突点が辺から見て三角形の外側にある。
			status = vpn % vn;
			if (status < 0.0f) break;	// 明らかに外側であることが検出された時点で判定を中断
		}
		if (status < 0.0f) continue;

		// 衝突点がこれまで検出されたものよりも線分の始点寄りにあり、
		// なおかつ今回検査した三角形の中にある。よってこれを最新の最初期衝突位置候補として更新する。
		min_rate = rate;
		norm = vn;		// 今回衝突したポリゴンの法線を記録する。
		result = true;
	}

	// 得られた最初期衝突位置をhitに設定し、status を返す。
	hit = line[0] + vl.rate(min_rate);
	return result;
}

bool
CCol3DCollision::isHitPOLYandSPHERE(C3DVec& hit, C3DVec& norm, C3DVec * vec, u16 * idx, int idxnum, C3DVec pos, float r)
{
	short idx0, idx1, idx2, n;
	C3DVec v[2];

	// 3頂点以上なければ三角形にはならないため、そもそも衝突しない。
	if (idxnum < 3) return false;

	// 線分のBoundingBoxを作る
	CCol3DBound bound_sphere;
	CCol3DBound bound_poly;

	// 球は不変なので先に作っておく。
	// 球が内接するBoundingBox
	C3DVec half(r, r, r);
	C3DVec min = pos - half;
	C3DVec max = pos + half;
	bound_sphere.init(min);
	bound_sphere.update(max);

	float min_dist = r;
	bool result = false;

	idx0 = idx[0];
	idx1 = idx[1];
	n = 0;
	v[n] = vec[idx1] - vec[idx0];

	for (int i = 2; i < idxnum; ++i, idx0 = idx1, idx1 = idx2, n = 1 - n) {
		idx2 = idx[i];

		// 三角形の2辺をベクトルとする。
		v[1 - n] = vec[idx2] - vec[idx1];

		// 法線ベクトルを単位ベクトルとして求める
		C3DVec vc = v[n] / v[1 - n];		// 外積

		// 外積の長さが0であればどちらか一方の辺は長さが0であり、
		// この三角形?の面積は0なので判定の必要がない。
		if (vc.sqlen() < DEFAULT_ZERO) continue;

		// 長さが0ではないので外積を単位ベクトル化した上で判定を行う。
		C3DVec vn = vc.unit();

		// 球のBoundingBoxとポリゴンのBoundingBoxが接触していない場合、
		// そもそも断絶された位置にあるので判定を行う必要がない。
		bound_poly.init(vec[idx0]);
		bound_poly.update(vec[idx1]);
		bound_poly.update(vec[idx2]);
		if (!bound_poly.isOverlap(bound_sphere)) continue;

		// 球の中心から面への距離を出す。


		// 線分の始点、終点それぞれと面との距離を出す。法線の単位ベクトルとの内積値が面との距離。
		C3DVec vp = pos - vec[i];
		float dist = vp % vn;

		// これまで観測された点よりも遠ければ無視する。
		if (fabs(dist) > min_dist) continue;

		// この面に対して、球の中心から垂線を下した点を求める。
		// 中心点は面に対し法線方向に dist の距離にあるため、
		// 球の中心から法線の逆方向に dist の距離の点が垂線との交点となる。
		C3DVec foot = pos + vn.rate(-dist);

		// 垂線との交点がポリゴン内に入っているか否かを判定する。
		// 各辺のベクトルと辺の終点から貫通点へのベクトルの外積と、法線との内積が全て正である場合、
		// 垂線との交点は三角形の中にある。
		bool status = true;
		for (int j = 0; j < 3; j++) {
			int b = i - 2 + j;
			int e = i - 2 + (j + 1) % 3;
			C3DVec v1 = vec[e] - vec[b];
			C3DVec v2 = foot - vec[e];
			C3DVec vpn = v1 / v2;	// 辺ベクトルと、辺ベクトル先端から衝突点へのベクトルの外積

			// 求めた外積と法線の内積が負であれば衝突点が辺から見て三角形の外側にある。
			if (vpn % vn < 0.0f) {
				// 球との判定の場合、球は大きさを持つため中心からの垂線が三角形の外側にあっても
				// 辺との距離が球の半径以下であれば衝突と見なさねばならない。
				dist = distanceLine(foot, vec[b], vec[e], pos);		// 円中心から辺への最短距離

				// 辺までの距離が球の半径以下であり、
				// それがこれまで当たっているとされた面との距離より近ければ
				// 処理を継続する理由がある。min_dist の初期値は球の半径であるため、
				// これまで見つけたどの衝突点よりも近ければ間違いなく球の中に入っている。
				status = status && (dist < min_dist);

				// 球の中心点が三角形上になくとも辺からの距離が半径以下である場合、
				// 他の2辺は判定上「点は三角形の内側にある」となることが大半である。
				// ただし頂点付近にある場合はまれに2辺とも「三角形外にある」となるが、
				// その場合もっとも近い辺上の点はその2辺が共有する頂点となるため、
				// 最初にこの判定を行った段階で処理を打ち切って問題はない。

				// 角にあたったため、角から球の中心にかけてのベクトルを法線とする。
				vn = foot - pos;
				vn = vn.unit();
				break;
			}
		}
		if (!status) continue;

		// 衝突点がこれまで検出されたものよりも球の中心に近く、
		// なおかつ今回検査した三角形と球が接触している。
		// よってこれを最新の衝突位置候補として更新する。
		min_dist = fabs(dist);
		norm = vn;		// 今回衝突したポリゴンの法線を記録する。
		hit = foot;		// 中心からの垂線との交点を記録する。
		result = true;	// 少なくとも当たった点を見つけた	
	}

	return result;
}

float
CCol3DCollision::distanceBOXandPOINT(C3DVec * vec, float * len, C3DVec& pos)
{
	C3DVec v;
	C3DVec vcenter = pos - vec[0];
	for (int i = 1; i < 4; i++) {
		if (len[i] <= 0) continue;
		C3DVec vu = vec[i].unit();
		float s = fabsf((vcenter % vu) / len[i]);
		if (s > 1.0f) v = v + vu.rate((1.0f - s) * len[i]);		
	}
	return v.len();
}

// 指定された点から線分上で最も近い点への距離を返す。
float
CCol3DCollision::distanceLine(C3DVec& hit, C3DVec& begin, C3DVec& end, C3DVec& pos)
{
	C3DVec v1 = end - begin;	// 始点から終点に向かうベクトル
	C3DVec v2 = pos - begin;	// 始点からみた点の位置
	C3DVec vu = v1.unit();
	float dot = vu % v2;

	// 内積値が負である場合は始点の側に線分上からはみ出した位置にあるので、始点への距離を返す。
	if (dot < 0.0f) {
		// 最近点は線分の始点となる。
		hit = begin;
		return v2.len();
	}
	// 内積値が線分の長さを超えたら終点の側に線分上からはみ出した位置にあるので、終点への距離を返す。
	if (dot > v1.len()) {
		// 最近点は線分の終点となる。
		hit = end;
		C3DVec v3 = pos - end;		// 終点からみた点の位置
		return v3.len();
	}

	// 最近点は点から線分に下した垂線と線分との交点となる。
	hit = vu.rate(dot);	// 線分方向の単位ベクトルを内積値倍すると垂線との交点が得られる。

	// 指定された点から線分上に垂線が下せる場合は、その垂線の長さを距離とする。
	C3DVec cross = vu / v2;
	return cross.len();
}


/*
	ある点posから見て距離rにある線分begin-end上の点のうち、beginに近いものを求める。
	該当する点が線分上になければ false を返す。
*/
bool
CCol3DCollision::getR_point(C3DVec& hit, C3DVec& begin, C3DVec& end, C3DVec& pos, float r)
{
	C3DVec vl = end - begin;
	C3DVec vp = pos - begin;			// 線分の始点から見たposまでのベクトル
	float L = vl.len();					// 線分の長さ
	vl = vl.unit();						// 線分の始点から終点方向の単位ベクトル
	C3DVec cross = vl / vp;				// 線分方向と点までの外積
	float dot = vl % vp;				// 線分方向と点までの内積
	float hsq = cross.sqlen();			// 外積の長さ二乗は点から線分を含む直線までの距離
	if (hsq < r*r) return false;		// 距離がrより遠ければ、線分上に該当する点は存在しない。

	float w = sqrtf(r * r - hsq);		// 点posから直線状に垂線を下した位置から前後 w の位置に、距離r の点がある。

	float dp1 = dot - w;				// 始点から一つ目の点までの距離
	float dp2 = dot + w;				// 始点から二つ目の点までの距離

	if (dp1 >= 0.0f && dp1 <= L) {		// 一つ目の点が線分の範囲にあれば、この点が回答となる。
		hit = begin + vl.rate(dp1);
		return true;
	}
	if (dp2 >= 0.0f && dp2 <= L) {		// 一つ目が外れでも二つ目が線分の範囲に入っていればこちらを回答とする。
		hit = begin + vl.rate(dp2);
		return true;
	}
	return false;						// どちらも線分上になければ該当する点は存在しない。
}
