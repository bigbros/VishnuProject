#include <string.h>
#include "CVSNUtil.h"
#include "CVSNEngineHeap.h"

int
CVSNUtil::strHash(const char * str)
{
	int hash = 0;
	for (const char * ptr = str; *ptr != 0; ptr++) {
		hash = hash * 31 + (int)*ptr;
	}
	return hash;
}

const char *
CVSNUtil::jointPath(const char * path, const char * filename)
{
	char * buf = (char *)vsnMemAlloc(strlen(path) + strlen(filename) + 2);
	strcpy(buf, path);
	strcat(buf, "/");
	strcat(buf, filename);
	return buf;
}

void
CVSNUtil::freePath(const char * joint_path)
{
	vsnMemFree((void *)joint_path);
}

const char *
CVSNUtil::pathDir(const char * path)
{
	int last = 0;
	for (int i = 0; path[i]; i++) {
		if (path[i] == '\\' || path[i] == '/') last = i;
	}
	char * buf = (char *)vsnMemAlloc(last + 1);
	strncpy(buf, path, last);
	buf[last] = 0;
	return buf;
}

