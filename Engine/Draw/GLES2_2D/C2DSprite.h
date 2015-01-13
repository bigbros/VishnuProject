#ifndef C2DSprite_h
#define C2DSprite_h

#include "Types2D.h"
#include "Types3D.h"
#include "C2DObj.h"
#include "C2DTexOT.h"


class C2DSprite : public C2DObj
{
	friend class C2DTexOT;
	friend class C2DDrawQueue;
	friend class C2DQueueItem;
private:
	struct TEXLINK {
		TEXLINK();
		C2DSprite	*	prev;
		C2DSprite	*	next;
	};
	TEXLINK				m_texlink;	// 同じテクスチャおよびプライオリティを持つスプライトのリンク
	C2DTexOT		*	m_texOT;	// テクスチャ単位OTのポインタ
	CGLTex			*	m_tex;		// テクスチャ本体

	C2DQueueItem	*	m_queue;	// 使用しているキューアイテム
	int					m_qidx;		// キュー内のインデックス
	C3DVec			*	m_rgba;		// 描画色
	C3DVec			*	m_uv;		// テクスチャのUV値
	C2DMat			*	m_drawmat;	// 描画用のマトリクス

	C3DVec				m_luv;		// ローカル保持用uv
	C3DVec				m_lrgba;		// ローカル保持用RGBA

	float				m_width;	// スプライトの幅
	float				m_height;	// スプライトの高さ


public:
	C2DSprite();
	virtual ~C2DSprite();

	void setTex(CGLTex * pTex);
	void setSize(const float width, const float height);
	void setRGBA(C3DVec& rgba);

	// 注意: C2DSpriteでは、テクスチャを左上原点で扱う。
	// 正規化アトラスを用いる場合は実効領域のサイズに関わらずアトラス全体の左上原点正規化座標となるので注意。
	void setUVnorm(C2DVec& uv0, C2DVec& uv1);

	// 注意: C2DSpriteでは、テクスチャを左上原点で扱う。
	// ピクセル指定を用いる場合は実効領域の左上からピクセル単位での指定となるため、こちらの利用を推奨。
	void setUVpx(C2DVec& uv0, C2DVec& uv1);

private:
	bool calcProcedure(bool recalc);
	bool priorityProcedure(int pri);
};


#endif // C2DSprite_h
