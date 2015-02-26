#include "C3DCelestialSphere.h"
#include "CGLTex.h"

C3DCelestialModel::C3DCelestialModel(float r, const char * modelName)
	: C3DDrawable()
	, m_vertices(0)
	, m_indices(0)
	, m_vertnum(0)
	, m_idxnum(0)
	, m_modelName(0)
	, m_texture(0)
	, m_ready(false)
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

C3DCelestialModel::~C3DCelestialModel()
{
	destruction();
	delete[] m_modelName;
	delete[] m_vertices;
	delete[] m_indices;
	delete m_texture;
}

void
C3DCelestialModel::setup(C3DDrawEnv * env)
{
	C3DCelestialSphereShader * shader = 0;	// �V���p�V�F�[�_���ǂ��Ɏ������悤���v�Ē��c

	// �e�N�X�`�����ݒ肳��Ă���΁A���̃e�N�X�`����L���ɂ���B
	if (m_texture) {
		m_texture->set(GL_TEXTURE0);
		glUniform1i(shader->m_u_tex, 0);
	}

	// ���̃��f���̑S���_���(���W�A�@���Auv�l�A���_�J���[�A�Ǐ]�{�[������уE�F�C�g�l)���]���ς݂̃o�b�t�@��L���ɂ��A
	// ���̃o�b�t�@�ɃC���^�[���[�����ꂽ�e���̃I�t�Z�b�g���w�肷��B
	glBindBuffer(GL_ARRAY_BUFFER, m_idxVert);
	glVertexAttribPointer(shader->m_a_vert, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX), (const GLvoid *)(char *)offsetof(VERTEX, vert));
	glVertexAttribPointer(shader->m_a_uv, 2, GL_FLOAT, GL_FALSE, sizeof(VERTEX), (const GLvoid *)(char *)offsetof(VERTEX, uv));

	// �C���f�b�N�X�o�b�t�@��L���ɂ���
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_idxIndex);

	// �����܂ł̏����́A�������f����p���邷�ׂẴI�u�W�F�N�g�`�撼�O��1�񂾂����Ηǂ��B

}

void
C3DCelestialModel::cleanup(C3DDrawEnv * env)
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

bool
C3DCelestialModel::recovery()
{
	setBuffer();
	return true;
}

bool
C3DCelestialModel::destruction()
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
C3DCelestialModel::newVertices(int num)
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
C3DCelestialModel::newIndices(int num)
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
C3DCelestialModel::setBuffer()
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

// ���_���X�g����уC���f�b�N�X���X�g�����B
void
C3DCelestialModel::createVertex()
{
	// ���_�̍쐬
	newVertices((V_RESO + 1) * (H_RESO + 1));
	for (int v = 0; v <= V_RESO; v++) {
		float z = cosf(F_PI * v / (2.0f * H_RESO));
		for (int h = 0; h <= H_RESO; h++) {
			float theta = 2.0f * F_PI * h / H_RESO;
			float x = cosf(theta);
			float y = sinf(theta);

			VERTEX * vert = m_vertices + v * (H_RESO + 1) + h;
			vert->vert.x = x;
			vert->vert.y = y;
			vert->vert.z = z;
			vert->uv.u = (float)h / (float)H_RESO;
			vert->uv.v = (float)v / (float)V_RESO;
		}
	}

	// �C���f�b�N�X�̍쐬
	newIndices((V_RESO + 1) * (H_RESO + 1));
	int idx = 0;
	for (int v = 0; v < V_RESO; v++) {
		m_indices[idx++] = H_RESO * v;
		m_indices[idx++] = H_RESO * (v + 1);
		for (int h = 1; h <= H_RESO; h++) {
			m_indices[idx++] = H_RESO * v + h;
			m_indices[idx++] = H_RESO * (v + 1) + h;
		}
	}
}