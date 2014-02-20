
// Map.cpp

#include "Globals.h"

#include "Map.h"

#include "ClientHandle.h"
#include "World.h"
#include "Chunk.h"
#include "Entities/Player.h"
#include "FastRandom.h"





cMapDecorator::cMapDecorator(cMap * a_Map, eType a_Type, int a_X, int a_Z, int a_Rot)
	: m_Map(a_Map)
	, m_Type(a_Type)
	, m_PixelX(a_X)
	, m_PixelZ(a_Z)
	, m_Rot(a_Rot)
	, m_Player(NULL)
{
}





cMapDecorator::cMapDecorator(cMap * a_Map, cPlayer * a_Player)
	: m_Map(a_Map)
	, m_Type(E_TYPE_PLAYER)
	, m_Player(a_Player)
{
	Update();
}





void cMapDecorator::Update(void)
{
	if (m_Player != NULL)
	{
		ASSERT(m_Map != NULL);
		unsigned int PixelWidth = m_Map->GetPixelWidth();

		int InsideWidth  = (m_Map->GetWidth()  / 2) - 1;
		int InsideHeight = (m_Map->GetHeight() / 2) - 1;

		int PixelX = (m_Player->GetPosX() - m_Map->GetCenterX()) / PixelWidth;
		int PixelZ = (m_Player->GetPosZ() - m_Map->GetCenterZ()) / PixelWidth;

		// Center of pixel
		m_PixelX = (2 * PixelX) + 1;
		m_PixelZ = (2 * PixelZ) + 1;

		if ((PixelX > -InsideWidth) && (PixelX <= InsideWidth) && (PixelZ > -InsideHeight) && (PixelZ <= InsideHeight))
		{
			double Yaw = m_Player->GetYaw();

			if (m_Map->GetDimension() == dimNether)
			{
				cFastRandom Random;

				Int64 WorldAge = m_Player->GetWorld()->GetWorldAge();

				// TODO 2014-02-19 xdot: Refine
				m_Rot = Random.NextInt(16, WorldAge);
			}
			else
			{
				m_Rot = (Yaw * 16) / 360;
			}

			m_Type = E_TYPE_PLAYER;
		}
		else
		{
			if ((abs(PixelX) > 320.0) || (abs(PixelZ) > 320.0))
			{
				// TODO 2014-02-18 xdot: Remove decorator
			}

			m_Rot = 0;

			m_Type = E_TYPE_PLAYER_OUTSIDE;

			// Move to border
			if (PixelX <= -InsideWidth)
			{
				m_PixelX = (2 * -InsideWidth) + 1;
			}
			if (PixelZ <= -InsideHeight)
			{
				m_PixelZ = (2 * -InsideHeight) + 1;
			}
			if (PixelX > InsideWidth)
			{
				m_PixelX = (2 * InsideWidth) + 1;
			}
			if (PixelZ > InsideHeight)
			{
				m_PixelZ = (2 * InsideHeight) + 1;
			}
		}
	}
}





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
	/*
	unsigned int PixelWidth = GetPixelWidth();

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
	m_World->DoWithChunk(ChunkX, ChunkZ, CalculatePixelCb);
	*/

	m_Data[a_Z + (a_X * m_Height)] = 4;

	return true;
}





void cMap::UpdateDecorators(void)
{
	for (cMapDecoratorList::iterator it = m_Decorators.begin(); it != m_Decorators.end(); ++it)
	{
		it->Update();
	}
}





void cMap::UpdateClient(cPlayer * a_Player)
{
	ASSERT(a_Player != NULL);
	cClientHandle * Handle = a_Player->GetClientHandle();

	if (Handle == NULL)
	{
		return;
	}

	Int64 WorldAge = a_Player->GetWorld()->GetWorldAge();

	// Remove invalid clients
	for (cMapClientList::iterator it = m_Clients.begin(); it != m_Clients.end();)
	{
		// Check if client is active
		if (it->m_LastUpdate < WorldAge - 5)
		{
			// Remove associated decorators
			for (cMapDecoratorList::iterator it2 = m_Decorators.begin(); it2 != m_Decorators.end();)
			{
				if (it2->GetPlayer()->GetClientHandle() == Handle)
				{
					// Erase decorator
					cMapDecoratorList::iterator temp = it2;
					++it2;
					m_Decorators.erase(temp);
				}
				else
				{
					++it2;
				}
			}

			// Erase client
			cMapClientList::iterator temp = it;
			++it;
			m_Clients.erase(temp);
		}
		else
		{
			++it;
		}
	}

	// Linear search for client state
	for (cMapClientList::iterator it = m_Clients.begin(); it != m_Clients.end(); ++it)
	{
		if (it->m_Handle == Handle)
		{
			it->m_LastUpdate = WorldAge;

			if (it->m_SendInfo)
			{
				Handle->SendMapInfo(m_ID, m_Scale);

				it->m_SendInfo = false;

				return;
			}

			++it->m_NextDecoratorUpdate;

			if (it->m_NextDecoratorUpdate >= 4)
			{
				// TODO 2014-02-19 xdot
				// This is dangerous as the player object may have been destroyed before the decorator is erased from the list
				UpdateDecorators();

				Handle->SendMapDecorators(m_ID, m_Decorators);

				it->m_NextDecoratorUpdate = 0;
			}
			else
			{
				++it->m_DataUpdate;

				unsigned int Y = (it->m_DataUpdate * 11) % m_Width;

				const Byte * Colors = &m_Data[Y * m_Height];

				Handle->SendMapColumn(m_ID, Y, 0, Colors, m_Height);
			}

			return;
		}
	}

	// New player, construct a new client state
	cMapClient MapClient;

	MapClient.m_LastUpdate = WorldAge;
	MapClient.m_SendInfo = true;
	MapClient.m_Handle = a_Player->GetClientHandle();

	m_Clients.push_back(MapClient);

	// Insert new decorator
	cMapDecorator PlayerDecorator(this, a_Player);

	m_Decorators.push_back(PlayerDecorator);
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

	for (cMapClientList::iterator it = m_Clients.begin(); it != m_Clients.end(); ++it)
	{
		it->m_SendInfo = true;
	}
}





void cMap::SendTo(cClientHandle & a_Client)
{
	a_Client.SendMapInfo(m_ID, m_Scale);

	for (unsigned int i = 0; i < m_Width; ++i)
	{
		const Byte* Colors = &m_Data[i * m_Height];

		a_Client.SendMapColumn(m_ID, i, 0, Colors, m_Height);
	}

	a_Client.SendMapDecorators(m_ID, m_Decorators);
}





unsigned int cMap::GetNumPixels(void) const
{
	return m_Width * m_Height;
}





unsigned int cMap::GetPixelWidth(void) const
{
	return pow(2, m_Scale);
}





