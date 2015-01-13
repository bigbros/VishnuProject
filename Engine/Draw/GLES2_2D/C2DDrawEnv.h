#ifndef C2DDrawEnv_h
#define C2DDrawEnv_h

#include "Types2D.h"
#include "dtypes.h"
#include "C2DObj.h"
#include "CGLObj.h"
#include "C2DDefaultShader.h"
#include "C2DDrawQueue.h"
#include "C2DTexOT.h"
#include "CGLEnv.h"

class C2DDrawEnv;

class C2DRootObj : public C2DObj
{
	friend class C2DDrawEnv;
private:
	C2DRootObj();
	virtual ~C2DRootObj();

public:
	bool calcProcedure(bool is_recalc);
	void render();
	void destroyChildren();
};

class C2DDrawEnv : public IGLDrawEnv
{
	friend class C2DTexOT;
private:
	struct TEXLIST {
		TEXLIST();
		C2DTexOT	*	begin;
		C2DTexOT	*	end;
	};
	C2DMat				m_rootMatrix;
	C2DRootObj		*	m_objRoot;

	C3DMat				m_projection;

	// ����2D�`��ŗp�����Ă���e�N�X�`���ɂ��āA
	// ���̃e�N�X�`�����g�p���Ă���I�u�W�F�N�g���A�e�N�X�`���P�ʂ�Ordering Table�����Ă���B
	TEXLIST					m_texlist;

	// �f�t�H���g�ŗp���� shader object
	C2DDefaultShader	*	m_shader;

	// �`��ɗp���钸�_�o�b�t�@��ێ�����B
	// �܂��Auniform�]���L���[�����B
	C2DDrawQueue		*	m_drawQueue;

public:
	C2DDrawEnv();
	~C2DDrawEnv();

	void setResolution(int width, int height);
	void Render();
	void Clear();

	inline C2DObj * getRootObj() { return m_objRoot; }
	C2DTexOT * getTexOT(CGLTex * pTex);

	inline void CalcWorld() { m_objRoot->recalcMatrix(m_rootMatrix, false); }

	// �f�t�H���g�V�F�[�_�̐ݒ��RootObject�̐���
	bool init(C2DDefaultShader * pShader);
	inline C2DDefaultShader * getDefaultShader() { return m_shader; }
};


#endif // C3DDrawEnv_h
