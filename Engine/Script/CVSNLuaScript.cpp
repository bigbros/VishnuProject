#include <string.h>
#include "CVSNScriptTask.h"
#include "CVSNLuaScript.h"
#include "IVSNScriptUtilFunc.h"
#include "platform.h"

#define CALLBACK_TABLE "_CALLBACK"
#define TASK_TABLE "_TASK"


extern "C" {

	static void * vsnLuaAlloc(void * ud, void * ptr, size_t osize, size_t nsize)
	{
		CVSNLuaScript * pEnv = (CVSNLuaScript *)ud;

		if (nsize) {
			// realloc
			return pEnv->realloc(ptr, nsize);
		}
		else {
			// free
			pEnv->free(ptr);
			return 0;
		}
	}

	static const char * vsnLuaReader(lua_State * L, void * data, size_t * size)
	{
		CVSNLuaScript * pEnv = (CVSNLuaScript *)data;
		size_t newsize = pEnv->getScriptSize();
		if (!newsize) {
			*size = 0;
			return 0;
		}
		char * buf = (char *)pEnv->getScriptBuf();
		*size = newsize;
		pEnv->readScriptSize(newsize);
		return (const char *)buf;
	}

	// タスクのインスタンスが消える(CVSNScriptTaskのデストラクタのタイミング)際呼び出され、
	// 対応するuserdataを保持しているTASK_TABLEの要素に nil を放り込む。他でこのuserdataを保持していなければ
	// GCの餌食となり、対応領域は消える。
	static void vsnLuaRemove(void * pInfo)
	{
		LuaTASKINFO * info = (LuaTASKINFO *)pInfo;
		lua_State * L = info->L;
		lua_getglobal(L, TASK_TABLE);
		lua_pushinteger(L, info->index);
		lua_pushnil(L);
		lua_settable(L, -3);
		lua_pop(L, 1);
	}

	static int vsnDebugLog(lua_State * L)
	{
		CVSNLuaScriptArgs args(L);
		const char * msg = args.getString(0);
		LOG(msg);
		return 0;
	}

	static int vsnLuaCreateTask(lua_State * L)
	{
		CVSNLuaScriptArgs args(L);	// 残ると困るのでスタック上に生成
		const char * taskName = args.getString(0);
		CVSNScriptTask * pTask = IFactory::createTask(taskName, args);

		if (pTask) {
			lua_Integer taskId = (lua_Integer)pTask->getTaskID();
			lua_getglobal(L, TASK_TABLE);
			lua_pushinteger(L, taskId);
			LuaTASKINFO * info = (LuaTASKINFO *)lua_newuserdata(L, sizeof(LuaTASKINFO));
			info->info.remove = vsnLuaRemove;
			info->L = L;
			info->index = taskId;
			pTask->attachScriptData((CVSNScriptTask::TASKINFO *)info);

			lua_settable(L, -3);

			lua_pushinteger(L, info->index);
			lua_gettable(L, -2);
			lua_remove(L, -2);
		}
		else {
			lua_pushnil(L);
		}
		return 1;
	}

	static int vsnLuaKillTask(lua_State * L)
	{
		int count = 0;
		CVSNLuaScriptArgs args(L);	// 残ると困るのでスタック上に生成
		for (int i = 0; i < args.argc(); i++) {
			CVSNScriptTask * pTask = args.getTask(i);
			if (pTask) {
				pTask->kill();
				count++;
			}
		}
		lua_pushinteger(L, (lua_Integer)count);
		return 1;
	}

	static int vsnLuaCommand(lua_State * L)
	{
		CVSNLuaScriptArgs args(L);		// 残ると困るのでスタック上に生成
		CVSNLuaScriptArgs ret(L, true);	// 戻り値用
		CVSNScriptTask * pTask = args.getTask(0);
		int count = 0;
		if (pTask) {
			if(pTask->command(args, ret)) count = ret.argc();
		}
		return count;
	}

	static int vsnUtility(lua_State * L)
	{
		CVSNLuaScriptArgs args(L);
		CVSNLuaScriptArgs ret(L, true);
		const char * funcName = args.getString(0);
		int count = 0;
		if (IVSNScriptUtilFunc::execUtilFunc(funcName, args, ret)) {
			count = ret.argc();
		}
		return count;
	}

	static int vsnLuaLoadScript(lua_State * L)
	{
		CVSNLuaScriptArgs args(L);
		CVSNLuaScript * scriptSystem = CVSNLuaScript::findScriptEnv(L);

		const char * scriptName = args.getString(0);
		bool result = scriptSystem->reserveLoadScript(scriptName);
		lua_pushboolean(L, (result) ? 1 : 0);
		return 1;
	}

	static int vsnLuaInclude(lua_State * L)
	{
		CVSNLuaScriptArgs args(L);
		CVSNLuaScript * scriptSystem = CVSNLuaScript::findScriptEnv(L);
		const char * scriptName = args.getString(0);
		bool result = scriptSystem->loadScript(scriptName);
		lua_pushboolean(L, (result) ? 1 : 0);
		return 1;
	}
}
CVSNLuaScript * CVSNLuaScript::ms_begin;
CVSNLuaScript * CVSNLuaScript::ms_end;


