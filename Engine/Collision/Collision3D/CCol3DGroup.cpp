#include <iostream>

#include "CCol3DGroup.h"
#include "CCol3DCollision.h"

CCol3D::CCol3D(TYPE type,C3DMat * ref, u32 * flag, u32 bit)
	: m_parent(0)
	, m_prev(0)
	, m_next(0)
	, m_ref(ref)
	, m_flag(flag)
	, m_bit(bit)
	, m_type(type)
{

}

CCol3D::~CCol3D()
{
	strip();
}

// ���g�����ݏ������Ă���GROUP����菜��
void
CCol3D::strip()
{
	// ���g���폜����ۂ�
	if (m_parent) {
		m_parent->m_count--;
		if (m_prev) {
			m_prev->m_next = m_next;
		}
		else {
			m_parent->m_begin = m_next;
		}
		if (m_next) {
			m_next->m_prev = m_prev;
		}
		else {
			m_parent->m_end = m_prev;
		}
	}
	m_parent = 0;
	m_prev = 0;
	m_next = 0;
}

CCol3DGroup::CCol3DGroup(u32 gID)
	: m_begin(0)
	, m_end(0)
	, m_gID(gID)
	, m_prev(0)
	, m_next(0)
	, m_count(0)
{
	// �����Ɠ�����manager�o�^
	CCol3DGroupMgr::getInstance().add(this);
}

CCol3DGroup::~CCol3DGroup()
{
	// �O���[�v�ɏ������� collider ���c���Ă���ΑS�č폜����
	if (m_begin && m_end) {
		CCol3D * col = m_begin;
		while (col) {
			CCol3D * nxt = col->m_next;
			delete col;
			col = nxt;
		}
	}
	// group �� manager ����폜
	strip();
}

void
CCol3DGroup::add(CCol3D * collider)
{
	collider->strip();
	collider->m_parent = this;
	collider->m_prev = m_end;
	if (collider->m_prev) {
		m_end->m_next = collider;
	}
	else {
		m_begin = collider;
	}
	m_end = collider;
	m_count++;
}

void
CCol3DGroup::strip()
{
	CCol3DGroupMgr& mgr = CCol3DGroupMgr::getInstance();
	if (m_prev) {
		m_prev->m_next = m_next;
	}
	else {
		mgr.m_begin = m_next;
	}
	if (m_next) {
		m_next->m_prev = m_prev;
	}
	else {
		mgr.m_end = m_prev;
	}
	m_prev = m_next = 0;
}

void
CCol3DGroup::update()
{
	// �������邷�ׂĂ�collider object �ɂ��� update() ���Ăяo���B
	// �����I�u�W�F�N�g�͎��g�̕ێ�������W�l�ɂ��čČv�Z�̕K�v������΂�����s���B
	for (CCol3D * collider = m_begin; collider; collider = collider->m_next) {
		collider->update();
	}
}

#define CODE(_a, _b)  ((_a & 0xff) | ((_b & 0xff) << 8))

