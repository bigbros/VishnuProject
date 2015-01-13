#ifndef C3DMaterial_h
#define C3DMaterial_h

#include "CGLBase.h"
#include "C3DDefaultShader.h"

class CGLTex;

class C3DMaterial : public CGLBase
{
public:
	enum {
		TEXTURE = 0x00000001,		// テクスチャマッピングの有無(off: 白無地)
		NORMAL = 0x00000002,		// 法線マッピングの有無(off: シェーディングに頂点の法線情報を使用)
		DIFFUSE = 0x00000004,		// diffuseシェーディングの有無(off: 頂点RGBA値使用)
		SPECULAR = 0x00000008		// specularの有無(off:specularなし)
	};
private:
	CGLTex		*	m_texture;		// テクスチャ
	CGLTex		*	m_normal;		// 法線マップ
	float			m_shininess;		// スペキュラの輝度

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

	inline void setShininess(float shiness) {
		m_shininess = shiness;
		m_switch |= SPECULAR;
	}

	void setup(C3DDefaultShader * shader);
};



#endif // C3DMaterial_h
