#ifndef C3DSkyDome_h
#define C3DSkyDome_h

#include "C3DMaterial.h"
#include "C3DDrawable.h"

#include "C3DSkyDomeShader.h"

class C3DSkyDome;

class C3DSkyDomeModel : public C3DDrawable
{
	friend class C3DSkyDome;
private:
	enum {
		V_RESO		=	12,
		H_RESO		=	24
	};
	// メッシュ頂点情報。後で増えることがあるので、構造体にしておく。
	struct VEC3 {
		float	x;
		float	y;
		float	z;
	};
	struct UV {
		float	u;
		float	v;
	};
	struct VERTEX {
		VEC3	vert;		// 座標(4*3 = 12byte)
		UV		uv;			// テクスチャUV値(8byte)
	};

	// メッシュ情報
	VERTEX	*	m_vertices;		// 頂点
	u16		*	m_indices;		// 頂点index
	u16			m_vertnum;		// 頂点の数
	u16			m_idxnum;		// 頂点indexの数

	// マテリアル情報
	CGLTex	*	m_texture;	// 天球テクスチャは直接指定する

	GLuint		m_idxVert;		// 頂点バッファ
	GLuint		m_idxIndex;		// インデックスバッファ
	bool		m_ready;		// バッファ転送済み

	int			m_reso_v;		// 緯度方向分割数
	int			m_reso_h;		// 経度方向分割数

	float		m_r;			// 半径(便宜上)

	const char	*	m_modelName;



public:

	C3DSkyDomeModel(C3DSkyDomeShader * shader, float r, CGLTex * pTex, const char * modelName = 0, int h_reso = H_RESO, int v_reso = V_RESO);
	virtual ~C3DSkyDomeModel();

	void setup(C3DShader * pShader);
	void cleanup(C3DShader * pShader);

	bool recovery();
	bool destruction();

protected:
	bool newVertices(int num = 0);
	bool newIndices(int num = 0);
	inline void setTexture(CGLTex * texture) { m_texture = texture; }

	void setBuffer();

	void createVertex();

};

class C3DSkyDome : public C3DDrawObj
{
private:
	C3DSkyDomeModel		*	m_model;
	CGLTex				*	m_tex;
	C3DVec					m_color;
public:
	C3DSkyDome(C3DSkyDomeShader * shader, CGLTex * pTex);
	virtual ~C3DSkyDome();

	void render(C3DShader * pShader);

protected:
	// このオブジェクトのマトリクス計算後に行う処理を記述
	bool calcProcedure(bool is_recalc);
};

#endif // C3DSkyDome_h
