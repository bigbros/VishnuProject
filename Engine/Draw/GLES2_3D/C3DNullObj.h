#ifndef C3DNullObj_h
#define C3DNullObj_h

#include "C3DObj.h"

class C3DNullObj : public C3DObj
{
public:
	C3DNullObj();
	virtual ~C3DNullObj();

	bool calcProcedure(bool is_recalc);
	void render();

};

#endif // C3DNullObj_h
