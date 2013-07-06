
// BiomeViewWnd.h

// Declares the cBiomeViewWnd class representing the window that displays biomes

#include "WndProcThunk.h"
#include "BiomeRenderer.h"
#include "BiomeCache.h"
#include "Pixmap.h"





class cBiomeViewWnd
{
public:
	cBiomeViewWnd(void);
	
	bool Create(HWND a_ParentWnd, LPCTSTR a_Title);
	
protected:
	HWND m_Wnd;
	CWndProcThunk<cBiomeViewWnd> m_Thunk;
	
	cBiomeRenderer m_Renderer;
	cPixmap        m_Pixmap;
	
	bool  m_IsLButtonDown;
	POINT m_MouseDown;
	
	LRESULT WndProc(HWND a_Wnd, UINT a_Msg, WPARAM wParam, LPARAM lParam);
	
	// Message handlers:
	LRESULT OnClose      (void);
	LRESULT OnCommand    (WPARAM wParam, LPARAM lParam);
	LRESULT OnLButtonDown(WPARAM wParam, LPARAM lParam);
	LRESULT OnMouseMove  (WPARAM wParam, LPARAM lParam);
	LRESULT OnLButtonUp  (WPARAM wParam, LPARAM lParam);
	LRESULT OnPaint      (void);
	LRESULT OnTimer      (WPARAM wParam);
} ;




