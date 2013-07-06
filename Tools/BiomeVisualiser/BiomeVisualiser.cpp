
// BiomeVisualiser.cpp

// Implements the cBiomeVisualiser class representing the entire app. Also implements the WinMain() entrypoint

#include "Globals.h"
#include "time.h"
#include "BiomeVisualiser.h"





int WINAPI WinMain(HINSTANCE a_Instance, HINSTANCE a_PrevInstance, LPSTR a_CmdLine, int a_ShowCmd)
{
	cBiomeVisualiser App;
	return App.Run();
}





cBiomeVisualiser::cBiomeVisualiser(void)
	// : m_Logger(Printf("BiomeVisualiser_%08x", time(NULL)))
{
}





int cBiomeVisualiser::Run(void)
{
	if (!m_MainWnd.Create(GetDesktopWindow(), TEXT("BiomeVisualiser")))
	{
		LOGERROR("Cannot create main window: %d", GetLastError());
		return 1;
	}
	
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}  // while (GetMessage)
	return msg.lParam;
}




