#ifndef CVSNUnixFS_h
#define CVSNUnixFS_h

#include "CVSNStorage.h"

class CVSNUnixFS : public CVSNStorage
{
public:
	CVSNUnixFS(const char * basePath);
	virtual ~CVSNUnixFS();

	bool init();

	void * readData(const char * name, size_t * size);
	void * readText(const char * name, size_t * size);
	void closeData(void * ptr);

private:
	size_t fileSize(int fd);
	void * loadFile(const char * path, size_t margine, size_t * size);

	const char	*	m_basePath;
};

#endif // CVSNUnixFS_h
