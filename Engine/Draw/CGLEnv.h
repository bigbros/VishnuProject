#ifndef CGLEnv_h
#define CGLEnv_h

#include <GLES2/gl2.h>
#include "CGLBase.h"
#include "CGLObj.h"

class IGLDrawEnv : public CGLBase
{
public:
	IGLDrawEnv();
	virtual ~IGLDrawEnv();

	virtual void setResolution(int width, int height) = 0;
	virtual void Clear() = 0;
};

class CGLEnv : public CGLBase
{
	friend class CGLObj;
public:
	enum ENVID {
		C3D,
		C2D,
		ENV_MAX
	};
private:
	// 現在生成されているGLオブジェクトのリスト
	CGLObj			*	m_begin;
	CGLObj			*	m_end;

	// 画面解像度
	int					m_width;
	int					m_height;

	// 描画環境
	IGLDrawEnv		*	m_drawEnv[ENV_MAX];
public:
	CGLEnv();
	virtual ~CGLEnv();

	static CGLEnv& getInstance();

	inline void setDrawEnv(ENVID id, IGLDrawEnv * pEnv) { m_drawEnv[id] = pEnv; }
	template <typename T>
	inline T * DrawEnv(ENVID id) {
		return (T *)m_drawEnv[id];
	}

	// 描画環境全体のクリア(2D,3Dまとめて)
	void Clear();

	// 画面解像度設定
	void setResolution(int width, int height);

	// 画面解像度取得
	inline const int width() const { return m_width; }
	inline const int height() const { return m_height; }

	// 画面クリア
	void screenClear();

	// 描画終了待機
	void screenFinish();

	// GL環境の再構築を行う。シェーダのロードおよび、モデルの頂点バッファ等の再登録など。
	bool recovery();

	// GL環境の破棄を行う。シェーダ/プログラムの破棄、頂点/インデックスバッファの破棄など。
	bool destruction();

};

#endif // CGLEnv_h
