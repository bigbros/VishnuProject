#ifndef C3DDrawable_h
#define C3DDrawable_h

#include "CGLObj.h"
#include "C3DObj.h"
#include "C3DDrawEnv.h"

class C3DDrawable;
class C3DShader;

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
	virtual void render(C3DShader * pShader) = 0;

protected:
	template<typename T>
	inline T * getDrawable() {
		return (T *)m_drawable;
	}
};

class C3DDrawable : public CGLObj
{
	friend class C3DDrawObj;
	friend class C3DShader;
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
	C3DShader	*	m_shader;

public:
	C3DDrawable(C3DShader * shader);
	virtual ~C3DDrawable();

	void registUser(C3DDrawObj * pObj);

	virtual void setup(C3DShader * pShader) = 0;
	virtual void cleanup(C3DShader * pShader) = 0;

	void Render();
};

#endif // C3DDrawable_h