CVSNLuaScript::CVSNLuaScript()
: CVSNScript()
, m_funcMemAlloc(0)
, m_funcMemReAlloc(0)
, m_funcMemFree(0)
, m_storage(CVSNPlatform::getInstance().Module<CVSNStorage>(PF_STORAGE, "DATA"))
, m_scriptBuf(0)
{
	m_prev = ms_end;
	if (m_prev) {
		m_prev->m_next = this;
	}
	else {
		ms_begin = this;
	}
	ms_end = this;
}

CVSNLuaScript::~CVSNLuaScript()
{
	if (m_prev){
		m_prev->m_next = m_next;
	}
	else {
		ms_begin = m_next;
	}
	if (m_next) {
		m_next->m_prev = m_prev;
	}
	else {
		ms_end = m_prev;
	}
}

CVSNLuaScript *
CVSNLuaScript::findScriptEnv(lua_State * L)
{
	for (CVSNLuaScript * pEnv = ms_begin; pEnv; pEnv = pEnv->m_next) {
		if (pEnv->m_luaState == L) return pEnv;
	}
	return 0;
}

bool
CVSNLuaScript::initScriptSystem(void * (*funcMemAlloc)(size_t size), void * (*funcMemReAlloc)(void * p, size_t size), void(*funcMemFree)(void * p))
{
	m_funcMemAlloc = funcMemAlloc;
	m_funcMemReAlloc = funcMemReAlloc;
	m_funcMemFree = funcMemFree;

	m_luaState = lua_newstate(vsnLuaAlloc, this);
	luaL_openlibs(m_luaState);

	// タスク関数登録
	lua_register(m_luaState, "Include", vsnLuaInclude);
	lua_register(m_luaState, "LoadScript", vsnLuaLoadScript);
	lua_register(m_luaState, "DebugLog", vsnDebugLog);

	lua_register(m_luaState, "vsnTaskCreate", vsnLuaCreateTask);
	lua_register(m_luaState, "vsnTaskKill", vsnLuaKillTask);
	lua_register(m_luaState, "vsnTaskCmd", vsnLuaCommand);
	lua_register(m_luaState, "vsnUtil", vsnUtility);

	// Luaは値に nil を放り込むとオブジェクトがGCによって回収されてしまうので「NULL値」を表現できない。
	// なので、最初に一つ "NULL" という名称で、NULLへのポインタを持つlightuserdataを定義しておく。
	lua_pushlightuserdata(m_luaState, 0);
	lua_setglobal(m_luaState, "NULL");

	// 定数定義
	CVSNDefines::defineAllConst(this);

	lua_newtable(m_luaState);
	lua_setglobal(m_luaState, TASK_TABLE);

	lua_newtable(m_luaState);
	lua_setglobal(m_luaState, CALLBACK_TABLE);

	return (m_luaState) ? true : false;
}


// スクリプト環境側に意味のある定数を定義する
bool
CVSNLuaScript::defineConstInt(const char * constName, int value)
{
	lua_pushinteger(m_luaState, (lua_Integer)value);
	lua_setglobal(m_luaState, constName);
	return true;
}

bool
CVSNLuaScript::defineConstString(const char * constName, const char * value)
{
	lua_pushstring(m_luaState, value);
	lua_setglobal(m_luaState, constName);
	return true;
}

