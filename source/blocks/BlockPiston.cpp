#include "Globals.h"
#include "BlockPiston.h"
#include "../cItem.h"
#include "../cWorld.h"
#include "../cRedstone.h"
#include "../cPlayer.h"
#include "../cPiston.h"



#define AddPistonDir(x, y, z, dir, amount) switch (dir) { case 0: (y)-=(amount); break; case 1: (y)+=(amount); break;\
													 case 2: (z)-=(amount); break; case 3: (z)+=(amount); break;\
													 case 4: (x)-=(amount); break; case 5: (x)+=(amount); break; }




cBlockPistonHandler::cBlockPistonHandler(BLOCKTYPE a_BlockID)
	: cBlockHandler(a_BlockID)
{
}

void cBlockPistonHandler::OnPlaced(cWorld *a_World, int a_X, int a_Y, int a_Z, int a_Dir)
{

}

void cBlockPistonHandler::OnDestroyed(cWorld *a_World, int a_X, int a_Y, int a_Z)
{
	char OldMeta = a_World->GetBlockMeta(a_X, a_Y, a_Z);

	int newX = a_X;
	int newY = a_Y;
	int newZ = a_Z;
	AddPistonDir(newX, newY, newZ, OldMeta & ~(8), 1);

	if (a_World->GetBlock(newX, newY, newZ) == E_BLOCK_PISTON_EXTENSION)
	{
		a_World->SetBlock(newX, newY, newZ, E_BLOCK_AIR, 0);
	}
}

void cBlockPistonHandler::PlaceBlock(cWorld *a_World, cPlayer *a_Player, NIBBLETYPE a_BlockMeta, int a_X, int a_Y, int a_Z, char a_Dir)
{
	
	a_World->SetBlock(a_X, a_Y, a_Z, m_BlockID, cPiston::RotationPitchToMetaData(a_Player->GetRotation(), a_Player->GetPitch()));
	OnPlacedByPlayer(a_World, a_Player, a_X, a_Y, a_Z, a_Dir);

	cRedstone Redstone(a_World);
	Redstone.ChangeRedstone(a_X, a_Y, a_Z, false);
}