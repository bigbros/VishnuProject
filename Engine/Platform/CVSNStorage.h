#ifndef CVSNStorage_h
#define CVSNStorage_h

#include "CVSNPlatform.h"

class CVSNStorage : public IVSNModule
{
public:
	CVSNStorage();
	virtual ~CVSNStorage();

	virtual void * readData(const char * name, size_t * size) = 0;
	virtual void * readText(const char * name, size_t * size) = 0;
	virtual void closeData(void * ptr) = 0;
};




#endif // CVSNStorage_h
