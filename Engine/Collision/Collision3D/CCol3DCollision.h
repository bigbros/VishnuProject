#ifndef CCol3DCollision_h
#define CCol3DCollision_h

#include"CCol3DTypes.h"

#define DEFAULT_THICK 1e-8f
#define DEFAULT_ZERO 1e-8f

class CCol3DCollision
{
public:
	// �V���v����3D�}�`���m�̔�����s�����[�e�B���e�B���\�b�h

	// �|���S�����b�V��vs�����ꂩ(MESH�n�̓I�[�_�[���傫�����ߒ���)
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

	// ����vs�����ꂩ
	static bool isHitSPHEREandSPHERE(CCol3DSPHERE * sphere1, CCol3DSPHERE * sphere2);
	static bool isHitSPHEREandLINE(C3DVec& hit, C3DVec& norm, CCol3DSPHERE * sphere, CCol3DLINE * line);
	static bool isHitSPHEREandPOINT(CCol3DSPHERE * sphere, CCol3DPOINT * point);

	// ����vs�����ꂩ
	// ����vs���̂͊��ɂ���
	static bool isHitLINEandLINE(CCol3DLINE * line1, CCol3DLINE * line2, float thick = DEFAULT_THICK);
	static bool isHitLINEandPOINT(CCol3DLINE * line, CCol3DPOINT * point, float thick = DEFAULT_THICK);

	// �_vs�_
	// ���Ƃ̑g�ݍ��킹�͊��ɂ���
	static bool isHitPOINTandPOINT(CCol3DPOINT * point1, CCol3DPOINT * point2, float thick = DEFAULT_THICK);

	// ���� thick �͓_������́u�����v�Ƃ��ċ@�\����B�����ɐ��l�I�ȏd�Ȃ肪�����Ƃ��A
	// ���ҊԂ̋��������̒l�ȉ��ɂȂ����Ƃ��ɏՓ˂Ɣ��f����B
	// �����̂⋅�̂ȂǁA���Ȃ��Ƃ��ǂ��炩������傫�����������}�`�ł���Εs�v�����A
	// ������_�͑傫���������Ȃ����߁A���̒l�������ƏՓ˂��邱�Ƃ��ɂ߂ċH�ɂȂ�B

private:
	static bool distanceHit(C3DVec& v1, C3DVec& v2, float dist);

	// ���������TriangleStrip�ŕ\���ꂽ�|���S�����b�V���̒��_�o�b�t�@�A�C���f�b�N�X�o�b�t�@��^���A�����Ƃ̊ԂŔ�����s���B
	// �߂�l�� true �ł���ꍇ�����̓��b�V���̂����ꂩ�̃|���S�����ђʂ��Ă���A�ђʂ��Ă���|���S���̂���
	// �ł������̎n�_�ɋ߂��ꏊ�̏Փ˓_��hit�ɁA�Փ˂����|���S���̖@���x�N�g����norm�ɕԂ����B
	// �߂�l�� false �̏ꍇ�A�����͂�����̃|���S�����ђʂ��Ă��Ȃ��B
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

