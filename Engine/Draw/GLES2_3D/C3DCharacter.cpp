#include <memory>
#include <exception>
#include "C3DCharacter.h"

C3DCharacter::C3DCharacter(C3DModel * model)
	: C3DDrawObj()
	, m_motion(0)
	, m_color(1.0f, 1.0f, 1.0f, 1.0f)
{
	try {
		useDrawable(model);
		int bonenum = model->getBoneSize();
		m_bonenum = bonenum;
		m_motion = new C3DQuat[bonenum];
		m_bonerot = new C3DQuat[bonenum];
		m_bonepos = new VEC3[bonenum];
		m_recalcmotion = (u8 *)CGLAlloc::malloc(bonenum);
		for (int i = 0; i < bonenum; i++) m_recalcmotion[i] = 1;	// �ŏ��̈��͂��ׂČv�Z���K�v
	}
	catch (std::bad_alloc& ex) {
		delete[] m_motion;
		CGLAlloc::free(m_recalcmotion);
		throw ex;
	}
}

C3DCharacter::~C3DCharacter()
{
	delete[] m_bonepos;
	delete[] m_bonerot;
	delete[] m_motion;
	CGLAlloc::free(m_recalcmotion);
}

bool
C3DCharacter::calcProcedure(bool recalc)
{
	m_flagUpdate = 0;
	if (!recalc && !m_ismotion) return true;
	m_flagUpdate = 1;	// �I�u�W�F�N�g��root matrix�͍Čv�Z���ꂽ�̂ő�0bit��1�ɂ���B

	// ���[�V������K�p���A�e�֐߂̃}�g���N�X�𐶐�
	C3DModel * model = getDrawable<C3DModel>();
	if (!model) return true;
	if (m_recalcmotion[0] & 1) {
		m_bonerot[0] = m_motion[0];
		m_bonepos[0].x = 0.0f;
		m_bonepos[0].y = 0.0f;
		m_bonepos[0].z = 0.0f;
		m_recalcmotion[0] = 2;
	}
	u32 flag = 1;
	for (int i = 1; i < m_bonenum; i++) {
		flag = flag << 1;
		int pidx = model->m_boneparent[i];
		m_recalcmotion[i] &= 1;
		if (1 & (m_recalcmotion[i] | (m_recalcmotion[pidx] >> 1))) {	// �Čv�Z���K�v�ł���Ώ�������

			C3DQuat * parent = m_bonerot + pidx;
			C3DVec bpos = model->m_bones[i] - model->m_bones[pidx];

			// �֐ߌ��_�ɐe�O���[�o����]�������A�e�O���[�o���ʒu�����Z����B
			bpos.z = 0.0f;
			C3DQuat r = parent->conj();
			bpos = r * bpos;
			bpos = bpos * (*parent);
			m_bonepos[i].x = bpos.x + m_bonepos[pidx].x;
			m_bonepos[i].y = bpos.y + m_bonepos[pidx].y;
			m_bonepos[i].z = bpos.z + m_bonepos[pidx].z;

			// ���[�J����]�ɐe�O���[�o����]��������B
			m_bonerot[i] = m_motion[i] * (*parent);

			m_recalcmotion[i] = 2;	// �Čv�Z�����̂ŁA�ȉ����g��e�Ƃ���{�[���͍Čv�Z���K�v�B
			m_flagUpdate |= flag;	// �Čv�Z�����̂ŋL�^����
		}
	}
	// m_flagUpdate �́A�X�V���ꂽmatrix�ɑΉ�����bit��1�ɂȂ��Ă���B
	// ���̒l�́A�����蔻��I�u�W�F�N�g���\���I�u�W�F�N�g�ɒǐ����ē������߂ɁA
	// �Čv�Z�̕K�v���𔻒f���邽�ߗp������B	
	m_ismotion = false;
	/*
		�V�F�[�_���ł́A�e�{�[���ɂ��Ă̌v�Z���A

		(1) ���f�����_����̃{�[����Έʒu�������āA�{�[���N�_���_�̍��W�ɂ���
		(2) ��Ōv�Z�����}�g���N�X��������

		�̂悤�ɍs���B
		�e�̂���{�[���ɂ��ẮA���ɐe�}�g���N�X���������Ă���̂ŁA�e�}�g���N�X�ɂ�镽�s�ړ��́A
		�e�̉^��������������Έʒu�v�Z�ƂȂ�B
	*/
	return true;
}

void
C3DCharacter::setBoneRotation(int bone, C3DQuat& rot)
{
	if (bone >= m_bonenum) return;
	m_motion[bone] = rot;
	m_recalcmotion[bone] = 1;
	m_ismotion = true;
}

void
C3DCharacter::render(C3DShader * pShader)
{
	C3DModel * model = getDrawable<C3DModel>();
	C3DDefaultShader * shader = (C3DDefaultShader *)pShader;
	// �`��̍ہA�S�̂̒��_�F�ɂ�����RGBA�l��]������
	glUniform4fv(shader->m_u_modelcol, 1, (GLfloat *)&m_color);

	// �`��̍ۂɗp����֐߂̃p�����[�^��]������(�`��I�u�W�F�N�g���Ƃɒl��]������)
	glUniform4fv(shader->m_u_bone, m_bonenum, (const GLfloat *)m_bonerot);
	glUniform3fv(shader->m_u_bonepos, m_bonenum, (const GLfloat *)m_bonepos);
	glUniformMatrix4fv(shader->m_u_matrix, 1, GL_FALSE, (const GLfloat *)&m_matrix);


	// �]���ς݂̒��_�ƃC���f�b�N�X�A����у}�g���N�X�A�{�[���ʒu���ŕ`�悷��B
	model->draw(shader);
//	glDrawElements(GL_TRIANGLES, model->m_idxnum, GL_UNSIGNED_SHORT, 0);
}
