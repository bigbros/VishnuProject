#include <stdio.h>
#include <string.h>
#include <memory>
#include <exception>
#include <GLES2/gl2.h>
#include "C3DModel.h"

#include "CGLTex.h"

C3DModel::C3DModel(C3DShader * shader, const char * modelName)
	: C3DDrawable(shader)
	, m_vertices(0)
	, m_indices(0)
	, m_vertnum(0)
	, m_idxnum(0)
	, m_bones(0)
	, m_boneparent(0)
	, m_bonenum(0)
	, m_modelName(0)
	, m_matBegin(0)
	, m_matEnd(0)
	, m_ready(false)
{
	char * buf = 0;
	try {
		int len = strlen(modelName);
		char * buf = new char[len + 1];
		strcpy(buf, modelName);
		m_modelName = buf;
	}
	catch (std::bad_alloc& ex) {
		delete[] buf;
		throw ex;
	}
}

C3DModel::~C3DModel()
{
	destruction();
	delete[] m_modelName;
	delete[] m_vertices;
	delete[] m_indices;
	delete[] m_bones;
	delete[] m_boneparent;
	C3DMaterial * mat = m_matBegin;
	while (mat) {
		C3DMaterial * nxt = mat->m_next;
		delete mat;
		mat = nxt;
	}
}

bool
C3DModel::newVertices(int num)
{
	try {
		VERTEX * p = new VERTEX[num];
		delete[] m_vertices;
		m_vertices = p;
		m_vertnum = num;
		return true;
	}
	catch (std::bad_alloc&) {
		return false;
	}
}

bool
C3DModel::newIndices(int num)
{
	try {
		INDEXTYPE * p = new INDEXTYPE[num];
		delete[] m_indices;
		m_indices = p;
		m_idxnum = num;
		return true;
	}
	catch (std::bad_alloc&) {
		return false;
	}
}
bool
C3DModel::newBones(int num)
{
	C3DVec * bones = 0;
	u8 * parents = 0;
	try{
		bones = new C3DVec[num];
		parents = new u8[num];
		delete[] m_bones;
		delete[] m_boneparent;
		m_bones = bones;
		m_boneparent = parents;
		m_bonenum = num;
		return true;
	}
	catch (std::bad_alloc&) {
		delete[] bones;
		delete[] parents;
		return false;
	}
}
void
C3DModel::setBuffer()
{
	GLuint bufIdx[2];
	glGenBuffers(2, bufIdx);

	m_idxVert = bufIdx[0];
	glBindBuffer(GL_ARRAY_BUFFER, m_idxVert);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VERTEX) * m_vertnum, m_vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	m_idxIndex = bufIdx[1];
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_idxIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INDEXTYPE) * m_idxnum, m_indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	m_ready = true;
}

void
C3DModel::setup(C3DShader * pShader)
{
	C3DDefaultShader * shader = (C3DDefaultShader *)pShader;

	// このモデルの全頂点情報(座標、法線、uv値、頂点カラー、追従ボーンおよびウェイト値)が転送済みのバッファを有効にし、
	// そのバッファにインターリーヴされた各情報のオフセットを指定する。
	glBindBuffer(GL_ARRAY_BUFFER, m_idxVert);
	glVertexAttribPointer(shader->m_a_vert, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX), (const GLvoid *)(char *)offsetof(VERTEX, vert));
	glVertexAttribPointer(shader->m_a_norm, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX), (const GLvoid *)(char *)offsetof(VERTEX, norm));
	glVertexAttribPointer(shader->m_a_tang, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX), (const GLvoid *)(char *)offsetof(VERTEX, tang));
	glVertexAttribPointer(shader->m_a_uv, 2, GL_FLOAT, GL_FALSE, sizeof(VERTEX), (const GLvoid *)(char *)offsetof(VERTEX, uv));

	glVertexAttribPointer(shader->m_a_bone, 4, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(VERTEX), (const GLvoid *)(char *)offsetof(VERTEX, bone));
	glVertexAttribPointer(shader->m_a_wght, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(VERTEX), (const GLvoid *)(char *)offsetof(VERTEX, wght));
	glVertexAttribPointer(shader->m_a_rgba, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(VERTEX), (const GLvoid *)(char *)offsetof(VERTEX, rgba));

	// インデックスバッファを有効にする
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_idxIndex);

	glUniform4fv(shader->m_u_bonepos, m_bonenum, (const GLfloat *)m_bones);	// モーションを与えない状態でのボーン起点

	// ここまでの処理は、同じモデルを用いるすべてのオブジェクト描画直前に1回だけやれば良い。
}

void
C3DModel::cleanup(C3DShader * env)
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

bool
C3DModel::recovery()
{
	setBuffer();
	return true;
}

bool
C3DModel::destruction()
{
	if (m_ready) {
		GLuint bufIdx[2];
		bufIdx[0] = m_idxVert;
		bufIdx[1] = m_idxIndex;
		glDeleteBuffers(2, bufIdx);
		m_ready = false;
	}
	return true;
}

void
C3DModel::setMaterial(C3DMaterial * material)
{
	material->m_prev = m_matEnd;
	if (material->m_prev) {
		material->m_prev->m_next = material;
	}
	else {
		m_matBegin = material;
	}
	m_matEnd = material;
}

void
C3DModel::draw(C3DShader * shader)
{
	// モデルに使用されている全マテリアルでの描画を行う。
	for (C3DMaterial * mat = m_matBegin; mat; mat = mat->m_next) {
		mat->setup((C3DDefaultShader *)shader);
		glDrawElements(GL_TRIANGLES, mat->params.indicesNum, idxType, (void *)(mat->params.indicesBegin * sizeof(INDEXTYPE)));
	}	
}