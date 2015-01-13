#ifndef CVSNPNGTex_h
#define CVSNPNGTex_h

#include "CGLTex.h"

class CVSNPNGTex : public CGLTex
{
public:
	CVSNPNGTex(const char * objName, const char * pngName);
	virtual ~CVSNPNGTex();

	bool recovery();

private:
	bool loadPng(const char * pngName);

	const char *	m_pngName;
};

#endif // CVSNPNGTex_h
