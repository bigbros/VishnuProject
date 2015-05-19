#ifndef C3DModel_h
#define C3DModel_h

#include <GLES2/gl2.h>

#include "Types3D.h"
#include "C3DDrawable.h"
#include "C3DDrawEnv.h"
#include "C3DMaterial.h"

class C3DCharacter;
class CGLTex;

class C3DModel : public C3DDrawable
{
	friend class C3DCharacter;
protected:
	// ���b�V�����_���B��ő����邱�Ƃ�����̂ŁA�\���̂ɂ��Ă����B
	struct VEC3 {
		float	x;
		float	y;
		float	z;
	};
	struct UV {
		float	u;
		float	v;
	};
	struct VERTEX {
		VEC3	vert;		// ���W(4*3 = 12byte)
		VEC3	norm;		// �@��(4x3 = 12byte)
		VEC3	tang;		// �ڐ�(4x3 = 12byte)
		UV		uv;			// �e�N�X�`��UV�l(8byte)
		u8		bone[4];	// �Q�ƃ{�[����index
		u8		wght[4];	// �e�{�[���̎Q�ƃE�F�C�g
		u8		rgba[4];	// ���_�J���[(4byte)
	};

	/*
		��L�@���Ɛڐ��A����т����̊O�ςɂ���ċ��߂���x�N�g���ɂ���āA
		�e�N�X�`���𒆐S�Ƃ����n�̎���������߂���B
		vertex shader ���Ō����x�N�g�������̌n�ɂ�����x�N�g���ɕϊ����A
		NormalMap �Ɏg�p����B
	*/

	// ���b�V�����
	VERTEX	*	m_vertices;		// ���_
	u16		*	m_indices;		// ���_index
	u16			m_vertnum;		// ���_�̐�
	u16			m_idxnum;		// ���_index�̐�

	// �{�[�����(�ő�64�܂�)
	C3DVec	*	m_bones;		// ���i�̋N�_���(�e���i���_����̑���)
	u8		*	m_boneparent;	// �e���i�̐e��index�Ŏ���([0]�͕K���N�_���e)
	int			m_bonenum;		// ���i�̐�

	// �}�e���A�����
	C3DMaterial * m_material;	// �}�e���A�����

	GLuint		m_idxVert;		// ���_�o�b�t�@
	GLuint		m_idxIndex;		// �C���f�b�N�X�o�b�t�@
	bool		m_ready;		// �o�b�t�@�]���ς�

	const char	*	m_modelName;



public:
	// ���i����Ԃ�
	inline int getBoneSize() throw() { return m_bonenum; }

	C3DModel(C3DShader * shader, const char * modelName = 0);
	virtual ~C3DModel();

	void setup(C3DShader * env);
	void cleanup(C3DShader * env);

	bool recovery();
	bool destruction();

protected:
	bool newVertices(int num = 0);
	bool newIndices(int num = 0);
	bool newBones(int num = 0);
	inline void setMaterial(C3DMaterial * material) { m_material = material; }

	void setBuffer();
};



#endif // C3DModel_h
