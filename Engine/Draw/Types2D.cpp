#include <stdio.h>
#include <string.h>
#include <exception>
#include "CGLAlloc.h"
#include "Types2D.h"

void *	C2DVec::operator new(size_t size){ return CGLAlloc::malloc(size); }
void *	C2DVec::operator new[](size_t size)	{	return CGLAlloc::malloc(size);	}
void	C2DVec::operator delete(void * p)	{ CGLAlloc::free(p); }
void	C2DVec::operator delete[](void * p)	{	CGLAlloc::free(p);				}

C2DVec::C2DVec() : x(0.0f), y(0.0f), w(1.0f) {}
C2DVec::C2DVec(float _x, float _y, float _w) : x(_x), y(_y), w(_w) {}
C2DVec::C2DVec(const C2DVec& v) : x(v.x), y(v.y), w(v.w) {}
C2DVec::~C2DVec() {}

const C2DMat
C2DVec::operator +(const C2DMat& m) throw()
{
	C2DMat ans(m);
	ans.v[2][0] = x;
	ans.v[2][1] = y;
	return ans;
}






void *	C2DMat::operator new(size_t size){ return CGLAlloc::malloc(size); }
void *	C2DMat::operator new[](size_t size)	{	return CGLAlloc::malloc(size);	}
void	C2DMat::operator delete(void * p)	{ CGLAlloc::free(p); }
void	C2DMat::operator delete[](void * p)	{	CGLAlloc::free(p);				}


C2DMat::C2DMat() {
	for (int y = 0; y < 3; y++) {
		for (int x = 0; x < 3; x++) {
			v[y][x] = (x == y) ? 1.0f : 0.0f;
		}
	}
}

C2DMat::C2DMat(const C2DMat& m) {
	memcpy(&v, &m.v, sizeof(v));
}

C2DMat::C2DMat(const float rot) {
	setRot(rot);
	v[2][0] = 0.0f;
	v[2][1] = 0.0f;
	v[0][2] = 0.0f;
	v[1][2] = 0.0f;
	v[2][2] = 1.0f;
}

C2DMat::C2DMat(const float width, const float height) {
	toScale(width, height);
}


C2DMat::~C2DMat() {}

const C2DMat&
C2DMat::operator +=(const C2DVec& u) throw () {
	v[2][0] += u.x;
	v[2][1] += u.y;
	return *this;
}

const C2DMat&
C2DMat::operator *=(const C2DMat& m) throw()
{
	*this = *this * m;
	return *this;
}

const C2DMat
C2DMat::operator +(const C2DVec& u) throw() {
	C2DMat m(*this);
	return m += u;
}

const C2DMat
C2DMat::operator *(const C2DMat& m) throw() {
	int x, y, z;
	C2DMat ret;
	for (y = 0; y < 3; y++) {
		for (x = 0; x < 3; x++) {
			ret.v[y][x] = 0.0f;
			for (z = 0; z < 3; z++) {
				ret.v[y][x] += v[y][z] * m.v[z][x];
			}
		}
	}
	return ret;
}

const C2DVec
C2DMat::operator *(const C2DVec& u) throw() {
	C2DVec ans;
	ans.x = u.x*v[0][0] + u.y*v[0][1] + u.w*v[0][2];
	ans.y = u.x*v[1][0] + u.y*v[1][1] + u.w*v[1][2];
	ans.w = u.x*v[2][0] + u.y*v[2][1] + u.w*v[2][2];
	return ans;
}

const C2DMat&
C2DMat::setRot(const float rot) throw() {
	float sin_h = sinf(rot);
	float cos_h = cosf(rot);

	v[0][0] = cos_h;
	v[1][0] = -sin_h;
	v[2][0] = 0.0f;

	v[0][1] = sin_h;
	v[1][1] = cos_h;
	v[2][1] = 0.0f;

	v[0][2] = 0.0f;
	v[1][2] = 0.0f;
	v[2][2] = 1.0f;

	return *this;
}

const C2DMat&
C2DMat::toScale(const float width, const float height) throw() {
	for (int i = 0; i < 9; i++) ((float *)v)[i] = 0.0f;
	v[0][0] = width;
	v[1][1] = height;
	v[2][2] = 1.0f;
	return *this;
}

const C2DMat
C2DMat::invert() const throw()
{
	C2DMat m1;
	C2DMat m2(*this);
	float buf;

	int i, j, k;
	for (i = 0; i < 3; i++) {
		buf = 1.0f / m2.v[i][i];
		for (j = 0; j < 3; j++) {
			m2.v[i][j] *= buf;
			m1.v[i][j] *= buf;
		}
		for (j = 0; j < 3; j++) {
			if (i != j) {
				buf = m2.v[j][i];
				for (k = 0; k < 3; k++) {
					m2.v[j][k] -= m2.v[i][k] * buf;
					m1.v[j][k] -= m1.v[i][k] * buf;
				}
			}
		}
	}
	return m1;
}

