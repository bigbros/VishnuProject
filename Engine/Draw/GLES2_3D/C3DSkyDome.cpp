#include "C3DDrawEnv.h"
#include "C3DSkyDome.h"
#include "CGLTex.h"

C3DSkyDomeModel::C3DSkyDomeModel(C3DSkyDomeShader * shader, float r, CGLTex * pTex, const char * modelName, int h_reso, int v_reso)
	: C3DDrawable(shader)
	, m_vertices(0)
	, m_indices(0)
	, m_vertnum(0)
	, m_idxnum(0)
	, m_modelName(0)
	, m_texture(pTex)
	, m_ready(false)
	, m_r(r)
	, m_reso_v(v_reso)
	, m_reso_h(h_reso)
{
	char * buf = 0;
	try {
		int len = strlen(modelName);
		char * buf = new char[len + 1];
		strcpy(buf, modelName);
		m_modelName = buf;
		createVertex();
	}
	catch (std::bad_alloc& ex) {
		delete[] buf;
		throw ex;
	}
}

C3DSkyDomeModel::~C3DSkyDomeModel()
{
	destruction();
	delete[] m_modelName;
	delete[] m_vertices;
	delete[] m_indices;
}

void
C3DSkyDomeModel::setup(C3DShader * pShader)
{
	C3DSkyDomeShader * shader = (C3DSkyDomeShader *)pShader;	// 天球用シェーダをどこに持たせようか思案中…

	shader->useProgram();

	// テクスチャが設定されていれば、そのテクスチャを有効にする。
	if (m_texture) {
		m_texture->set(GL_TEXTURE0);
		glUniform1i(shader->m_u_tex, 0);
	}

	// このモデルの全頂点情報(座標、法線、uv値、頂点カラー、追従ボーンおよびウェイト値)が転送済みのバッファを有効にし、
	// そのバッファにインターリーヴされた各情報のオフセットを指定する。
	glBindBuffer(GL_ARRAY_BUFFER, m_idxVert);
	glVertexAttribPointer(shader->m_a_vert, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX), (const GLvoid *)(char *)offsetof(VERTEX, vert));
	glVertexAttribPointer(shader->m_a_uv, 2, GL_FLOAT, GL_FALSE, sizeof(VERTEX), (const GLvoid *)(char *)offsetof(VERTEX, uv));

	// インデックスバッファを有効にする
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_idxIndex);
}

void
C3DSkyDomeModel::cleanup(C3DShader * pShader)
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

bool
C3DSkyDomeModel::recovery()
{
	setBuffer();
	return true;
}

bool
C3DSkyDomeModel::destruction()
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


bool
C3DSkyDomeModel::newVertices(int num)
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
C3DSkyDomeModel::newIndices(int num)
{
	try {
		u16 * p = new u16[num];
		delete[] m_indices;
		m_indices = p;
		m_idxnum = num;
		return true;
	}
	catch (std::bad_alloc&) {
		return false;
	}
}

void
C3DSkyDomeModel::setBuffer()
{
	GLuint bufIdx[2];
	glGenBuffers(2, bufIdx);

	m_idxVert = bufIdx[0];
	glBindBuffer(GL_ARRAY_BUFFER, m_idxVert);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VERTEX) * m_vertnum, m_vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	m_idxIndex = bufIdx[1];
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_idxIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u16) * m_idxnum, m_indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	m_ready = true;
}

// 頂点リストおよびインデックスリストを作る。
void
C3DSkyDomeModel::createVertex()
{
	// 頂点およびインデックス領域の生成
	newVertices((m_reso_v + 1) * (m_reso_h + 1));
	newIndices((m_reso_v) * (m_reso_h + 1) * 2);


	for (int i = 0; i <= m_reso_v; i++) {
		float t = (F_PI * i) / (2.0f * m_reso_v);
		float y = m_r * sinf(t) - 10.0f;
		float r = m_r * cosf(t);

		float tr = 0.5f * (float)(m_reso_v - i) / (float)m_reso_v;

		for (int j = 0; j <= m_reso_h; j++) {
			float theta = 2.0f * F_PI * j / m_reso_h;
			float cos_h = cosf(theta);
			float sin_h = sinf(theta);
			float x = r * cos_h;
			float z = r * sin_h;

			float u = 0.5f + tr * cos_h;
			float v = 0.5f + tr * sin_h;

			int idx = (i * (m_reso_h + 1) + j);
			if (idx >= m_vertnum) {
				LOG("*******BAD INDEX*********");
			}
			VERTEX * vertex = m_vertices + idx;
			vertex->vert.x = x;
			vertex->vert.y = y;
			vertex->vert.z = z;
			vertex->uv.u = u;
			vertex->uv.v = v;
		}
	}

	int idx = 0;
	for (int i = 0; i < m_reso_v; i++) {
		for (int j = 0; j <= m_reso_h; j++) {
			int base = i * (m_reso_h + 1) + j;
			if (base >= m_vertnum || idx >= m_idxnum) {
				LOG("*******BAD INDEX*********");
			}
			m_indices[idx++] = base;
			m_indices[idx++] = base + m_reso_h + 1;
		}
	}
}
C3DSkyDome::C3DSkyDome(C3DSkyDomeShader * shader, CGLTex * pTex)
	: C3DDrawObj()
	, m_model(0)
	, m_tex(pTex)
	, m_color(1.0f, 1.0f, 1.0f, 1.0f)
{
	try {
		m_model = new C3DSkyDomeModel(shader, 200.0f, m_tex, "celestial_sphere");
		m_model->setBuffer();
		useDrawable(m_model);
		
	} catch (std::bad_alloc& ex) {
		delete m_model;
		throw ex;
	}
}

C3DSkyDome::~C3DSkyDome()
{
	delete m_model;
}

void
C3DSkyDome::render(C3DShader * pShader)
{
	C3DSkyDomeModel * model = getDrawable<C3DSkyDomeModel>();
	C3DSkyDomeShader * shader = (C3DSkyDomeShader *)pShader;

	model->setup(shader);

	// 描画の際、全体の頂点色にかけるRGBA値を転送する
	glUniform4fv(shader->m_u_rgba, 1, (GLfloat *)&m_color);

	// 転送済みの頂点とインデックス、マトリクスで描画する。
	glDrawElements(GL_TRIANGLE_STRIP, model->m_idxnum, GL_UNSIGNED_SHORT, 0);
}

bool
C3DSkyDome::calcProcedure(bool is_recalc)
{
	return true;
}