#ifndef CGLFBO_h
#define CGLFBO_h

#include "CGLObj.h"
#include "Types3D.h"

class CGLFBO : public CGLObj
{
protected:
	int				m_fboNum;
	GLuint		*	m_fboIdx;
	GLuint		*	m_fboDepth;
	GLuint		*	m_fboTex;

	int				m_width;
	int				m_height;

	int				m_globalWidth;
	int				m_globalHeight;

	C3DMat			m_projection;
public:
	CGLFBO(int num, int width, int height);
	virtual ~CGLFBO();

	inline const int getLensNum() const { return m_fboNum; }

	void setGlobalResolution(int width, int height);

	void SwitchFBO(int lensId);

	virtual float getAngle() = 0;			// デフォルトの視野角を返す
	virtual float getAspect();			// デフォルトのアスペクト比を返す
	virtual C3DMat& getProjection();	// プロジェクションマトリクスの参照を返す。

	virtual void setLens(int lensId, GLint offsetUniform) = 0;
	virtual void renderGlobal() = 0;
	virtual void onChangeGlobalResolution(int width, int height);

	virtual void createVertex() = 0;
	virtual void destroyVertex() = 0;
	bool recovery();
	bool destruction();

protected:
	bool makeFrameBuffer();
	void destroyFrameBuffer();
};

#endif // CGLFBO_h
