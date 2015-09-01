#include <iostream>
#include "CCol3DCollision.h"

// �O�p�`��2�ӂ��x�N�g���Ƃ݂Ȃ��A���̊O�ς����߂邱�Ƃ�
// �O�p�`�̖@�������߂�B
// �f�[�^�Ƃ��Ė@�����������_���l�Ƀ}�g���N�X�ŉ�]����������A
// ���̏�ŊO�ς�p���ċ��߂��ق����v�Z�ʂ����Ȃ��B
void
CCol3DCollision::getNormal(C3DVec& norm, C3DVec *vert[3])
{
	C3DVec v0 = *vert[1] - *vert[0];
	C3DVec v1 = *vert[2] - *vert[1];
	C3DVec vc = v0 / v1;
	norm = vc.unit();
}

// ����O�p�`���A�ʂ̎O�p�`�̕ӂ���ȏ�ʉ߂��Ă��邩�𔻒肷��B
bool
CCol3DCollision::isHitTriangle(C3DVec * target[3], C3DVec& norm, C3DVec * test[3])
{
	float dist[3];
	// test���e���_�ɂ��āAtarget�̖ʂ���̋��������߂�
	for (int i = 0; i < 3; i++) {
		C3DVec v = *test[i] - *target[0];
		dist[i] = v % norm;	// �@����\���P�ʃx�N�g���Ƃ̓��ς��A���̂܂ܖʂ���̋���
	}

	// test �̊e�ӂ� target ������ʉ߂��Ă��邩���ׂ�
	for (int i = 0; i < 3; i++) {
		int nxt = (i + 1) % 3;
		// �o���̕����������ł���΂ǂ�����ʂ̓������ɂ���̂ŁA�ӂ͐�΂ɖʂ�ʉ߂��Ă��Ȃ��B
		if ((dist[i] * dist[nxt]) > 0.0f) continue;

		C3DVec v = *test[nxt] - *test[i];

		// �����_�̔�����߂�
		float rate = dist[i] / (dist[i] - dist[nxt]);
		C3DVec pos = *test[i] + v.rate(rate);		// �ʉߓ_

		// �ʂ̒ʉߓ_���O�p�`�����ɂ���΂��̕ӂ͎O�p�`target��ʉ߂��Ă���
		bool status = true;
		for (int j = 0; status && j < 3; j++) {
			int n = (j + 1) % 3;
			C3DVec v0 = *target[n] - *target[j];
			C3DVec v1 = pos - *target[n];
			C3DVec vc = v0 / v1;
			float dot = vc % norm;	// ����ꂽ�O�ςƖ@���̓���

			// �@���ƊO�ς��������������Ă���΁A���Ȃ��Ƃ����̕ӂɂƂ��Ă͎O�p�`�̓������ɒʉߓ_������B
			status = status && (dot >= 0.0f);
		}
		if (status) return true;	// �O�p�`������ʉ߂��Ă���ӂ��������̂ŁA�Փ˂��Ă���ƕ�
	}
	// ���Ȃ��Ƃ� test �̕ӂ� target ������ʉ߂��Ă��Ȃ��B
	// �������A�֌W���t�ɂ����ꍇ�ɂ͒ʉ߂��Ă��邩������Ȃ��B
	return false;
}
/*
	�|���S�����b�V�����m�̔���́A���p�ړI��������B
	�����܂Łu�����������ۂ��v�̏��ȊO�͕Ԃ��Ȃ��B
*/
bool
CCol3DCollision::isHitMESHandMESH(CCol3DMESH * mesh_target, CCol3DMESH * mesh_offence)
{
	CCol3DMESH * A = mesh_target;
	CCol3DMESH * B = mesh_offence;

	// �ǂ��炩����������3�ȏ㒸�_�������Ȃ���Δ��肵�Ȃ��B
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

			// �@�����m�̊O�ς�����0�ł���Ε��s�Ȃ̂Ŕ���s�v
			C3DVec cross = norm_a / norm_b;
			if (cross.sqlen() <= DEFAULT_ZERO) continue;

			bound_b.init(*vec_b[0]);
			bound_b.update(*vec_b[1]);
			bound_b.update(*vec_b[1]);

			// BoundingBox ���d�Ȃ��Ă��Ȃ���΋�ԓI�ɒf�₳�ꂽ�ʒu�ɂ���̂Ŕ���s�v
			if (!bound_b.isOverlap(bound_a)) continue;

			// �����������̂���������A�����̏�ŃX�e�[�^�X��Ԃ��ďI��
			if (isHitTriangle(vec_a, norm_a, vec_b)) return true;	// B�̕ӂ̒���A�̖ʂ�ʉ߂�����̂����邩
			if (isHitTriangle(vec_b, norm_b, vec_a)) return true;	// A�̕ӂ̒���B�̖ʂ�ʉ߂�����̂����邩
		}
	}
	// �Փ˂��Ă���O�p�`������������Ȃ���ΏՓ˂��Ă��Ȃ��Ɣ��肷��
	return false;
}

