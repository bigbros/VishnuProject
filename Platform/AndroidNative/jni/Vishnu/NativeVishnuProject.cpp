#include <jni.h>
#include <EGL/egl.h>
//#include <GLES/gl.h>
#include <GLES2/gl2.h>
#include <android/sensor.h>
#include <android/log.h>
#include <android/native_activity.h>
#include "android_native_app_glue.h"

#include "CTestEnv.h"
#include "CVSNLuaScript.h"
#include "platform.h"
#include "CVSNKVObj.h"
#include "CVSNPthread.h"
#include "CVSNUnixFS.h"

/**
 * Our saved state data.
 */
struct saved_state {
    float angle;
    int32_t x;
    int32_t y;
};

struct UserData {
	GLuint						programObject;
	CVSNGameEnvironment		*	pEnv;
	long						prevTime;
	CVSNPointing			*	pPointing;
	CVSNPthread				*	pThread;
	CVSNUnixFS				*	pStorage;
};

static UserData * m_pUser = 0;

struct engine {
    struct android_app* app;
    void * userData;
    ASensorManager* sensorManager;
    const ASensor* accelerometerSensor;
    ASensorEventQueue* sensorEventQueue;

    int animating;
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
    int32_t width;
    int32_t height;
    struct saved_state state;
};

static long getTime() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	long ret = (long)tv.tv_sec * 1000L + (long)tv.tv_usec / 1000L;
	return ret;
}

/**
 * Initialize an EGL context for the current display.
 */
static int engine_init_display(struct engine* engine) {
    // initialize OpenGL ES and EGL

	LOG("engine_init_display\n");
    /*
     * Here specify the attributes of the desired configuration.
     * Below, we select an EGLConfig with at least 8 bits per color
     * component compatible with on-screen windows
     */
    const EGLint attribs[] = {
//            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_ALPHA_SIZE, 8,
            EGL_DEPTH_SIZE, 8,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_NONE
    };

    const EGLint contextAttribs[] = {
    		EGL_CONTEXT_CLIENT_VERSION, 2,
    		EGL_NONE
    };
    EGLint w, h, dummy, format;
    EGLint numConfigs;
    EGLConfig config;
    EGLSurface surface;
    EGLContext context;

    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    eglInitialize(display, 0, 0);

    /* Here, the application chooses the configuration it desires. In this
     * sample, we have a very simplified selection process, where we pick
     * the first EGLConfig that matches our criteria */
    eglChooseConfig(display, attribs, &config, 1, &numConfigs);

    /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
     * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
     * As soon as we picked a EGLConfig, we can safely reconfigure the
     * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

    ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);

    surface = eglCreateWindowSurface(display, config, engine->app->window, NULL);
    context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        // LOGW("Unable to eglMakeCurrent");
        return -1;
    }

    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);

    engine->display = display;
    engine->context = context;
    engine->surface = surface;
    engine->width = w;
    engine->height = h;
    engine->state.angle = 0;


    // Initialize GL state.

    return 0;
}

/**
 * Tear down the EGL context currently associated with the display.
 */
static void engine_term_display(struct engine* engine) {
    if (engine->display != EGL_NO_DISPLAY) {
        eglMakeCurrent(engine->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (engine->context != EGL_NO_CONTEXT) {
            eglDestroyContext(engine->display, engine->context);
        }
        if (engine->surface != EGL_NO_SURFACE) {
            eglDestroySurface(engine->display, engine->surface);
        }
        eglTerminate(engine->display);
    }
    engine->animating = 0;
    engine->display = EGL_NO_DISPLAY;
    engine->context = EGL_NO_CONTEXT;
    engine->surface = EGL_NO_SURFACE;
}

/**
 * Process the next input event.
 */
static int32_t engine_handle_input(struct android_app* app, AInputEvent* event) {
    struct engine* engine = (struct engine*)app->userData;
    UserData * userData = (UserData *)engine->userData;
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
        engine->animating = 1;
        engine->state.x = AMotionEvent_getX(event, 0);
        engine->state.y = AMotionEvent_getY(event, 0);

        int count = AMotionEvent_getPointerCount(event);
    	int x = AMotionEvent_getX(event, 0);
    	int y = AMotionEvent_getY(event, 0);
    	float pressure = AMotionEvent_getPressure(event, 0);
    	int id = AMotionEvent_getPointerId(event, 0);
    	switch(AMotionEvent_getAction(event)) {
    	case AMOTION_EVENT_ACTION_DOWN: {
    		userData->pPointing->setEvent(id, IVSNPointListener::PT_TAP, x, y, pressure);
    		break;
    	}
    	case AMOTION_EVENT_ACTION_UP: {
    		userData->pPointing->setEvent(id, IVSNPointListener::PT_RELEASE, x, y, pressure);
    		break;
    	}
    	case AMOTION_EVENT_ACTION_MOVE: {
    		userData->pPointing->setEvent(id, IVSNPointListener::PT_DRAG, x, y, pressure);
    		break;
    	}
    	default: break;
    	}
        return 1;
    }
    return 0;
}

/**
 * Just the current frame in the display.
 */
static void engine_draw_frame(struct engine* engine) {
    if (engine->display == NULL) {
        // No display.
        return;
    }
    UserData * pUser = (UserData *)engine->userData;
    if(pUser) {
    	int deltaT;
	    long now = getTime();
   	    deltaT = (int)(now - pUser->prevTime);
   	    pUser->prevTime = now;
   	    pUser->pEnv->Update(deltaT);
    }
    eglSwapBuffers(engine->display, engine->surface);
}

