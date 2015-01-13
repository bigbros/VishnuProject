#include <stdlib.h>
#include <string.h>
#include "CVSNJsonKVObj.h"
#include "CVSNEngineHeap.h"


CVSNJsonKVObj::CONTEXT::STACK *
CVSNJsonKVObj::CONTEXT::newStack(CVSNKVObj * pObj) {
		STACK * item = new STACK();
		item->next = pCurrent;
		pCurrent = item;
		item->pObj = pObj;
		if (!item->next) pRoot = pObj;
		return item;
	}

void
CVSNJsonKVObj::CONTEXT::removeStack()
{
	STACK * item = pCurrent;
	pCurrent = item->next;
	delete item;
}

CVSNJsonKVObj::CONTEXT::CONTEXT() : CVSNObj(), pRoot(0), pCurrent(0), reserve_key(0) {}
CVSNJsonKVObj::CONTEXT::~CONTEXT() {
		delete[] reserve_key;
		STACK * next;
		while (pCurrent) {
			next = pCurrent->next;
			delete pCurrent;
			pCurrent = next;
		}
	}

int
CVSNJsonKVObj::CONTEXT::add(CVSNKVObj * pObj)
{
	if (!pCurrent) {
		newStack(pObj);
		return 1;
	}
	if (reserve_key && pCurrent->pObj->isMap()) {
		pCurrent->pObj->add(reserve_key, pObj);
		vsnMemFree((char *)reserve_key);
		reserve_key = 0;
	}
	else if (pCurrent->pObj->isArray()) {
		pCurrent->pObj->add(pObj);
	}
	else {
		return 0;
	}

	if (pObj->isMap() || pObj->isArray()) {
		newStack(pObj);
	}
	return 1;
}

int
CVSNJsonKVObj::CONTEXT::setKey(const unsigned char * keyString, int length)
{
	if (!pCurrent->pObj->isMap()) return 0;

	char * buf = (char *)vsnMemAlloc(length + 1);
	strncpy(buf, (const char *)keyString, length);
	buf[length] = 0;
	reserve_key = buf;
	return 1;
}

int
CVSNJsonKVObj::CONTEXT::removeMap() {
	if (pCurrent->pObj->isMap()) {
		removeStack();
		return 1;
	}
	return 0;
}

int
CVSNJsonKVObj::CONTEXT::removeArray() {
	if (pCurrent->pObj->isArray()) {
		removeStack();
		return 1;
	}
	return 0;
}

extern "C" {
	static int reformat_null(void * ctx) {
		return CVSNJsonKVObj::reformat_null(ctx);
	}

	static int reformat_boolean(void * ctx, int boolean) {
		return CVSNJsonKVObj::reformat_boolean(ctx, boolean);
	}

	static int reformat_number(void * ctx, const char * s, unsigned int l) {
		return CVSNJsonKVObj::reformat_number(ctx, s, l);
	}

	static int reformat_string(void * ctx, const unsigned char * stringVal, unsigned int stringLen) {
		return CVSNJsonKVObj::reformat_string(ctx, stringVal, stringLen);
	}

	static int reformat_map_key(void * ctx, const unsigned char * stringVal, unsigned int stringLen)
	{
		return CVSNJsonKVObj::reformat_map_key(ctx, stringVal, stringLen);
	}

	static int reformat_start_map(void * ctx)
	{
		return CVSNJsonKVObj::reformat_start_map(ctx);
	}

	static int reformat_end_map(void * ctx)
	{
		return CVSNJsonKVObj::reformat_end_map(ctx);
	}

	static int reformat_start_array(void * ctx)
	{
		return CVSNJsonKVObj::reformat_start_array(ctx);
	}

	static int reformat_end_array(void * ctx)
	{
		return CVSNJsonKVObj::reformat_end_array(ctx);
	}

	static yajl_callbacks callbacks = {
		reformat_null,
		reformat_boolean,
		0,
		0,
		reformat_number,
		reformat_string,
		reformat_start_map,
		reformat_map_key,
		reformat_end_map,
		reformat_start_array,
		reformat_end_array
	};
}

