#ifndef CVSNTask_h
#define CVSNTask_h

#include "CVSNObj.h"

class CVSNTask;
class CVSNTaskMgr;

struct TASKLIST {
	CVSNTask	*	begin;
	CVSNTask	*	end;
};
struct TASKLINK {
	CVSNTask	*	prev;
	CVSNTask	*	next;
};


class CVSNTask : public CVSNObj
{
	friend class CVSNTaskMgr;
public:
	enum PHASE {
		P_INPUT = 0,		// input by periferal
		P_INPUT_HL,			// High level Input
		P_SCRIPT,		// script execution

		P_BEFORE,
		P_NORMAL,
		P_AFTER,

		P_JUDGE,		// Judgement on the frame.

		P_DRAW,

		PHASE_NUMS,

		NOT_REGIST = -1
	};
private:
	CVSNTask(CVSNTask&);
	void operator =(CVSNTask&);

protected:
	CVSNTask(PHASE phase);
	virtual ~CVSNTask();

public:
	bool birth();
	void kill();
	void change_parent(CVSNTask * pTask = 0);

	virtual void update(int deltaT) = 0;
	virtual void die();
	virtual void on_pause();	// pause(for mobile device)
	virtual void on_resume();	// resume(for mobile device)
	virtual void on_change();	// screen size has been changed(portlait <-> landscape, window size or screen resolution, etc.)

	virtual void on_surface();
	virtual void on_detach();

	virtual void on_finish();	// finish program
protected:
	void set_parent(CVSNTask * pParent = 0);
	void link_parent();

private:

	CVSNTask			*	m_pParent;
	PHASE					m_phase;
	bool					m_is_dead;

	TASKLIST				m_childs;
	TASKLINK				m_sisters;

	TASKLINK				m_execute;
	TASKLINK				m_dead;
};

class CVSNTaskMgr
{
	friend class CVSNTask;

private:
	CVSNTaskMgr();
	~CVSNTaskMgr();
public:
	static CVSNTaskMgr& getInstance();

	void Update(int deltaT);

	void onPause();		// pause for mobile devices
	void onResume();	// resume for mobile devices
	void onChange();	// screen size has been changed.

	void onSurface();	// surface recovery.
	void onDetach();	// destruct GL context.

	void onFinish();	// finish program.

private:
	bool registBirth(CVSNTask * pTask);
	void dieReserve(CVSNTask * pTask);
	void execTask(int deltaT);
	void removeTask();

private:
	TASKLIST				m_execute[CVSNTask::PHASE_NUMS];

	TASKLIST				m_die;
};



#endif // CVSNTask_h
