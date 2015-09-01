#ifndef C3DFBO_h
#define C3DFBO_h

#include "CGLFBO.h"
#include "CGLShader.h"
#include "dtypes.h"

class C3DFBOShader : public CGLShader
{
public:
	// uniforms
	GLint				m_u_tex;		// 使用するテクスチャ

	// attribs
	GLint				m_a_vert;		// 頂点vec2(一辺が1.0の正方形を描く形で与える。
	GLint				m_a_uv;
public:
	C3DFBOShader();
	virtual ~C3DFBOShader();

protected:
	void setShaderParams(GLuint program);
};

class C3DFBO : public CGLFBO
{
public:
	C3DFBO(const char * shaderPath, int width, int height);
	virtual ~C3DFBO();

	void setLens(int lensId, GLint offsetUniform);
	void renderGlobal();
	void onChangeGlobalResolution(int width, int height);
	float getAngle();			// デフォルトの視野角を返す

private:
	void createShader(const char * shaderPath);
	void createVertex();
private:
	C3DFBOShader	*	m_shader;

	struct VEC {
		float x;
		float y;
		float u;
		float v;
	};
	VEC				*	m_vertices;
	u16				*	m_indices;
	GLuint			m_idxVert;		// 頂点バッファ
	GLuint			m_idxIndex;		// インデックスバッファ
};


#endif // C3DFBO_h
