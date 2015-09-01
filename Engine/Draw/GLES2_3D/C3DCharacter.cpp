#include <memory>
#include <exception>
#include "C3DCharacter.h"

C3DCharacter::C3DCharacter(C3DModel * model)
	: C3DDrawObj(0)
	, m_motion(0)
	, m_color(1.0f, 1.0f, 1.0f, 1.0f)
{
	try {
		useDrawable(model);
		int bonenum = model->getBoneSize();
		m_motion = new C3DQuat[bonenum];
		m_recalcmotion = (u8 *)CGLAlloc::malloc(bonenum);
		for (int i = 0; i < bonenum; i++) m_recalcmotion[i] = 1;	// 最初の一回はすべて計算が必要
		makeMatrices(bonenum);
	}
	catch (std::bad_alloc& ex) {
		delete[] m_motion;
		CGLAlloc::free(m_recalcmotion);
		throw ex;
	}
}

C3DCharacter::~C3DCharacter()
{
	delete[] m_motion;
	CGLAlloc::free(m_recalcmotion);
}

bool
C3DCharacter::calcProcedure(bool recalc)
{
	m_flagUpdate = 0;
	if (!recalc && !m_ismotion) return true;
	m_flagUpdate = 1;	// オブジェクトのroot matrixは再計算されたので第0bitを1にする。

	// モーションを適用し、各関節のマトリクスを生成
	C3DModel * model = getDrawable<C3DModel>();
	if (!model) return true;
	m_recalcmotion[0] = (recalc) ? 2 : 0;
	u32 flag = 1;
	for (int i = 1; i < m_matnum; i++) {
		flag = flag << 1;
		int pidx = model->m_boneparent[i];
		m_recalcmotion[i] &= 1;
		if (1 & (m_recalcmotion[i] | (m_recalcmotion[pidx] >> 1))) {	// 再計算が必要であれば処理する
			C3DMat * parent = m_matrices + pidx;
			C3DVec bpos = model->m_bones[i] - model->m_bones[pidx];
			m_matrices[i] = m_motion[i];
			m_matrices[i] += bpos;	// 親boneに対する相対位置に並行移動する
			m_matrices[i] *= *parent;
			m_recalcmotion[i] = 2;	// 再計算したので、以下自身を親とするボーンは再計算が必要。
			m_flagUpdate |= flag;	// 再計算したので記録する
		}
	}
	// m_flagUpdate は、更新されたmatrixに対応するbitが1になっている。
	// この値は、あたり判定オブジェクトが表示オブジェクトに追随して動くために、
	// 再計算の必要性を判断するため用いられる。	
	m_ismotion = false;
	/*
		シェーダ側では、各ボーンについての計算を、

		(1) モデル原点からのボーン絶対位置を引いて、ボーン起点原点の座標にする
		(2) 上で計算したマトリクスをかける

		のように行う。
		親のいるボーンについては、既に親マトリクスがかかっているので、各マトリクスによる平行移動は、
		親の運動も加味した絶対位置計算となる。
	*/
	return true;
}

void
C3DCharacter::setBoneRotation(int bone, C3DQuat& rot)
{
	if (bone >= m_matnum) return;
	m_motion[bone] = rot;
	m_recalcmotion[bone] = 1;
	m_ismotion = true;
}

void
C3DCharacter::render(C3DShader * pShader)
{
	C3DModel * model = getDrawable<C3DModel>();
	C3DDefaultShader * shader = (C3DDefaultShader *)pShader;
	// 描画の際、全体の頂点色にかけるRGBA値を転送する
	glUniform4fv(shader->m_u_modelcol, 1, (GLfloat *)&m_color);

	// 描画の際に用いる関節のマトリクスを転送する(描画オブジェクトごとに値を転送する)
	glUniformMatrix4fv(shader->m_u_bone, m_matnum, GL_FALSE, (const GLfloat *)m_matrices);	// ボーンのワールドマトリクス

	// 転送済みの頂点とインデックス、およびマトリクス、ボーン位置情報で描画する。
	glDrawElements(GL_TRIANGLE_STRIP, model->m_idxnum, GL_UNSIGNED_SHORT, 0);
}
