#ifndef C2DDefaultShader_h
#define C2DDefaultShader_h

#include "CGLShader.h"

class C2DDefaultShader : public CGLShader
{
public:
	// uniforms
	GLint				m_u_projection;	// 2Dプロジェクションマトリクス
	GLint				m_u_mat;		// リサイズ/回転/並行移動マトリクス(vec3)の配列
	GLint				m_u_uv;			// 対角の正規化UV(左上/右下)をvec4形式(x,y)-(z,w)で与えたものの配列
	GLint				m_u_tex;		// 使用するテクスチャ
	GLint				m_u_rgba;		// 描画RGBA値

	// attribs
	GLint				m_a_vert;		// 頂点vec2(一辺が1.0の正方形を描く形で与える。
	GLint				m_a_params;		// 頂点に関するパラメータ(vec2)。現在のところ.x が「使用するuniform配列のindex」というところだけ。

public:
	C2DDefaultShader();
	virtual ~C2DDefaultShader();

protected:
	void setShaderParams(GLuint program);
};

#endif // C2DDefaultShader_h
