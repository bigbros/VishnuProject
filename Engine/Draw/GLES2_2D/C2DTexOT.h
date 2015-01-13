#ifndef C2DTexOT_h
#define C2DTexOT_h

#include <GLES2/gl2.h>
#include "Types2D.h"
#include "CGLBase.h"
#include "CGLTex.h"
#include "C2DDefaultShader.h"

class C2DSprite;
class C2DDrawQueue;
class C2DQueueItem;

class C2DTexOT : public CGLBase
{
	friend class C2DDrawEnv;
	friend class C2DSprite;
	friend class C2DQueueItem;
public:
	enum {
		C2D_MAX_PRIORITY = 32
	};
private:
	struct TEXCHAIN {
		TEXCHAIN();
		C2DTexOT	*	prev;
		C2DTexOT	*	next;
	};

	struct QUEUELIST {
		QUEUELIST();
		C2DQueueItem	*	begin;
		C2DQueueItem	*	end;
	};
	CGLTex		*	m_tex;

	TEXCHAIN		m_chain;
	QUEUELIST		m_qlist[C2D_MAX_PRIORITY];

	int				m_useCount;

	C2DTexOT(CGLTex * pTex);
	virtual ~C2DTexOT();
public:

	void registSprite(C2DSprite * pSpr, int pri);
	int  removeSprite(C2DSprite * pSpr);

	inline bool isTex(CGLTex * pTex) { return (pTex == m_tex); }
	inline void setTex(GLenum target) { m_tex->set(target); }
	void render(C2DDrawQueue * queue, int pri);
};


#endif // C2DTexOT_h
