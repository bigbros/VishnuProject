#include <memory>
#include "CVSNScriptDriver.h"
#include "CVSNEngineHeap.h"

CVSNScriptDriver::CVSNScriptDriver(CVSNScript * pScriptSystem, const char * startupScript)
: CVSNTask(CVSNTask::P_SCRIPT)
, m_scriptSystem(0)
, m_leaved(false)
{
	if (!init(pScriptSystem, startupScript)) throw std::bad_alloc();
}

CVSNScriptDriver::~CVSNScriptDriver() {}

bool
CVSNScriptDriver::init(CVSNScript * scriptSystem, const char * startupScript)
{
	m_scriptSystem = scriptSystem;
	bool result = true;
	result = result && scriptSystem->initScriptSystem(vsnMemAlloc, vsnMemReAlloc, vsnMemFree);

	// �ŏ��̃X�N���v�g�����[�h�\�񂷂�
	result = result && scriptSystem->reserveLoadScript(startupScript);
	m_leaved = true;
	set_parent(0);
	result = result && birth();
	return result;
}

void
CVSNScriptDriver::update(int deltaT)
{
	// ���[�h�\�񂳂ꂽ�X�N���v�g������΁A
	// update�����O�Ƀ��[�h���s��init�������Ăяo���B
	if (m_leaved && m_scriptSystem->isLoadReserved()) {
		if (m_scriptSystem->loadReservedScript()) {
			m_leaved = false;
			m_scriptSystem->initScript();
		}
	}
	// �X�N���v�g���̖��t���[������
	m_scriptSystem->execUpdate(deltaT);

	// execUpdate() �̊ԂɃ��[�h�\�񂪍s���Ă�����A
	// ���݂̃X�N���v�g�� leave �������s��
	if (m_scriptSystem->isLoadReserved()) {
		m_scriptSystem->leaveScript();
		m_leaved = true;
	}
}

void
CVSNScriptDriver::die()
{
	// ���[�h�\�񂪂���Ă��Ȃ��t���[���Ɏ��񂾂̂ł����leave�������s���Ă��Ȃ��̂ŁA
	// leave�������s���Ă��玀�ʁB
	if (!m_scriptSystem->isLoadReserved()) {
		m_scriptSystem->leaveScript();
	}
}

void
CVSNScriptDriver::on_pause()
{
	m_scriptSystem->pauseScript();
}

void
CVSNScriptDriver::on_resume()
{
	m_scriptSystem->resumeScript();
}