/**
 * Process the next main command.
 */
static void engine_handle_cmd(struct android_app* app, int32_t cmd) {
    struct engine* engine = (struct engine*)app->userData;
    switch (cmd) {
        case APP_CMD_SAVE_STATE:
            // The system has asked us to save our current state.  Do so.
            engine->app->savedState = malloc(sizeof(struct saved_state));
            *((struct saved_state*)engine->app->savedState) = engine->state;
            engine->app->savedStateSize = sizeof(struct saved_state);
            break;
        case APP_CMD_INIT_WINDOW:
            // The window is being shown, get it ready.
            if (engine->app->window != NULL) {
                engine_init_display(engine);
                UserData * pUser = (UserData *)engine->userData;
                if(!pUser && !m_pUser) {
                	LOG("Initialize Vishnu.\n");
                	pUser = new UserData();
                	m_pUser = pUser;
                	ANativeActivity * nativeActivity = engine->app->activity;
                	LOG("-1\n");
                	//const char * internalPath = nativeActivity->internalDataPath;

                	std::string dataPath("/storage/emulated/0");
                	std::string vishnuPath = dataPath + "/Vishnu/";
                	LOG("-2\n");
                	LOG(vishnuPath.c_str());

                	size_t heapSize = MEGA(4);
                	void * heapBuf = malloc(heapSize);
                	CTestEnv * envGame = new CTestEnv(heapBuf, heapSize,
                			engine->width, engine->height);

                	CVSNPointing * pointing = new CVSNPointing();
                	CVSNUnixFS * storage = new CVSNUnixFS(vishnuPath.c_str());
                	CVSNPthread * vsnThread = new CVSNPthread();
                	CVSNPlatform::getInstance().registModule(pointing);
                	CVSNPlatform::getInstance().registModule(storage);
                	CVSNPlatform::getInstance().registModule(vsnThread);
                	pUser->pEnv = envGame;
                	pUser->prevTime = getTime();
                	pUser->pPointing = pointing;
                	pUser->pStorage = storage;
                	pUser->pThread = vsnThread;
                	engine->userData = pUser;
                	pUser->pEnv->Startup("shaders", "LuaScripts", "startup");
                	LOG("Completed.\n");
            	} else {
            		pUser = m_pUser;
            		pUser->pEnv->onSurface();
                	pUser->pEnv->onChange(engine->width, engine->height);
                }
            	engine->userData = pUser;
                engine_draw_frame(engine);
            }
            break;
        case APP_CMD_TERM_WINDOW:
            // The window is being hidden or closed, clean it up.
            engine_term_display(engine);
            break;
        case APP_CMD_GAINED_FOCUS:
            // When our app gains focus, we start monitoring the accelerometer.
            if (engine->accelerometerSensor != NULL) {
                ASensorEventQueue_enableSensor(engine->sensorEventQueue,
                        engine->accelerometerSensor);
                // We'd like to get 60 events per second (in us).
                ASensorEventQueue_setEventRate(engine->sensorEventQueue,
                        engine->accelerometerSensor, (1000L/60)*1000);
            }
            {
            	UserData * pUser = (UserData *)engine->userData;
            	pUser->pEnv->onResume();
            }
            break;
        case APP_CMD_LOST_FOCUS:
            // When our app loses focus, we stop monitoring the accelerometer.
            // This is to avoid consuming battery while not being used.
            if (engine->accelerometerSensor != NULL) {
                ASensorEventQueue_disableSensor(engine->sensorEventQueue,
                        engine->accelerometerSensor);
            }
            // Also stop animating.
            engine->animating = 0;
            engine_draw_frame(engine);
            {
            	UserData * pUser = (UserData *)engine->userData;
            	pUser->pEnv->onPause();
            }
            break;
    }
}

static void main_loop(struct engine * engine)
{
	for(;;) {
		int ident;
		int events;
		struct android_poll_source * source;

		while((ident = ALooper_pollAll(engine->animating ? 0 : -1, NULL, &events,
				(void **)&source)) >= 0) {
			if(source != NULL) {
				source->process(engine->app, source);
			}
			if(ident == LOOPER_ID_USER) {
				if(engine->accelerometerSensor != NULL) {
					ASensorEvent event;
					while(ASensorEventQueue_getEvents(engine->sensorEventQueue,
							&event, 1) > 0) {

						// get accelerometer parameter.
						// event.acceleration.x
						// event.acceleration.y
						// event.acceleration.z

					}
				}
			}

			if(engine->app->destroyRequested != 0) {
				engine_term_display(engine);
				return;
			}
		}
		if(engine->animating) {
			engine_draw_frame(engine);
		}
	}
}
void android_main(struct android_app* state)
{
	LOG("android_main\n");
	struct engine engine;
	LOG("-3\n");
	memset(&engine, 0, sizeof(engine));
	engine.userData = 0;

	state->userData = &engine;
	state->onAppCmd = engine_handle_cmd;
	state->onInputEvent = engine_handle_input;
	engine.app = state;

	engine.sensorManager = ASensorManager_getInstance();
	engine.accelerometerSensor = ASensorManager_getDefaultSensor(engine.sensorManager, ASENSOR_TYPE_ACCELEROMETER);
	engine.sensorEventQueue = ASensorManager_createEventQueue(engine.sensorManager, state->looper, LOOPER_ID_USER, NULL, NULL);

	if(state->savedState != NULL) {
		engine.state = *(struct saved_state *)state->savedState;
	}
	{
		main_loop(&engine);
	}
}
