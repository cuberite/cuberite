
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "BeaconEntity.h"
#include "../BlockArea.h"





cBeaconEntity::cBeaconEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World) :
	super(E_BLOCK_BEACON, a_BlockX, a_BlockY, a_BlockZ, a_World)
{
}





int cBeaconEntity::GetPyramidLevel(void)
{
	cBlockArea Area;
	int MinY = GetPosY() - 4;
	if (MinY < 0)
	{
		MinY = 0;
	}
	int MaxY = GetPosY() - 1;
	if (MaxY < 0)
	{
		MaxY = 0;
	}

	Area.Read(
		m_World,
		GetPosX() - 4, GetPosX() + 4,
		MinY, MaxY,
		GetPosZ() - 4, GetPosZ() + 4,
		cBlockArea::baTypes
	);

	int Layer = 1;
	int MiddleXZ = 4;

	for (int Y = Area.GetSizeY() - 1; Y > 0; Y--)
	{
		for (int X = MiddleXZ - Layer; X <= (MiddleXZ + Layer); X++)
		{
			for (int Z = MiddleXZ - Layer; Z <= (MiddleXZ + Layer); Z++)
			{
				if (!IsMineralBlock(Area.GetRelBlockType(X, Y, Z)))
				{
					return Layer - 1;
				}
			}
		}
		Layer++;
	}

	return Layer - 1;
}





bool cBeaconEntity::IsMineralBlock(BLOCKTYPE a_BlockType)
{
	switch (a_BlockType)
	{
		case E_BLOCK_DIAMOND_BLOCK:
		case E_BLOCK_GOLD_BLOCK:
		case E_BLOCK_IRON_BLOCK:
		case E_BLOCK_EMERALD_BLOCK:
		{
			return true;
		}
	}
	return false;
}





bool cBeaconEntity::Tick(float a_Dt, cChunk & a_Chunk)
{
	return false;
}





void cBeaconEntity::SaveToJson(Json::Value& a_Value)
{
}




void cBeaconEntity::SendTo(cClientHandle & a_Client)
{
}





void cBeaconEntity::UsedBy(cPlayer * a_Player)
{
}