bool
CCol3DCollision::isHitMESHandBOX(CCol3DMESH * mesh, CCol3DBOX * box)
{
	CCol3DMESH * M = mesh;
	CCol3DBOX * B = box;

	// �ǂ��炩����������3�ȏ㒸�_�������Ȃ���Δ��肵�Ȃ��B
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

		// �e���_����BOX���S�֌������x�N�g�������߂Ă����B
		for (int i = 0; i < 3; i++) {
			tbox[i] = B->vec[0] - *vert_a[i];
		}

		// �Ώۂ̎O�p�`�̖@���𕪗����Ƃ��Ĕ���
		C3DVec& vab = tbox[2];
		vu = norm_a.unit();
		r1 = 0;	// �@�������𕪗����Ƃ��邽�߁A�O�p�`���̌��݂�0
		r2 = getLengthOnAxis(&vu, B->vec);
		L = fabsf(vab % vu);	// �����������̓��e����
		if (L > r1 + r2) continue;	// ���̎O�p�`�ɂ͖��炩�ɏՓ˂��Ă��Ȃ����ߎ��̎O�p�`�̌����Ɉڂ��ėǂ��B

		// �e�ӂ𕪗����Ƃ�������
		bool status = false;
		for (int i = 0; i < 3; i++) {
			int nxt = (i + 1) % 3;

			// �O�p�`�̕ӂ𕪗����Ƃ��Ĕ���
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

			// BOX�̊e�����x�N�g�������Ƃ��Ĕ���
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
		// 3�ӂ̂����ꂩ�𕪗����Ƃ����ۂɁA������Ԃł��邱�Ƃ��m�F���ꂽ
		if (status) continue;

		// ���̎O�p�`�̊e�ӂƁABOX�̊e�����x�N�g���̊O�ς𕪗����Ƃ��Ĕ�����s���B

		// �@����3���̊O��
		for (int i = 1; i < 4; i++) {
			vu = norm_a / B->vec[i];
			if (vu.sqlen() < DEFAULT_ZERO) continue;	// �O�ς̒�����0�̏ꍇ�͌���������
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

		// �e�ӂ�3���̊O��
		status = false;
		for (int i = 0; !status && i < 3; i++) {
			int nxt = (i + 1) % 3;

			// �O�p�`�̕ӂ��擾
			C3DVec side = *vert_a[nxt] - *vert_a[i];
			for (int j = 0; j < 3; j++) {
				vu = side / B->vec[j];
				if (vu.sqlen() < DEFAULT_ZERO) continue;	// �O�ς̒�����0�̏ꍇ�͌���������
				vu = vu.unit();
				r1 = getLengthOnAxis2Triangle(&vu, vert_a);
				r2 = getLengthOnAxis(&vu, B->vec);
				L = 0.0f;
				for (int j = 0; j < 3; j++) {
					float len = fabsf(tbox[j] % vu);
					L = (len > L) ? len : L;
				}
				status = (L > r1 + r2);
				if (status) break;	// �������Ă���؋���������
			}
		}
		// �����܂ŁA�O�p�`��BOX���Փ˂��Ă��Ȃ��؋�������������Ă��Ȃ���ΏՓ˂��Ă���ƌ��Ȃ��B
		if (!status) return true;
	}
	// �Փ˂��Ă���O�p�`������������Ȃ���ΏՓ˂��Ă��Ȃ��Ɣ��肷��
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
	// �O�p�`�̕��������e�́A�e�ӂ��x�N�g���ƌ��Ȃ��A���Ƃ̓��ϐ�Βl���ő�̂��̂��̗p����B
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
	
	// A, B�̊e���𕪗����Ƃ�������(6�{��)
	float r1, r2, L;
	C3DVec vu;
	for (int i = 1; i < 4; i++) {
		// A���̎��ɂ�锻��
		vu = A->vec[i].unit();
		r1 = A->len[i];
		r2 = getLengthOnAxis(&vu, B->vec);
		L = fabsf(vu % vab);
		if (L > r1 + r2) return false;

		// B���̎��ɂ�锻��
		vu = B->vec[i].unit();
		r1 = B->len[i];
		r2 = getLengthOnAxis(&vu, A->vec);
		L = fabsf(vu % vab);
		if (L > r1 + r2) return false;
	}

	// A,B�̊e���ԂŐ����ȃx�N�g��(�O��)�����Ƃ�������(9�{��)
	for (int a = 1; a < 4; a++) {
		for (int b = 1; b < 4; b++) {
			C3DVec vc = A->vec[a] / B->vec[b];	// �O�ς����߂�

			// �O�ς̒�����0�ł���ꍇ��̕����x�N�g���͓��������������͐����΂������Ă��邽�߁A
			// �����ꂩ�̕����x�N�g���𕪗����Ƃ�������Ŋ��ɔ��肪�I����Ă���B
			// �܂��A�P�ʃx�N�g��������ۂɃ[�����Z���������邽�߁A���̃P�[�X�ł͏������X�L�b�v����B
			if (vc.sqlen() < DEFAULT_ZERO) continue;

			vu = vc.unit();	// �P�ʃx�N�g�����������̂𕪗����Ƃ��Ĕ��肷��
			r1 = getLengthOnAxis(&vu, A->vec);
			r2 = getLengthOnAxis(&vu, B->vec);
			L = fabsf(vu % vab);
			if (L > r1 + r2) return false;
		}
	}

	// 15�{�̕������S�Ăɂ����ďՓ˂��Ă��Ȃ��؋�������������Ȃ������B
	// ���̏ꍇ�Փ˂Ƃ݂Ȃ��B
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

	// ������BOX�̒��S���牓�������N�_�Ƃ���B
	for (int i = 0; i < 2; i++) {
		vl[i] = line->vec[i] - box->vec[0];
	}
	C3DVec lv = vl[1] - vl[0];

	// ������BOX�̒��S���牓�������N�_�Ƃ���B

	float r1, r2, L, L1, L2;
	C3DVec vu, v1, v2;
	for (int i = 1; i < 4; i++) {
		// BOX�̊e���𕪗����Ƃ��Ĕ���
		vu = A->vec[i].unit();
		r1 = A->len[i];
		r2 = fabsf(lv % vu);	// �����̕�������̓��e��
		L1 = fabsf(vu % vl[0]);
		L2 = fabsf(vu % vl[1]);
		L = (L1 > L2) ? L1 : L2;
		if (L > r1 + r2) return false;	// �ڐG���Ă��Ȃ��؋��𓾂��B

		// BOX�̊e���ƁA�����x�N�g�����̂��̂̊O�ς𕪗����Ƃ��Ĕ���
		C3DVec cross = A->vec[i] / lv;
		vu = cross.unit();
		r1 = getLengthOnAxis(&vu, A->vec);
		r2 = fabsf(lv % vu);
		L1 = fabsf(vu % vl[0]);
		L2 = fabsf(vu % vl[1]);
		L = (L1 > L2) ? L1 : L2;
		if (L > r1 + r2) return false;	// �ڐG���Ă��Ȃ��؋��𓾂��B
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
	// ������ŋ��ʂƐڐG����A�n�_�ɋ߂����̓_�𓾂�B���̓_�������Փ˓_�ƂȂ�B
	// ����:���������ʂƌ���/�ڐG���Ă���ꍇ�̂ݏՓ˂Ɣ��肳���B
	// �����̎n�_/�I�_���Ɋ��S�ɋ��̓����Ɋ܂܂��ꍇ�͏Փ˂ƌ��Ȃ���Ȃ��B
	bool result = getR_point(hit, line->vec[0], line->vec[1], sphere->vec, sphere->r);
	C3DVec n = hit - sphere->vec;	// ���������_���璆�S�_������
	norm = n.unit();	// �Փ˓_�����݂���ʂ̃x�N�g���́A���̂Ȃ̂ŁA���S���猩�����̍��W�̃x�N�g���𐳋K���������́B
	return result;
}

bool
CCol3DCollision::isHitSPHEREandPOINT(CCol3DSPHERE * sphere, CCol3DPOINT * point)
{
	// �_�Ƌ��ł���ΏՓ˓_�͎���(�_�̑��̍��W)
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
	C3DVec v1 = line->vec[1] - line->vec[0];	// �n�_����I�_�Ɍ������x�N�g��
	C3DVec v2 = point->vec - line->vec[0];		// �n�_����݂��_�̈ʒu
	C3DVec vu = v1.unit();
	C3DVec cross = vu / v2;
	float dot = vu % v2;
	
	// �O�ς̒����� thick �ȉ��ŁAdot �̒l�� 0�ȏ�|v1|�ȉ��ł���΃q�b�g�B
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
	C3DVec vl = line[1] - line[0];	// �����̃x�N�g��

	// 3���_�ȏ�Ȃ���ΎO�p�`�ɂ͂Ȃ�Ȃ����߁A���������Փ˂��Ȃ��B
	if (idxnum < 3) return false;

	// ������BoundingBox�����
	CCol3DBound bound_line;
	CCol3DBound bound_poly;

	// �����͕s�ςȂ̂Ő�ɍ���Ă����B
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

		// �O�p�`��2�ӂ��x�N�g���Ƃ���B
		v[1-n] = vec[idx2] - vec[idx1];

		// �@���x�N�g����P�ʃx�N�g���Ƃ��ċ��߂�
		C3DVec vc = v[n] / v[1 - n];		// �O��

		// �O�ς̒�����0�ł���΂ǂ��炩����̕ӂ͒�����0�ł���A
		// ���̎O�p�`?�̖ʐς�0�Ȃ̂Ŕ���̕K�v���Ȃ��B
		if (vc.sqlen() < DEFAULT_ZERO) continue;

		// ������0�ł͂Ȃ��̂ŊO�ς�P�ʃx�N�g����������Ŕ�����s���B
		C3DVec vn = vc.unit();


		// ���������̖@�������ʂƕ��s�ł���΁A���Ȃ��Ƃ����̖ʂɃq�b�g���邱�Ƃ͂Ȃ��B
		if (fabsf(vn % vl) < DEFAULT_ZERO) continue;	// ���ϒl��0�Ƃ݂Ȃ���ꍇ�A�@���Ƃ̊p�x�����p�Ȃ̂ŁA�ʂɑ΂����s�ł���B

		// ������BoundingBox�ƃ|���S����BoundingBox���ڐG���Ă��Ȃ��ꍇ�A
		// ���������f�₳�ꂽ�ʒu�ɂ���̂Ŕ�����s���K�v���Ȃ��B
		bound_poly.init(vec[idx0]);
		bound_poly.update(vec[idx1]);
		bound_poly.update(vec[idx2]);
		if (!bound_poly.isOverlap(bound_line)) continue;


		// �����̎n�_�A�I�_���ꂼ��ƖʂƂ̋������o���B�@���̒P�ʃx�N�g���Ƃ̓��ϒl���ʂƂ̋����B
		C3DVec vs = line[0] - vec[i];
		C3DVec ve = line[1] - vec[i];
		float start = vs % vn;
		float end = ve % vn;

		// �����̕����������ł���΁A�����̗��[�Ƃ����ʂ̈���ɂ��邽�߁A�����͖ʂ��ђʂ��Ă��Ȃ��B
		// �ʂ̖@�����ɂ���΋����l�̕����͐�,�@���̋t���ł���Ε��ƂȂ邽�߁A�Б��Ɋ���Ă���Η��҂̐ς͐��ƂȂ�B
		// ���[���ʂ����ނ悤�ɑ��݂��Ă���Η��҂̐ς����ƂȂ�B
		if (start * end > 0.0f) continue;

		// �����̗��[���ʂ����ނ悤�ɑ��݂���̂ŁA�����͖ʂ��ђʂ��Ă���B
		// �������ʂ�ʉ߂��Ă�����W�𓾂�B
		float rate = start / (start - end);	// �ђʓ_���ǂ̂��炢�����̎n�_�ɋ߂����̃��[�g

		// ����ȑO�ɋ��߂��Ă����l���傫����Ζ�������B
		// �����̒��ōł��n�_�ɋ߂����̂����߂邽�߁B
		if (rate > min_rate) continue;

		C3DVec pos = line[0] + vl.rate(rate);	// �ђʓ_�̍��W

		// ���Ȃ��Ƃ����̎��_�ŁA�Ώۂ̃|���S�����܂܂��ʂ�������ђʂ��Ă��邱�Ƃ͊m�肵�Ă���B
		// �������A�ђʓ_���O�p�`�̒��ɓ����Ă��Ȃ���΁A���ǂ��̃|���S���ɂ͓������Ă��Ȃ��B
		// �e�ӂ̃x�N�g���ƕӂ̏I�_����ђʓ_�ւ̃x�N�g���̊O�ςƁA�@���Ƃ̓��ς��S�Đ��ł���ꍇ�A
		// �ђʓ_�͎O�p�`�̒��ɂ���B
		float status = 1.0f;
		for (int j = 0; j < 3; j++) {
			int b = i - 2 + j;
			int e = i - 2 + (j + 1) % 3;
			C3DVec v1 = vec[e] - vec[b];
			C3DVec v2 = pos - vec[e];
			C3DVec vpn = v1 / v2;	// �Ӄx�N�g���ƁA�Ӄx�N�g����[����Փ˓_�ւ̃x�N�g���̊O��

			// ���߂��O�ςƖ@���̓��ς����ł���ΏՓ˓_���ӂ��猩�ĎO�p�`�̊O���ɂ���B
			status = vpn % vn;
			if (status < 0.0f) break;	// ���炩�ɊO���ł��邱�Ƃ����o���ꂽ���_�Ŕ���𒆒f
		}
		if (status < 0.0f) continue;

		// �Փ˓_������܂Ō��o���ꂽ���̂��������̎n�_���ɂ���A
		// �Ȃ������񌟍������O�p�`�̒��ɂ���B����Ă�����ŐV�̍ŏ����Փˈʒu���Ƃ��čX�V����B
		min_rate = rate;
		norm = vn;		// ����Փ˂����|���S���̖@�����L�^����B
		result = true;
	}

	// ����ꂽ�ŏ����Փˈʒu��hit�ɐݒ肵�Astatus ��Ԃ��B
	hit = line[0] + vl.rate(min_rate);
	return result;
}

bool
CCol3DCollision::isHitPOLYandSPHERE(C3DVec& hit, C3DVec& norm, C3DVec * vec, u16 * idx, int idxnum, C3DVec pos, float r)
{
	short idx0, idx1, idx2, n;
	C3DVec v[2];

	// 3���_�ȏ�Ȃ���ΎO�p�`�ɂ͂Ȃ�Ȃ����߁A���������Փ˂��Ȃ��B
	if (idxnum < 3) return false;

	// ������BoundingBox�����
	CCol3DBound bound_sphere;
	CCol3DBound bound_poly;

	// ���͕s�ςȂ̂Ő�ɍ���Ă����B
	// �������ڂ���BoundingBox
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

		// �O�p�`��2�ӂ��x�N�g���Ƃ���B
		v[1 - n] = vec[idx2] - vec[idx1];

		// �@���x�N�g����P�ʃx�N�g���Ƃ��ċ��߂�
		C3DVec vc = v[n] / v[1 - n];		// �O��

		// �O�ς̒�����0�ł���΂ǂ��炩����̕ӂ͒�����0�ł���A
		// ���̎O�p�`?�̖ʐς�0�Ȃ̂Ŕ���̕K�v���Ȃ��B
		if (vc.sqlen() < DEFAULT_ZERO) continue;

		// ������0�ł͂Ȃ��̂ŊO�ς�P�ʃx�N�g����������Ŕ�����s���B
		C3DVec vn = vc.unit();

		// ����BoundingBox�ƃ|���S����BoundingBox���ڐG���Ă��Ȃ��ꍇ�A
		// ���������f�₳�ꂽ�ʒu�ɂ���̂Ŕ�����s���K�v���Ȃ��B
		bound_poly.init(vec[idx0]);
		bound_poly.update(vec[idx1]);
		bound_poly.update(vec[idx2]);
		if (!bound_poly.isOverlap(bound_sphere)) continue;

		// ���̒��S����ʂւ̋������o���B


		// �����̎n�_�A�I�_���ꂼ��ƖʂƂ̋������o���B�@���̒P�ʃx�N�g���Ƃ̓��ϒl���ʂƂ̋����B
		C3DVec vp = pos - vec[i];
		float dist = vp % vn;

		// ����܂Ŋϑ����ꂽ�_����������Ζ�������B
		if (fabs(dist) > min_dist) continue;

		// ���̖ʂɑ΂��āA���̒��S���琂�����������_�����߂�B
		// ���S�_�͖ʂɑ΂��@�������� dist �̋����ɂ��邽�߁A
		// ���̒��S����@���̋t������ dist �̋����̓_�������Ƃ̌�_�ƂȂ�B
		C3DVec foot = pos + vn.rate(-dist);

		// �����Ƃ̌�_���|���S�����ɓ����Ă��邩�ۂ��𔻒肷��B
		// �e�ӂ̃x�N�g���ƕӂ̏I�_����ђʓ_�ւ̃x�N�g���̊O�ςƁA�@���Ƃ̓��ς��S�Đ��ł���ꍇ�A
		// �����Ƃ̌�_�͎O�p�`�̒��ɂ���B
		bool status = true;
		for (int j = 0; j < 3; j++) {
			int b = i - 2 + j;
			int e = i - 2 + (j + 1) % 3;
			C3DVec v1 = vec[e] - vec[b];
			C3DVec v2 = foot - vec[e];
			C3DVec vpn = v1 / v2;	// �Ӄx�N�g���ƁA�Ӄx�N�g����[����Փ˓_�ւ̃x�N�g���̊O��

			// ���߂��O�ςƖ@���̓��ς����ł���ΏՓ˓_���ӂ��猩�ĎO�p�`�̊O���ɂ���B
			if (vpn % vn < 0.0f) {
				// ���Ƃ̔���̏ꍇ�A���͑傫���������ߒ��S����̐������O�p�`�̊O���ɂ����Ă�
				// �ӂƂ̋��������̔��a�ȉ��ł���ΏՓ˂ƌ��Ȃ��˂΂Ȃ�Ȃ��B
				dist = distanceLine(foot, vec[b], vec[e], pos);		// �~���S����ӂւ̍ŒZ����

				// �ӂ܂ł̋��������̔��a�ȉ��ł���A
				// ���ꂪ����܂œ������Ă���Ƃ��ꂽ�ʂƂ̋������߂����
				// �������p�����闝�R������Bmin_dist �̏����l�͋��̔��a�ł��邽�߁A
				// ����܂Ō������ǂ̏Փ˓_�����߂���ΊԈႢ�Ȃ����̒��ɓ����Ă���B
				status = status && (dist < min_dist);

				// ���̒��S�_���O�p�`��ɂȂ��Ƃ��ӂ���̋��������a�ȉ��ł���ꍇ�A
				// ����2�ӂ͔����u�_�͎O�p�`�̓����ɂ���v�ƂȂ邱�Ƃ��唼�ł���B
				// ���������_�t�߂ɂ���ꍇ�͂܂��2�ӂƂ��u�O�p�`�O�ɂ���v�ƂȂ邪�A
				// ���̏ꍇ�����Ƃ��߂��ӏ�̓_�͂���2�ӂ����L���钸�_�ƂȂ邽�߁A
				// �ŏ��ɂ��̔�����s�����i�K�ŏ�����ł��؂��Ė��͂Ȃ��B

				// �p�ɂ����������߁A�p���狅�̒��S�ɂ����Ẵx�N�g����@���Ƃ���B
				vn = foot - pos;
				vn = vn.unit();
				break;
			}
		}
		if (!status) continue;

		// �Փ˓_������܂Ō��o���ꂽ���̂������̒��S�ɋ߂��A
		// �Ȃ������񌟍������O�p�`�Ƌ����ڐG���Ă���B
		// ����Ă�����ŐV�̏Փˈʒu���Ƃ��čX�V����B
		min_dist = fabs(dist);
		norm = vn;		// ����Փ˂����|���S���̖@�����L�^����B
		hit = foot;		// ���S����̐����Ƃ̌�_���L�^����B
		result = true;	// ���Ȃ��Ƃ����������_��������	
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

// �w�肳�ꂽ�_���������ōł��߂��_�ւ̋�����Ԃ��B
float
CCol3DCollision::distanceLine(C3DVec& hit, C3DVec& begin, C3DVec& end, C3DVec& pos)
{
	C3DVec v1 = end - begin;	// �n�_����I�_�Ɍ������x�N�g��
	C3DVec v2 = pos - begin;	// �n�_����݂��_�̈ʒu
	C3DVec vu = v1.unit();
	float dot = vu % v2;

	// ���ϒl�����ł���ꍇ�͎n�_�̑��ɐ����ォ��͂ݏo�����ʒu�ɂ���̂ŁA�n�_�ւ̋�����Ԃ��B
	if (dot < 0.0f) {
		// �ŋߓ_�͐����̎n�_�ƂȂ�B
		hit = begin;
		return v2.len();
	}
	// ���ϒl�������̒����𒴂�����I�_�̑��ɐ����ォ��͂ݏo�����ʒu�ɂ���̂ŁA�I�_�ւ̋�����Ԃ��B
	if (dot > v1.len()) {
		// �ŋߓ_�͐����̏I�_�ƂȂ�B
		hit = end;
		C3DVec v3 = pos - end;		// �I�_����݂��_�̈ʒu
		return v3.len();
	}

	// �ŋߓ_�͓_��������ɉ����������Ɛ����Ƃ̌�_�ƂȂ�B
	hit = vu.rate(dot);	// ���������̒P�ʃx�N�g������ϒl�{����Ɛ����Ƃ̌�_��������B

	// �w�肳�ꂽ�_���������ɐ�����������ꍇ�́A���̐����̒����������Ƃ���B
	C3DVec cross = vu / v2;
	return cross.len();
}


/*
	����_pos���猩�ċ���r�ɂ������begin-end��̓_�̂����Abegin�ɋ߂����̂����߂�B
	�Y������_��������ɂȂ���� false ��Ԃ��B
*/
bool
CCol3DCollision::getR_point(C3DVec& hit, C3DVec& begin, C3DVec& end, C3DVec& pos, float r)
{
	C3DVec vl = end - begin;
	C3DVec vp = pos - begin;			// �����̎n�_���猩��pos�܂ł̃x�N�g��
	float L = vl.len();					// �����̒���
	vl = vl.unit();						// �����̎n�_����I�_�����̒P�ʃx�N�g��
	C3DVec cross = vl / vp;				// ���������Ɠ_�܂ł̊O��
	float dot = vl % vp;				// ���������Ɠ_�܂ł̓���
	float hsq = cross.sqlen();			// �O�ς̒������͓_����������܂ޒ����܂ł̋���
	if (hsq < r*r) return false;		// ������r��艓����΁A������ɊY������_�͑��݂��Ȃ��B

	float w = sqrtf(r * r - hsq);		// �_pos���璼����ɐ������������ʒu����O�� w �̈ʒu�ɁA����r �̓_������B

	float dp1 = dot - w;				// �n�_�����ڂ̓_�܂ł̋���
	float dp2 = dot + w;				// �n�_�����ڂ̓_�܂ł̋���

	if (dp1 >= 0.0f && dp1 <= L) {		// ��ڂ̓_�������͈̔͂ɂ���΁A���̓_���񓚂ƂȂ�B
		hit = begin + vl.rate(dp1);
		return true;
	}
	if (dp2 >= 0.0f && dp2 <= L) {		// ��ڂ��O��ł���ڂ������͈̔͂ɓ����Ă���΂�������񓚂Ƃ���B
		hit = begin + vl.rate(dp2);
		return true;
	}
	return false;						// �ǂ����������ɂȂ���ΊY������_�͑��݂��Ȃ��B
}
