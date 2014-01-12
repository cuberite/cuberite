
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
	int Seed = IniFile.GetValueSetI("Generator", "Seed", 0);
	bool CacheOffByDefault = false;
	m_BiomeGen = cBiomeGen::CreateBiomeGen(IniFile, Seed, CacheOffByDefault);
	m_Renderer.SetSource(new cGeneratorBiomeSource(m_BiomeGen));
	IniFile.WriteFile("world.ini");
}





void cBiomeViewWnd::SetZoom(int a_NewZoom)
{
	m_Renderer.SetZoom(a_NewZoom);
	Redraw();
}





void cBiomeViewWnd::Redraw(void)
{
	if (m_Renderer.Render(m_Pixmap))
	{
		SetTimer(m_Wnd, TIMER_RERENDER, 200, NULL);
	}
	InvalidateRect(m_Wnd, NULL, FALSE);
}





LRESULT cBiomeViewWnd::WndProc(HWND a_Wnd, UINT a_Msg, WPARAM wParam, LPARAM lParam)
{
	switch (a_Msg)
	{
		case WM_CHAR:        return OnChar       (wParam, lParam);
		case WM_CLOSE:       return OnClose      ();
		case WM_COMMAND:     return OnCommand    (wParam, lParam);
		case WM_LBUTTONDOWN: return OnLButtonDown(wParam, lParam);
		case WM_LBUTTONUP:   return OnLButtonUp  (wParam, lParam);
		case WM_MOUSEMOVE:   return OnMouseMove  (wParam, lParam);
		case WM_PAINT:       return OnPaint      ();
		case WM_TIMER:       return OnTimer      (wParam);
	}
	return ::DefWindowProc(a_Wnd, a_Msg, wParam, lParam);
}





LRESULT cBiomeViewWnd::OnChar(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
		case '1': SetZoom(1); break;
		case '2': SetZoom(2); break;
		case '3': SetZoom(3); break;
		case '4': SetZoom(4); break;
		case '5': SetZoom(5); break;
		case '6': SetZoom(6); break;
		case '7': SetZoom(7); break;
		case '8': SetZoom(8); break;
		case 27:
		{
			// Esc pressed, exit
			PostQuitMessage(0);
			break;
		}
	}
	return 0;
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
	m_MouseDown = pnt;
	Redraw();
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




