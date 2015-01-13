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

	// 最初のスクリプトをロード予約する
	result = result && scriptSystem->reserveLoadScript(startupScript);
	m_leaved = true;
	set_parent(0);
	result = result && birth();
	return result;
}

void
CVSNScriptDriver::update(int deltaT)
{
	// ロード予約されたスクリプトがあれば、
	// update処理前にロードを行いinit処理を呼び出す。
	if (m_leaved && m_scriptSystem->isLoadReserved()) {
		if (m_scriptSystem->loadReservedScript()) {
			m_leaved = false;
			m_scriptSystem->initScript();
		}
	}
	// スクリプト側の毎フレーム処理
	m_scriptSystem->execUpdate(deltaT);

	// execUpdate() の間にロード予約が行われていたら、
	// 現在のスクリプトの leave 処理を行う
	if (m_scriptSystem->isLoadReserved()) {
		m_scriptSystem->leaveScript();
		m_leaved = true;
	}
}

void
CVSNScriptDriver::die()
{
	// ロード予約がされていないフレームに死んだのであればleave処理が行われていないので、
	// leave処理を行ってから死ぬ。
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
