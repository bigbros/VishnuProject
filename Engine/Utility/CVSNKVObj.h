#ifndef CVSNKVObj_h
#define CVSNKVObj_h

#include <memory>
#include "CVSNObj.h"
#include "CVSNUtil.h"

class CVSNKVObj;

class KeyValueException : public std::exception
{
public:
	KeyValueException(const char * msg);
	const char * what() throw();
private:
	const char *	m_msg;
};

class CVSNKVMap : public CVSNObj
{
	friend class CVSNKVObj;
private:
	struct CHAIN {
		CHAIN		*	prev;
		CHAIN		*	next;
		const char	*	key;
		CVSNKVObj	*	pObj;
		int				hash;
	};
	CHAIN	*	m_begin;
	CHAIN	*	m_end;
public:
	CVSNKVMap();
	virtual ~CVSNKVMap();

	void add(const char * key, CVSNKVObj * pObj);
	bool ContainsKey(const char * key);

	CVSNKVObj& operator[](const char * key);

	inline int items() {
		int count = 0;
		for (CHAIN * p = m_begin; p; p = p->next) count++;
		return count;
	}
	int txtlen();
	int ToJSON(char * buf, int index);


	inline void * begin() const { return m_begin; }
	inline void * next(void * p) const { return ((CHAIN *)p)->next; }
	inline void * prev(void * p) const { return ((CHAIN *)p)->prev; }
	inline const char * getKey(void * p) { return ((CHAIN *)p)->key; }
	inline CVSNKVObj& getValue(void * p) { return *(((CHAIN *)p)->pObj); }
};

class CVSNKVArray : public CVSNObj
{
private:
	enum {
		BLOCK = 64
	};
	int				m_max;
	int				m_use;
	CVSNKVObj	**	m_array;
public:
	CVSNKVArray();
	virtual ~CVSNKVArray();

	bool alloc(int size);
	bool set(int index, CVSNKVObj * pObj);
	void add(CVSNKVObj * pObj);

	CVSNKVObj& operator[](int index);

	int txtlen();
	int ToJSON(char * buf, int index);

	inline int length() { return m_use; }
};

class CVSNKVObj : public CVSNObj
{
public:
	enum TYPE {
		T_NULL,
		T_BOOL,
		T_INT,
		T_NUM,
		T_STR,
		T_MAP,
		T_ARR
	};
private:
	union VARIANT {
		bool			b;
		int				i;
		double			n;
		const char	*	str;
		CVSNKVMap	*	map;
		CVSNKVArray	*	arr;
	};

	TYPE		m_type;
	VARIANT		m_variant;

private:
	void clear();
	int strToInt(const char * str);
	double strToNum(const char * str);
public:
	CVSNKVObj();
	CVSNKVObj(bool v);
	CVSNKVObj(int v);
	CVSNKVObj(double v);
	CVSNKVObj(float v);
	CVSNKVObj(const char * v);
	virtual ~CVSNKVObj();

	CVSNKVObj& operator[](const char * key);
	CVSNKVObj& operator[](int index);

	CVSNKVObj& operator=(bool v);
	CVSNKVObj& operator=(int v);
	CVSNKVObj& operator=(double v);
	CVSNKVObj& operator=(float v);
	CVSNKVObj& operator=(const char * v);

	void add(const char * key, CVSNKVObj * pObj);
	void add(CVSNKVObj * pObj);
	void set(int index, CVSNKVObj * pObj);
	int length();

	int txtlen();
	int ToJSON(char * buf, int index);

	bool ContainsKey(const char * key);
	inline bool isMap() const { return (m_type == T_MAP); }
	inline bool isArray() const { return (m_type == T_ARR); }

	operator bool();
	operator int();
	operator double();
	operator float();
	operator const char *();

	inline const TYPE type() const { return m_type; }

	inline CVSNKVMap * getMap() const {
		if (!isMap()) return 0;
		return  m_variant.map;
	}

	static CVSNKVObj * Map();
	static CVSNKVObj * Array();
};

#endif // CVSNKVObj_h
