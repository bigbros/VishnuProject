#ifndef C2DRenderer_h
#define C2DRenderer_h

#include "Types2d.h"
#include "CGLBase.h"





class C2DRenderer : public CGLBase
{
private:
	struct VEC {
		float x;
		float y;
	};
	struct VERTEX {
		VEC		vert;
		u8		mat;	// reference matrix
		u8		pad;	// padding
	};

};

#endif // C2DRenderer_h
