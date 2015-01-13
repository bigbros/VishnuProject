#include <stdio.h>
#include <string.h>
#include "PlatformUtil.h"
#include "CVSNPlatform.h"
#include "CVSNUtil.h"

IVSNModule::IVSNModule(PFCATEGORY category, const char * ID)
: m_category(category)
, m_ID(ID)
, m_prev(0)
, m_next(0)
{
	m_hash = CVSNUtil::strHash(ID);
}

IVSNModule::~IVSNModule()
{
	CVSNPlatform& platform = CVSNPlatform::getInstance();
	if (m_prev) {
		m_prev->m_next = m_next;
	}
	else {
		platform.m_modules[m_category].begin = m_next;
	}
	if (m_next) {
		m_next->m_prev = m_prev;
	}
	else {
		platform.m_modules[m_category].end = m_prev;
	}
}


CVSNPlatform::CVSNPlatform()
{
	for (int i = 0; i < PF_NUMS; i++) {
		m_modules[i].begin = 0;
		m_modules[i].end = 0;
	}
}

CVSNPlatform::~CVSNPlatform()
{
	for (int i = 0; i < PF_NUMS; i++) {
		IVSNModule * pModule = m_modules[i].begin;
		while (pModule) {
			IVSNModule * pNext = pModule->m_next;
			delete pModule;
			pModule = pNext;
		}
	}
}

CVSNPlatform&
CVSNPlatform::getInstance()
{
	static CVSNPlatform instance;
	return instance;
}

bool
CVSNPlatform::registModule(IVSNModule * pModule)
{
	int idx = pModule->m_category;
	pModule->m_prev = m_modules[idx].end;
	if (pModule->m_prev) {
		pModule->m_prev->m_next = pModule;
	}
	else {
		m_modules[idx].begin = pModule;
	}
	m_modules[idx].end = pModule;

	return pModule->init();
}

IVSNModule *
CVSNPlatform::getModule(PFCATEGORY category, const char * ID)
{
	int hash = CVSNUtil::strHash(ID);

	for (IVSNModule * pModule = m_modules[category].begin; pModule; pModule = pModule->m_next) {
		if (pModule->getCategory() == category && pModule->getHash() == hash && !strcmp(pModule->getID(), ID)) {
			return pModule;
		}
	}
	return 0;
}
