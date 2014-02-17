
// Map.cpp

#include "Globals.h"

#include "Map.h"

#include "ClientHandle.h"
#include "World.h"
#include "Chunk.h"
#include "Entities/Player.h"





cMap::cMap(unsigned int a_ID, cWorld * a_World)
	: m_ID(a_ID)
	, m_Width(cChunkDef::Width * 8)
	, m_Height(cChunkDef::Width * 8)
	, m_Scale(3)
	, m_CenterX(0)
	, m_CenterZ(0)
	, m_World(a_World)
{
	m_Data.assign(m_Width * m_Height, 0);

	Printf(m_Name, "map_%i", m_ID);
}





cMap::cMap(unsigned int a_ID, int a_CenterX, int a_CenterZ, cWorld * a_World, unsigned int a_Scale)
	: m_ID(a_ID)
	, m_Width(cChunkDef::Width * 8)
	, m_Height(cChunkDef::Width * 8)
	, m_Scale(a_Scale)
	, m_CenterX(a_CenterX)
	, m_CenterZ(a_CenterZ)
	, m_World(a_World)
{
	m_Data.assign(m_Width * m_Height, 0);

	Printf(m_Name, "map_%i", m_ID);
}





template <typename T>
T Clamp(T a_X, T a_Min, T a_Max)
{
	return std::min(std::max(a_X, a_Min), a_Max);
}





void cMap::UpdateRadius(int a_PixelX, int a_PixelZ, unsigned int a_Radius)
{
	int PixelRadius = a_Radius / GetPixelWidth();

	unsigned int StartX = Clamp(a_PixelX - PixelRadius, 0, (int)m_Width);
	unsigned int StartZ = Clamp(a_PixelZ - PixelRadius, 0, (int)m_Height);

	unsigned int EndX   = Clamp(a_PixelX + PixelRadius, 0, (int)m_Width);
	unsigned int EndZ   = Clamp(a_PixelZ + PixelRadius, 0, (int)m_Height);

	for (unsigned int X = StartX; X < EndX; ++X)
	{
		for (unsigned int Z = StartZ; Z < EndZ; ++Z)
		{
			int dX = X - a_PixelX;
			int dZ = Z - a_PixelZ;

			if ((dX * dX) + (dZ * dZ) < (PixelRadius * PixelRadius))
			{
				UpdatePixel(X, Z);
			}
		}
	}
}





void cMap::UpdateRadius(cPlayer & a_Player, unsigned int a_Radius)
{
	unsigned int PixelWidth = GetPixelWidth();

	int PixelX = (a_Player.GetPosX() - m_CenterX) / PixelWidth + (m_Width  / 2);
	int PixelZ = (a_Player.GetPosZ() - m_CenterZ) / PixelWidth + (m_Height / 2);

	UpdateRadius(PixelX, PixelZ, a_Radius);
}





bool cMap::UpdatePixel(unsigned int a_X, unsigned int a_Z)
{
	/*unsigned int PixelWidth = GetPixelWidth();

	int BlockX = m_CenterX + ((a_X - m_Width)  * PixelWidth);
	int BlockZ = m_CenterZ + ((a_Y - m_Height) * PixelWidth);

	int ChunkX, ChunkY, ChunkZ;
	m_World->BlockToChunk(BlockX, 0, BlockZ, ChunkX, ChunkY, ChunkZ);

	int RelX = BlockX - (ChunkX * cChunkDef::Width);
	int RelZ = BlockZ - (ChunkZ * cChunkDef::Width);

	class cCalculatePixelCb :
		public cChunkCallback
	{
		cMap * m_Map;

		int m_RelX, m_RelZ;

		ColorID m_PixelData;

	public:
		cCalculatePixelCb(cMap * a_Map, int a_RelX, int a_RelZ)
			: m_Map(a_Map), m_RelX(a_RelX), m_RelZ(a_RelZ), m_PixelData(4) {}

		virtual bool Item(cChunk * a_Chunk) override
		{
			if (a_Chunk == NULL)
			{
				return false;
			}

			unsigned int PixelWidth = m_Map->GetPixelWidth();

			for (unsigned int X = m_RelX; X < m_RelX + PixelWidth; ++X)
			{
				for (unsigned int Z = m_RelZ; Z < m_RelZ + PixelWidth; ++Z)
				{
					int Height = a_Chunk->GetHeight(X, Z);

					if (Height > 0)
					{
						// TODO
					}
				}
			}

			m_PixelData = 8; // Debug

			return false;
		}

		ColorID GetPixelData(void) const
		{
			return m_PixelData;
		}
	} CalculatePixelCb(this, RelX, RelZ);

	ASSERT(m_World != NULL);
	m_World->DoWithChunk(ChunkX, ChunkZ, CalculatePixelCb);*/

	m_Data[a_Z + (a_X * m_Height)] = 4;

	return true;
}





void cMap::UpdateTrackedPlayers(void)
{
	cTrackedPlayerList NewList;

	for (cTrackedPlayerList::iterator it = m_TrackedPlayers.begin(); it != m_TrackedPlayers.end(); ++it)
	{
		cPlayer * Player = *it;

		UpdateRadius(*Player, DEFAULT_RADIUS);

		if (true)
		{
			NewList.insert(Player);
		}
	}

	std::swap(m_TrackedPlayers, NewList);
}





void cMap::AddTrackedPlayer(cPlayer * a_Player)
{
	ASSERT(a_Player != NULL);
	m_TrackedPlayers.insert(a_Player);
}





void cMap::EraseData(void)
{
	m_Data.assign(m_Width * m_Height, 0);
}





eDimension cMap::GetDimension(void) const
{
	ASSERT(m_World != NULL);
	return m_World->GetDimension();
}






void cMap::Resize(unsigned int a_Width, unsigned int a_Height)
{
	if ((m_Width == a_Width) && (m_Height == a_Height))
	{
		return;
	}

	m_Width = a_Width;
	m_Height = a_Height;

	m_Data.assign(m_Width * m_Height, 0);
}





void cMap::SetPosition(int a_CenterX, int a_CenterZ)
{
	if ((m_CenterX == a_CenterX) && (m_CenterZ == a_CenterZ))
	{
		return;
	}

	m_CenterX = a_CenterX;
	m_CenterZ = a_CenterZ;
}





void cMap::SetScale(unsigned int a_Scale)
{
	if (m_Scale == a_Scale)
	{
		return;
	}

	m_Scale = a_Scale;
}





void cMap::SendTo(cClientHandle & a_Client)
{
	a_Client.SendMapInfo(m_ID, m_Scale);

	for (unsigned int i = 0; i < m_Width; ++i)
	{
		const Byte* Colors = &m_Data[i * m_Height];

		a_Client.SendMapColumn(m_ID, i, 0, Colors, m_Height);
	}
}





unsigned int cMap::GetNumPixels(void) const
{
	return m_Width * m_Height;
}





unsigned int cMap::GetPixelWidth(void) const
{
	return pow(2, m_Scale);
}





