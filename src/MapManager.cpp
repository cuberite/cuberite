
// MapManager.cpp

#include "Globals.h"

#include "MapManager.h"

#include "World.h"
#include "WorldStorage/MapSerializer.h"





cMapManager::cMapManager(cWorld * a_World)
	: m_World(a_World)
{
	ASSERT(m_World != nullptr);
}





bool cMapManager::DoWithMap(UInt32 a_ID, cMapCallback a_Callback)
{
	cCSLock Lock(m_CS);
	cMap * Map = GetMapData(a_ID);

	if (Map == nullptr)
	{
		return false;
	}
	else
	{
		a_Callback(*Map);
		return true;
	}
}





void cMapManager::TickMaps()
{
	cCSLock Lock(m_CS);
	for (auto & Map : m_MapData)
	{
		Map.Tick();
	}
}





cMap * cMapManager::GetMapData(unsigned int a_ID)
{
	if (a_ID < m_MapData.size())
	{
		return &m_MapData[a_ID];
	}
	else
	{
		return nullptr;
	}
}





cMap * cMapManager::CreateMap(int a_CenterX, int a_CenterY, unsigned int a_Scale)
{
	cCSLock Lock(m_CS);

	if (m_MapData.size() >= 65536)
	{
		LOGWARN("Could not craft map - Too many maps in use");
		return nullptr;
	}

	cMap Map(static_cast<unsigned>(m_MapData.size()), a_CenterX, a_CenterY, m_World, a_Scale);

	m_MapData.push_back(Map);

	return &m_MapData[Map.GetID()];
}





void cMapManager::LoadMapData(void)
{
	cCSLock Lock(m_CS);

	cIDCountSerializer IDSerializer(m_World->GetDataPath());

	if (!IDSerializer.Load())
	{
		return;
	}

	unsigned int MapCount = IDSerializer.GetMapCount();

	m_MapData.clear();

	for (unsigned int i = 0; i < MapCount; ++i)
	{
		cMap Map(i, m_World);

		cMapSerializer Serializer(m_World->GetDataPath(), &Map);

		if (!Serializer.Load())
		{
			LOGWARN("Could not load map #%i", Map.GetID());
		}

		m_MapData.push_back(Map);
	}
}





void cMapManager::SaveMapData(void)
{
	cCSLock Lock(m_CS);

	if (m_MapData.empty())
	{
		return;
	}

	cIDCountSerializer IDSerializer(m_World->GetDataPath());

	IDSerializer.SetMapCount(static_cast<unsigned>(m_MapData.size()));

	if (!IDSerializer.Save())
	{
		LOGERROR("Could not save idcounts.dat");
		return;
	}

	for (cMapList::iterator it = m_MapData.begin(); it != m_MapData.end(); ++it)
	{
		cMap & Map = *it;

		cMapSerializer Serializer(m_World->GetDataPath(), &Map);

		if (!Serializer.Save())
		{
			LOGWARN("Could not save map #%i", Map.GetID());
		}
	}
}





