#ifndef IVSNScriptUtilFunc_h
#define IVSNScriptUtilFunc_h

#include "CVSNObj.h"
#include "CVSNScript.h"

class IVSNScriptUtilFunc : public CVSNObj
{
private:
	static IVSNScriptUtilFunc	*	m_begin;
	static IVSNScriptUtilFunc	*	m_end;

	IVSNScriptUtilFunc	*	m_prev;
	IVSNScriptUtilFunc	*	m_next;

	int							m_hash;
	const char				*	m_name;
public:
	IVSNScriptUtilFunc(const char * name);
	virtual ~IVSNScriptUtilFunc();

	virtual bool utilFunc(CVSNScriptArgs& args, CVSNScriptArgs& ret) = 0;

	static bool execUtilFunc(const char * name, CVSNScriptArgs& args, CVSNScriptArgs& ret);
};


#endif // CVSNScriptPropertyFunc_h
