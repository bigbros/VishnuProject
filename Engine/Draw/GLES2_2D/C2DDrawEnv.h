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

	// 現在2D描画で用いられているテクスチャについて、
	// そのテクスチャを使用しているオブジェクトを、テクスチャ単位でOrdering Table化してある。
	TEXLIST					m_texlist;

	// デフォルトで用いる shader object
	C2DDefaultShader	*	m_shader;

	// 描画に用いる頂点バッファを保持する。
	// また、uniform転送キューを持つ。
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

	// デフォルトシェーダの設定とRootObjectの生成
	bool init(C2DDefaultShader * pShader);
	inline C2DDefaultShader * getDefaultShader() { return m_shader; }
};


#endif // C3DDrawEnv_h
