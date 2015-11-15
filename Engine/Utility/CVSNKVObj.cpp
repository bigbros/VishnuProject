#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory>
#include "CVSNKVObj.h"

KeyValueException::KeyValueException(const char * msg)
	: std::exception()
	, m_msg(msg)
{}
// KeyValueException::~KeyValueException(){}

const char *
KeyValueException::what() throw()
{
	return m_msg;
}

CVSNKVMap::CVSNKVMap()
	: CVSNObj()
	, m_begin(0)
	, m_end(0)
{}

CVSNKVMap::~CVSNKVMap()
{
	CHAIN * pChain = m_begin;

	while (pChain) {
		CHAIN * pNext = pChain->next;
		delete pChain->pObj;
		delete[] pChain->key;
		delete pChain;
		pChain = pNext;
	}
}

void
CVSNKVMap::add(const char * key, CVSNKVObj * pObj)
{
	char * buf = 0;
	CHAIN * pChain = 0;
	try {
		int len = strlen(key);
		buf = new char[len + 1];
		strcpy(buf, key);

		pChain = new CHAIN();
		pChain->pObj = pObj;
		pChain->key = buf;
		pChain->hash = CVSNUtil::strHash(key);

		pChain->next = 0;
		pChain->prev = m_end;
		if (pChain->prev) {
			pChain->prev->next = pChain;
		}
		else {
			m_begin = pChain;
		}
		m_end = pChain;
	}
	catch (std::bad_alloc& ex) {
		delete[] buf;
		delete pChain;
		throw ex;
	}
}

bool
CVSNKVMap::ContainsKey(const char * key)
{
	int hash = CVSNUtil::strHash(key);
	for (CHAIN * pChain = m_begin; pChain; pChain = pChain->next) {
		if (pChain->hash == hash && !strcmp(pChain->key, key)) return true;
	}
	return false;
}

CVSNKVObj&
CVSNKVMap::operator[](const char * key)
{
	int hash = CVSNUtil::strHash(key);
	for (CHAIN * pChain = m_begin; pChain; pChain = pChain->next) {
		if (pChain->hash == hash && !strcmp(pChain->key, key)) return *(pChain->pObj);
	}
	throw KeyValueException("invalid key or index.");
}

int
CVSNKVMap::txtlen()
{
	int len = 1;	// '{'
	int delim = 0;
	for (CHAIN * pChain = m_begin; pChain; pChain = pChain->next) {
		len += delim;	// ,
		len += strlen(pChain->key) + 3;	// "":
		len += pChain->pObj->txtlen();
		delim = 1;
	}
	len += 1;	// '}'
	return len;
}

int
CVSNKVMap::ToJSON(char * buf, int index)
{
	int idx = index;
	int delim = 0;
	buf[idx++] = '{';

	for (CHAIN * pChain = m_begin; pChain; pChain = pChain->next) {
		if (delim) buf[idx++] = delim;
		sprintf(buf + idx, "\"%s\":", pChain->key);
		idx += strlen(buf + idx);
		idx += pChain->pObj->ToJSON(buf, idx);
		delim = ',';
	}
	buf[idx++] = '}';
	buf[idx] = 0;
	return idx - index;
}

CVSNKVArray::CVSNKVArray()
	: CVSNObj()
	, m_max(0)
	, m_use(0)
	, m_array(0)
{}

CVSNKVArray::~CVSNKVArray()
{
	for (int i = 0; i < m_use; i++) delete m_array[i];
	delete[] m_array;
}

bool
CVSNKVArray::alloc(int size)
{
	if (m_use > size) return false;

	try {
		int num = (size + BLOCK - 1) / BLOCK;
		int next_size = num * BLOCK;
		CVSNKVObj ** tmp = new CVSNKVObj *[next_size];
		for (int i = 0; i < next_size; i++) tmp[i] = 0;
		if (m_array) {
			memcpy(tmp, m_array, m_use * sizeof(CVSNKVObj *));
			delete m_array;
		}
		m_array = tmp;
		m_max = next_size;
		return true;
	}
	catch (std::bad_alloc&ex) {
		throw ex;
	}
}

bool
CVSNKVArray::set(int index, CVSNKVObj * pObj)
{
	if (index >= m_max) {
		alloc(index + 1);
	}
	delete m_array[index];
	m_array[index] = pObj;
	if (index >= m_use) m_use = index + 1;
	return true;
}

