#include <string.h>
#include <memory>
#include <exception>
#include "C3DDrawEnv.h"
#include "C3DDrawable.h"
#include "CGLAlloc.h"

#include "C3DCelestialSphere.h"

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
	, m_objCelestialSphere(0)
	, m_objCamera(0)
	, m_shader(0)
	, m_lightVec(1.0f, 0.0f, 0.0f)
	, m_lightColor(1.0f, 1.0f, 1.0f)
	, m_ambientColor(0.0f, 0.0f, 0.0f)
	, m_nearClip(DEFAULT_NEAR)
	, m_farClip(DEFAULT_FAR)
	, m_angle(DEFAULT_ANGLE)
{
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
C3DDrawEnv::init(C3DDefaultShader * pShader, C3DCelestialSphereShader * pCelestial)
{
	m_shader = pShader;
	m_celestialShader = pCelestial;
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
	m_shader = 0;
}


void
C3DDrawEnv::Render()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	if (!m_objCamera) return;

	// zバッファを無効にする
	glDisable(GL_DEPTH_TEST);

	// このフレームにおけるcamera->view->projection行列を一つにまとめたものをプロジェクション用に送る
	C3DMat proj = m_objCamera->m_cameraInvert * (m_objCamera->m_view * m_projection);

	// 天球の描画
	// 天球はカメラを中心とした半径rの球体で、法線はすべて内側を向いている。
	// 常にカメラを中心として描画されるため、カメラは絶対に天球の外側に出ることはできない。
	// また、常にカメラを中心として描画されるため、カメラの位置は意味を持たず、その向きだけが描画範囲に関係する。
	if (m_objCelestialSphere) {
		m_celestialShader->useProgram();
		glUniformMatrix4fv(m_celestialShader->m_u_projection, 1, GL_FALSE, (const GLfloat *)&proj);

		// カメラ逆行列を送っておく。その回転成分のみを用いて天球を回転させることで、常にカメラを中心として天球が回転する
		glUniformMatrix4fv(m_celestialShader->m_u_camera, 1, GL_FALSE, (const GLfloat *)&(m_objCamera->m_cameraInvert));

		m_objCelestialSphere->render(this);	// 天球は他のオブジェクトヒエラルキーとは独立している。
		glUseProgram(0);
	}


	// パノラマ(遠景)の描画
	// パノラマは左右の辺でループするように作られた遠景の2D画像であり、
	// カメラの向きによって描画位置と範囲、高さが決まる。
	// 天球同様にカメラの位置は意味を持たず、遠景に近づくことはできない。




	// ここから通常オブジェクトの描画

	// zバッファを有効にする
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// デフォルトシェーダを使用
	m_shader->useProgram();

	// 光源方向、光源色、環境光色を設定する
	glUniform4fv(m_shader->m_u_light, 1, (GLfloat *)&m_lightVec);			// 光源ベクトル
	glUniform4fv(m_shader->m_u_rgba,		1, (GLfloat *)&m_lightColor);		// 光源色
	glUniform4fv(m_shader->m_u_ambient,	1, (GLfloat *)&m_ambientColor);	// 環境色

	// このフレームにおけるcamera->view->projection行列を一つにまとめたものをプロジェクション用に送る
	glUniformMatrix4fv(m_shader->m_u_projection, 1, GL_FALSE, (const GLfloat *)&proj);

	// スペキュラ処理のためカメラ逆行列を送っておく。
	glUniformMatrix4fv(m_shader->m_u_camera, 1, GL_FALSE, (const GLfloat *)&(m_objCamera->m_cameraInvert));

	// この条件で各オブジェクトの描画を開始する。
	// 描画対象は C3DDrawable 単位で処理する。
	for (C3DDrawable * pDraw = m_begin; pDraw; pDraw = pDraw->m_sisters.next) {
		pDraw->Render();
	}
}
