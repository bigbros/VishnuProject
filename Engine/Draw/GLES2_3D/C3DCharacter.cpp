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
		for (int i = 0; i < bonenum; i++) m_recalcmotion[i] = 1;	// �ŏ��̈��͂��ׂČv�Z���K�v
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
	m_flagUpdate = 1;	// �I�u�W�F�N�g��root matrix�͍Čv�Z���ꂽ�̂ő�0bit��1�ɂ���B

	// ���[�V������K�p���A�e�֐߂̃}�g���N�X�𐶐�
	C3DModel * model = getDrawable<C3DModel>();
	if (!model) return true;
	m_recalcmotion[0] = (recalc) ? 2 : 0;
	u32 flag = 1;
	for (int i = 1; i < m_matnum; i++) {
		flag = flag << 1;
		int pidx = model->m_boneparent[i];
		m_recalcmotion[i] &= 1;
		if (1 & (m_recalcmotion[i] | (m_recalcmotion[pidx] >> 1))) {	// �Čv�Z���K�v�ł���Ώ�������
			C3DMat * parent = m_matrices + pidx;
			C3DVec bpos = model->m_bones[i] - model->m_bones[pidx];
			m_matrices[i] = m_motion[i];
			m_matrices[i] += bpos;	// �ebone�ɑ΂��鑊�Έʒu�ɕ��s�ړ�����
			m_matrices[i] *= *parent;
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
	// �`��̍ہA�S�̂̒��_�F�ɂ�����RGBA�l��]������
	glUniform4fv(shader->m_u_modelcol, 1, (GLfloat *)&m_color);

	// �`��̍ۂɗp����֐߂̃}�g���N�X��]������(�`��I�u�W�F�N�g���Ƃɒl��]������)
	glUniformMatrix4fv(shader->m_u_bone, m_matnum, GL_FALSE, (const GLfloat *)m_matrices);	// �{�[���̃��[���h�}�g���N�X

	// �]���ς݂̒��_�ƃC���f�b�N�X�A����у}�g���N�X�A�{�[���ʒu���ŕ`�悷��B
	glDrawElements(GL_TRIANGLE_STRIP, model->m_idxnum, GL_UNSIGNED_SHORT, 0);
}
