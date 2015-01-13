#ifndef CVSNPlatform_h
#define CVSNPlatform_h

#include "CVSNObj.h"

class IVSNModule;

enum PFCATEGORY {
	PF_STORAGE = 0,		// storage data access(file system/memory archive/etc.)
	PF_PROC,			// process control
	PF_SOUND,			// BGM/SE sound devices
	PF_POINTING,		// mouse / touch panel 
	PF_PERIPHERAL,		// game controllers
	PF_SENSOR,			// 3-axis acceleration, and other.
	PF_UNIQUE,			// unique devices.

	PF_NUMS
};

class CVSNPlatform
{
	friend class IVSNModule;
public:

private:
	CVSNPlatform();
	~CVSNPlatform();
public:
	static CVSNPlatform& getInstance();

	bool registModule(IVSNModule * pModule);

	IVSNModule * getModule(PFCATEGORY category, const char * ID);

	template <typename T>
	inline T * Module(PFCATEGORY category, const char * ID) {
		return (T *)getModule(category, ID);
	}


private:
	struct MODULES {
		IVSNModule	*	begin;
		IVSNModule	*	end;
	};

	MODULES		m_modules[PF_NUMS];
};

class IVSNModule : public CVSNObj
{
	friend class CVSNPlatform;
private:
	PFCATEGORY		m_category;
	const char	*	m_ID;
	int				m_hash;

	IVSNModule	*	m_prev;
	IVSNModule	*	m_next;
public:
	IVSNModule(PFCATEGORY category, const char * ID);
	virtual ~IVSNModule();

	inline const PFCATEGORY	getCategory() const { return m_category; }
	inline const char *		getID() const { return m_ID; }
	inline int				getHash() const { return m_hash; }

	virtual bool init() = 0;
};

#endif // CVSNPlatform_h
