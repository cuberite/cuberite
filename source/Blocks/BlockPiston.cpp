#include "Globals.h"
#include "BlockPiston.h"
#include "../Item.h"
#include "../World.h"
#include "../Redstone.h"
#include "../Player.h"
#include "../Piston.h"



#define AddPistonDir(x, y, z, dir, amount) switch (dir) { case 0: (y)-=(amount); break; case 1: (y)+=(amount); break;\
													 case 2: (z)-=(amount); break; case 3: (z)+=(amount); break;\
													 case 4: (x)-=(amount); break; case 5: (x)+=(amount); break; }




cBlockPistonHandler::cBlockPistonHandler(BLOCKTYPE a_BlockType)
	: cBlockHandler(a_BlockType)
{
}





void cBlockPistonHandler::OnPlaced(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ, int a_Dir)
{
}





void cBlockPistonHandler::OnDestroyed(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	char OldMeta = a_World->GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);

	int newX = a_BlockX;
	int newY = a_BlockY;
	int newZ = a_BlockZ;
	AddPistonDir(newX, newY, newZ, OldMeta & ~(8), 1);

	if (a_World->GetBlock(newX, newY, newZ) == E_BLOCK_PISTON_EXTENSION)
	{
		a_World->SetBlock(newX, newY, newZ, E_BLOCK_AIR, 0);
	}
}





void cBlockPistonHandler::PlaceBlock(cWorld * a_World, cPlayer * a_Player, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Dir)
{
	
	a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, m_BlockType, cPiston::RotationPitchToMetaData(a_Player->GetRotation(), a_Player->GetPitch()));
	OnPlacedByPlayer(a_World, a_Player, a_BlockX, a_BlockY, a_BlockZ, a_Dir);

	cRedstone Redstone(a_World);
	Redstone.ChangeRedstone(a_BlockX, a_BlockY, a_BlockZ, false);
}




