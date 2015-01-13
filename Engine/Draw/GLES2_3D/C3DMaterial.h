#ifndef C3DMaterial_h
#define C3DMaterial_h

#include "CGLBase.h"
#include "C3DDefaultShader.h"

class CGLTex;

class C3DMaterial : public CGLBase
{
public:
	enum {
		TEXTURE = 0x00000001,		// �e�N�X�`���}�b�s���O�̗L��(off: �����n)
		NORMAL = 0x00000002,		// �@���}�b�s���O�̗L��(off: �V�F�[�f�B���O�ɒ��_�̖@�������g�p)
		DIFFUSE = 0x00000004,		// diffuse�V�F�[�f�B���O�̗L��(off: ���_RGBA�l�g�p)
		SPECULAR = 0x00000008		// specular�̗L��(off:specular�Ȃ�)
	};
private:
	CGLTex		*	m_texture;		// �e�N�X�`��
	CGLTex		*	m_normal;		// �@���}�b�v
	float			m_shininess;		// �X�y�L�����̋P�x

	int				m_switch;		// �}�e���A���@�\�̃X�C�b�`
public:
	C3DMaterial();
	virtual ~C3DMaterial();

	inline void setTexture(CGLTex * pTex) {
		m_texture = pTex;
		m_switch |= TEXTURE;
	}

	inline void setNormal(CGLTex * pTex) {
		m_normal = pTex;
		m_switch |= NORMAL;
	}

	inline void setShininess(float shiness) {
		m_shininess = shiness;
		m_switch |= SPECULAR;
	}

	void setup(C3DDefaultShader * shader);
};



#endif // C3DMaterial_h
