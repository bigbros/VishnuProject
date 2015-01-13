#ifndef CVSNWin32FS_h
#define CVSNWin32FS_h

#include "CVSNStorage.h"

class CVSNWin32FS : public CVSNStorage
{
public:
	CVSNWin32FS(const char * basePath);
	virtual ~CVSNWin32FS();

	bool init();

	void * readData(const char * name, size_t * size);
	void * readText(const char * name, size_t * size);
	void closeData(void * ptr);

private:
	size_t fileSize(int fd);
	void * loadFile(const char * path, size_t margine, size_t * size);

	const char	*	m_basePath;
};

#endif // CVSNWin32FS_h
