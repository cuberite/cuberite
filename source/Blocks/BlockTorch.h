
#pragma once

#include "BlockHandler.h"
#include "../Torch.h"
#include "../World.h"





class cBlockTorchHandler :
	public cBlockHandler
{
public:
	cBlockTorchHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}
	
	
	virtual bool GetPlacementBlockTypeMeta(
		cWorld * a_World, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, 
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		// Find proper placement. Use the player-supplied one as the default, but fix if not okay:
		if (!TorchCanBePlacedAt(a_World, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace))
		{
			a_BlockFace = FindSuitableFace(a_World, a_BlockX, a_BlockY, a_BlockZ);
			
			if (a_BlockFace == BLOCK_FACE_BOTTOM)
			{
				return false;
			}
		}
		a_BlockType = m_BlockType;
		a_BlockMeta = cTorch::DirectionToMetaData(a_BlockFace);
		return true;
	}
	

	virtual bool DoesAllowBlockOnTop(void) override
	{
		return false;
	}


	static bool CanBePlacedOn(BLOCKTYPE a_BlockType, char a_Direction)
	{
		switch (a_BlockType)
		{
			case E_BLOCK_GLASS:
			case E_BLOCK_FENCE:
			case E_BLOCK_NETHER_BRICK_FENCE:
			{
				return (a_Direction == 0x1);  // allow only direction "standing on floor"
			}
			
			default:
			{
				return g_BlockIsSolid[a_BlockType];
			}
		}
	}
	
	
	static bool TorchCanBePlacedAt(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace)
	{
		// TODO: If placing a torch from below, check all 4 XZ neighbors, place it on that neighbor instead
		// How to propagate that change up?
		// Simon: The easiest way is to calculate the position two times, shouldn´t cost much cpu power :)

		if (a_BlockFace == BLOCK_FACE_BOTTOM)
		{
			return false;
		}

		AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, true);

		return CanBePlacedOn(a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ), a_BlockFace);
	}
	
	
	/// Finds a suitable Face for the Torch. Returns BLOCK_FACE_BOTTOM on failure
	static char FindSuitableFace(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ)
	{
		for (int i = 1; i <= 5; i++)
		{
			if (TorchCanBePlacedAt(a_World, a_BlockX, a_BlockY, a_BlockZ, i))
			{
				return i;
			}
		}
		return BLOCK_FACE_BOTTOM;
	}


	virtual bool CanBePlacedAt(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace) override
	{
		if (TorchCanBePlacedAt(a_World, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace))
		{
			return true;
		}

		return (FindSuitableFace(a_World, a_BlockX, a_BlockY, a_BlockZ) != BLOCK_FACE_BOTTOM);
	}


	virtual bool CanBeAt(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		char Face = cTorch::MetaDataToDirection(a_World->GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ));
		return TorchCanBePlacedAt(a_World, a_BlockX, a_BlockY, a_BlockZ, Face);
	}


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// Always drop meta = 0
		a_Pickups.push_back(cItem(m_BlockType, 1, 0));
	}


	virtual const char * GetStepSound(void) override
	{
		return "step.wood";
	}
} ;




