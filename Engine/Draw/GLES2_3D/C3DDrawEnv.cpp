#include <string.h>
#include <memory>
#include <exception>
#include "C3DDrawEnv.h"
#include "C3DDrawable.h"
#include "CGLAlloc.h"

#include "C3DShader.h"

#define DEFAULT_NEAR	20.0f
#define DEFAULT_FAR		300.0f
#define DEFAULT_ANGLE	(F_PI / 2.0f)


C3DRootObj::C3DRootObj() : C3DObj() {
	setVisible(true);
}
C3DRootObj::~C3DRootObj() {}
bool C3DRootObj::calcProcedure(bool is_recalc) { return true; }
void C3DRootObj::render() {}

void
C3DRootObj::destroyChildren()
{
	C3DObj * child = m_childs.begin;
	while (child) {
		C3DObj * next = child->m_sisters.next;
		delete child;
		child = next;
	}
}

C3DDrawEnv::C3DDrawEnv()
	: IGLDrawEnv()
	, m_objRoot(0)
	, m_objCamera(0)
	, m_lightVec(1.0f, 0.0f, 0.0f)
	, m_lightColor(1.0f, 1.0f, 1.0f)
	, m_ambientColor(0.0f, 0.0f, 0.0f)
	, m_nearClip(DEFAULT_NEAR)
	, m_farClip(DEFAULT_FAR)
	, m_angle(DEFAULT_ANGLE)
{
	for (int i = 0; i < C3DShader::DP_MAX; i++) {
		m_shaders[i].begin = 0;
		m_shaders[i].end = 0;
	}
}

C3DDrawEnv::~C3DDrawEnv()
{
	Clear();
}

void
C3DDrawEnv::setLightCol(float r, float g, float b, float a)
{
	m_lightColor.set(r, g, b, a);
}

void
C3DDrawEnv::setAmbient(float r, float g, float b, float a)
{
	m_ambientColor.set(r, g, b, a);
}

bool
C3DDrawEnv::init()
{
	if (!m_objRoot) m_objRoot = new C3DRootObj();
	return true;
}

void
C3DDrawEnv::setResolution(int width, int height)
{
	m_aspect = 1.0f;
	m_projection.v[0][0] = (width > height) ? 1.0f : ((float)height / (float)width);
	m_projection.v[1][1] = (width < height) ? 1.0f : ((float)width / (float)height);
}

void
C3DDrawEnv::Clear()
{
	if(m_objRoot) m_objRoot->destroyChildren();
	delete m_objRoot;
	m_objRoot = 0;

	for (int i = 0; i < C3DShader::DP_MAX; i++) {
		C3DShader * shader = m_shaders[i].begin;
		while (shader) {
			C3DShader * nxt = shader->m_sisters.next;
			delete shader;
			shader = nxt;
		}
		m_shaders[i].begin = 0;
		m_shaders[i].end = 0;
	}
}


void
C3DDrawEnv::Render()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	if (!m_objCamera) return;

	// このフレームにおけるcamera->view->projection行列を一つにまとめたものをプロジェクション用に送る
	C3DMat proj = m_objCamera->m_cameraInvert * (m_objCamera->m_view * m_projection);

	// ここから通常オブジェクトの描画
	for (int i = 0; i < C3DShader::DP_MAX; i++) {
		for (C3DShader * pShader = m_shaders[i].begin; pShader; pShader = pShader->m_sisters.next) {
			pShader->useProgram();
			pShader->preConfig(&m_lightVec, &m_lightColor, &m_ambientColor, &proj, &(m_objCamera->m_cameraInvert));
			pShader->render();
		}
	}
}

void
C3DDrawEnv::setShader(C3DShader::DRAWPHASE phase, C3DShader * shader)
{
	shader->m_sisters.prev = m_shaders[phase].end;
	if (!shader->m_sisters.prev) {
		m_shaders[phase].begin = shader;
	}
	else {
		shader->m_sisters.prev->m_sisters.next = shader;
	}
	m_shaders[phase].end = shader;
}

void
C3DDrawEnv::removeShader(C3DShader * shader)
{
	C3DShader::DRAWPHASE phase = shader->m_phase;
	if (shader->m_sisters.prev) {
		shader->m_sisters.prev->m_sisters.next = shader->m_sisters.next;
	}
	else {
		m_shaders[phase].begin = shader->m_sisters.next;
	}
	if (shader->m_sisters.next) {
		shader->m_sisters.next->m_sisters.prev = shader->m_sisters.prev;
	}
	else {
		m_shaders[phase].end = shader->m_sisters.prev;
	}
}

C3DShader *
C3DDrawEnv::findShader(int sid)
{
	for (int i = 0; i < C3DShader::DP_MAX; i++) {
		for (C3DShader * shader = m_shaders[i].begin; shader; shader = shader->m_sisters.next) {
			if (shader->m_sid == sid) return shader;
		}
	}
	return 0;
}