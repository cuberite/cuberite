
// MapManager.cpp

#include "Globals.h"

#include "MapManager.h"

#include "World.h"
#include "WorldStorage/MapSerializer.h"





cMapManager::cMapManager(cWorld * a_World)
	: m_World(a_World)
{
	ASSERT(m_World != NULL);
}





bool cMapManager::DoWithMap(unsigned int a_ID, cMapCallback & a_Callback)
{
	cCSLock Lock(m_CS);
	cMap * Map = GetMapData(a_ID);

	if (Map == NULL)
	{
		return false;
	}
	else
	{
		a_Callback.Item(Map);
		return true;
	}
}





bool cMapManager::ForEachMap(cMapCallback & a_Callback)
{
	cCSLock Lock(m_CS);
	for (cMapList::iterator itr = m_MapData.begin(); itr != m_MapData.end(); ++itr)
	{
		cMap * Map = &(*itr);
		if (a_Callback.Item(Map))
		{
			return false;
		}
	}  // for itr - m_MapData[]
	return true;
}





cMap * cMapManager::GetMapData(unsigned int a_ID)
{
	if (a_ID < m_MapData.size())
	{
		return &m_MapData[a_ID];
	}
	else
	{
		return NULL;
	}
}





cMap * cMapManager::CreateMap(int a_CenterX, int a_CenterY, int a_Scale)
{
	cCSLock Lock(m_CS);

	if (m_MapData.size() >= 65536)
	{
		LOGWARN("Could not craft map - Too many maps in use");
		return NULL;
	}

	cMap Map(m_MapData.size(), a_CenterX, a_CenterY, m_World, a_Scale);

	m_MapData.push_back(Map);

	return &m_MapData[Map.GetID()];
}





unsigned int cMapManager::GetNumMaps(void) const
{
	return m_MapData.size();
}





void cMapManager::LoadMapData(void)
{
	cCSLock Lock(m_CS);

	cIDCountSerializer IDSerializer(m_World->GetName());

	if (!IDSerializer.Load())
	{
		return;
	}

	unsigned int MapCount = IDSerializer.GetMapCount();

	m_MapData.clear();

	for (unsigned int i = 0; i < MapCount; ++i)
	{
		cMap Map(i, m_World);

		cMapSerializer Serializer(m_World->GetName(), &Map);

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

	cIDCountSerializer IDSerializer(m_World->GetName());

	IDSerializer.SetMapCount(m_MapData.size());

	if (!IDSerializer.Save())
	{
		LOGERROR("Could not save idcounts.dat");
		return;
	}

	for (cMapList::iterator it = m_MapData.begin(); it != m_MapData.end(); ++it)
	{
		cMap & Map = *it;

		cMapSerializer Serializer(m_World->GetName(), &Map);

		if (!Serializer.Save())
		{
			LOGWARN("Could not save map #%i", Map.GetID());
		}
	}
}





