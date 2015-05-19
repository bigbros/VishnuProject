#ifndef C3DCelestialSphere_h
#define C3DCelestialSphere_h

#include "C3DMaterial.h"
#include "C3DDrawable.h"

#include "C3DCelestialSphereShader.h"

class C3DCelestialSphere;

class C3DCelestialModel : public C3DDrawable
{
	friend class C3DCelestialSphere;
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

	C3DCelestialModel(float r, CGLTex * pTex, const char * modelName = 0, int h_reso = H_RESO, int v_reso = V_RESO);
	virtual ~C3DCelestialModel();

	void setup(C3DDrawEnv * env);
	void cleanup(C3DDrawEnv * env);

	bool recovery();
	bool destruction();

protected:
	bool newVertices(int num = 0);
	bool newIndices(int num = 0);
	inline void setTexture(CGLTex * texture) { m_texture = texture; }

	void setBuffer();

	void createVertex();

};

class C3DCelestialSphere : public C3DDrawObj
{
private:
	C3DCelestialModel		*	m_model;
	CGLTex					*	m_tex;
	C3DVec						m_color;
public:
	C3DCelestialSphere(CGLTex * pTex);
	virtual ~C3DCelestialSphere();

	void render(C3DDrawEnv * env);

protected:
	// このオブジェクトのマトリクス計算後に行う処理を記述
	bool calcProcedure(bool is_recalc);
};

#endif // C3DCelestialSphere_h
