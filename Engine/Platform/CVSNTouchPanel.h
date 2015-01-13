#ifndef CVSNTouchPanel_h
#define CVSNTouchPanel_h

#include "CVSNPointing.h"	// pointing devices.

class CVSNPointListener : public IVSNPointListener
{
public:
	CVSNPointListener();
	~CVSNPointListener();

	void listenEvent(int touchId, IVSNPointListener::EVENT ev, int x, int y, float force);
};


class CVSNTouchPanel
{
	friend class CVSNPointListener;
public:
	struct ITEM {
		int							touchId;
		IVSNPointListener::EVENT	ev;
		int							x;
		int							y;
		float						force;
	};

private:
	enum {
		QUEUE_SIZE = 1024
	};
	ITEM		m_queue[QUEUE_SIZE];

	int			m_ptReadTop;
	int			m_ptNextTop;

	int			m_ptRec;
	int			m_ptRead;

private:
	CVSNTouchPanel();
	~CVSNTouchPanel();

public:
	static CVSNTouchPanel& getInstance();

	inline void seekTop() { m_ptRead = m_ptReadTop; }
	const ITEM * getItem();

	void flip();

private:
	bool addItem(int touchId, IVSNPointListener::EVENT ev, int x, int y, float force);
};

#endif //CVSNTouchPanel_h
