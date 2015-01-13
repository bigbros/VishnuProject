#ifndef C2DNullObj_h
#define C2DNullObj_h

#include "Types2D.h"
#include "C2DObj.h"


class C2DNullObj : public C2DObj
{
public:
	C2DNullObj();
	virtual ~C2DNullObj();

private:
	bool calcProcedure(bool recalc);
};


#endif // C2DNullObj_h
