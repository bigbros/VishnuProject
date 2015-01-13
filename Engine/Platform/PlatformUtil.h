#ifndef PlatformUtil_h
#define PlatformUtil_h

#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32
#include <Windows.h>
#ifdef _USRDLL
extern "C" {
	void LogFunc(const char * msg);
}
#define LOG(msg) LogFunc(msg);
#else
#define LOG(msg) OutputDebugString(msg)
#endif
#endif

#ifdef WIN64
#include <Windows.h>
#define LOG(msg) OutputDebugString(msg)
#endif

#ifdef ANDROID
#ifdef ANDROID_NATIVE
#include <android/log.h>
#define LOG(msg...) __android_log_write(ANDROID_LOG_DEBUG, "Vishnu", msg)
#else
extern "C" {
	void LogOutput(const char *);
}
#define LOG(msg) LogOutput(msg)
#endif
#endif

#endif // PlatformUtil_h
