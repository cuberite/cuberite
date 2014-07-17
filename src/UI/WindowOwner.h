
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

	/// Returns the block position at which the element owning the window is
	virtual void GetBlockPos(int & a_BlockX, int & a_BlockY, int & a_BlockZ) = 0;
	
private:
	cWindow * m_Window;
} ;





/**
Window owner that is associated with a block entity (chest, furnace, ...)
*/
class cBlockEntityWindowOwner :
	public cWindowOwner
{
public:
	cBlockEntityWindowOwner(void) :
		m_BlockEntity(NULL)
	{
	}
	
	void SetBlockEntity(cBlockEntity * a_BlockEntity)
	{
		m_BlockEntity = a_BlockEntity;
	}
	
	virtual void GetBlockPos(int & a_BlockX, int & a_BlockY, int & a_BlockZ) override
	{
		a_BlockX = m_BlockEntity->GetPosX();
		a_BlockY = m_BlockEntity->GetPosY();
		a_BlockZ = m_BlockEntity->GetPosZ();
	}
	
private:
	cBlockEntity * m_BlockEntity;
} ;





/**
Window owner that is associated with an entity (chest minecart)
*/
class cEntityWindowOwner :
	public cWindowOwner
{
public:
	cEntityWindowOwner(void) :
		m_Entity(NULL)
	{
	}
	
	void SetEntity(cEntity * a_Entity)
	{
		m_Entity = a_Entity;
	}

	virtual void GetBlockPos(int & a_BlockX, int & a_BlockY, int & a_BlockZ) override
	{
		a_BlockX = (int)floor(m_Entity->GetPosX() + 0.5);
		a_BlockY = (int)floor(m_Entity->GetPosY() + 0.5);
		a_BlockZ = (int)floor(m_Entity->GetPosZ() + 0.5);
	}
	
private:
	cEntity * m_Entity;
} ;




