
// BiomeViewWnd.cpp

// Implements the cBiomeViewWnd class representing the window that displays biomes

#include "Globals.h"
#include "BiomeViewWnd.h"
#include "BiomeCache.h"
#include "GeneratorBiomeSource.h"
#include "../iniFile/iniFile.h"





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
	
	// Create a regular STATIC window, then override its window procedure with our own. No need for obnoxious RegisterWindowClass() stuff.
	m_Wnd = CreateWindow("STATIC", a_Title, WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, 400, 300, a_ParentWnd, NULL, GetModuleHandle(NULL), NULL);
	if (m_Wnd == NULL)
	{
		LOGERROR("Cannot create main window: %d", GetLastError());
		return false;
	}
	SetWindowLongPtr(m_Wnd, GWLP_WNDPROC, m_Thunk);
	
	cIniFile IniFile;
	cBiomeGen * BioGen = new cBioGenMultiStepMap(2);
	BioGen->Initialize(IniFile);
	m_Renderer.SetSource(new cGeneratorBiomeSource(BioGen));
	
	return true;
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




