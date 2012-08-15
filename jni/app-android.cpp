#include "Globals.h"

#include <jni.h>
#include <sys/time.h>
#include <time.h>
#include <stdint.h>

#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <assert.h>

#include "cCriticalSection.h"
#include "cRoot.h"

#include <android/log.h>

cCriticalSection g_CriticalSection;

JNIEnv* g_CurrentJNIEnv = 0;
jobject g_JavaRenderer = 0;

/* Called when program/activity is created */
extern "C" void Java_com_mcserver_MainThread_NativeOnCreate( JNIEnv*  env )
{
	g_CriticalSection.Lock();
	g_CurrentJNIEnv = env;
	//if( !cLogger::GetSingletonPtr() ) new cLogger();
	__android_log_print(ANDROID_LOG_ERROR,"Arashi", "%s", "Logging from C++!");
	g_CriticalSection.Unlock();
	
	cRoot Root;
	Root.Start();
}

extern "C" void Java_com_mcserver_MCServerActivity_NativeCleanUp( JNIEnv*  env )
{
	g_CriticalSection.Lock();
	g_CurrentJNIEnv = env;

	g_CriticalSection.Unlock();
}

/* Call to initialize the graphics state */
extern "C" void Java_com_ballz_CppWrapperRenderer_NativeInitGL( JNIEnv*  env, jobject  thiz )
{
	g_CriticalSection.Lock();
	g_CurrentJNIEnv = env;
	g_JavaRenderer = thiz;

	g_CriticalSection.Unlock();
}

extern "C" void Java_com_ballz_CppWrapperRenderer_NativeResize( JNIEnv*  env, jobject  thiz, jint w, jint h )
{
	g_CriticalSection.Lock();
	g_CurrentJNIEnv = env;
	g_JavaRenderer = thiz;

	g_CriticalSection.Unlock();
}

extern "C" void Java_com_ballz_CppWrapperRenderer_NativeRender( JNIEnv*  env, jobject  thiz )
{
	g_CriticalSection.Lock();
	g_CurrentJNIEnv = env;
	g_JavaRenderer = thiz;


	g_CriticalSection.Unlock();
}

extern "C" void Java_com_ballz_CppWrapperGLSurfaceView_NativeTouchScreen( JNIEnv*  env, jobject  thiz, jint mouseid, jint touched )
{
	g_CriticalSection.Lock();
	g_CurrentJNIEnv = env;

	g_CriticalSection.Unlock();
}

extern "C" void Java_com_ballz_CppWrapperGLSurfaceView_NativeTouchEvent( JNIEnv*  env, jobject  thiz, jint mouseid, jfloat x, jfloat y )
{
	g_CriticalSection.Lock();
	g_CurrentJNIEnv = env;

	g_CriticalSection.Unlock();
}