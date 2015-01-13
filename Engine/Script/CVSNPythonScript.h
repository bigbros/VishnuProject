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

	// �Ăяo������
	void precall();

	// �R�[���o�b�N�ɗ^���邽�߂̈����N���X�𓾂�
	CVSNScriptArgs& args();

	// �R�[���o�b�N�̌Ăяo��
	void call();

private:
	CVSNPythonScriptArgs	m_myArgs;
};

#endif // CVSNPythonScript_h