int
CVSNJsonKVObj::reformat_null(void * ctx)
{
	CONTEXT * cont = (CONTEXT *)ctx;
	CVSNKVObj * pObj = new CVSNKVObj();
	return cont->add(pObj);
}

int
CVSNJsonKVObj::reformat_boolean(void * ctx, int boolean)
{
	CONTEXT * cont = (CONTEXT *)ctx;
	CVSNKVObj * obj = new CVSNKVObj();
	*obj = (boolean != 0);
	return cont->add(obj);
}

int
CVSNJsonKVObj::reformat_number(void * ctx, const char * s, unsigned int l)
{
	CONTEXT * cont = (CONTEXT *)ctx;
	char * buf = (char *)vsnMemAlloc(l + 1);
	strncpy(buf, s, l);
	buf[l] = 0;

	CVSNKVObj * obj = new CVSNKVObj();
	char * errptr;
	long int ret = strtol(buf, &errptr, 0);
	if (*errptr == '\0') {
		*obj = (int)ret;
	}
	else {
		double dret = strtod(buf, &errptr);
		if (*errptr == '\0') {
			*obj = dret;
		}
	}
	vsnMemFree(buf);
	return cont->add(obj);
}

int
CVSNJsonKVObj::reformat_string(void * ctx, const unsigned char * stringVal, unsigned int stringLen)
{
	CONTEXT * cont = (CONTEXT *)ctx;
	char * buf = (char *)vsnMemAlloc(stringLen + 1);
	strncpy(buf, (const char *)stringVal, stringLen);
	buf[stringLen] = 0;
	CVSNKVObj * obj = new CVSNKVObj();
	*obj = buf;
	vsnMemFree(buf);
	return cont->add(obj);
}

int
CVSNJsonKVObj::reformat_map_key(void * ctx, const unsigned char * stringVal, unsigned int stringLen)
{
	CONTEXT * cont = (CONTEXT *)ctx;
	return cont->setKey(stringVal, stringLen);
}

int
CVSNJsonKVObj::reformat_start_map(void * ctx)
{
	CONTEXT * cont = (CONTEXT *)ctx;
	CVSNKVObj * map = CVSNKVObj::Map();
	return cont->add(map);
}

int
CVSNJsonKVObj::reformat_end_map(void * ctx)
{
	CONTEXT * cont = (CONTEXT *)ctx;
	return cont->removeMap();
}

int
CVSNJsonKVObj::reformat_start_array(void * ctx)
{
	CONTEXT * cont = (CONTEXT *)ctx;
	CVSNKVObj * arr = CVSNKVObj::Array();
	return cont->add(arr);
}

int
CVSNJsonKVObj::reformat_end_array(void * ctx)
{
	CONTEXT * cont = (CONTEXT *)ctx;
	return cont->removeArray();
}

CVSNKVObj *
CVSNJsonKVObj::ReadJSON(const char * jsonText)
{
	yajl_handle hand;
	yajl_parser_config cfg = {1, 1};
	yajl_status stat;

	CONTEXT context;

	int length = strlen(jsonText);

	hand = yajl_alloc(&callbacks, &cfg, 0, (void *)&context);

	try {
		stat = yajl_parse(hand, (const unsigned char *)jsonText, length);
		if (stat != yajl_status_ok) throw KeyValueException("JSON parse error.");
	}
	catch (KeyValueException&) {
		delete context.pCurrent;
		context.pCurrent = 0;
	}
	yajl_free(hand);
	return context.pRoot;
}

const char *
CVSNJsonKVObj::ToJSON(CVSNKVObj * obj)
{
	int len = obj->txtlen();	// テキストの長さを得る
	char * buf = (char *)vsnMemAlloc(len + 1);
	if (len != obj->ToJSON(buf, 0)) {
		vsnMemFree(buf);
		return 0;
	}
	return buf;
}