// スクリプトファイルのロード処理を記述する
bool
CVSNLuaScript::loadScriptFullPath(const char * fullPath, const char * scriptName)
{
	static struct {
		const char * suffix;
		bool		 is_bin;
	} suffix[] = {
			{ ".lua", false },
			{ ".lc", true },
			{ 0, false }
	};
	char * tmpName = (char *)m_funcMemAlloc(strlen(fullPath) + 5);

	int suffix_index = 0;
	size_t size;
	const char * chunk;
	bool is_bin = false;
	do {
		strcpy(tmpName, fullPath);
		strcat(tmpName, suffix[suffix_index].suffix);
		is_bin = suffix[suffix_index].is_bin;
		// バッファにチャンクを読み込む
		chunk = (suffix[suffix_index].is_bin)
			? (const char *)m_storage->readData(tmpName, &size)
			: (const char *)m_storage->readText(tmpName, &size);
		suffix_index++;
	} while (!chunk && suffix[suffix_index].suffix);
	if (!chunk) return false;
	m_scriptBuf = (void *)chunk;
	m_scriptSize = (is_bin) ? size :  strlen(chunk);
	m_scriptRead = 0;

	for (const char * ptr = chunk; *ptr; ptr++) {
		if (*ptr == 0x1a) {
			LOG("find EOF");
			*(char *)ptr = 0;
		}
	}

	int status = lua_load(m_luaState, vsnLuaReader, this, scriptName, "bt");
	m_storage->closeData(m_scriptBuf);
	m_scriptBuf = 0;
	bool result = procLuaState(m_luaState, status);

	if (result) {
		status = lua_pcall(m_luaState, 0, 0, 0);
		result = procLuaState(m_luaState, status);
	}
	return result;
}

// ロードしたスクリプトの初期化処理を記述する
bool
CVSNLuaScript::initScript()
{
	lua_getglobal(m_luaState, "Init");
	int status = lua_pcall(m_luaState, 0, 1, 0);
	bool result = procLuaState(m_luaState, status);
	if (result) {
		int ret = lua_toboolean(m_luaState, 1);
		lua_pop(m_luaState, 1);
		result = (ret != 0);
	}
	return result;
}

// 毎フレーム行うべきスクリプトの処理を記述する。
bool
CVSNLuaScript::execUpdate(int deltaT)
{
	lua_getglobal(m_luaState, "Execute");
	lua_pushinteger(m_luaState, (lua_Integer)deltaT);
	int status = lua_pcall(m_luaState, 1, 0, 0);
	bool result = procLuaState(m_luaState, status);
	return result;
}


// スクリプトを去る際に行う処理を記述する。
bool
CVSNLuaScript::leaveScript()
{
	lua_getglobal(m_luaState, "Leave");
	int status = lua_pcall(m_luaState, 0, 0, 0);
	bool result = procLuaState(m_luaState, status);
	return result;
}

bool
CVSNLuaScript::pauseScript()
{
	lua_getglobal(m_luaState, "OnPause");
	int status = lua_pcall(m_luaState, 0, 0, 0);
	bool result = procLuaState(m_luaState, status);
	return result;
}

bool
CVSNLuaScript::resumeScript()
{
	lua_getglobal(m_luaState, "OnResume");
	int status = lua_pcall(m_luaState, 0, 0, 0);
	bool result = procLuaState(m_luaState, status);
	return result;
}

bool
CVSNLuaScript::procLuaState(lua_State * luaState, int status)
{
	switch (status)
	{
	case LUA_OK: return true;
	case LUA_ERRSYNTAX: break;
	case LUA_ERRRUN: break;
	case LUA_ERRERR: break;
	case LUA_ERRMEM: break;
	case LUA_ERRGCMM: break;
	default: break;
	}
	// エラーメッセージが積まれている
	const char * errmsg = lua_tostring(luaState, 1);
	char buf[1024];
	sprintf(buf, "Lua script error: %s", errmsg);
	LOG(buf);
	return false;
}


CVSNLuaScriptArgs::CVSNLuaScriptArgs(lua_State * luaState, bool isCallback)
	: CVSNScriptArgs()
	, m_luaState(luaState)
	, m_setCount(0)
{
	if (!isCallback) m_setCount = lua_gettop(luaState);
}

CVSNLuaScriptArgs::~CVSNLuaScriptArgs() {}

