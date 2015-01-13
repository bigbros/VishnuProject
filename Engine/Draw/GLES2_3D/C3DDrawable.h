#ifndef C3DDrawable_h
#define C3DDrawable_h

#include "CGLObj.h"
#include "C3DObj.h"
#include "C3DDrawEnv.h"

class C3DDrawable;

class C3DDrawObj : public C3DObj
{
	friend class C3DDrawable;
private:
	struct DRAWLINK {
		DRAWLINK();
		C3DDrawObj		*	prev;
		C3DDrawObj		*	next;
	};
	C3DDrawable	*	m_drawable;
	DRAWLINK		m_drawlink;
public:
	C3DDrawObj(int matnum = 1);
	virtual ~C3DDrawObj();

	void useDrawable(C3DDrawable * drawable);

	// このオブジェクトをレンダリングする
	virtual void render(C3DDrawEnv * env) = 0;

protected:
	template<typename T>
	inline T * getDrawable() {
		return (T *)m_drawable;
	}
};

class C3DDrawable : public CGLObj
{
	friend class C3DDrawObj;
	friend class C3DDrawEnv;
private:
	struct USER {
		USER();
		C3DDrawObj	*	begin;
		C3DDrawObj	*	end;
	};
	struct LINK {
		LINK();
		C3DDrawable	*	prev;
		C3DDrawable	*	next;
	};

	USER			m_users;
	LINK			m_sisters;
	C3DDrawEnv	*	m_env;

public:
	C3DDrawable();
	virtual ~C3DDrawable();

	void registUser(C3DDrawObj * pObj);

	virtual void setup(C3DDrawEnv * env) = 0;
	virtual void cleanup(C3DDrawEnv * env) = 0;

	void Render();
};

#endif // C3DDrawable_h
