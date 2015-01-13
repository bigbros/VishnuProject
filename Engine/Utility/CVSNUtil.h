#ifndef CVSNUtil_h
#define CVSNUtil_h

class CVSNUtil
{
public:
	static int strHash(const char * str);

	static const char * jointPath(const char * path, const char * filename);
	static void freePath(const char * joint_path);
};


#endif // CVSNUtil_h
