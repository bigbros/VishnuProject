#ifndef C2DDrawQueue_h
#define C2DDrawQueue_h

#include <GLES2/gl2.h>
#include "dtypes.h"

#include "Types2D.h"
#include "Types3D.h"	// C3DVec ‚ðRGBA’l‚È‚Ç‚ÅŽg—p

#include "CGLObj.h"
#include "C2DDefaultShader.h"


class C2DTexOT;
class C2DSprite;
class C2DQueueItem;

class C2DDrawQueue : public CGLObj
{
public:
	enum {
		MAX_DRAW_ITEMS = 32
	};
private:
	struct VEC {
		u8 x;
		u8 y;
	};
	struct VERTEX {
		VEC		vert;
		u8		idx;	// reference matrix
		u8		pad;	// padding
	};

	GLuint		m_vert;
	GLuint		m_idx;

	VERTEX	*	m_vertBuf;
	u8		*	m_idxBuf;
	
	C2DDefaultShader	*	m_shader;

	C2DTexOT	*	m_lastTex;
	int				m_matCount;
public:
	C2DDrawQueue(C2DDefaultShader * shader);
	virtual ~C2DDrawQueue();

	void bufferEnable(C2DDefaultShader * shader);
	void drawInit();
	void drawItem(C2DQueueItem * pSprite);

	bool recovery();
	bool destruction();

private:
	void makeBuffer(C2DDefaultShader * shader);
};

#endif // C2DDrawQueue_h
