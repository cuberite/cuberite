
#pragma once

#include "cBlockEntity.h"





class cWindow;





/**
Implements the base behavior expected from a class that can handle UI windows for block entities.
*/
class cWindowOwner
{
public:
	cWindowOwner() : m_Window( NULL ) {}
	void CloseWindow() { m_Window = NULL; }
	void OpenWindow( cWindow* a_Window ) { m_Window = a_Window; }

	cWindow* GetWindow() { return m_Window; }

	void SetEntity(cBlockEntity * a_Entity) { m_Entity = a_Entity; }
	void GetBlockPos(int & a_BlockX, int & a_BlockY, int & a_BlockZ)
	{
		a_BlockX = m_Entity->GetPosX();
		a_BlockY = m_Entity->GetPosY();
		a_BlockZ = m_Entity->GetPosZ();
	}
	
private:
	cWindow      * m_Window;
	cBlockEntity * m_Entity;
};




