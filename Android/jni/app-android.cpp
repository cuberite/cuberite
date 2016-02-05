#include "Globals.h"

#include <jni.h>
#include <android/log.h>

#include "OSSupport/File.h"
#include "OSSupport/NetworkSingleton.h"

#include "Root.h"
#include "WebAdmin.h"
#include "MemorySettingsRepository.h"
#include "Logger.h"

#ifdef _WIN32 // For IntelliSense parsing
typedef void jobject;
typedef int jint;
typedef bool jboolean;
typedef void JavaVM;
typedef void JNIEnv;
#endif




class cRootLauncher
{
public:

	void Start()
	{
		m_Root.Start(cpp14::make_unique<cMemorySettingsRepository>());
	}

	void Stop()
	{
		if (!IsStopped())
		{
			m_Root.QueueExecuteConsoleCommand("stop");
		}
	}

	bool IsStopped()
	{
		return m_Root.GetServer() == nullptr;
	}

	cRoot & GetInstance()
	{
		return m_Root;
	}

private:

	cRoot m_Root;

} RootLauncher;





extern "C"
{
	jint JNI_OnLoad(JavaVM* vm, void* reserved)
	{
		return JNI_VERSION_1_4;
	}

	/* Called when program/activity is created */
	JNIEXPORT void JNICALL Java_org_cuberite_CuberiteActivity_NativeOnCreate(JNIEnv*  env, jobject thiz)
	{
		cFile::CreateFolder(FILE_IO_PREFIX "cuberite");

		// Initialize logging subsystem:
		cLogger::InitiateMultithreading();

		// Initialize LibEvent:
		cNetworkSingleton::Get().Initialise();

		try
		{
			RootLauncher.Start();
		}
		catch (std::exception & e)
		{
			LOGERROR("Standard exception: %s", e.what());
		}
		catch (...)
		{
			LOGERROR("Unknown exception!");
		}

		// Shutdown all of LibEvent:
		cNetworkSingleton::Get().Terminate();
	}

	JNIEXPORT void JNICALL Java_org_cuberite_CuberiteActivity_NativeCleanUp(JNIEnv*  env, jobject thiz)
	{
		RootLauncher.Stop();
	}

	JNIEXPORT jboolean JNICALL Java_org_cuberite_CuberiteActivity_NativeIsServerRunning(JNIEnv* env, jobject thiz)
	{
		return !RootLauncher.IsStopped();
	}

	JNIEXPORT jint JNICALL Java_org_cuberite_CuberiteActivity_NativeGetWebAdminPort(JNIEnv* env, jobject thiz)
	{
		if (!RootLauncher.IsStopped() && (RootLauncher.GetInstance().GetWebAdmin() != nullptr))
		{
			return std::atoi(RootLauncher.GetInstance().GetWebAdmin()->GetIPv4Ports().c_str());
		}
		return 0;
	}
}