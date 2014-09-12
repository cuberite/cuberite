
#pragma once

#include "../BlockEntities/BlockEntity.h"
#include "../Entities/Entity.h"
#include "Window.h"

/*
Being a descendant of cWindowOwner means that the class can own one window. That window can be
queried, opened by other players, closed by players and finally destroyed.
Also, a cWindowOwner can be queried for the block coords where the window is displayed. That will be used
for entities / players in motion to close their windows when they get too far away from the window "source".
*/





// class cWindow;





/**
Base class for the window owning
*/
class cWindowOwner
{
public:
	cWindowOwner() :
		m_Window(NULL)
	{
	}
	
	virtual ~cWindowOwner()
	{
	}
	
	void CloseWindow(void)
	{
		m_Window = NULL;
	}
	
	void OpenWindow(cWindow * a_Window)
	{
		m_Window = a_Window;
		m_Window->SetOwner(this);
	}

	cWindow * GetWindow(void) const
	{
		return m_Window;
	}
	
private:
	cWindow * m_Window;
};




