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

	// z�o�b�t�@�𖳌��ɂ���
	glDisable(GL_DEPTH_TEST);

	// ���̃t���[���ɂ�����camera->view->projection�s�����ɂ܂Ƃ߂����̂��v���W�F�N�V�����p�ɑ���
	C3DMat proj = m_objCamera->m_cameraInvert * (m_objCamera->m_view * m_projection);

	// �V���̕`��
	// �V���̓J�����𒆐S�Ƃ������ar�̋��̂ŁA�@���͂��ׂē����������Ă���B
	// ��ɃJ�����𒆐S�Ƃ��ĕ`�悳��邽�߁A�J�����͐�΂ɓV���̊O���ɏo�邱�Ƃ͂ł��Ȃ��B
	// �܂��A��ɃJ�����𒆐S�Ƃ��ĕ`�悳��邽�߁A�J�����̈ʒu�͈Ӗ����������A���̌����������`��͈͂Ɋ֌W����B
	if (m_objCelestialSphere) {
		m_celestialShader->useProgram();
		glUniformMatrix4fv(m_celestialShader->m_u_projection, 1, GL_FALSE, (const GLfloat *)&proj);

		// �J�����t�s��𑗂��Ă����B���̉�]�����݂̂�p���ēV������]�����邱�ƂŁA��ɃJ�����𒆐S�Ƃ��ēV������]����
		glUniformMatrix4fv(m_celestialShader->m_u_camera, 1, GL_FALSE, (const GLfloat *)&(m_objCamera->m_cameraInvert));

		m_objCelestialSphere->render(this);	// �V���͑��̃I�u�W�F�N�g�q�G�����L�[�Ƃ͓Ɨ����Ă���B
		glUseProgram(0);
	}


	// �p�m���}(���i)�̕`��
	// �p�m���}�͍��E�̕ӂŃ��[�v����悤�ɍ��ꂽ���i��2D�摜�ł���A
	// �J�����̌����ɂ���ĕ`��ʒu�Ɣ͈́A���������܂�B
	// �V�����l�ɃJ�����̈ʒu�͈Ӗ����������A���i�ɋ߂Â����Ƃ͂ł��Ȃ��B




	// ��������ʏ�I�u�W�F�N�g�̕`��

	// z�o�b�t�@��L���ɂ���
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// �f�t�H���g�V�F�[�_���g�p
	m_shader->useProgram();

	// ���������A�����F�A�����F��ݒ肷��
	glUniform4fv(m_shader->m_u_light, 1, (GLfloat *)&m_lightVec);			// �����x�N�g��
	glUniform4fv(m_shader->m_u_rgba,		1, (GLfloat *)&m_lightColor);		// �����F
	glUniform4fv(m_shader->m_u_ambient,	1, (GLfloat *)&m_ambientColor);	// ���F

	// ���̃t���[���ɂ�����camera->view->projection�s�����ɂ܂Ƃ߂����̂��v���W�F�N�V�����p�ɑ���
	glUniformMatrix4fv(m_shader->m_u_projection, 1, GL_FALSE, (const GLfloat *)&proj);

	// �X�y�L���������̂��߃J�����t�s��𑗂��Ă����B
	glUniformMatrix4fv(m_shader->m_u_camera, 1, GL_FALSE, (const GLfloat *)&(m_objCamera->m_cameraInvert));

	// ���̏����Ŋe�I�u�W�F�N�g�̕`����J�n����B
	// �`��Ώۂ� C3DDrawable �P�ʂŏ�������B
	for (C3DDrawable * pDraw = m_begin; pDraw; pDraw = pDraw->m_sisters.next) {
		pDraw->Render();
	}
}
