#ifndef C3DModel_h
#define C3DModel_h

#include <GLES2/gl2.h>

#include "Types3D.h"
#include "C3DDrawable.h"
#include "C3DDrawEnv.h"
#include "C3DMaterial.h"

class C3DCharacter;
class CGLTex;

class C3DModel : public C3DDrawable
{
	friend class C3DCharacter;
protected:
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
		VEC3	norm;		// 法線(4x3 = 12byte)
		VEC3	tang;		// 接線(4x3 = 12byte)
		UV		uv;			// テクスチャUV値(8byte)
		u8		bone[4];	// 参照ボーンのindex
		u8		wght[4];	// 各ボーンの参照ウェイト
		u8		rgba[4];	// 頂点カラー(4byte)
	};

	/*
		上記法線と接線、およびそれらの外積によって求められるベクトルによって、
		テクスチャを中心とした系の軸方向が定められる。
		vertex shader 側で光源ベクトルをこの系におけるベクトルに変換し、
		NormalMap に使用する。
	*/

	// メッシュ情報
	VERTEX	*	m_vertices;		// 頂点
	u16		*	m_indices;		// 頂点index
	u16			m_vertnum;		// 頂点の数
	u16			m_idxnum;		// 頂点indexの数

	// ボーン情報(最大64個まで)
	C3DVec	*	m_bones;		// 骨格の起点情報(親骨格原点からの相対)
	u8		*	m_boneparent;	// 各骨格の親をindexで持つ([0]は必ず起点が親)
	int			m_bonenum;		// 骨格の数

	// マテリアル情報
	C3DMaterial * m_material;	// マテリアル情報

	GLuint		m_idxVert;		// 頂点バッファ
	GLuint		m_idxIndex;		// インデックスバッファ
	bool		m_ready;		// バッファ転送済み

	const char	*	m_modelName;



public:
	// 骨格数を返す
	inline int getBoneSize() throw() { return m_bonenum; }

	C3DModel(C3DShader * shader, const char * modelName = 0);
	virtual ~C3DModel();

	void setup(C3DShader * env);
	void cleanup(C3DShader * env);

	bool recovery();
	bool destruction();

protected:
	bool newVertices(int num = 0);
	bool newIndices(int num = 0);
	bool newBones(int num = 0);
	inline void setMaterial(C3DMaterial * material) { m_material = material; }

	void setBuffer();
};



#endif // C3DModel_h
