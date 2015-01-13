#ifndef IVSNPointing_h
#define IVSNPointing_h

#include "CVSNPlatform.h"

class CVSNPointing;

class IVSNPointListener : public CVSNObj
{
	friend class CVSNPointing;
public:
	enum EVENT {
		PT_TAP,
		PT_DRAG,
		PT_RELEASE
	};
private:
	IVSNPointListener	*	m_prev;
	IVSNPointListener	*	m_next;

	CVSNPointing		*	m_parent;
public:
	IVSNPointListener();
	virtual ~IVSNPointListener();

	virtual void listenEvent(int touchId, EVENT ev, int x, int y, float force) = 0;
};


class CVSNPointing : public IVSNModule
{
	friend class IVSNPointListener;
private:
	IVSNPointListener	*	m_begin;
	IVSNPointListener	*	m_end;
public:
	CVSNPointing();
	virtual ~CVSNPointing();

	bool init();
	void registListener(IVSNPointListener * pListener);
	void setEvent(int touchId, IVSNPointListener::EVENT ev, int x, int y, float force);
};

#endif // IVSNPointing_h
