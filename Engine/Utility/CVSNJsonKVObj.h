#ifndef CVSNJsonKVObj_h
#define CVSNJsonKVObj_h

#include "CVSNEngineHeap.h"
#include "CVSNKVObj.h"
#include <yajl/yajl_parse.h>
#include <yajl/yajl_gen.h>

class CVSNJsonKVObj
{
private:
	struct CONTEXT : public CVSNObj {
		struct STACK {
			STACK * next;
			CVSNKVObj * pObj;
			inline void * operator new(size_t s){ return vsnMemAlloc(s); }
			inline void operator delete(void *p) { vsnMemFree(p); }
		};
		CVSNKVObj * pRoot;
		STACK * pCurrent;
		const char * reserve_key;

	private:
		STACK * newStack(CVSNKVObj * pObj);
		void removeStack();

	public:
		CONTEXT();
		virtual ~CONTEXT();
		int add(CVSNKVObj * pObj);
		int setKey(const unsigned char * keyString, int length);
		int removeMap();
		int removeArray();
	};

public:
	static int reformat_null(void * ctx);
	static int reformat_boolean(void * ctx, int boolean);
	static int reformat_number(void * ctx, const char * s, unsigned int l);
	static int reformat_string(void * ctx, const unsigned char * stringVal, unsigned int stringLen);
	static int reformat_map_key(void * ctx, const unsigned char * stringVal, unsigned int stringLen);
	static int reformat_start_map(void * ctx);
	static int reformat_end_map(void * ctx);
	static int reformat_start_array(void * ctx);
	static int reformat_end_array(void * ctx);

	static CVSNKVObj * ReadJSON(const char * jsonText);
	static const char * ToJSON(CVSNKVObj * obj);
};


#endif // CVSNJsonKVObj_h
