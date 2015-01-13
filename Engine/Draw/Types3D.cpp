#include <stdio.h>
#include <string.h>
#include <exception>
#include "CGLAlloc.h"
#include "Types3D.h"

void *	C3DVec::operator new(size_t size)	{	return CGLAlloc::malloc(size);	}
void *	C3DVec::operator new[](size_t size)	{	return CGLAlloc::malloc(size);	}
void	C3DVec::operator delete(void * p)	{	CGLAlloc::free(p);				}
void	C3DVec::operator delete[](void * p)	{	CGLAlloc::free(p);				}

C3DVec::C3DVec() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
C3DVec::C3DVec(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
C3DVec::C3DVec(const C3DVec& vax, float theta) { axis(vax, theta); }
C3DVec::C3DVec(const C3DVec& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
C3DVec::~C3DVec() {}


const C3DVec&
C3DVec::axis(const C3DVec& v, float theta) throw()
{
	float hrad = theta / 2.0f;
	float t_sin = sinf(hrad);
	x = t_sin*v.x;
	y = t_sin*v.y;
	z = t_sin*v.z;
	w = cosf(hrad);
	return *this;
}

const C3DVec
C3DVec::operator *(const C3DMat& m) throw()
{
	C3DVec ans;
	ans.x = x*m.v[0][0] + y*m.v[1][0] + z*m.v[2][0] + w*m.v[3][0];
	ans.y = x*m.v[0][1] + y*m.v[1][1] + z*m.v[2][1] + w*m.v[3][1];
	ans.z = x*m.v[0][2] + y*m.v[1][2] + z*m.v[2][2] + w*m.v[3][2];
	ans.w = x*m.v[0][3] + y*m.v[1][3] + z*m.v[2][3] + w*m.v[3][3];
	return ans;
}

const C3DMat
C3DVec::operator +(const C3DMat& m) throw()
{
	C3DMat ans(m);
	ans.v[3][0] = x;
	ans.v[3][1] = y;
	ans.v[3][2] = z;
	return ans;
}






void *	C3DMat::operator new(size_t size)	{	return CGLAlloc::malloc(size);	}
void *	C3DMat::operator new[](size_t size)	{	return CGLAlloc::malloc(size);	}
void	C3DMat::operator delete(void * p)	{	CGLAlloc::free(p);				}
void	C3DMat::operator delete[](void * p)	{	CGLAlloc::free(p);				}


C3DMat::C3DMat() {
	for(int y = 0; y < 4; y++) {
		for(int x = 0; x < 4; x++) {
			v[y][x] = (x == y) ? 1.0f : 0.0f;
		}
	}
}
C3DMat::C3DMat(const C3DQuat& q) {
	setQuat(q);
}

C3DMat::C3DMat(const C3DMat& m) {
	memcpy(&v, &m.v, sizeof(v));
}

C3DMat::~C3DMat() {}

const C3DMat&
C3DMat::operator +=(const C3DVec& u) throw () {
	v[3][0] += u.x;
	v[3][1] += u.y;
	v[3][2] += u.z;
	return *this;
}

const C3DMat&
C3DMat::operator *=(const C3DMat& m) throw()
{
	*this = *this * m;
	return *this;
}

const C3DMat
C3DMat::operator +(const C3DVec& u) throw() {
	C3DMat m(*this);
	return m += u;
}

const C3DMat
C3DMat::operator *(const C3DMat& m) throw() {
	int x, y, z;
	C3DMat ret;
	for(y = 0; y < 4; y++) {
		for(x = 0; x < 4; x++) {
			ret.v[y][x] = 0.0f;
			for(z = 0; z < 4; z++) {
				ret.v[y][x] += v[y][z] * m.v[z][x];
			}
		}
	}
	return ret;
}

const C3DVec
C3DMat::operator *(const C3DVec& u) throw() {
	C3DVec ans;
	ans.x = u.x*v[0][0] + u.y*v[0][1] + u.z*v[0][2] + u.w*v[0][3];
	ans.y = u.x*v[1][0] + u.y*v[1][1] + u.z*v[1][2] + u.w*v[1][3];
	ans.z = u.x*v[2][0] + u.y*v[2][1] + u.z*v[2][2] + u.w*v[2][3];
	ans.w = u.x*v[3][0] + u.y*v[3][1] + u.z*v[3][2] + u.w*v[3][3];
	return ans;
}

const C3DMat&
C3DMat::setQuat(const C3DVec& u) throw()
{
	v[0][0] = 1.0f - 2.0f * (u.y*u.y + u.z*u.z);
	v[0][1] = 2.0f * (u.x*u.y + u.w*u.z);
	v[0][2] = 2.0f * (u.x*u.z - u.w*u.y);
	v[0][3] = 0.0f;

	v[1][0] = 2.0f * (u.x*u.y - u.w*u.z);
	v[1][1] = 1.0f - 2.0f * (u.x*u.x + u.z*u.z);
	v[1][2] = 2.0f * (u.y*u.z + u.w*u.x);
	v[1][3] = 0.0f;

	v[2][0] = 2.0f * (u.x*u.z + u.w*u.y);
	v[2][1] = 2.0f * (u.y*u.z - u.w*u.x);
	v[2][2] = 1.0f - 2.0f * (u.x*u.x + u.y*u.y);
	v[2][3] = 0.0f;

	v[3][0] = 0.0f;
	v[3][1] = 0.0f;
	v[3][2] = 0.0f;
	v[3][3] = 1.0f;

	return *this;
}

const C3DMat
C3DMat::invert() const throw()
{
	C3DMat m1;
	C3DMat m2(*this);
	float buf;

	int i, j, k;
	for (i = 0; i < 4; i++) {
		buf = 1.0f / m2.v[i][i];
		for (j = 0; j < 4; j++) {
			m2.v[i][j] *= buf;
			m1.v[i][j] *= buf;
		}
		for (j = 0; j < 4; j++) {
			if (i != j) {
				buf = m2.v[j][i];
				for (k = 0; k < 4; k++) {
					m2.v[j][k] -= m2.v[i][k] * buf;
					m1.v[j][k] -= m1.v[i][k] * buf;
				}
			}
		}
	}
	return m1;
}

