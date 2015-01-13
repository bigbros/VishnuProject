#ifndef C3DDrawEnv_h
#define C3DDrawEnv_h

#include "Types3D.h"
#include "dtypes.h"
#include "C3DObj.h"
#include "CGLObj.h"
#include "C3DCameraObj.h"
#include "C3DDefaultShader.h"
#include "CGLEnv.h"

class C3DDrawEnv;
class C3DDrawable;

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
	C3DMat				m_rootMatrix;
	C3DRootObj		*	m_objRoot;

	C3DCameraObj	*	m_objCamera;
	C3DMat				m_projection;
	C3DVec				m_lightVec;
	C3DVec				m_lightColor;
	C3DVec				m_ambientColor;

	// �f�t�H���g�̃J�����p�����[�^: �����I�Ɏw�肵�Ȃ����肱�̒l�ƂȂ�B
	float				m_aspect;
	float				m_nearClip;
	float				m_farClip;
	float				m_angle;

	// �f�t�H���g�ŗp���� shader object
	C3DDefaultShader	*	m_shader;

	// C3DDrawable�̃��X�g
	C3DDrawable			*	m_begin;
	C3DDrawable			*	m_end;
public:
	C3DDrawEnv();
	~C3DDrawEnv();

	void setResolution(int width, int height);
	void Render();
	void Clear();

	inline const float getAngle() const { return m_angle;  }
	inline const float getAspect() const{ return m_aspect; }
	inline const float getNearClip() const { return m_nearClip; }
	inline const float getFarClip() const { return m_farClip; }


	inline void setCamera(C3DCameraObj * pCamera) {
		if (m_objCamera) m_objCamera->m_isActive = false;
		m_objCamera = pCamera;
		m_objCamera->m_isActive = true;
		m_objCamera->setVisible(true);
	}

	// �����ݒ�
	inline void setLightVec(C3DVec& v) {
		m_lightVec = v.unit();
	}
	void setLightCol(float r, float g, float b, float a);	// �����F
	void setAmbient(float r, float g, float b, float a);	// ����

	inline C3DObj * getRootObj() { return m_objRoot; }

	inline void CalcWorld() { m_objRoot->recalcMatrix(&m_rootMatrix, false); }

	// �f�t�H���g�V�F�[�_�̐ݒ��RootObject�̐���
	bool init(C3DDefaultShader * pShader);
	inline C3DDefaultShader * getDefaultShader() { return m_shader; }

};


#endif // C3DDrawEnv_h
