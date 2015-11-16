#ifndef C3DDrawEnv_h
#define C3DDrawEnv_h

#include "Types3D.h"
#include "C3DObj.h"
#include "CGLObj.h"
#include "CGLFBO.h"
#include "C3DCameraObj.h"
#include "CGLEnv.h"
#include "C3DShader.h"

class C3DDrawEnv;
class C3DDrawable;
class C3DShader;

class C3DRootObj : public C3DObj
{
	friend class C3DDrawEnv;
private:
	C3DRootObj();
	virtual ~C3DRootObj();

public:
	bool calcProcedure(bool is_recalc);
	void render();
	void destroyChildren();
};

class C3DDrawEnv : public IGLDrawEnv
{
	friend class C3DDrawable;
private:
	C3DMat			m_rootMatrix;
	C3DRootObj	*	m_objRoot;

	C3DCameraObj	*	m_objCamera;
	C3DMat				m_projection;
	C3DVec				m_lightVec;
	C3DVec				m_lightColor;
	C3DVec				m_ambientColor;

	// デフォルトのカメラパラメータ: 明示的に指定しない限りこの値となる。
	float				m_aspect;
	float				m_nearClip;
	float				m_farClip;
	float				m_angle;


private:

	struct SHADER {
		C3DShader		*	begin;
		C3DShader		*	end;
	};

	SHADER			m_shaders[C3DShader::DP_MAX];

	CGLFBO		*	m_fbo;

public:
	C3DDrawEnv();
	~C3DDrawEnv();

	void setResolution(int width, int height);
	void Render();
	void Clear();

	inline const float getAngle() const { return m_fbo->getAngle(); /* return m_angle; */ }
	inline const float getAspect() const{ return m_fbo->getAspect();	/* return m_aspect; */ }
	inline const float getNearClip() const { return m_nearClip; }
	inline const float getFarClip() const { return m_farClip; }


	inline void setCamera(C3DCameraObj * pCamera) {
		if (m_objCamera) m_objCamera->m_isActive = false;
		m_objCamera = pCamera;
		m_objCamera->m_isActive = true;
		m_objCamera->setVisible(true);
	}

	inline C3DCameraObj * getCamera() { return m_objCamera; }

	// 光源設定
	inline void setLightVec(C3DVec& v) {
		m_lightVec = v.unit();
	}
	void setLightCol(float r, float g, float b, float a);	// 光源色
	void setAmbient(float r, float g, float b, float a);	// 環境光

	inline C3DObj * getRootObj() { return m_objRoot; }

	inline void CalcWorld() { m_objRoot->recalcMatrix(m_rootMatrix, false); }

	// デフォルトシェーダの設定とRootObjectの生成
	bool init(CGLFBO * fbo);
	void setShader(C3DShader::DRAWPHASE phase, C3DShader * shader);
	void removeShader(C3DShader * shader);

	C3DShader * findShader(int sid);
};


#endif // C3DDrawEnv_h
