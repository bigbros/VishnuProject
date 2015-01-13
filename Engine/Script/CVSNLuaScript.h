#ifndef CVSNLuaScript_h
#define CVSNLuaScript_h

#include "CVSNScript.h"
#include "CVSNScriptTask.h"
#include "platform.h"

extern "C" {
#include <lua.h>
#include <lualib.h>
}

class CVSNLuaScriptArgs;
class CVSNLuaScriptCallback;

struct LuaTASKINFO {
	CVSNScriptTask::TASKINFO	info;
	lua_State				*	L;
	lua_Integer					index;
};

class CVSNLuaScript : public CVSNScript
{
	friend class CVSNLuaScriptArgs;
	friend class CVSNLuaScriptCallback;
public:
	CVSNLuaScript();
	virtual ~CVSNLuaScript();

	// スクリプトシステム環境の初期化を記述する。
	// システムが推奨するアロケータ関数ポインタが渡される。
	bool initScriptSystem(
		void * (*funcMemAlloc)(size_t size),
		void * (*funcMemReAlloc)(void * p, size_t size),
		void(*funcMemFree)(void * p)
		);

	// スクリプト環境側に意味のある定数を定義する
	bool defineConstInt(const char * constName, int value);
	bool defineConstString(const char * constName, const char * value);

	// スクリプトファイルのロード処理を記述する
	bool loadScriptFullPath(const char * fullPath, const char * scriptName);

	// ロードしたスクリプトの初期化処理を記述する
	bool initScript();

	// 毎フレーム行うべきスクリプトの処理を記述する。
	bool execUpdate(int deltaT);

	// スクリプトを去る際に行う処理を記述する。
	bool leaveScript();

	// デバイスによるpause時の処理
	bool pauseScript();

	// デバイスによるresume時の処理
	bool resumeScript();

	inline	void *	malloc(size_t size) { return m_funcMemAlloc(size); }
	inline	void *	realloc(void * p, size_t size) { return m_funcMemReAlloc(p, size); }
	inline	void	free(void * p) { return m_funcMemFree(p);  }

	inline void * getScriptBuf() const { return (char *)m_scriptBuf + m_scriptRead;  }
	inline size_t getScriptSize() const { return m_scriptSize - m_scriptRead; }
	inline void   readScriptSize(size_t rd) { m_scriptRead += rd;  }

	static CVSNLuaScript * findScriptEnv(lua_State * L);
private:
	static bool procLuaState(lua_State * luaState, int status);

private:

	void *	(*m_funcMemAlloc)(size_t);
	void *	(*m_funcMemReAlloc)(void *, size_t);
	void	(*m_funcMemFree)(void *);

	CVSNStorage		*	m_storage;

	lua_State		*	m_luaState;
	void			*	m_scriptBuf;
	size_t				m_scriptSize;
	size_t				m_scriptRead;

	CVSNLuaScript	*	m_prev;
	CVSNLuaScript	*	m_next;

	static CVSNLuaScript * ms_begin;
	static CVSNLuaScript * ms_end;
};


class CVSNLuaScriptArgs : public CVSNScriptArgs
{
	friend class CVSNLuaScriptCallback;
public:
	CVSNLuaScriptArgs(lua_State * luaState, bool isCallback = false);
	virtual ~CVSNLuaScriptArgs();

	int argc();		// 引数の数取得

	// 値の設定
	int addBool(bool v);
	int addInt(int v);
	int addFloat(float v);
	int addString(const char * v);
	int addTable(CVSNKVObj * v);

	// 値の取得
	bool  getBool(int idx);
	int   getInt(int idx);
	float getFloat(int idx);
	const char * getString(int idx);
	CVSNKVObj * getTable(int idx);

	CVSNScriptTask * getTask(int idx);
	CVSNScriptCallback * getCallback(int idx);

private:
	void put_value(CVSNKVObj& kvobj);
	CVSNKVObj * get_value();

	lua_State		*	m_luaState;
	int					m_setCount;
};

class CVSNLuaScriptCallback : public CVSNScriptCallback
{
	friend class CVSNLuaScriptArgs;
private:
	static int ms_cbKeySeed;
	inline int generateKey() {
		return ++ms_cbKeySeed;
	}
private:
	CVSNLuaScriptCallback(lua_State * luaState, int cbKey);
	virtual ~CVSNLuaScriptCallback();
public:

	// 呼び出し準備
	void precall();

	// コールバックに与えるための引数クラスを得る
	CVSNScriptArgs& args();

	// コールバックの呼び出し
	void call();

private:
	lua_State		*	m_luaState;
	int					m_cbKey;

	CVSNLuaScriptArgs	m_myArgs;
};


#endif // CVSNLuaScript_h

