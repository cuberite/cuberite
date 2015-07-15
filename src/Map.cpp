
// Map.cpp

#include "Globals.h"

#include "Map.h"

#include "World.h"
#include "Chunk.h"
#include "Entities/Player.h"
#include "FastRandom.h"
#include "Blocks/BlockHandler.h"





cMap::cMap(unsigned int a_ID, cWorld * a_World)
	: m_ID(a_ID)
	, m_Width(cChunkDef::Width * 8)
	, m_Height(cChunkDef::Width * 8)
	, m_Scale(3)
	, m_CenterX(0)
	, m_CenterZ(0)
	, m_World(a_World)
{
	m_Data.assign(m_Width * m_Height, E_BASE_COLOR_TRANSPARENT);

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
	m_Data.assign(m_Width * m_Height, E_BASE_COLOR_TRANSPARENT);

	Printf(m_Name, "map_%i", m_ID);
}





void cMap::Tick()
{
	for (const auto & Client : m_ClientsInCurrentTick)
	{
		Client->SendMapData(*this, 0, 0);
	}
	m_ClientsInCurrentTick.clear();
	m_Decorators.clear();
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

	int PixelX = (int) (a_Player.GetPosX() - m_CenterX) / PixelWidth + (m_Width  / 2);
	int PixelZ = (int) (a_Player.GetPosZ() - m_CenterZ) / PixelWidth + (m_Height / 2);

	UpdateRadius(PixelX, PixelZ, a_Radius);
}





bool cMap::UpdatePixel(unsigned int a_X, unsigned int a_Z)
{
	unsigned int PixelWidth = GetPixelWidth();

	int BlockX = m_CenterX + ((a_X - (m_Width  / 2)) * PixelWidth);
	int BlockZ = m_CenterZ + ((a_Z - (m_Height / 2)) * PixelWidth);

	int ChunkX, ChunkZ;
	cChunkDef::BlockToChunk(BlockX, BlockZ, ChunkX, ChunkZ);

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
			: m_Map(a_Map), m_RelX(a_RelX), m_RelZ(a_RelZ), m_PixelData(E_BASE_COLOR_TRANSPARENT) {}

		virtual bool Item(cChunk * a_Chunk) override
		{
			if (!a_Chunk->IsValid())
			{
				return false;
			}

			if (m_Map->GetDimension() == dimNether)
			{
				// TODO 2014-02-22 xdot: Nether maps

				return false;
			}

			static const std::array<unsigned char, 4> BrightnessID = { { 3, 0, 1, 2 } };  // Darkest to lightest
			BLOCKTYPE TargetBlock;
			NIBBLETYPE TargetMeta;

			auto Height = a_Chunk->GetHeight(m_RelX, m_RelZ);
			auto ChunkHeight = cChunkDef::Height;
			a_Chunk->GetBlockTypeMeta(m_RelX, Height, m_RelZ, TargetBlock, TargetMeta);
			auto ColourID = BlockHandler(TargetBlock)->GetMapBaseColourID(TargetMeta);

			if (IsBlockWater(TargetBlock))
			{
				ChunkHeight /= 4;
				while (((--Height) != -1) && IsBlockWater(a_Chunk->GetBlock(m_RelX, Height, m_RelZ)))
				{
					continue;
				}
			}
			else if (ColourID == 0)
			{
				while (((--Height) != -1) && ((ColourID = BlockHandler(a_Chunk->GetBlock(m_RelX, Height, m_RelZ))->GetMapBaseColourID(a_Chunk->GetMeta(m_RelX, Height, m_RelZ))) == 0))
				{
					continue;
				}
			}

			// Multiply base color ID by 4 and add brightness ID
			m_PixelData = ColourID * 4 + BrightnessID[Clamp<size_t>(static_cast<size_t>(Height / (ChunkHeight / BrightnessID.size())), 0, BrightnessID.size() - 1)];
			return false;
		}

		ColorID GetPixelData(void) const
		{
			return m_PixelData;
		}
	} CalculatePixelCb(this, RelX, RelZ);

	ASSERT(m_World != nullptr);
	m_World->DoWithChunk(ChunkX, ChunkZ, CalculatePixelCb);

	SetPixel(a_X, a_Z, CalculatePixelCb.GetPixelData());

	return true;
}





void cMap::UpdateClient(cPlayer * a_Player)
{
	ASSERT(a_Player != nullptr);
	m_Decorators.emplace_back(CreateDecorator(a_Player));
	m_ClientsInCurrentTick.push_back(a_Player->GetClientHandle());
}





eDimension cMap::GetDimension(void) const
{
	ASSERT(m_World != nullptr);
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
	m_CenterX = a_CenterX;
	m_CenterZ = a_CenterZ;
}





bool cMap::SetPixel(unsigned int a_X, unsigned int a_Z, cMap::ColorID a_Data)
{
	if ((a_X < m_Width) && (a_Z < m_Height))
	{
		m_Data[a_Z * m_Width + a_X] = a_Data;

		return true;
	}
	else
	{
		return false;
	}
}





cMap::ColorID cMap::GetPixel(unsigned int a_X, unsigned int a_Z)
{
	if ((a_X < m_Width) && (a_Z < m_Height))
	{
		return m_Data[a_Z * m_Width + a_X];
	}
	else
	{
		return E_BASE_COLOR_TRANSPARENT;
	}
}





unsigned int cMap::GetNumPixels(void) const
{
	return m_Width * m_Height;
}





const cMapDecorator cMap::CreateDecorator(const cEntity * a_TrackedEntity)
{
	int InsideWidth = (GetWidth() / 2) - 1;
	int InsideHeight = (GetHeight() / 2) - 1;
	
	// Center of pixel
	int PixelX = (int)(a_TrackedEntity->GetPosX() - GetCenterX()) / GetPixelWidth();
	int PixelZ = (int)(a_TrackedEntity->GetPosZ() - GetCenterZ()) / GetPixelWidth();

	cMapDecorator::eType Type;
	int Rot;

	if ((PixelX > -InsideWidth) && (PixelX <= InsideWidth) && (PixelZ > -InsideHeight) && (PixelZ <= InsideHeight))
	{
		double Yaw = a_TrackedEntity->GetYaw();

		if (GetDimension() == dimNether)
		{
			cFastRandom Random;

			// TODO 2014-02-19 xdot: Refine
			Rot = Random.NextInt(16);
		}
		else
		{
			Rot = CeilC(((Yaw - 11.25) * 16) / 360);
		}

		Type = cMapDecorator::eType::E_TYPE_PLAYER;
	}
	else
	{
		if ((PixelX > 320.0) || (PixelZ > 320.0))
		{
			;
		}

		Rot = 0;
		Type = cMapDecorator::eType::E_TYPE_PLAYER_OUTSIDE;

		// Move to border
		if (PixelX <= -InsideWidth)
		{
			PixelX = -InsideWidth;
		}
		if (PixelZ <= -InsideHeight)
		{
			PixelZ = -InsideHeight;
		}
		if (PixelX > InsideWidth)
		{
			PixelX = InsideWidth;
		}
		if (PixelZ > InsideHeight)
		{
			PixelZ = InsideHeight;
		}
	}

	return {Type, (unsigned)(2 * PixelX + 1), (unsigned)(2 * PixelZ + 1), Rot};
}




unsigned int cMap::GetPixelWidth(void) const
{
	return (int) pow(2.0, (double) m_Scale);
}





