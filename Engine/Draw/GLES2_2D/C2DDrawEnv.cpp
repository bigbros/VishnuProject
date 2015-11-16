#include "C2DDrawEnv.h"

C2DRootObj::C2DRootObj() : C2DObj() {
	setVisible(true);
}
C2DRootObj::~C2DRootObj() {}
bool C2DRootObj::calcProcedure(bool is_recalc) { return true; }
void C2DRootObj::render() {}

void
C2DRootObj::destroyChildren()
{
	C2DObj * child = m_childs.begin;
	while (child) {
		C2DObj * next = child->m_sisters.next;
		delete child;
		child = next;
	}
}



C2DDrawEnv::TEXLIST::TEXLIST() : begin(0), end(0) {}

C2DDrawEnv::C2DDrawEnv()
	: IGLDrawEnv()
	, m_rootMatrix()
	, m_objRoot(0)
	, m_projection()
	, m_shader(0)
	, m_drawQueue(0)
{

}

C2DDrawEnv::~C2DDrawEnv()
{
	Clear();
}

bool
C2DDrawEnv::init(C2DDefaultShader * pShader)
{
	m_shader = pShader;
	if (!m_objRoot) m_objRoot = new C2DRootObj();
	if (!m_drawQueue) m_drawQueue = new C2DDrawQueue(m_shader);
	return true;
}

// 2D描画系は、左下原点の1.0 = 1pixel座標系で設定する。
void
C2DDrawEnv::setResolution(int width, int height)
{
	m_projection.v[0][0] = 2.0f / (float)width;
	m_projection.v[1][1] = -2.0f / (float)height;
	m_projection.v[2][2] = 1.0f;
	m_projection.v[3][0] = -1.0f;
	m_projection.v[3][1] = 1.0f;
}

void
C2DDrawEnv::Clear()
{
	C2DTexOT * ot = m_texlist.begin;
	while (ot) {
		C2DTexOT * nx = ot->m_chain.next;
		delete ot;
		ot = nx;
	}
	m_objRoot->destroyChildren();
	delete m_objRoot;
	delete m_drawQueue;
	m_objRoot = 0;
	m_shader = 0;
}

C2DTexOT *
C2DDrawEnv::getTexOT(CGLTex * pTex)
{
	// 既存のものがある場合はそれを返す。
	for (C2DTexOT * pOT = m_texlist.begin; pOT; pOT = pOT->m_chain.next) if (pOT->isTex(pTex)) return pOT;

	// 既存のものがなければ新規に作成する。
	return new C2DTexOT(pTex);
}

void
C2DDrawEnv::Render()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	// zバッファを無効にする
	glDisable(GL_DEPTH_TEST);

	// デフォルトシェーダを使用
	m_shader->useProgram();

	glUniformMatrix4fv(m_shader->m_u_projection, 1, GL_FALSE, (const GLfloat *)&m_projection);


	// この条件で各オブジェクトの描画を開始する。
	// 描画対象は各プライオリティごと、C2DTexOT単位で処理する。
	m_drawQueue->bufferEnable(m_shader);
	m_drawQueue->drawInit();
	for (int i = 0; i < C2DTexOT::C2D_MAX_PRIORITY; i++) {
		for (C2DTexOT * pOT = m_texlist.begin; pOT; pOT = pOT->m_chain.next) pOT->render(m_drawQueue, i);
	}
}
