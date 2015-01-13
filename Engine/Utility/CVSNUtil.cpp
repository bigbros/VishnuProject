#include <string.h>
#include "CVSNUtil.h"
#include "CVSNEngineHeap.h"

int
CVSNUtil::strHash(const char * str)
{
	int hash = 0;
	for (const char * ptr = str; *ptr; ptr = ptr++) {
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