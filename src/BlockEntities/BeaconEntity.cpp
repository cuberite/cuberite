
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "BeaconEntity.h"
#include "../BlockArea.h"





cBeaconEntity::cBeaconEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World) :
	super(E_BLOCK_BEACON, a_BlockX, a_BlockY, a_BlockZ, a_World)
{
}





int cBeaconEntity::GetPyramidLevel()
{
	cBlockArea Area;
	Area.Read(
		m_World, 
		GetPosX() - 4, 
		GetPosX() + 4, 
		GetPosY() - 5, 
		GetPosY() - 1, 
		GetPosZ() - 4, 
		GetPosZ() + 4
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
					return Layer;
				}
			}
		}
		Layer++;
	}

	return Layer;
}





bool cBeaconEntity::IsMineralBlock(BLOCKTYPE a_BlockType)
{
	switch(a_BlockType)
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