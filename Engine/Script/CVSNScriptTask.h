#ifndef CVSNScriptTask_h
#define CVSNScriptTask_h

#include <memory>
#include <exception>
#include "CVSNTask.h"
#include "CVSNScript.h"


class CVSNScriptTask : public CVSNTask
{
public:
	struct TASKINFO {
		VSNSCROBJ		objhdr;
		CVSNScriptTask * pTask;
		void(*remove)(void * data);
	};
public:
	CVSNScriptTask(CVSNTask::PHASE phase = CVSNTask::P_NORMAL);
	virtual ~CVSNScriptTask();

	bool create(CVSNScriptArgs& args);
	virtual bool command(CVSNScriptArgs& args, CVSNScriptArgs& ret);

	inline void attachScriptData(TASKINFO * pInfo) {
		pInfo->objhdr.id = VSNSCROBJ::TASK;
		pInfo->pTask = this;
		m_scriptData = pInfo;
	}
	inline unsigned int getTaskID() const { return m_taskId; }

protected:
	virtual bool init(CVSNScriptArgs& args) = 0;

private:
	static unsigned int		ms_idSeed;

	unsigned int			m_taskId;
	TASKINFO			*	m_scriptData;
};

class CVSNDefines
{
public:
	struct DEFINE_INT {
		const char *	label;
		int				value;
	};
	struct DEFINE_STR {
		const char *	label;
		const char *	value;
	};
private:
	static CVSNDefines	*	ms_begin;
	static CVSNDefines	*	ms_end;

	CVSNDefines			*	m_prev;
	CVSNDefines			*	m_next;

	DEFINE_INT	*	m_defInt;
	DEFINE_STR	*	m_defStr;

private:
	bool defineConst(CVSNScript * scriptSystem);

public:
	CVSNDefines(DEFINE_INT * defInt, DEFINE_STR * defStr);
	virtual ~CVSNDefines();

	static bool defineAllConst(CVSNScript * scriptSystem);
};

class IFactory
{
private:
	static IFactory		*	ms_begin;
	static IFactory		*	ms_end;

	IFactory	*	m_prev;
	IFactory	*	m_next;

	const char *	m_taskName;
	int				m_taskNameHash;
public:
	IFactory(const char * taskName);
	virtual ~IFactory();

	virtual CVSNScriptTask * factory(CVSNScriptArgs& args) = 0;
	static CVSNScriptTask * createTask(const char * taskName, CVSNScriptArgs& args);
};

template<class T>
class CVSNFactory : public IFactory
{
public:
	CVSNFactory(const char * taskName) : IFactory(taskName) {}
	~CVSNFactory() {}

	CVSNScriptTask * factory(CVSNScriptArgs& args) {
		try {
			T * pTask = new T();
			if (!pTask->create(args)) {
				delete pTask;
				pTask = 0;
			}
			return pTask;
		}
		catch (std::bad_alloc&) {
			return 0;
		}
	}
};

#endif // CVSNScriptTask_h
