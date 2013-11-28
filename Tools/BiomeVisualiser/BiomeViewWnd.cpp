
// BiomeViewWnd.cpp

// Implements the cBiomeViewWnd class representing the window that displays biomes

#include "Globals.h"
#include "BiomeViewWnd.h"
#include "BiomeCache.h"
#include "GeneratorBiomeSource.h"
#include "iniFile/iniFile.h"





const int TIMER_RERENDER = 1200;





cBiomeViewWnd::cBiomeViewWnd(void) :
	m_Wnd(NULL),
	m_Thunk(&cBiomeViewWnd::WndProc, this),
	m_IsLButtonDown(false)
{
}





bool cBiomeViewWnd::Create(HWND a_ParentWnd, LPCTSTR a_Title)
{
	ASSERT(m_Wnd == NULL);
	
	InitBiomeView();
	
	// Create a regular STATIC window, then override its window procedure with our own. No need for obnoxious RegisterWindowClass() stuff.
	m_Wnd = CreateWindow("STATIC", a_Title, WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, 400, 300, a_ParentWnd, NULL, GetModuleHandle(NULL), NULL);
	if (m_Wnd == NULL)
	{
		LOGERROR("Cannot create main window: %d", GetLastError());
		return false;
	}
	SetWindowLongPtr(m_Wnd, GWLP_WNDPROC, m_Thunk);
	
	return true;
}





void cBiomeViewWnd::InitBiomeView(void)
{
	cIniFile IniFile;
	IniFile.ReadFile("world.ini");
	AString BiomeGenName = IniFile.GetValueSet("Generator", "BiomeGen", "");
	if (BiomeGenName.empty())
	{
		LOGWARN("[Generator] BiomeGen value not set in world.ini, using \"MultiStepMap\".");
		BiomeGenName = "MultiStepMap";
	}
	
	int Seed = IniFile.GetValueSetI("Generator", "Seed", 0);
	
	bool CacheOffByDefault = false;
	if (NoCaseCompare(BiomeGenName, "constant") == 0)
	{
		m_BiomeGen = new cBioGenConstant;
		CacheOffByDefault = true;  // we're generating faster than a cache would retrieve data :)
	}
	else if (NoCaseCompare(BiomeGenName, "checkerboard") == 0)
	{
		m_BiomeGen = new cBioGenCheckerboard;
		CacheOffByDefault = true;  // we're (probably) generating faster than a cache would retrieve data
	}
	else if (NoCaseCompare(BiomeGenName, "voronoi") == 0)
	{
		m_BiomeGen = new cBioGenVoronoi(Seed);
	}
	else if (NoCaseCompare(BiomeGenName, "distortedvoronoi") == 0)
	{
		m_BiomeGen = new cBioGenDistortedVoronoi(Seed);
	}
	else if (NoCaseCompare(BiomeGenName, "twolevel") == 0)
	{
		m_BiomeGen = new cBioGenTwoLevel(Seed);
	}
	else
	{
		if (NoCaseCompare(BiomeGenName, "multistepmap") != 0)
		{
			LOGWARNING("Unknown BiomeGen \"%s\", using \"MultiStepMap\" instead.", BiomeGenName.c_str());
		}
		m_BiomeGen = new cBioGenMultiStepMap(Seed);

		/*
		// Performance-testing:
		LOGINFO("Measuring performance of cBioGenMultiStepMap...");
		clock_t BeginTick = clock();
		for (int x = 0; x < 5000; x++)
		{
			cChunkDef::BiomeMap Biomes;
			m_BiomeGen->GenBiomes(x * 5, x * 5, Biomes);
		}
		clock_t Duration = clock() - BeginTick;
		LOGINFO("cBioGenMultiStepMap for 5000 chunks took %d ticks (%.02f sec)", Duration, (double)Duration / CLOCKS_PER_SEC);
		//*/
	}
	
	// Add a cache, if requested:
	int CacheSize = IniFile.GetValueSetI("Generator", "BiomeGenCacheSize", CacheOffByDefault ? 0 : 64);
	if (CacheSize > 0)
	{
		if (CacheSize < 4)
		{
			LOGWARNING("Biomegen cache size set too low, would hurt performance instead of helping. Increasing from %d to %d",
				CacheSize, 4
			);
			CacheSize = 4;
		}
		LOGD("Using a cache for biomegen of size %d.", CacheSize);
		m_BiomeGen = new cBioGenCache(m_BiomeGen, CacheSize);
	}
	m_BiomeGen->InitializeBiomeGen(IniFile);
	m_Renderer.SetSource(new cGeneratorBiomeSource(m_BiomeGen));
	IniFile.WriteFile("world.ini");
}





