#ifndef CVSNScript_h
#define CVSNScript_h

class CVSNScriptArgs;			// 引数型
class CVSNScriptCallback;		// コールバック型

#include "CVSNObj.h"
#include "CVSNKVObj.h"

struct VSNSCROBJ {
	enum ID {
		TASK,
	};
	ID	id;
};

class CVSNScriptTask;
class CVSNScriptArgs;
class CVSNScriptCallback;

class CVSNScript : public CVSNObj
{
public:
	CVSNScript();
	virtual ~CVSNScript();

	bool setScriptDirectoryPath(const char * dirPath);
	inline const char * getScriptDirectoryPath() const { return m_scriptPath; }

	bool reserveLoadScript(const char * scriptName);
	bool isLoadReserved();
	bool loadReservedScript();

	bool loadScript(const char * scriptName);

	// スクリプトシステム環境の初期化を記述する。
	// システムが推奨するアロケータ関数ポインタが渡される。
	virtual bool initScriptSystem(
		void * (*funcMemAlloc)(size_t size),
		void * (*funcMemReAlloc)(void * p, size_t size),
		void(*funcMemFree)(void * p)
	) = 0;

	// スクリプト環境側に意味のある定数を定義する
	virtual bool defineConstInt(const char * constName, int value) = 0;
	virtual bool defineConstString(const char * constName, const char * value) = 0;

	// ロードしたスクリプトの初期化処理を記述する
	virtual bool initScript() = 0;

	// 毎フレーム行うべきスクリプトの処理を記述する。
	virtual bool execUpdate(int deltaT) = 0;

	// スクリプトを去る際に行う処理を記述する。
	virtual bool leaveScript() = 0;

	// デバイスによってゲームがバックグラウンドに回された場合の処理
	virtual bool pauseScript() = 0;

	// バックグラウンドからフォアグラウンドに復帰した場合の処理
	virtual bool resumeScript() = 0;

protected:
	const char * makeScriptPath(const char * scriptName);
	void destroyScriptPath(const char * scriptPath);

	// スクリプトファイルのロード処理を記述する
	virtual bool loadScriptFullPath(const char * fullPath, const char * scriptName) = 0;

private:
	char *		m_scriptPath;
	char *		m_loadReserve;
};

// スクリプト引数型: スクリプトから呼ばれるとき、あるいはスクリプトにコールバックを返す時に用いられる。
class CVSNScriptArgs : public CVSNObj
{
protected:
	CVSNScriptArgs();
	virtual ~CVSNScriptArgs();

public:
	virtual int argc() = 0;		// 引数の数取得

	// 値の設定
	virtual int addBool(bool v) = 0;
	virtual int addInt(int v) = 0;
	virtual int addFloat(float v) = 0;
	virtual int addString(const char * v) = 0;
	virtual int addTable(CVSNKVObj * v) = 0;

	// 値の取得
	virtual bool  getBool(int idx) = 0;
	virtual int   getInt(int idx) = 0;
	virtual float getFloat(int idx) = 0;
	virtual const char * getString(int idx) = 0;
	virtual CVSNKVObj * getTable(int idx) = 0;

	virtual CVSNScriptTask * getTask(int idx) = 0;
	virtual CVSNScriptCallback * getCallback(int idx) = 0;
};

class CVSNScriptCallback : public CVSNObj
{
protected:
	CVSNScriptCallback();
	virtual ~CVSNScriptCallback();

public:
	// 呼び出し準備
	virtual void precall() = 0;

	// コールバックに与えるための引数クラスを作る
	virtual CVSNScriptArgs& args() = 0;

	// コールバックの呼び出し
	virtual void call() = 0;
};


#endif // CVSNScript_h
