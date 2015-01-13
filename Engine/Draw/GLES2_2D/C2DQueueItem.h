#ifndef C2DQueueItem_h
#define C2DQueueItem_h

#include "Types2D.h"
#include "C2DDrawQueue.h"

class C2DTexOT;

class C2DQueueItem : public CGLBase
{
	friend class C2DTexOT;
	friend class C2DSprite;
	friend class C2DDrawQueue;
private:
	enum {
		QUEUE_MAX = C2DDrawQueue::MAX_DRAW_ITEMS
	};

	struct QUEUELINK {
		QUEUELINK();
		C2DQueueItem	*	prev;
		C2DQueueItem	*	next;
	};
	QUEUELINK		m_link;

	int				m_useItem;
	C2DTexOT	*	m_ot;
	int				m_pri;

	C2DSprite	*	m_sprites[QUEUE_MAX];
	C2DMat			m_matrices[ QUEUE_MAX ];
	C3DVec			m_rgba[ QUEUE_MAX ];
	C3DVec			m_uv[ QUEUE_MAX ];


public:
	C2DQueueItem(C2DTexOT * ot, int pri);
	virtual ~C2DQueueItem();

	inline int getBlank() { return C2DDrawQueue::MAX_DRAW_ITEMS - m_useItem;  }
	bool alloc(C2DSprite * pSpr);
	int remove(C2DSprite * pSpr);

	void kick(C2DDefaultShader * shader);
};

#endif // C2DQueueItem_h
