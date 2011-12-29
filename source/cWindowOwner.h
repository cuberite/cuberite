#pragma once

#include "MemoryLeak.h"

class cWindow;
class cBlockEntity;
class cWindowOwner
{
public:
	cWindowOwner() : m_Window( 0 ) {}
	void CloseWindow() { m_Window = 0; }
	void OpenWindow( cWindow* a_Window ) { m_Window = a_Window; }

	cWindow* GetWindow() { return m_Window; }

	void SetEntity(cBlockEntity *a_Entity) { m_Entity = a_Entity; }
	cBlockEntity *GetEntity() { return m_Entity; }
private:
	cWindow* m_Window;
	cBlockEntity *m_Entity;
};