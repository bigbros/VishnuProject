#ifndef CCol3DTypes_h
#define CCol3DTypes_h

#include"CCol3DGROUP.h"

// �|���S�����b�V��
class CCol3DMESH : public CCol3D
{
public:
	CCol3DMESH(C3DMat * ref, u32 * flag, u32 bit, C3DVec * vert, int vnum, u16 * idx, int inum);
	virtual ~CCol3DMESH();
	C3DVec	*	vertorg;	// ����]��Ԃ̒��_�o�b�t�@
	u16		*	indices;	// �C���f�b�N�X�o�b�t�@
	int			vert_num;	// ���_��
	int			idx_num;	// 

	C3DVec	*	vertices;	// ��]��̒��_�o�b�t�@

	void update(C3DMat& mat);
};

// ��]�\������
class CCol3DBOX : public CCol3D
{
public:
	CCol3DBOX(C3DMat * ref, u32 * flag, u32 bit, C3DVec& size, C3DMat& m);
	virtual ~CCol3DBOX();
	C3DVec		org[4];	// ���[�J�����W��ŉ�]���ꂽ��Ԃ̊e�����x�N�g��
	float		len[4];	// �e�����x�N�g���̒���(size�̔���)
	C3DVec		size;	// ��]���Ă��Ȃ���Ԃɂ����钼���̂̊e�������T�C�Y(�ӂ̒���)

	C3DVec		vec[4];	// [0]���s�ړ��ςݒ��S�ʒu / [1]�`[3] ��]�ς݂̊e�������x�N�g��(�T�C�Y�̔�����)

	// ���݂̃}�g���N�X�l�� vec[] �̓��e���Čv�Z����
	void update(C3DMat& m);

	float getLengthOnAxis(C3DVec& axis);

private:
	static C3DVec	axis_x;
	static C3DVec	axis_y;
	static C3DVec	axis_z;
};

// ����
class CCol3DSPHERE : public CCol3D
{
public:
	CCol3DSPHERE(C3DMat * ref, u32 * flag, u32 bit, C3DVec& pos, float r);
	virtual ~CCol3DSPHERE();
	C3DVec	org;	// ���[�J�����W�ʒu
	float	r;		// ���a

	C3DVec	vec;	// �}�g���N�X�ňړ����ꂽ���S

	void update(C3DMat& m);
};

// ����
class CCol3DLINE : public CCol3D
{
public:
	CCol3DLINE(C3DMat * ref, u32 * flag, u32 bit, C3DVec * pos);
	virtual ~CCol3DLINE();
	C3DVec	org[2];
	C3DVec	vec[2];	// [0]�n�_ / [1]�I�_

	void update(C3DMat& m);
};

// �_
class CCol3DPOINT : public CCol3D
{
public:
	CCol3DPOINT(C3DMat * ref, u32 * flag, u32 bit, C3DVec& vec);
	virtual ~CCol3DPOINT();
	C3DVec	org;
	C3DVec	vec;	// �ʒu
	void update(C3DMat& m);
};

#endif // CCOl3DTypes_h
