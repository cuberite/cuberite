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
JavaVM* g_JavaVM = 0;
//jobject g_JavaActivity = 0;

cRoot * pRoot = NULL;


class cMainThread :
	public cIsThread
{
public:
	cMainThread() :
	  cIsThread("cMainThread")
	  {
		  //Start();
		  __android_log_print(ANDROID_LOG_ERROR,"MCServer", "%s", "cMainThread");
	  }

	  void Stop(void)
	  {
		  m_ShouldTerminate = true;
		  Wait();
	  }

protected:

	virtual void Execute(void) override
	{
		__android_log_print(ANDROID_LOG_ERROR,"MCServer", "%s", "Execute");
		pRoot = new cRoot();
		pRoot->Start();
		delete pRoot;
	}

} ;

cMainThread * pMainThread = NULL;

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
	__android_log_print(ANDROID_LOG_ERROR,"MCServer", "%s", "JNI_OnLoad JNI_OnLoad JNI_OnLoad JNI_OnLoad");
	g_JavaVM = vm;
	return JNI_VERSION_1_4;
}

/* Called when program/activity is created */
extern "C" void Java_com_mcserver_MCServerActivity_NativeOnCreate( JNIEnv*  env, jobject thiz )
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

// 	__android_log_print(ANDROID_LOG_ERROR,"MCServer", "%s", "Before mainthread");
// 	pMainThread = new cMainThread();
// 	pMainThread->Start();
// 	__android_log_print(ANDROID_LOG_ERROR,"MCServer", "%s", "AFter mainthread");

	pRoot = new cRoot();
	pRoot->Start();
	delete pRoot; pRoot = NULL;
}





extern "C" void Java_com_mcserver_MCServerActivity_NativeCleanUp( JNIEnv*  env, jobject thiz )
{
	g_CriticalSection.Lock();
	g_CurrentJNIEnv = env;
	g_JavaThread = thiz;
	g_CriticalSection.Unlock();

	__android_log_print(ANDROID_LOG_ERROR,"MCServer", "pRoot: %p", pRoot);
	if( pRoot != NULL )
	{
		pRoot->ServerCommand("stop");
	}
// 	pMainThread->Stop();
// 	delete pMainThread; pMainThread = NULL;
}




extern "C" jboolean Java_com_mcserver_MCServerActivity_NativeIsServerRunning( JNIEnv* env, jobject thiz )
{
	return pRoot != NULL;
}