
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Village.h"

#include "World.h"





////////////////////////////////////////////////////////////////////////////////
// cVillageHouse:





bool cVillageHouse::IsValidHouse()
{
	// TODO: check the inside and the outside
	return true;
}





bool cVillageHouse::IsDeserted()
{
	return m_Village->GetVillageTime() - m_LastVillagerTime >= HOUSE_VALID_TIME;
}





void cVillageHouse::UpdateHouse()
{
	m_LastVillagerTime = m_Village->GetVillageTime();
}





////////////////////////////////////////////////////////////////////////////////
// cVillage:





void cVillage::Tick(std::chrono::milliseconds a_Dt)
{
	m_Tick += a_Dt.count();

	for (std::size_t I = 0; I < m_Houses.size(); I++)
	{
		cVillageHouse & House = m_Houses[I];

		if (House.IsDeserted())
		{
			RemoveHouse(I);
		}
	}
}





void cVillage::UpdateBoundaries()
{
	if (m_Houses.size() == 0)
	{
		return;
	}

	// Updating Center
	m_AggregateCenter = {};
	m_Center = {};

	for (cVillageHouse & House : m_Houses)
	{
		m_AggregateCenter += House.GetPosition();
	}
	m_Center = m_AggregateCenter / m_Houses.size();

	// Updating Radius
	m_Radius = 0;

	for (std::size_t I = 0; I < m_Houses.size(); I++)
	{
		cVillageHouse & House = m_Houses.at(I);

		float DistanceFromCenter = (House.GetPosition() - m_Center).Length();
		if (DistanceFromCenter > m_Radius)
		{
			if (DistanceFromCenter > VILLAGE_MAXIMUM_RADIUS)
			{
				RemoveHouse(I);
				continue;
			}

			m_Radius = DistanceFromCenter;
		}
	}
}





bool cVillage::AddHouse(const Vector3i & a_DoorPosition)
{
	cVillageHouse NewHouse = {this, a_DoorPosition};

	if (NewHouse.IsValidHouse())
	{
		m_Houses.push_back(NewHouse);
		UpdateBoundaries();
		return true;
	}

	return false;
}





void cVillage::RemoveHouse(std::size_t a_HouseIndex)
{
	m_Houses.erase(m_Houses.begin() + a_HouseIndex);
	UpdateBoundaries();
}
