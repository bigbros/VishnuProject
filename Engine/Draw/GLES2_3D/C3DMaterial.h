#ifndef C3DMaterial_h
#define C3DMaterial_h

#include "CGLBase.h"
#include "C3DDefaultShader.h"

class CGLTex;
class C3DModel;

class C3DMaterial : public CGLBase
{
	friend class C3DModel;
public:
	enum {
		TEXTURE = 0x00000001,		// �e�N�X�`���}�b�s���O�̗L��(off: �����n)
		NORMAL = 0x00000002,		// �@���}�b�s���O�̗L��(off: �V�F�[�f�B���O�ɒ��_�̖@�������g�p)
		SPECULAR = 0x00000004,		// specular�}�b�s���O�̗L��(off:�S�̂��Œ�shininess�ŏ���)
		DIFFUSE = 0x00000008,		// diffuse�V�F�[�f�B���O�̗L��(off: ���_RGBA�l�g�p)
	};

	struct RGB {
		float r;
		float g;
		float b;
		float a;
	};
	struct VEC {
		float x;
		float y;
		float z;
		float w;
	};

	struct MATERIAL {
		u32		indicesBegin;	// �C���f�b�N�X�o�b�t�@���ł��̃}�e���A�����g�p����͈͂̊J�n�v�f
		u32		indicesNum;		// ���̃}�e���A�����g�p���ĕ`�悳���index�̐�
		RGB		ambient;
		RGB		diffuse;
		RGB		emissive;
		RGB		transparency;
		RGB		specular;
		RGB		reflection;
		VEC		bump;
		float	ambientFactor;
		float	diffuseFactor;
		float	emissiveFactor;
		float	bumpFactor;
		float	transparencyFactor;
		float	shininess;
		float	reflectionFactor;
	};


private:
	C3DMaterial	*	m_prev;
	C3DMaterial	*	m_next;

	CGLTex		*	m_texture;		// �e�N�X�`��
	CGLTex		*	m_normal;		// �@���}�b�v
	CGLTex		*	m_specular;		// specularmap

	MATERIAL		params;

//	float			m_shininess;	// �X�y�L�����̋P�x

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

	inline void setSpecular(CGLTex * pTex) {
		m_specular = pTex;
		m_switch |= SPECULAR;
	}

	inline void setMaterialParams(MATERIAL * mat) {
		memcpy(&params, mat, sizeof(MATERIAL));
	}
	/*
	inline void setShininess(float shiness) {
		m_shininess = shiness;
	}
	*/
	void setup(C3DDefaultShader * shader);

private:
	void setTexture(int mask, CGLTex * tex, GLenum target, int num, GLint shaderIdx, GLint flagIdx);
};



#endif // C3DMaterial_h
