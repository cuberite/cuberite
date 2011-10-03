#pragma once

#include "MemoryLeak.h"

class cWindow;
class cWindowOwner
{
public:
	cWindowOwner() : m_Window( 0 ) {}
	void CloseWindow() { m_Window = 0; }
	void OpenWindow( cWindow* a_Window ) { m_Window = a_Window; }

	cWindow* GetWindow() { return m_Window; }
private:
	cWindow* m_Window;
};