void
CVSNKVArray::add(CVSNKVObj * pObj)
{
	try {
		if (m_use == m_max) {
			int next_size = m_max + BLOCK;
			CVSNKVObj ** tmp = new CVSNKVObj*[next_size];
			memcpy(tmp, m_array, m_use * sizeof(CVSNKVObj*));
			delete[] m_array;
			m_array = tmp;
			m_max = next_size;
		}
		m_array[m_use] = pObj;
		m_use++;
	}
	catch (std::bad_alloc& ex) {
		throw ex;
	}
}

int
CVSNKVArray::txtlen()
{
	int len = 1;	// '['
	int delim = 0;
	for (int i = 0; i < m_use; i++) {
		len += delim;
		len += m_array[i]->txtlen();
		delim = 1;
	}
	len += 1;
	return len;
}

int
CVSNKVArray::ToJSON(char * buf, int index)
{
	int idx = index;
	int delim = 0;
	buf[idx++] = '[';
	for (int i = 0; i < m_use; i++) {
		if (delim) buf[idx++] = delim;
		idx += m_array[i]->ToJSON(buf, idx);
		delim = ',';
	}
	buf[idx++] = ']';
	buf[idx] = 0;
	return idx - index;
}


CVSNKVObj&
CVSNKVArray::operator[](int index)
{
	if (index < 0 || index >= m_use) throw KeyValueException("array index out of range.");
	return *(m_array[index]);
}


CVSNKVObj::CVSNKVObj() : CVSNObj(), m_type(T_NULL){}
CVSNKVObj::CVSNKVObj(bool v) : CVSNObj(), m_type(T_NULL) { *this = v; }
CVSNKVObj::CVSNKVObj(int v) : CVSNObj(), m_type(T_NULL) { *this = v; }
CVSNKVObj::CVSNKVObj(double v) : CVSNObj(), m_type(T_NULL) { *this = v; }
CVSNKVObj::CVSNKVObj(float v) : CVSNObj(), m_type(T_NULL) { *this = v; }
CVSNKVObj::CVSNKVObj(const char * v) : CVSNObj(), m_type(T_NULL) { *this = v; }

CVSNKVObj::~CVSNKVObj()
{
	clear();
}

void
CVSNKVObj::clear()
{
	switch (m_type) {
	case T_STR: delete[] m_variant.str; break;
	case T_MAP: delete m_variant.map;	break;
	case T_ARR: delete m_variant.arr;	break;
	default: break;
	}
	m_type = T_NULL;
}

CVSNKVObj&
CVSNKVObj::operator[](const char * key)
{
	if (m_type != T_MAP) throw KeyValueException("not a key-value format.");
	return (*m_variant.map)[key];
}

CVSNKVObj&
CVSNKVObj::operator[](int index)
{
	if (m_type != T_ARR) throw KeyValueException("not an array.");
	return (*m_variant.arr)[index];
}

CVSNKVObj&
CVSNKVObj::operator=(bool v)
{
	clear();
	m_variant.b = v;
	m_type = T_BOOL;
	return *this;
}

CVSNKVObj&
CVSNKVObj::operator=(int v)
{
	clear();
	m_variant.i = v;
	m_type = T_INT;
	return *this;
}
CVSNKVObj&
CVSNKVObj::operator=(double v)
{
	clear();
	m_variant.n = v;
	m_type = T_NUM;
	return *this;
}

CVSNKVObj&
CVSNKVObj::operator=(float v)
{
	*this = (double)v;
	return *this;
}

CVSNKVObj&
CVSNKVObj::operator=(const char * v)
{
	char * buf = 0;
	try {
		int len = strlen(v);
		buf = new char[len + 1];
		strcpy(buf, v);

		clear();
		m_variant.str = buf;
		m_type = T_STR;
		return *this;
	}
	catch (std::bad_alloc& ex) {
		throw ex;
	}
}
void
CVSNKVObj::add(const char * key, CVSNKVObj * pObj)
{
	if (m_type != T_MAP) throw KeyValueException("not a key-value format.");
	m_variant.map->add(key, pObj);
}

void
CVSNKVObj::add(CVSNKVObj * pObj)
{
	if (m_type != T_ARR) throw KeyValueException("not an array.");
	m_variant.arr->add(pObj);
}

void
CVSNKVObj::set(int index, CVSNKVObj * pObj)
{
	if (m_type != T_ARR) throw KeyValueException("not an array.");
	m_variant.arr->set(index, pObj);
}

