
#pragma once

#include "cBlockEntity.h"
#include "cEntity.h"





class cWindow;





/**
Base class for the behavior expected from a class that can handle UI windows for block entities.
*/
class cWindowOwner
{
public:
	cWindowOwner() :
		m_Window(NULL)
	{
	}
	
	void CloseWindow(void)
	{
		m_Window = NULL;
	}
	
	void OpenWindow(cWindow * a_Window)
	{
		m_Window = a_Window;
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
		a_BlockX = (int)(m_Entity->GetPosX());
		a_BlockY = (int)(m_Entity->GetPosY());
		a_BlockZ = (int)(m_Entity->GetPosZ());
	}
	
private:
	cEntity * m_Entity;
} ;




