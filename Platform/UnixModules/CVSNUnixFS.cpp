#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory>
#include "CVSNUnixFS.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

CVSNUnixFS::CVSNUnixFS(const char * basePath)
	: CVSNStorage()
	, m_basePath(0)
{
	char * buf = (char *)malloc(strlen(basePath) + 1);
	if (!buf) throw std::bad_alloc();
	strcpy(buf, basePath);
	m_basePath = buf;
}

CVSNUnixFS::~CVSNUnixFS()
{
	free((void *)m_basePath);
}

bool
CVSNUnixFS::init()
{
	if (!m_basePath) return false;
	return true;
}

size_t
CVSNUnixFS::fileSize(int fd)
{
	struct stat stbuf;
	if (fstat(fd, &stbuf) == -1) {
		return 0;
	}
	return stbuf.st_size;
}

void *
CVSNUnixFS::loadFile(const char * path, size_t margine, size_t * size)
{
	int fd = open(path, O_RDONLY);
	if (fd == -1) {
		char buf[1024];
		sprintf(buf, "CVSNUnixFS::File not found: %s", path);
		*size = 0;
		return 0;
	}
	size_t file_size = fileSize(fd);

	char * buf = (char *)malloc(file_size + margine);
	int read_size = 0;
	int status;
	do {
		status = read(fd, buf + read_size, 1024);
		if (status < 0) {
			close(fd);
			free(buf);
			*size = 0;
			return 0;
		}
		read_size += status;
	} while ((read_size < file_size) && (status > 0));
	int err = errno;
	close(fd);
	*size = read_size + margine;
	return buf;
}

void *
CVSNUnixFS::readData(const char * name, size_t * size)
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
CVSNUnixFS::readText(const char * name, size_t * size)
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
CVSNUnixFS::closeData(void * ptr)
{
	free(ptr);
}
