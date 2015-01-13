#ifndef CVSNScript_h
#define CVSNScript_h

class CVSNScriptArgs;			// �����^
class CVSNScriptCallback;		// �R�[���o�b�N�^

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

	// �X�N���v�g�V�X�e�����̏��������L�q����B
	// �V�X�e������������A���P�[�^�֐��|�C���^���n�����B
	virtual bool initScriptSystem(
		void * (*funcMemAlloc)(size_t size),
		void * (*funcMemReAlloc)(void * p, size_t size),
		void(*funcMemFree)(void * p)
	) = 0;

	// �X�N���v�g�����ɈӖ��̂���萔���`����
	virtual bool defineConstInt(const char * constName, int value) = 0;
	virtual bool defineConstString(const char * constName, const char * value) = 0;

	// ���[�h�����X�N���v�g�̏������������L�q����
	virtual bool initScript() = 0;

	// ���t���[���s���ׂ��X�N���v�g�̏������L�q����B
	virtual bool execUpdate(int deltaT) = 0;

	// �X�N���v�g������ۂɍs���������L�q����B
	virtual bool leaveScript() = 0;

	// �f�o�C�X�ɂ���ăQ�[�����o�b�N�O���E���h�ɉ񂳂ꂽ�ꍇ�̏���
	virtual bool pauseScript() = 0;

	// �o�b�N�O���E���h����t�H�A�O���E���h�ɕ��A�����ꍇ�̏���
	virtual bool resumeScript() = 0;

protected:
	const char * makeScriptPath(const char * scriptName);
	void destroyScriptPath(const char * scriptPath);

	// �X�N���v�g�t�@�C���̃��[�h�������L�q����
	virtual bool loadScriptFullPath(const char * fullPath, const char * scriptName) = 0;

private:
	char *		m_scriptPath;
	char *		m_loadReserve;
};

// �X�N���v�g�����^: �X�N���v�g����Ă΂��Ƃ��A���邢�̓X�N���v�g�ɃR�[���o�b�N��Ԃ����ɗp������B
class CVSNScriptArgs : public CVSNObj
{
protected:
	CVSNScriptArgs();
	virtual ~CVSNScriptArgs();

public:
	virtual int argc() = 0;		// �����̐��擾

	// �l�̐ݒ�
	virtual int addBool(bool v) = 0;
	virtual int addInt(int v) = 0;
	virtual int addFloat(float v) = 0;
	virtual int addString(const char * v) = 0;
	virtual int addTable(CVSNKVObj * v) = 0;

	// �l�̎擾
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
	// �Ăяo������
	virtual void precall() = 0;

	// �R�[���o�b�N�ɗ^���邽�߂̈����N���X�����
	virtual CVSNScriptArgs& args() = 0;

	// �R�[���o�b�N�̌Ăяo��
	virtual void call() = 0;
};


#endif // CVSNScript_h
