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
#include "cMakeDir.h"
#include "ToJava.h"

#include <android/log.h>

cCriticalSection g_CriticalSection;

JNIEnv* g_CurrentJNIEnv = 0;
jobject g_JavaThread = 0;
//jobject g_JavaActivity = 0;

cRoot * pRoot = NULL;





/* Called when program/activity is created */
extern "C" void Java_com_mcserver_MainThread_NativeOnCreate( JNIEnv*  env, jobject  thiz )
{
	g_CriticalSection.Lock();
	g_CurrentJNIEnv = env;
	g_JavaThread = thiz;
	//if( !cLogger::GetSingletonPtr() ) new cLogger();
	__android_log_print(ANDROID_LOG_ERROR,"MCServer", "%s", "Logging from C++!");
	g_CriticalSection.Unlock();

	//CallJavaFunction_Void_Void(g_JavaActivity, "TestTest" );
	//CallJavaFunction_Void_String(g_JavaThread, "AddToLog", "herpderpderp!!" );
	
	mkdir("/sdcard/mcserver", S_IRWXU | S_IRWXG | S_IRWXO);

	pRoot = new cRoot();
	pRoot->Start();
	delete pRoot;
}





extern "C" void Java_com_mcserver_MainThread_NativeCleanUp( JNIEnv*  env, jobject  thiz )
{
	g_CriticalSection.Lock();
	g_CurrentJNIEnv = env;
	g_JavaThread = thiz;
	g_CriticalSection.Unlock();

	pRoot->ServerCommand("stop");
}




