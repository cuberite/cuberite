#include "Globals.h"

#include <jni.h>
#include <sys/time.h>
#include <time.h>
#include <stdint.h>

#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <assert.h>

#include "OSSupport/CriticalSection.h"
#include "OSSupport/File.h"
#include "ToJava.h"
#include "inifile/iniFile.h"

#include "Root.h"
#include "WebAdmin.h"

#include <android/log.h>

#ifdef _WIN32 // For IntelliSense parsing
typedef void jobject;
typedef int jint;
typedef bool jboolean;
typedef void JavaVM;
typedef void JNIEnv;
#endif

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
	//__android_log_print(ANDROID_LOG_ERROR,"MCServer", "%s", "JNI_OnLoad JNI_OnLoad JNI_OnLoad JNI_OnLoad");
	g_JavaVM = vm;
	return JNI_VERSION_1_4;
}

/* Called when program/activity is created */
extern "C" void Java_com_mcserver_MCServerActivity_NativeOnCreate( JNIEnv*  env, jobject thiz )
{
	g_CriticalSection.Lock();
	g_CurrentJNIEnv = env;
	g_JavaThread = thiz;
	//__android_log_print(ANDROID_LOG_ERROR,"MCServer", "%s", "Logging from C++!");
	g_CriticalSection.Unlock();
	
	cFile::CreateFolder("/sdcard/mcserver");

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
		pRoot->QueueExecuteConsoleCommand("stop");
	}
}




extern "C" jboolean Java_com_mcserver_MCServerActivity_NativeIsServerRunning( JNIEnv* env, jobject thiz )
{
	return pRoot != NULL;
}




extern "C" jint Java_com_mcserver_MCServerActivity_NativeGetWebAdminPort( JNIEnv* env, jobject thiz )
{
	if( pRoot != NULL && pRoot->GetWebAdmin() != NULL )
	{
		return atoi(pRoot->GetWebAdmin()->GetIPv4Ports().c_str());
	}
	return 0;
}
