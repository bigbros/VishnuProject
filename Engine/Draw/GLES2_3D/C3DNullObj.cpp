#include "C3DNullObj.h"

C3DNullObj::C3DNullObj() : C3DObj() {}
C3DNullObj::~C3DNullObj() {}

bool
C3DNullObj::calcProcedure(bool is_recalc)
{
	return true;
}

void
C3DNullObj::render(){}
