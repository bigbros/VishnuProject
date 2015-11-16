#include "C3DCameraObj.h"
#include "C3DDrawEnv.h"

C3DCameraObj::C3DCameraObj()
	: C3DObj()
	, m_isActive(false)
	, m_lookAt(0)
{
	C3DDrawEnv * env = CGLEnv::getInstance().DrawEnv<C3DDrawEnv>(CGLEnv::C3D);
	float aspect = env->getAspect();
	float nearclip = env->getNearClip();
	float farclip = env->getFarClip();
	float angle = env->getAngle();
	setView(angle, aspect, nearclip, farclip);
}

C3DCameraObj::~C3DCameraObj()
{
}

bool
C3DCameraObj::calcProcedure(bool is_recalc)
{
	if (m_isActive && is_recalc) {
		m_cameraInvert = m_matrix.invert();
	}
	return true;
}

void
C3DCameraObj::render(){}

float
C3DCameraObj::angleDiagonal2Horizontal(float d_angle, float width, float height)
{
	d_angle = d_angle / 2.0f;
	float dlen = sqrtf(width*width + height*height);
	float dist = dlen * cosf(d_angle) / sin(d_angle);
	float h_angle = atanf(width / dist) * 2.0f;
	return h_angle;
}

void
C3DCameraObj::setView(float angle, float aspect, float clip_near, float clip_far)
{
	C3DMat mat;
	float r = tanf(angle / 2.0f);

	mat.v[0][0] = -1.0f / r;
	mat.v[1][1] = -1.0f / r;

	mat.v[2][2] = clip_far / (clip_far - clip_near);
	mat.v[3][2] = (clip_near * clip_far) / (clip_far - clip_near);

	mat.v[2][3] = 1.0f;
	mat.v[3][3] = 0.0f;

	m_view = mat;
}


void
C3DCameraObj::lookAtTarget()
{

}
