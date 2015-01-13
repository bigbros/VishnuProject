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

	// �X�N���v�g�V�X�e�����̏��������L�q����B
	// �V�X�e������������A���P�[�^�֐��|�C���^���n�����B
	bool initScriptSystem(
		void * (*funcMemAlloc)(size_t size),
		void * (*funcMemReAlloc)(void * p, size_t size),
		void(*funcMemFree)(void * p)
		);

	// �X�N���v�g�����ɈӖ��̂���萔���`����
	bool defineConstInt(const char * constName, int value);
	bool defineConstString(const char * constName, const char * value);

	// �X�N���v�g�t�@�C���̃��[�h�������L�q����
	bool loadScriptFullPath(const char * fullPath, const char * scriptName);

	// ���[�h�����X�N���v�g�̏������������L�q����
	bool initScript();

	// ���t���[���s���ׂ��X�N���v�g�̏������L�q����B
	bool execUpdate(int deltaT);

	// �X�N���v�g������ۂɍs���������L�q����B
	bool leaveScript();

	// �f�o�C�X�ɂ��pause���̏���
	bool pauseScript();

	// �f�o�C�X�ɂ��resume���̏���
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

	int argc();		// �����̐��擾

	// �l�̐ݒ�
	int addBool(bool v);
	int addInt(int v);
	int addFloat(float v);
	int addString(const char * v);
	int addTable(CVSNKVObj * v);

	// �l�̎擾
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

	// �Ăяo������
	void precall();

	// �R�[���o�b�N�ɗ^���邽�߂̈����N���X�𓾂�
	CVSNScriptArgs& args();

	// �R�[���o�b�N�̌Ăяo��
	void call();

private:
	lua_State		*	m_luaState;
	int					m_cbKey;

	CVSNLuaScriptArgs	m_myArgs;
};


#endif // CVSNLuaScript_h

