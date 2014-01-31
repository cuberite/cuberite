
#include "Globals.h"
#include "BlockPiston.h"
#include "../Item.h"
#include "../World.h"
#include "../Entities/Player.h"
#include "../Piston.h"





#define AddPistonDir(x, y, z, dir, amount) \
	switch (dir) \
	{ \
		case 0: (y) -= (amount); break; \
		case 1: (y) += (amount); break; \
		case 2: (z) -= (amount); break; \
		case 3: (z) += (amount); break; \
		case 4: (x) -= (amount); break; \
		case 5: (x) += (amount); break; \
	}




cBlockPistonHandler::cBlockPistonHandler(BLOCKTYPE a_BlockType)
	: cBlockHandler(a_BlockType)
{
}





void cBlockPistonHandler::OnDestroyed(cChunkInterface * a_ChunkInterface, cWorldInterface * a_WorldInterface, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	NIBBLETYPE OldMeta = a_ChunkInterface->GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);

	int newX = a_BlockX;
	int newY = a_BlockY;
	int newZ = a_BlockZ;
	AddPistonDir(newX, newY, newZ, OldMeta & ~(8), 1);

	if (a_ChunkInterface->GetBlock(newX, newY, newZ) == E_BLOCK_PISTON_EXTENSION)
	{
		a_ChunkInterface->SetBlock(a_WorldInterface, newX, newY, newZ, E_BLOCK_AIR, 0);
	}
}





bool cBlockPistonHandler::GetPlacementBlockTypeMeta(
	cChunkInterface * a_ChunkInterface, cPlayer * a_Player,
	int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, 
	int a_CursorX, int a_CursorY, int a_CursorZ,
	BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
)
{
	a_BlockType = m_BlockType;
	a_BlockMeta = cPiston::RotationPitchToMetaData(a_Player->GetYaw(), a_Player->GetPitch());
	return true;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cBlockPistonHeadHandler:

cBlockPistonHeadHandler::cBlockPistonHeadHandler(void) :
	super(E_BLOCK_PISTON_EXTENSION)
{
}





void cBlockPistonHeadHandler::OnDestroyedByPlayer(cWorld * a_World, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	NIBBLETYPE OldMeta = a_World->GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);

	int newX = a_BlockX;
	int newY = a_BlockY;
	int newZ = a_BlockZ;
	AddPistonDir(newX, newY, newZ, OldMeta & ~(8), -1);

	BLOCKTYPE Block = a_World->GetBlock(newX, newY, newZ);
	if ((Block == E_BLOCK_STICKY_PISTON) || (Block == E_BLOCK_PISTON))
	{
		a_World->DigBlock(newX, newY, newZ);
		if (a_Player->IsGameModeCreative())
		{
			return; // No pickups if creative
		}

		cItems Pickups;
		Pickups.push_back(cItem(Block, 1));
		a_World->SpawnItemPickups(Pickups, a_BlockX + 0.5, a_BlockY + 0.5, a_BlockZ + 0.5);
	}
}





