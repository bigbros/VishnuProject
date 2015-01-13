#include <stdlib.h>
#include <string.h>
#include <memory>
#include "CVSNWin32FS.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <io.h>

CVSNWin32FS::CVSNWin32FS(const char * basePath)
	: CVSNStorage()
	, m_basePath(0)
{
	char * buf = (char *)malloc(strlen(basePath) + 1);
	if (!buf) throw std::bad_alloc();
	strcpy(buf, basePath);
	m_basePath = buf;
}

CVSNWin32FS::~CVSNWin32FS()
{
	free((void *)m_basePath);
}

bool
CVSNWin32FS::init()
{
	if (!m_basePath) return false;
	return true;
}

size_t
CVSNWin32FS::fileSize(int fd)
{
	struct stat stbuf;
	if (fstat(fd, &stbuf) == -1) {
		return 0;
	}
	return stbuf.st_size;
}

void *
CVSNWin32FS::loadFile(const char * path, size_t margine, size_t * size)
{
	int fd = _open(path, O_RDONLY|O_BINARY);
	if (fd == -1) {
		char buf[1024];
		sprintf(buf, "CVSNWin32FS::File not found: %s", path);
		*size = 0;
		return 0;
	}
	size_t file_size = fileSize(fd);

	char * buf = (char *)malloc(file_size + margine);
	size_t read_size = 0;
	int status;
	do {
		int s = (file_size - read_size < 1024) ? (file_size - read_size) : 1024;
		status = _read(fd, buf + read_size, s);
		if (status < 0) {
			int err = errno;
			_close(fd);
			free(buf);
			*size = 0;
			return 0;
		}
		read_size += status;
	} while ((read_size < file_size) && (status > 0));
	int err = errno;
	_close(fd);
	*size = read_size + margine;
	return buf;
}

void *
CVSNWin32FS::readData(const char * name, size_t * size)
{
	int len = strlen(m_basePath) + strlen(name) + 2;
	char * path = (char *)malloc(len);
	strcpy(path, m_basePath);
	strcat(path, "/");
	strcat(path, name);

	void * ret = loadFile(path, 0, size);

	free(path);
	return ret;
}

void *
CVSNWin32FS::readText(const char * name, size_t * size)
{
	int len = strlen(m_basePath) + strlen(name) + 2;
	char * path = (char *)malloc(len);
	strcpy(path, m_basePath);
	strcat(path, "/");
	strcat(path, name);

	char * data = (char *)loadFile(path, 1, size);

	free(path);

	if (!data){
		*size = 0;
		return 0;
	}
	if (data[*size - 2] == 0x1a) {
		data[*size - 2] = 0;
		--*size;
	}
	data[*size - 1] = 0;	// termination '\0'

	return data;

}

void
CVSNWin32FS::closeData(void * ptr)
{
	free(ptr);
}
