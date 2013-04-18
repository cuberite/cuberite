
#pragma once

#include "BlockHandler.h"
#include "../Player.h"
#include "../TNTEntity.h"




class cBlockTNTHandler : public cBlockHandler
{
public:
	cBlockTNTHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}
	
	virtual void OnUse(cWorld * a_World, cPlayer *a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) override
	{
		if (a_Player->GetEquippedWeapon().m_ItemType == E_ITEM_FLINT_AND_STEEL)
		{
			a_World->BroadcastSoundEffect("random.fuse", a_BlockX * 8, a_BlockY * 8, a_BlockZ * 8, 0.5f, 0.6f);
			cTNTEntity *TNT = new cTNTEntity(a_BlockX,a_BlockY,a_BlockZ,4); //4 seconds to boom
			TNT->Initialize(a_World);
			a_World->SetBlock(a_BlockX,a_BlockY,a_BlockZ, E_BLOCK_AIR, 0);
		}
	}
	
	virtual bool IsUseable() override
	{
		return true;
	}
};