int
CVSNLuaScriptArgs::argc()
{
	return m_setCount;
}

int
CVSNLuaScriptArgs::addBool(bool v)
{
	lua_pushboolean(m_luaState, v ? 1 : 0);
	return m_setCount++;
}

int
CVSNLuaScriptArgs::addInt(int v)
{
	lua_pushinteger(m_luaState, (lua_Integer)v);
	return m_setCount++;
}

int
CVSNLuaScriptArgs::addFloat(float v)
{
	lua_pushnumber(m_luaState, (lua_Number)v);
	return m_setCount++;
}

int
CVSNLuaScriptArgs::addString(const char * v)
{
	lua_pushstring(m_luaState, v);
	return m_setCount++;
}

int
CVSNLuaScriptArgs::addTable(CVSNKVObj * v)
{
	put_value(*v);
	return m_setCount++;
}

void
CVSNLuaScriptArgs::put_value(CVSNKVObj& kvobj)
{
	switch (kvobj.type()) {
	case CVSNKVObj::T_ARR: {
		lua_newtable(m_luaState);
		for (int i = 0; i < kvobj.length(); i++) {
			// Lua の配列は 1 から始まるので、+1 した値を用いる。
			lua_pushinteger(m_luaState, (lua_Integer)(i + 1));
			put_value(kvobj[i]);
			lua_settable(m_luaState, -3);
		}
		return;
	}
	case CVSNKVObj::T_MAP: {
		lua_newtable(m_luaState);
		CVSNKVMap * pMap = kvobj.getMap();
		void * p = pMap->begin();
		while (p) {
			const char * key = pMap->getKey(p);
			lua_pushstring(m_luaState, key);
			put_value(pMap->getValue(p));
			lua_settable(m_luaState, -3);
			p = pMap->next(p);
		}
		return;
	}
	case CVSNKVObj::T_BOOL: lua_pushboolean(m_luaState, (bool)kvobj ? 1 : 0); break;
	case CVSNKVObj::T_INT: lua_pushinteger(m_luaState, (lua_Integer)(int)kvobj); break;
	case CVSNKVObj::T_NUM: lua_pushnumber(m_luaState, (lua_Number)(double)kvobj); break;
	case CVSNKVObj::T_STR: lua_pushstring(m_luaState, (const char *)kvobj); break;
	case CVSNKVObj::T_NULL: lua_getglobal(m_luaState, "NULL"); break;	// 最初に NULL として定義した値
	}
}

// 値の取得
bool
CVSNLuaScriptArgs::getBool(int idx)
{
	int ret = lua_toboolean(m_luaState, idx + 1);
	return (ret != 0);
}

int
CVSNLuaScriptArgs::getInt(int idx)
{
	lua_Integer ret = lua_tointeger(m_luaState, idx + 1);
	return (int)ret;
}

float
CVSNLuaScriptArgs::getFloat(int idx)
{
	lua_Number ret = lua_tonumber(m_luaState, idx + 1);
	return (float)ret;
}

const char *
CVSNLuaScriptArgs::getString(int idx)
{
	const char * ret = lua_tostring(m_luaState, idx + 1);
	return ret;
}

CVSNKVObj *
CVSNLuaScriptArgs::getTable(int idx)
{
	lua_pushvalue(m_luaState, idx + 1);
	CVSNKVObj * table = get_value();
	lua_pop(m_luaState, 1);
	return table;
}