bool
CVSNKVObj::ContainsKey(const char * key)
{
	if (m_type != T_MAP) return false;
	return m_variant.map->ContainsKey(key);
}

int
CVSNKVObj::length()
{
	switch (m_type) {
	case T_ARR: return m_variant.arr->length();
	case T_STR: return strlen(m_variant.str);
	case T_MAP: return m_variant.map->items();
	default: return 1;
	}
}

int
CVSNKVObj::txtlen()
{
	switch (m_type) {
	case T_ARR: return m_variant.arr->txtlen();
	case T_MAP: return m_variant.map->txtlen();
	case T_STR: return strlen(m_variant.str) + 2;
	case T_BOOL: return (m_variant.b) ? 4 : 5;
	case T_INT: {
		char buf[30];
		sprintf(buf, "%d", m_variant.i);
		return strlen(buf);
	}
	case T_NUM: {
		char buf[1024];
		sprintf(buf, "%e", m_variant.n);
		return strlen(buf);
	}
	case T_NULL: return 4;
	default: return 0;
	}
}

int
CVSNKVObj::ToJSON(char * buf, int index)
{
	switch (m_type) {
	case T_NULL: strcpy(buf + index, "null"); break;
	case T_BOOL: strcpy(buf + index, (m_variant.b) ? "true" : "false");	break;
	case T_INT: sprintf(buf + index, "%d", m_variant.i); break;
	case T_NUM: sprintf(buf + index, "%e", m_variant.n); break;
	case T_STR: sprintf(buf + index, "\"%s\"", m_variant.str); break;
	case T_ARR: m_variant.arr->ToJSON(buf, index); break;
	case T_MAP: m_variant.map->ToJSON(buf, index); break;
	}
	return strlen(buf + index);
}

CVSNKVObj::operator bool()
{
	switch (m_type) {
	case T_BOOL: return m_variant.b;
	case T_INT: return (m_variant.i != 0);
	case T_NUM: return (m_variant.n != 0.0f);
	case T_STR:
		if (!strcmp(m_variant.str, "true")) return true;
		if (!strcmp(m_variant.str, "false")) return false;
	default: throw KeyValueException("could not cast to boolean.");
	}
}
CVSNKVObj::operator int()
{
	switch (m_type) {
	case T_BOOL: return (m_variant.b) ? 1 : 0;
	case T_INT: return m_variant.i;
	case T_NUM: return (int)m_variant.n;
	case T_STR: {
		char * errptr;
		long int ret = strtol(m_variant.str, &errptr, 0);
		if (*errptr == '\0') return (int)ret;
	}
	default: throw KeyValueException("could not cast to integer.");
	}
}

CVSNKVObj::operator double()
{
	switch (m_type) {
	case T_BOOL: return (m_variant.b) ? 1.0f : 0.0f;
	case T_INT: return (double)m_variant.i;
	case T_NUM: return m_variant.n;
	case T_STR: {
		char * errptr;
		double ret = strtod(m_variant.str, &errptr);
		if (*errptr == '\0') return ret;
	}
	default: throw KeyValueException("could not cast to number.");
	}
}

CVSNKVObj::operator float()
{
	return (float)(double)*this;
}

CVSNKVObj::operator const char *()
{
	static char buf[1024];
	switch (m_type) {
	case T_BOOL: return (m_variant.b) ? "true" : "false";
	case T_INT: sprintf(buf, "%d", m_variant.i);	return buf;
	case T_NUM: sprintf(buf, "%f", m_variant.n);	return buf;
	case T_STR: return m_variant.str;
	default: throw KeyValueException("could not cast to string.");
	}
}


CVSNKVObj *
CVSNKVObj::Map()
{
	CVSNKVObj * pObj = 0;
	CVSNKVMap * pMap = 0;
	try {
		pObj = new CVSNKVObj();
		pMap = new CVSNKVMap();
		pObj->m_type = T_MAP;
		pObj->m_variant.map = pMap;
		return pObj;
	}
	catch (std::bad_alloc& ex) {
		delete pMap;
		delete pObj;
		throw ex;
	}
}

CVSNKVObj *
CVSNKVObj::Array()
{
	CVSNKVObj * pObj = 0;
	CVSNKVArray * pArray = 0;
	try {
		pObj = new CVSNKVObj();
		pArray = new CVSNKVArray();
		pObj->m_type = T_ARR;
		pObj->m_variant.arr = pArray;
		return pObj;
	}
	catch (std::bad_alloc& ex) {
		delete pArray;
		delete pObj;
		throw ex;
	}
}

