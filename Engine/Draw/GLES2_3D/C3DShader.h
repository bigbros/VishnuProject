#ifndef C3DShader_h
#define C3DShader_h

#include "CGLShader.h"
#include "C3DDrawable.h"

class C3DDrawEnv;
class C3DDrawable;

class C3DShader : public CGLShader
{
	friend class C3DDrawEnv;
	friend class C3DDrawable;
public:
	enum DRAWPHASE{
		DP_SKYDOME = 0,		// 天球
		DP_OBJECT,			// 通常オブジェクト

		DP_MAX				// 描画フェーズ数
	};

	enum {
		SID_SYSTEM = 0xff000000
	};

	C3DShader(int id, C3DDrawEnv * env, DRAWPHASE phase);
	virtual ~C3DShader();
	virtual GLuint uniformOffset();
	virtual void preConfig(C3DVec * lightVec, C3DVec * lightCol, C3DVec * ambientCol, C3DMat * projection, C3DMat * cameraInvert) = 0;
	virtual void after() = 0;
	void render();

private:
	C3DDrawable		*	m_begin;
	C3DDrawable		*	m_end;

	DRAWPHASE			m_phase;
	C3DDrawEnv		*	m_env;
	int					m_sid;	// shader ID

	struct SHADER_LINK {
		SHADER_LINK();
		C3DShader	*	prev;
		C3DShader	*	next;
	};
	SHADER_LINK			m_sisters;
};

#endif // C3DShaderBase_h
