#pragma once

#include <jni.h>
#include <android/log.h>
extern JNIEnv* g_CurrentJNIEnv;
extern JavaVM* g_JavaVM;
extern jobject g_JavaThread;
//extern jobject g_JavaActivity;

//__android_log_vprint(ANDROID_LOG_ERROR,"MCServer", a_Format, argList);

static void CallJavaFunction_Void_String( jobject a_Object, const std::string & a_FunctionName, const std::string & a_StringParam )
{
	JNIEnv * oldEnv = g_CurrentJNIEnv;
	int status = g_JavaVM->AttachCurrentThread(&g_CurrentJNIEnv, NULL);
	__android_log_print(ANDROID_LOG_ERROR,"MCServer", "STATUS: %i old: %p new: %p", status, oldEnv, g_CurrentJNIEnv );
	jstring str = g_CurrentJNIEnv->NewStringUTF( a_StringParam.c_str() );


	//__android_log_print(ANDROID_LOG_ERROR,"MCServer", "JNIEnv: %i Object: %i", g_CurrentJNIEnv, a_Object );
	jclass cls = g_CurrentJNIEnv->GetObjectClass( a_Object );
	//__android_log_print(ANDROID_LOG_ERROR,"MCServer", "jclass: %i", cls );
	jmethodID mid = g_CurrentJNIEnv->GetMethodID( cls, a_FunctionName.c_str(), "(Ljava/lang/String;)V"); // void a_FunctionName( String )
	//__android_log_print(ANDROID_LOG_ERROR,"MCServer", "jmethodID: %i", mid );
	if (mid != 0)
	{
		
		__android_log_print(ANDROID_LOG_ERROR,"MCServer", "Going to call right NOW! %s", a_FunctionName.c_str() );
		g_CurrentJNIEnv->CallVoidMethod( a_Object, mid, str );
	}
	else
	{
		__android_log_print(ANDROID_LOG_ERROR,"MCServer", "It was 0, derp" );
	}

	if( oldEnv != g_CurrentJNIEnv )
	{
		g_JavaVM->DetachCurrentThread();
	}
}


static void CallJavaFunction_Void_Void( jobject a_Object, const std::string & a_FunctionName )
{
	//__android_log_print(ANDROID_LOG_ERROR,"MCServer", "JNIEnv: %i Object: %i", g_CurrentJNIEnv, a_Object );
	jclass cls = g_CurrentJNIEnv->GetObjectClass( a_Object );
	//__android_log_print(ANDROID_LOG_ERROR,"MCServer", "jclass: %i", cls );
	jmethodID mid = g_CurrentJNIEnv->GetMethodID( cls, a_FunctionName.c_str(), "()V"); // void a_FunctionName( String )
	//__android_log_print(ANDROID_LOG_ERROR,"MCServer", "jmethodID: %i", mid );
	if (mid != 0)
	{
		//__android_log_print(ANDROID_LOG_ERROR,"MCServer", "Going to call right NOW! %s", a_FunctionName.c_str() );
		g_CurrentJNIEnv->CallVoidMethod( a_Object, mid );
	}
	else
	{
		__android_log_print(ANDROID_LOG_ERROR,"MCServer", "It was 0, derp" );
	}
}