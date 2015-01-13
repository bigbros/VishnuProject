#ifndef CVSNPythonScript_h
#define CVSNPythonScript_h

#include "CVSNScript.h"
#include "CVSNScriptTask.h"

class CVSNPythonScriptArgs;
class CVSNPythonScriptCallback;

class CVSNPythonScript : public CVSNScript
{
	friend class CVSNPythonScriptArgs;
	friend class CVSNPythonScriptCallback;
public:
	CVSNPythonScript();
	virtual ~CVSNPythonScript();

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
	inline	void	free(void * p) { return m_funcMemFree(p); }

private:

	void *	(*m_funcMemAlloc)(size_t);
	void *	(*m_funcMemReAlloc)(void *, size_t);
	void(*m_funcMemFree)(void *);
};


class CVSNPythonScriptArgs : public CVSNScriptArgs
{
	friend class CVSNPythonScriptCallback;
public:
	CVSNPythonScriptArgs(bool isCallback = false);
	virtual ~CVSNPythonScriptArgs();

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

	int					m_setCount;
};

class CVSNPythonScriptCallback : public CVSNScriptCallback
{
	friend class CVSNPythonScriptArgs;
private:
	static int ms_cbKeySeed;
	inline int generateKey() {
		return ++ms_cbKeySeed;
	}
private:
	CVSNPythonScriptCallback();
	virtual ~CVSNPythonScriptCallback();
public:

	// 呼び出し準備
	void precall();

	// コールバックに与えるための引数クラスを得る
	CVSNScriptArgs& args();

	// コールバックの呼び出し
	void call();

private:
	CVSNPythonScriptArgs	m_myArgs;
};

#endif // CVSNPythonScript_h
