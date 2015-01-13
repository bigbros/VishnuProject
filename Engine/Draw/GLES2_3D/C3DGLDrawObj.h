#ifndef C3DGLDrawObj_h
#define C3DGLDrawObj_h

#include "C3DGLObj.h"

class C3DDrawEnv;
class C3DObj;
class C3DGLDrawObj;

class C3DDrawItem : public C3DObj
{
	friend class C3DGLDrawObj;
private:

public:
	C3DDrawItem(C3DObj * parent = 0);
	virtual ~C3DDrawItem();

	// オブジェクト固有値を uniform に設定させる。
	virtual void setUniform(C3DDrawEnv& env) = 0;
};

class C3DGLDrawObj : public C3DGLObj
{
	friend class C3DDrawEnv;
public:
	C3DGLDrawObj();
	virtual ~C3DGLDrawObj();

	inline void reset() { m_item = m_begin; }
	inline C3DDrawItem * get_next() {
		C3DDrawItem * ret = m_item;
		m_item = m_item->m_drawNext;
		return ret;
	}
	
	virtual void render(C3DDrawEnv& env) = 0;

protected:
	C3DDrawItem	*	m_begin;
	C3DDrawItem	*	m_end;

private:
	C3DDrawItem	*	m_item;
};


#endif // C3DGLDrawObj_h