LRESULT cBiomeViewWnd::WndProc(HWND a_Wnd, UINT a_Msg, WPARAM wParam, LPARAM lParam)
{
	switch (a_Msg)
	{
		case WM_CLOSE:       return OnClose();
		case WM_COMMAND:     return OnCommand(wParam, lParam);
		case WM_LBUTTONDOWN: return OnLButtonDown(wParam, lParam);
		case WM_LBUTTONUP:   return OnLButtonUp  (wParam, lParam);
		case WM_MOUSEMOVE:   return OnMouseMove  (wParam, lParam);
		case WM_PAINT:       return OnPaint();
		case WM_TIMER:       return OnTimer(wParam);
	}
	return ::DefWindowProc(a_Wnd, a_Msg, wParam, lParam);
}





LRESULT cBiomeViewWnd::OnClose(void)
{
	PostQuitMessage(0);
	return 0;
}





LRESULT cBiomeViewWnd::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: Handle menu commands, when we get menu
	return 0;
}





LRESULT cBiomeViewWnd::OnLButtonDown(WPARAM wParam, LPARAM lParam)
{
	m_IsLButtonDown = true;
	GetCursorPos(&m_MouseDown);
	return 0;
}





LRESULT cBiomeViewWnd::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	if (!m_IsLButtonDown)
	{
		return 0;
	}
	POINT pnt;
	GetCursorPos(&pnt);
	m_Renderer.MoveViewBy(pnt.x - m_MouseDown.x, pnt.y - m_MouseDown.y);
	if (m_Renderer.Render(m_Pixmap))
	{
		SetTimer(m_Wnd, TIMER_RERENDER, 200, NULL);
	}
	m_MouseDown = pnt;
	InvalidateRect(m_Wnd, NULL, FALSE);
	return 0;
}





LRESULT cBiomeViewWnd::OnLButtonUp(WPARAM wParam, LPARAM lParam)
{
	OnMouseMove(wParam, lParam);  // Last movement - if the mouse move hasn't been reported due to speed
	m_IsLButtonDown = false;
	InvalidateRect(m_Wnd, NULL, FALSE);
	return 0;
}





LRESULT cBiomeViewWnd::OnPaint(void)
{
	PAINTSTRUCT ps;
	HDC DC = BeginPaint(m_Wnd, &ps);
	
	RECT rc;
	GetClientRect(m_Wnd, &rc);
	int Wid = rc.right - rc.left;
	int Hei = rc.bottom - rc.top;
	if ((m_Pixmap.GetWidth() != Wid) || (m_Pixmap.GetHeight() != Hei))
	{
		m_Pixmap.SetSize(Wid, Hei);
		if (m_Renderer.Render(m_Pixmap))
		{
			SetTimer(m_Wnd, TIMER_RERENDER, 200, NULL);
		}
	}
	
	m_Pixmap.DrawToDC(DC, 0, 0);

	EndPaint(m_Wnd, &ps);
	return 0;
}





LRESULT cBiomeViewWnd::OnTimer(WPARAM wParam)
{
	switch (wParam)
	{
		case TIMER_RERENDER:
		{
			if (!m_Renderer.Render(m_Pixmap))
			{
				KillTimer(m_Wnd, TIMER_RERENDER);
			}
			InvalidateRect(m_Wnd, NULL, FALSE);
			break;
		}
	}
	return 0;
}




