#ifndef C3DCelestialSphere_h
#define C3DCelestialSphere_h

#include "C3DMaterial.h"
#include "C3DDrawable.h"

class C3DCelestialModel : public C3DDrawable
{
private:
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
		UV		uv;			// �e�N�X�`��UV�l(8byte)
	};

	// ���b�V�����
	VERTEX	*	m_vertices;		// ���_
	u16		*	m_indices;		// ���_index
	u16			m_vertnum;		// ���_�̐�
	u16			m_idxnum;		// ���_index�̐�

	// �}�e���A�����
	C3DMaterial * m_material;	// �}�e���A�����

	GLuint		m_idxVert;		// ���_�o�b�t�@
	GLuint		m_idxIndex;		// �C���f�b�N�X�o�b�t�@
	bool		m_ready;		// �o�b�t�@�]���ς�

	const char	*	m_modelName;



public:

	C3DCelestialModel(float r, const char * modelName = 0);
	virtual ~C3DCelestialModel();

	void setup(C3DDrawEnv * env);
	void cleanup(C3DDrawEnv * env);

	bool recovery();
	bool destruction();

protected:
	bool newVertices(int num = 0);
	bool newIndices(int num = 0);
	inline void setMaterial(C3DMaterial * material) { m_material = material; }

	void setBuffer();

};

class C3DCelestialSphere : public C3DDrawObj
{
private:
	C3DCelestialModel	*	m_model;
public:
	C3DCelestialSphere();
	virtual ~C3DCelestialSphere();

protected:
	// ���̃I�u�W�F�N�g�̃}�g���N�X�v�Z��ɍs���������L�q
	bool calcProcedure(bool is_recalc);
};

#endif // C3DCelestialSphere_h
