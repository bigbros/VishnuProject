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
		TEXTURE = 0x00000001,		// テクスチャマッピングの有無(off: 白無地)
		NORMAL = 0x00000002,		// 法線マッピングの有無(off: シェーディングに頂点の法線情報を使用)
		SPECULAR = 0x00000004,		// specularマッピングの有無(off:全体を固定shininessで処理)
		DIFFUSE = 0x00000008,		// diffuseシェーディングの有無(off: 頂点RGBA値使用)
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
		u32		indicesBegin;	// インデックスバッファ中でこのマテリアルを使用する範囲の開始要素
		u32		indicesNum;		// このマテリアルを使用して描画されるindexの数
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

	CGLTex		*	m_texture;		// テクスチャ
	CGLTex		*	m_normal;		// 法線マップ
	CGLTex		*	m_specular;		// specularmap

	MATERIAL		params;

//	float			m_shininess;	// スペキュラの輝度

	int				m_switch;		// マテリアル機能のスイッチ
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
