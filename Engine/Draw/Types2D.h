#ifndef Types2D_h
#define Types2D_h

#define _USE_MATH_DEFINES
#include <stdio.h>
#include <math.h>
#include <cmath>

#include "dtypes.h"



#define F_PI ((float)M_PI)

struct C2DMat;

struct C2DVec
{
	float x;
	float y;
	float w;

	C2DVec();
	C2DVec(float _x, float _y, float _w = 1.0f);
	C2DVec(const C2DVec& v);
	~C2DVec();

	inline const C2DVec operator +(const C2DVec& v) throw() { return C2DVec(x + v.x, y + v.y, w*v.w); }
	inline const C2DVec operator -(const C2DVec& v) throw() { return C2DVec(x - v.x, y - v.y, w*v.w); }
	inline const float  operator %(const C2DVec& v) throw() { return x*v.x + y*v.y; }

	const C2DMat operator +(const C2DMat& m) throw();

	inline C2DVec& set(float _x, float _y, float _w = 1.0f) throw() {
		x = _x;
		y = _y;
		w = _w;
		return *this;
	}

	inline const float sqlen() throw() { return x*x + y*y; }
	inline const float len() throw() { return sqrtf(sqlen()); }
	inline const C2DVec unit() {
		float l = len();
		return C2DVec(x / l, y / l, w);
	}
	inline const C2DVec& adjust() {
		x /= w;
		y /= w;
		w /= w;
		return *this;
	}

	const C2DVec operator *(const C2DMat& m) throw();

	void *	operator new(size_t size);
	void	operator delete(void * p);
	void *	operator new[](size_t size);
	void	operator delete[](void * p);
};

struct C2DMat
{
	float v[3][3];

	C2DMat();
	C2DMat(const C2DMat& m);
	C2DMat(const float rot);
	C2DMat(const float width, const float height);
	~C2DMat();

	inline const float * operator [](int index) throw() { return v[index]; }
	inline const C2DMat& operator =(const float rot) throw() { return setRot(rot); }

	const C2DMat  operator +(const C2DVec& vec) throw();
	const C2DMat& operator +=(const C2DVec& v) throw();
	const C2DMat& operator *=(const C2DMat& m) throw();
	const C2DMat  operator *(const C2DMat& m) throw();
	const C2DVec  operator *(const C2DVec& v) throw();

	const C2DMat& setRot(const float rot) throw();
	const C2DMat& toScale(const float width, const float height) throw();
	const C2DMat invert() const throw();

	void *	operator new(size_t size);
	void	operator delete(void * p);
	void *	operator new[](size_t size);
	void	operator delete[](void * p);
};


#endif // Types2D_h
