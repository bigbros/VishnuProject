#ifndef C2DSprite_h
#define C2DSprite_h

#include "Types2D.h"
#include "Types3D.h"
#include "C2DObj.h"
#include "C2DTexOT.h"


class C2DSprite : public C2DObj
{
	friend class C2DTexOT;
	friend class C2DDrawQueue;
	friend class C2DQueueItem;
private:
	struct TEXLINK {
		TEXLINK();
		C2DSprite	*	prev;
		C2DSprite	*	next;
	};
	TEXLINK				m_texlink;	// �����e�N�X�`������уv���C�I���e�B�����X�v���C�g�̃����N
	C2DTexOT		*	m_texOT;	// �e�N�X�`���P��OT�̃|�C���^
	CGLTex			*	m_tex;		// �e�N�X�`���{��

	C2DQueueItem	*	m_queue;	// �g�p���Ă���L���[�A�C�e��
	int					m_qidx;		// �L���[���̃C���f�b�N�X
	C3DVec			*	m_rgba;		// �`��F
	C3DVec			*	m_uv;		// �e�N�X�`����UV�l
	C2DMat			*	m_drawmat;	// �`��p�̃}�g���N�X

	C3DVec				m_luv;		// ���[�J���ێ��puv
	C3DVec				m_lrgba;		// ���[�J���ێ��pRGBA

	float				m_width;	// �X�v���C�g�̕�
	float				m_height;	// �X�v���C�g�̍���


public:
	C2DSprite();
	virtual ~C2DSprite();

	void setTex(CGLTex * pTex);
	void setSize(const float width, const float height);
	void setRGBA(C3DVec& rgba);

	// ����: C2DSprite�ł́A�e�N�X�`�������㌴�_�ň����B
	// ���K���A�g���X��p����ꍇ�͎����̈�̃T�C�Y�Ɋւ�炸�A�g���X�S�̂̍��㌴�_���K�����W�ƂȂ�̂Œ��ӁB
	void setUVnorm(C2DVec& uv0, C2DVec& uv1);

	// ����: C2DSprite�ł́A�e�N�X�`�������㌴�_�ň����B
	// �s�N�Z���w���p����ꍇ�͎����̈�̍��ォ��s�N�Z���P�ʂł̎w��ƂȂ邽�߁A������̗��p�𐄏��B
	void setUVpx(C2DVec& uv0, C2DVec& uv1);

private:
	bool calcProcedure(bool recalc);
	bool priorityProcedure(int pri);
};


#endif // C2DSprite_h
