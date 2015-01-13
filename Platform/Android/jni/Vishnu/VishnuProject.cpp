#include <jni.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <android/log.h>
#include "CVSNGameEnvironment.h"
#include "CTestEnv.h"
#include "CVSNLuaScript.h"
#include "platform.h"
#include "CVSNPthread.h"
#include "CVSNUnixFS.h"

struct UserData {
	void * heapBuf;
	size_t heapSize;
	const char * pathData;
	CVSNGameEnvironment	*	pEnv;
	CVSNPointing		*	pPointing;
	CVSNUnixFS			*	pStorage;
	CVSNPthread			*	pThread;
	long	preTime;
};

static UserData * pContext = 0;
static JNIEnv * jnienv = 0;
struct LOG_QUEUE {
	LOG_QUEUE	*	prev;
	LOG_QUEUE	*	next;
	const char	*	msg;
};

static LOG_QUEUE * begin = 0;
static LOG_QUEUE * end = 0;

static void addLog(const char * msg)
{
	char * buf = (char *)malloc(strlen(msg) + 1);
	strcpy(buf, msg);
	LOG_QUEUE * item = (LOG_QUEUE *)malloc(sizeof(LOG_QUEUE));
	item->next = 0;
	item->prev = end;
	if(item->prev) {
		item->prev->next = item;
	} else {
		begin = item;
	}
	end = item;
	item->msg = buf;
}

static void removeLog(LOG_QUEUE * item)
{
	free((void *)item->msg);
	if(item->prev) {
		item->prev->next = item->next;
	} else {
		begin = item->next;
	}
	if(item->next) {
		item->next->prev = item->prev;
	} else {
		end = item->prev;
	}
	free(item);
}

extern "C" {

static void SendLog(JNIEnv * env)
{
	jclass clazz = env->FindClass("com/madtape/vishnuproject/VishnuLog");
	jmethodID mid = env->GetStaticMethodID(clazz, "addLog", "(Ljava/lang/String;)V");

	LOG_QUEUE * item = begin;
	while(item) {
		LOG_QUEUE * nxt = item->next;
		jstring str = env->NewStringUTF(item->msg);
		env->CallStaticVoidMethod(clazz, mid, str);
		removeLog(item);
		item = nxt;
	}
}

void LogOutput(const char * msg)
{
	__android_log_write(ANDROID_LOG_DEBUG, "Vishnu", msg);
	addLog(msg);
	if(!jnienv) return;
	SendLog(jnienv);
}

static long getTime() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	long ret = (long)tv.tv_sec * 1000L + (long)tv.tv_usec / 1000L;
	return ret;
}

static const char *
getJavaString(JNIEnv * env, jstring jstr)
{
	const char * str = env->GetStringUTFChars(jstr, 0);
	char * buf = (char *)malloc(strlen(str) + 1);
	strcpy(buf, str);
	env->ReleaseStringUTFChars(jstr, str);
	return buf;
}

JNIEXPORT jlong JNICALL
Java_com_madtape_vishnuproject_GLES2View_getContextNdk(
		JNIEnv * env, jobject obj)
{
	return (jlong)pContext;
}

JNIEXPORT jlong JNICALL
Java_com_madtape_vishnuproject_GLES2View_setEnvironmentNdk(
		JNIEnv * env, jobject obj,
		jint jheapSize, jstring dataPath)
{
	UserData * data = new UserData();
	size_t heapSize = (size_t)jheapSize;
	void * heapBuf = malloc(heapSize);
	data->heapSize = heapSize;
	data->heapBuf = heapBuf;
	data->pathData = getJavaString(env, dataPath);
	data->pPointing = 0;
	data->pStorage = 0;
	data->pThread = 0;
	pContext = data;

	return (jlong)data;
}

