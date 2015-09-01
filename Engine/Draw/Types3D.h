#ifndef Types3D_h
#define Types3D_h

#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

#include "dtypes.h"

#define F_PI ((float)M_PI)

struct C3DMat;

struct C3DVec
{
	float x;
	float y;
	float z;
	float w;

	C3DVec();
	C3DVec(float _x, float _y, float _z, float _w = 1.0f);
	C3DVec(const C3DVec& axis, float theta);
	C3DVec(const C3DVec& v);
	~C3DVec();

	inline const C3DVec operator +(const C3DVec& v) throw() { return C3DVec(x + v.x, y + v.y, z + v.z, w*v.w); }
	inline const C3DVec operator -(const C3DVec& v) throw() { return C3DVec(x - v.x, y - v.y, z - v.z, w*v.w); }
	inline const float  operator %(const C3DVec& v) throw() { return x*v.x + y*v.y + z*v.z; }
	inline const C3DVec operator /(const C3DVec& v) throw() { return C3DVec(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x); }

	const C3DMat operator +(const C3DMat& m) throw();

	inline C3DVec& set(float _x, float _y, float _z, float _w = 1.0f) throw() {
		x = _x;	
		y = _y;
		z = _z;
		w = _w;
		return *this;
	}

	inline const float sqlen() throw() { return x*x + y*y + z*z; }
	inline const float len() throw() { return sqrtf(sqlen()); }
	inline const C3DVec unit() {
		float l = len();
		return C3DVec(x / l, y / l, z / l, w);
	}
	inline const C3DVec rate(float rt) {
		return C3DVec(x * rt, y * rt, z * rt, w);
	}
	inline const C3DVec& adjust() {
		x /= w;
		y /= w;
		z /= w;
		w /= w;
		return *this;
	}

	const C3DVec& axis(const C3DVec& v, float theta) throw();

	inline const C3DVec conj(const C3DVec& v) throw() { return C3DVec(-x, -y, -z, w); }
	inline const C3DVec operator *(const C3DVec& q) throw() {
		C3DVec ans;
		ans.w = w*q.w - x*q.x - y*q.y - z*q.z;
		ans.x = w*q.x + x*q.w + y*q.z - z*q.y;
		ans.y = w*q.y - x*q.z + y*q.w + z*q.x;
		ans.z = w*q.z + x*q.y - y*q.x + z*q.w;
		return ans;
	}
	const C3DVec operator *(const C3DMat& m) throw();

	void *	operator new(size_t size);
	void	operator delete(void * p);
	void *	operator new[](size_t size);
	void	operator delete[](void * p);
};

typedef C3DVec C3DQuat;

struct C3DMat
{
	float v[4][4];

	C3DMat();
	C3DMat(const C3DQuat& q);
	C3DMat(const C3DMat& m);
	~C3DMat();

	inline const float * operator [](int index) throw() { return v[index]; }

	inline const C3DMat& operator =(const C3DQuat& q) throw() { return setQuat(q); }
	const C3DMat  operator +(const C3DVec& vec) throw();
	const C3DMat& operator +=(const C3DVec& v) throw();
	const C3DMat& operator *=(const C3DMat& m) throw();
	const C3DMat  operator *(const C3DMat& m) throw();
	const C3DVec  operator *(const C3DVec& v) throw();

	const C3DMat& setQuat(const C3DQuat& q) throw();
	const C3DMat invert() const throw();

	void *	operator new(size_t size);
	void	operator delete(void * p);
	void *	operator new[](size_t size);
	void	operator delete[](void * p);
};

#endif // Types3D_h
