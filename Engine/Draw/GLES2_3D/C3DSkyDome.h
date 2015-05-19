#ifndef C3DSkyDome_h
#define C3DSkyDome_h

#include "C3DMaterial.h"
#include "C3DDrawable.h"

#include "C3DSkyDomeShader.h"

class C3DSkyDome;

class C3DSkyDomeModel : public C3DDrawable
{
	friend class C3DSkyDome;
private:
	enum {
		V_RESO		=	12,
		H_RESO		=	24
	};
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
	CGLTex	*	m_texture;	// �V���e�N�X�`���͒��ڎw�肷��

	GLuint		m_idxVert;		// ���_�o�b�t�@
	GLuint		m_idxIndex;		// �C���f�b�N�X�o�b�t�@
	bool		m_ready;		// �o�b�t�@�]���ς�

	int			m_reso_v;		// �ܓx����������
	int			m_reso_h;		// �o�x����������

	float		m_r;			// ���a(�֋X��)

	const char	*	m_modelName;



public:

	C3DSkyDomeModel(C3DSkyDomeShader * shader, float r, CGLTex * pTex, const char * modelName = 0, int h_reso = H_RESO, int v_reso = V_RESO);
	virtual ~C3DSkyDomeModel();

	void setup(C3DShader * pShader);
	void cleanup(C3DShader * pShader);

	bool recovery();
	bool destruction();

protected:
	bool newVertices(int num = 0);
	bool newIndices(int num = 0);
	inline void setTexture(CGLTex * texture) { m_texture = texture; }

	void setBuffer();

	void createVertex();

};

class C3DSkyDome : public C3DDrawObj
{
private:
	C3DSkyDomeModel		*	m_model;
	CGLTex				*	m_tex;
	C3DVec					m_color;
public:
	C3DSkyDome(C3DSkyDomeShader * shader, CGLTex * pTex);
	virtual ~C3DSkyDome();

	void render(C3DShader * pShader);

protected:
	// ���̃I�u�W�F�N�g�̃}�g���N�X�v�Z��ɍs���������L�q
	bool calcProcedure(bool is_recalc);
};

#endif // C3DSkyDome_h
