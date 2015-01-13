#ifndef CGLEnv_h
#define CGLEnv_h

#include <GLES2/gl2.h>
#include "CGLBase.h"
#include "CGLObj.h"

class IGLDrawEnv : public CGLBase
{
public:
	IGLDrawEnv();
	virtual ~IGLDrawEnv();

	virtual void setResolution(int width, int height) = 0;
	virtual void Clear() = 0;
};

class CGLEnv : public CGLBase
{
	friend class CGLObj;
public:
	enum ENVID {
		C3D,
		C2D,
		ENV_MAX
	};
private:
	// ���ݐ�������Ă���GL�I�u�W�F�N�g�̃��X�g
	CGLObj			*	m_begin;
	CGLObj			*	m_end;

	// ��ʉ𑜓x
	int					m_width;
	int					m_height;

	// �`���
	IGLDrawEnv		*	m_drawEnv[ENV_MAX];
public:
	CGLEnv();
	virtual ~CGLEnv();

	static CGLEnv& getInstance();

	inline void setDrawEnv(ENVID id, IGLDrawEnv * pEnv) { m_drawEnv[id] = pEnv; }
	template <typename T>
	inline T * DrawEnv(ENVID id) {
		return (T *)m_drawEnv[id];
	}

	// �`����S�̂̃N���A(2D,3D�܂Ƃ߂�)
	void Clear();

	// ��ʉ𑜓x�ݒ�
	void setResolution(int width, int height);

	// ��ʉ𑜓x�擾
	inline const int width() const { return m_width; }
	inline const int height() const { return m_height; }

	// ��ʃN���A
	void screenClear();

	// �`��I���ҋ@
	void screenFinish();

	// GL���̍č\�z���s���B�V�F�[�_�̃��[�h����сA���f���̒��_�o�b�t�@���̍ēo�^�ȂǁB
	bool recovery();

	// GL���̔j�����s���B�V�F�[�_/�v���O�����̔j���A���_/�C���f�b�N�X�o�b�t�@�̔j���ȂǁB
	bool destruction();

};

#endif // CGLEnv_h