CVSNKVObj *
CVSNLuaScriptArgs::get_value()
{
	CVSNKVObj * obj = 0;
	int type = lua_type(m_luaState, -1);
	if (type == LUA_TTABLE) {
		bool is_array = false;
		lua_pushnil(m_luaState);
		while (lua_next(m_luaState, -2)) {
			// 最初に拾った key のtypeが LUA_TNUMBER であればarray, LUA_TSTRINGであればmap扱い。
			// 既に確定しているtypeと異なれば例外を発する。
			if (!obj) {
				int type = lua_type(m_luaState, -2);
				switch (type) {
				case LUA_TNUMBER: obj = CVSNKVObj::Array();	is_array = true;	 break;
				case LUA_TSTRING: obj = CVSNKVObj::Map();	is_array = false;	 break;
				}
			}
			CVSNKVObj * val = get_value();
			if (is_array) {
				int index = lua_tointeger(m_luaState, -2);
				obj->set(index - 1, val);	// Lua の配列は 1 から始まるので、-1 した値を index として用いる。
			}
			else {
				const char * key = lua_tostring(m_luaState, -2);
				obj->add(key, val);
			}
			lua_pop(m_luaState, 1);
		}
		return obj;
	}
	obj = new CVSNKVObj();
	bool is_boolean = (type == LUA_TBOOLEAN);
	bool is_number = (type == LUA_TNUMBER);
	bool is_string = (type == LUA_TSTRING);
	bool is_null = (type == LUA_TNIL) || ((type == LUA_TLIGHTUSERDATA) && ((lua_topointer(m_luaState, -1) == 0)));
	lua_Number tmpNumber = (is_number) ? lua_tonumber(m_luaState, -1): 0.0;
	bool is_integer = !is_null && is_number && (tmpNumber == ((int)tmpNumber));

	if (is_boolean) {
		*obj = (0 != lua_toboolean(m_luaState, -1));
	}
	else if (is_integer) {
		*obj = (int)tmpNumber;
	}
	else if (is_number) {
		*obj = tmpNumber;
	}
	else if (is_string) {
		*obj = lua_tostring(m_luaState, -1);
	}
	return obj;
}

CVSNScriptTask *
CVSNLuaScriptArgs::getTask(int idx)
{
	CVSNScriptTask::TASKINFO * pInfo = (CVSNScriptTask::TASKINFO *)lua_touserdata(m_luaState, idx + 1);
	if (!pInfo) {
		
		return 0;	// そもそもuserdataではない場合、ここではnullが帰る。未定義変数をタスクとして渡した場合の防御策。
	}
	if (pInfo->objhdr.id != VSNSCROBJ::TASK) return 0;
	CVSNScriptTask * pTask = pInfo->pTask;
	return pTask;
}

CVSNScriptCallback *
CVSNLuaScriptArgs::getCallback(int idx)
{
	// 格納した際のindex値でCVSNLuaScriptCallbackを作る
	CVSNLuaScriptCallback * callback = new CVSNLuaScriptCallback(m_luaState, idx);

	// コールバックを呼び出すときは、LCALLBACK_TABLEで指定されたLuaテーブルから関数の値を取り出してスタックに積んで実行
	return callback;
}

int CVSNLuaScriptCallback::ms_cbKeySeed = 0;

CVSNLuaScriptCallback::CVSNLuaScriptCallback(lua_State * luaState, int idx)
: CVSNScriptCallback()
, m_luaState(luaState)
, m_myArgs(luaState, true)
{
	int cbKey = generateKey();

	m_cbKey = cbKey;
	lua_getglobal(m_luaState, CALLBACK_TABLE);
	lua_pushinteger(m_luaState, cbKey);
	lua_pushvalue(m_luaState, idx + 1);
	lua_settable(m_luaState, -3);
	lua_pop(m_luaState, 1);
}

CVSNLuaScriptCallback::~CVSNLuaScriptCallback()
{
	// このコールバックが不要になったので、Luaテーブル側の対応するindexについて、値をnilにして解放する。
	lua_getglobal(m_luaState, CALLBACK_TABLE);
	lua_pushinteger(m_luaState, m_cbKey);
	lua_pushnil(m_luaState);
	lua_settable(m_luaState, -3);
	lua_pop(m_luaState, 1);
}


void
CVSNLuaScriptCallback::precall()
{
	// これから値を設定させてコールバックを呼ばせるので、まず呼び出し先の関数をスタックに積んでおく。
	lua_getglobal(m_luaState, CALLBACK_TABLE);
	lua_pushinteger(m_luaState, m_cbKey);
	lua_gettable(m_luaState, -2);
}

CVSNScriptArgs&
CVSNLuaScriptCallback::args()
{
	// これでスタック最上位にコールバック関数が積まれていることになる。
	return m_myArgs;
}

void
CVSNLuaScriptCallback::call()
{
	int status = lua_pcall(m_luaState, m_myArgs.argc(), 0, 0);
	CVSNLuaScript::procLuaState(m_luaState, status);
	lua_pop(m_luaState, 1);
	m_myArgs.m_setCount = 0;	// 次回呼び出しのためカウントをリセット
}
