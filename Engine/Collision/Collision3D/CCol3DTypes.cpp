#include "CCol3DTypes.h"

CCol3DMESH::CCol3DMESH(C3DMat * ref, u32 * flag, u32 bit, C3DVec * vert, int vnum, u16 * idx, int inum)
	: CCol3D(C_MESH, ref, flag, bit)
	, vertorg(vert)
	, indices(idx)
	, vert_num(vnum)
	, idx_num(inum)
{
	vertices = nullptr;
	try {
		vertices = new C3DVec[vert_num];
	}
	catch (std::bad_alloc& ex) {
		throw ex;
	}
}

CCol3DMESH::~CCol3DMESH()
{
	delete[] vertices;
}

void
CCol3DMESH::update(C3DMat& m)
{
	vertices[0] = vertorg[0] * m;
	m_bound.init(vertices[0]);
	for (int i = 1; i < vert_num; i++) {
		vertices[i] = vertorg[i] * m;
		m_bound.update(vertices[i]);
	}
}

CCol3DBOX::CCol3DBOX(C3DMat * ref, u32 * flag, u32 bit, C3DVec& size, C3DMat& m)
	: CCol3D(C_BOX, ref, flag, bit)
{
	len[0] = 0.0f;
	len[1] = size.x / 2.0f;
	len[2] = size.y / 2.0f;
	len[3] = size.z / 2.0f;

	vec[0] = C3DVec(0.0f, 0.0f, 0.0f);		// box���_
	vec[1] = C3DVec(len[1], 0.0f, 0.0f);	// x�����x�N�g��
	vec[2] = C3DVec(0.0f, len[2], 0.0f);	// y�����x�N�g��
	vec[3] = C3DVec(0.0f, 0.0f, len[3]);	// z�����x�N�g��

	// �����ʒu�Ɖ�]��\��matrix����������Ԃ�org�Ƃ���B�A
	for (int i = 0; i < 4; i++) {
		org[i] = vec[i] * m;
	}
	// �v�Z���ɂ́A�����̃x�N�g���ɂ����matrix����������Ō��_���W�������A�Ƃ����������s���B
}

CCol3DBOX::~CCol3DBOX(){}

C3DVec	CCol3DBOX::axis_x(1.0f, 0.0f, 0.0f);
C3DVec	CCol3DBOX::axis_y(0.0f, 1.0f, 0.0f);
C3DVec	CCol3DBOX::axis_z(0.0f, 0.0f, 1.0f);

float
CCol3DBOX::getLengthOnAxis(C3DVec& axis)
{
	float len = 0.0f;
	for (int i = 1; i < 4; i++) {
		len += fabsf(vec[i] % axis);
	}
	return len;
}

void
CCol3DBOX::update(C3DMat& m)
{
	// �����x�N�g������]�����āA��Ԓ��̍��W�l�ɂ�����A�����x�N�g���͌��_���W�������ăx�N�g���ɂ���B
	vec[0] = org[0] * m;
	for (int i = 1; i < 4; i++) {
		vec[i] = org[i] * m;
		vec[i] = vec[i] - org[0];
	}
	// ��]���ʂ�����A�e�ӂ������ꂩ�̎��ƕ��s�ȍŏ��̒����̂𓾂�
	m_bound.init(vec[0]);
	float x = getLengthOnAxis(axis_x);
	float y = getLengthOnAxis(axis_y);
	float z = getLengthOnAxis(axis_z);
	C3DVec vmax(vec[0].x + x, vec[0].y + y, vec[0].z + z);
	C3DVec vmin(vec[0].x - x, vec[0].y - y, vec[0].z - z);
	m_bound.update(vmax);
	m_bound.update(vmin);
}

CCol3DSPHERE::CCol3DSPHERE(C3DMat * ref, u32 * flag, u32 bit, C3DVec& pos, float rr)
	: CCol3D(C_SPHERE, ref, flag, bit)
	, org(pos)
	, r(rr)
{
}

CCol3DSPHERE::~CCol3DSPHERE() {}

void
CCol3DSPHERE::update(C3DMat& m)
{
	vec = org * m;
	C3DVec vmax(vec.x + r, vec.y + r, vec.z + r);
	C3DVec vmin(vec.x - r, vec.y - r, vec.z - r);
	m_bound.init(vmax);
	m_bound.update(vmin);
}


CCol3DLINE::CCol3DLINE(C3DMat * ref, u32 * flag, u32 bit, C3DVec * pos)
	: CCol3D(C_LINE, ref, flag, bit)
{
	org[0] = pos[0];
	org[1] = pos[1];
}

CCol3DLINE::~CCol3DLINE() {}

void
CCol3DLINE::update(C3DMat& m)
{
	for (int i = 0; i < 2; i++) {
		vec[i] = org[i] * m;
	}
	m_bound.init(vec[0]);
	m_bound.update(vec[1]);
}

CCol3DPOINT::CCol3DPOINT(C3DMat * ref, u32 * flag, u32 bit, C3DVec& pos)
	: CCol3D(C_POINT, ref, flag, bit)
{
	org = pos;
}

CCol3DPOINT::~CCol3DPOINT() {}

void
CCol3DPOINT::update(C3DMat& m)
{
	vec = org * m;
	m_bound.init(vec);
}
