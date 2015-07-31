#pragma once

#include "../BlockEntities/BlockEntity.h"
#include "../Entities/Entity.h"
#include "Window.h"

/* Being a descendant of cWindowOwner means that the class can own one window. That window can be
queried, opened by other players, closed by players and finally destroyed.
Also, a cWindowOwner can be queried for the block coords where the window is displayed. That will be used
for entities / players in motion to close their windows when they get too far away from the window "source". */





/** Base class for the window owning */
class cWindowOwner
{
public:
	cWindowOwner() :
		m_Window(nullptr)
	{
	}

	virtual ~cWindowOwner()
	{
	}

	void CloseWindow(void)
	{
		m_Window = nullptr;
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

	/** Returns the block position at which the element owning the window is */
	virtual Vector3i GetBlockPos(void) = 0;

private:
	cWindow * m_Window;
};





/** Window owner that is associated with a block entity (chest, furnace, ...) */
class cBlockEntityWindowOwner :
	public cWindowOwner
{
public:
	cBlockEntityWindowOwner(cBlockEntity * a_BlockEntity) :
		m_BlockEntity(a_BlockEntity)
	{
	}

	virtual Vector3i GetBlockPos(void) override
	{
		return Vector3i(m_BlockEntity->GetPosX(), m_BlockEntity->GetPosY(), m_BlockEntity->GetPosZ());
	}

private:
	cBlockEntity * m_BlockEntity;
};





/** Window owner that is associated with an entity (chest minecart etc.) */
class cEntityWindowOwner :
	public cWindowOwner
{
public:
	cEntityWindowOwner(cEntity * a_Entity) :
		m_Entity(a_Entity)
	{
	}

	virtual Vector3i GetBlockPos(void) override
	{
		return m_Entity->GetPosition().Floor();
	}

private:
	cEntity * m_Entity;
};