// �w�肳�ꂽ�O���[�v�ԂŏՓ˂��Ă�����̂����邩�m�F����
void
CCol3DGroup::judge(CCol3DGroup * target)
{
	for (CCol3D * item1 = m_begin; item1; item1 = item1->m_next) {
		for (CCol3D * item2 = target->m_begin; item2; item2 = item2->m_next) {

			// ���҂�BoundingBox���d�Ȃ��Ă��Ȃ���΋�ԓI�Ɋu������Ă��邽�߁A
			// ���������Փ˂𔻒肷��K�v���Ȃ��B
			if (!item1->m_bound.isOverlap(item2->m_bound)) continue;

			CCol3D * A = item1;
			CCol3D * B = item2;
			if (B->m_type < A->m_type) {
				CCol3D * c = A;
				A = B;
				B = c;
			}
			int code = CODE(A->m_type, B->m_type);
			bool is_hit = false;		// �����������ۂ��̃X�e�[�^�X
			bool is_point = false;		// ���������ʒu������ꍇ��true
			C3DVec hit;		// �ʒu���Ƃ��ꍇ�A���̈ʒu��Ԃ��B
			C3DVec norm;	// ���������ʂ̌���
			switch (code) {
			case CODE(CCol3D::C_MESH, CCol3D::C_MESH):		is_hit = CCol3DCollision::isHitMESHandMESH((CCol3DMESH *)A, (CCol3DMESH *)B);							break;
			case CODE(CCol3D::C_MESH, CCol3D::C_BOX):		is_hit = CCol3DCollision::isHitMESHandBOX((CCol3DMESH *)A, (CCol3DBOX *)B);								break;
			case CODE(CCol3D::C_MESH, CCol3D::C_SPHERE):	is_hit = is_point = CCol3DCollision::isHitMESHandSPHERE(hit, norm, (CCol3DMESH *)A, (CCol3DSPHERE *)B);	break;
			case CODE(CCol3D::C_MESH, CCol3D::C_LINE):		is_hit = is_point = CCol3DCollision::isHitMESHandLINE(hit, norm, (CCol3DMESH *)A, (CCol3DLINE *)B);		break;
			case CODE(CCol3D::C_MESH, CCol3D::C_POINT):		is_hit = is_point = CCol3DCollision::isHitMESHandPOINT(hit, norm, (CCol3DMESH *)A, (CCol3DPOINT *)B);	break;
			case CODE(CCol3D::C_BOX, CCol3D::C_BOX):		is_hit = CCol3DCollision::isHitBOXandBOX((CCol3DBOX *)A, (CCol3DBOX *)B);								break;
			case CODE(CCol3D::C_BOX, CCol3D::C_SPHERE):		is_hit = CCol3DCollision::isHitBOXandSPHERE((CCol3DBOX *)A, (CCol3DSPHERE *)B);							break;
			case CODE(CCol3D::C_BOX, CCol3D::C_LINE):		is_hit = CCol3DCollision::isHitBOXandLINE((CCol3DBOX *)A, (CCol3DLINE *)B);								break;
			case CODE(CCol3D::C_BOX, CCol3D::C_POINT):		is_hit = CCol3DCollision::isHitBOXandPOINT((CCol3DBOX *)A, (CCol3DPOINT *)B);							break;
			case CODE(CCol3D::C_SPHERE, CCol3D::C_SPHERE):	is_hit = CCol3DCollision::isHitSPHEREandSPHERE((CCol3DSPHERE *)A, (CCol3DSPHERE *)B);					break;
			case CODE(CCol3D::C_SPHERE, CCol3D::C_LINE):	is_hit = is_point = CCol3DCollision::isHitSPHEREandLINE(hit, norm, (CCol3DSPHERE *)A, (CCol3DLINE *)B);	break;
			case CODE(CCol3D::C_SPHERE, CCol3D::C_POINT):	is_hit = CCol3DCollision::isHitSPHEREandPOINT((CCol3DSPHERE *)A, (CCol3DPOINT *)B);						break;
			case CODE(CCol3D::C_LINE, CCol3D::C_LINE):		is_hit = CCol3DCollision::isHitLINEandLINE((CCol3DLINE *)A, (CCol3DLINE *)B);							break;
			case CODE(CCol3D::C_LINE, CCol3D::C_POINT):		is_hit = CCol3DCollision::isHitLINEandPOINT((CCol3DLINE *)A, (CCol3DPOINT *)B);							break;
			case CODE(CCol3D::C_POINT, CCol3D::C_POINT):	is_hit = CCol3DCollision::isHitPOINTandPOINT((CCol3DPOINT *)A, (CCol3DPOINT *)B);						break;
			}

			// �Փ˂����o���ꂽ�ꍇ�̏���
			if (is_hit) {


			}
		}
	}
}


CCol3DGroupMgr::CCol3DGroupMgr() {}
CCol3DGroupMgr::~CCol3DGroupMgr() {}

CCol3DGroupMgr&
CCol3DGroupMgr::getInstance()
{
	static CCol3DGroupMgr instance;
	return instance;
}

void
CCol3DGroupMgr::add(CCol3DGroup * grp)
{
	grp->strip();
	grp->m_prev = m_end;
	if (grp->m_prev) {
		m_end->m_next = grp;
	}
	else {
		m_begin = grp;
	}
	m_end = grp;
}

CCol3DGroup *
CCol3DGroupMgr::findGroup(u32 gID)
{
	for (CCol3DGroup * grp = m_begin; grp; grp = grp->m_next) {
		if (grp->m_gID == gID) return grp;
	}
	return 0;
}
