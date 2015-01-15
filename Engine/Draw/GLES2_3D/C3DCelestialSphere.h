#ifndef C3DCelestialSphere_h
#define C3DCelestialSphere_h

#include "C3DMaterial.h"
#include "C3DDrawable.h"

class C3DCelestialModel : public C3DDrawable
{
private:
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
	C3DMaterial * m_material;	// マテリアル情報

	GLuint		m_idxVert;		// 頂点バッファ
	GLuint		m_idxIndex;		// インデックスバッファ
	bool		m_ready;		// バッファ転送済み

	const char	*	m_modelName;



public:

	C3DCelestialModel(float r, const char * modelName = 0);
	virtual ~C3DCelestialModel();

	void setup(C3DDrawEnv * env);
	void cleanup(C3DDrawEnv * env);

	bool recovery();
	bool destruction();

protected:
	bool newVertices(int num = 0);
	bool newIndices(int num = 0);
	inline void setMaterial(C3DMaterial * material) { m_material = material; }

	void setBuffer();

};

class C3DCelestialSphere : public C3DDrawObj
{
private:
	C3DCelestialModel	*	m_model;
public:
	C3DCelestialSphere();
	virtual ~C3DCelestialSphere();

protected:
	// このオブジェクトのマトリクス計算後に行う処理を記述
	bool calcProcedure(bool is_recalc);
};

#endif // C3DCelestialSphere_h