JNIEXPORT void JNICALL
Java_com_madtape_vishnuproject_GLES2View_disposeNdk(
		JNIEnv * env, jobject obj,
		jlong ptr)
{
	UserData * data = (UserData *)ptr;

	if(!data) return;

	delete data->pEnv;
	delete data->pPointing;
	delete data->pStorage;
	delete data->pThread;
	free((void *)data->pathData);
	free(data->heapBuf);
	delete data;

	pContext = 0;
}

JNIEXPORT void JNICALL
Java_com_madtape_vishnuproject_GLES2View_onSurfaceCreatedNdk(
		JNIEnv * env, jobject obj,
		jlong ptr, int width, int height)
{
	jnienv = env;
	UserData * data = (UserData *)ptr;
	CTestEnv * envGame = new CTestEnv(data->heapBuf, data->heapSize, width, height);
	CVSNPointing * pPointing = new CVSNPointing();
	CVSNUnixFS * pStorage = new CVSNUnixFS(data->pathData);
	CVSNPthread * pThread = new CVSNPthread();
	CVSNPlatform::getInstance().registModule(pPointing);
	CVSNPlatform::getInstance().registModule(pStorage);
	CVSNPlatform::getInstance().registModule(pThread);
	envGame->Startup("shaders", "LuaScripts", "startup");
	data->pEnv = envGame;
	data->pPointing = pPointing;
	data->pStorage = pStorage;
	data->pThread = pThread;
	data->preTime = getTime();
	jnienv = 0;
}

JNIEXPORT void JNICALL
Java_com_madtape_vishnuproject_GLES2View_onSurfaceChangedNdk(
		JNIEnv * env, jobject obj,
		jlong ptr, jint width, jint height)
{
	jnienv = env;
	UserData * data = (UserData *)ptr;
	data->pEnv->onChange(width, height);
	jnienv = 0;
}

JNIEXPORT void JNICALL
Java_com_madtape_vishnuproject_GLES2View_onDrawFrameNdk(
		JNIEnv * env, jobject obj,
		jlong ptr)
{
	SendLog(env);
	UserData * data = (UserData *)ptr;
	if(!data) return;
	jnienv = env;
	int deltaT;
	long now = getTime();
	deltaT = (int)(now - data->preTime);
	char buf[1024];
	data->preTime = now;
	data->pEnv->Update(deltaT);
	jnienv = 0;
}

JNIEXPORT void JNICALL
Java_com_madtape_vishnuproject_GLES2View_onTouchEventNdk(
		JNIEnv * env, jobject obj,
		jlong ptr, jint type, jint id, jfloat x, jfloat y, jfloat force)
{
	jnienv = env;
	UserData * data = (UserData *)ptr;
	data->pPointing->setEvent(id, (IVSNPointListener::EVENT)type, (int)x, (int)y, force);
	jnienv = 0;
}

JNIEXPORT void JNICALL
Java_com_madtape_vishnuproject_GLES2View_onPauseNdk(
		JNIEnv * env, jobject obj,
		jlong ptr)
{
	jnienv = 0;
	UserData * data = (UserData *)ptr;
	data->pEnv->onPause();
}

JNIEXPORT void JNICALL
Java_com_madtape_vishnuproject_GLES2View_onResumeNdk(
		JNIEnv * env, jobject obj,
		jlong ptr)
{
	jnienv = 0;
	UserData * data = (UserData *)ptr;
	data->pEnv->onResume();
}

JNIEXPORT void JNICALL
Java_com_madtape_vishnuproject_GLES2View_onSurfaceNdk(
		JNIEnv * env, jobject obj,
		jlong ptr)
{
	jnienv = 0;
	UserData * data = (UserData *)ptr;
	data->pEnv->onSurface();
}

JNIEXPORT void JNICALL
Java_com_madtape_vishnuproject_GLES2View_onDetachNdk(
		JNIEnv * env, jobject obj,
		jlong ptr)
{
	jnienv = 0;
	UserData * data = (UserData *)ptr;
	data->pEnv->onDetach();
}
}
