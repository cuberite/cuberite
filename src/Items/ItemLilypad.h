
#pragma once

#include "ItemHandler.h"
#include "../Entities/Player.h"
#include "Vector3.h"
#include "../LineBlockTracer.h"
#include "BlockInfo.h"





class cItemLilypadHandler :
	public cItemHandler
{
	typedef cItemHandler super;

public:
	cItemLilypadHandler(BLOCKTYPE a_BlockType)
		: cItemHandler(a_BlockType)
	{

	}

	virtual bool IsPlaceable(void) override
	{
		return false; // Set as not placeable so OnItemUse is called
	}

	virtual bool OnItemUse(cWorld * a_World, cPlayer * a_Player, const cItem & a_Item, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace) override
	{
		if (a_BlockFace > BLOCK_FACE_NONE)
		{
			// Clicked on the side of a submerged block; vanilla allows placement, so should we
			AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace);
			a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_LILY_PAD, 0);
			if (!a_Player->IsGameModeCreative())
				a_Player->GetInventory().RemoveOneEquippedItem();
			return true;
		}

		class cCallbacks :
			public cBlockTracer::cCallbacks
		{
		public:
			cCallbacks(cWorld * a_World) :
				m_HasHitFluid(false),
				m_World(a_World)
			{
			}

			virtual bool OnNextBlock(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, char a_EntryFace) override
			{
				if (IsBlockWater(a_BlockType))
				{
					if ((a_BlockMeta != 0) || (a_EntryFace == BLOCK_FACE_NONE)) // The hit block should be a source. The FACE_NONE check is clicking whilst submerged
					{
						return false;
					}
					a_EntryFace = BLOCK_FACE_YP; // Always place pad at top of water block
					AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, (eBlockFace)a_EntryFace);
					BLOCKTYPE Block = m_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ);
					if (
						!IsBlockWater(Block) &&
						cBlockInfo::FullyOccupiesVoxel(Block)
						)
					{
						// Can't place lilypad on air/in another block!
						return true;
					}
					m_HasHitFluid = true;
					m_Pos.Set(a_BlockX, a_BlockY, a_BlockZ);
					return true;
				}
				return false;
			}

			Vector3i m_Pos;
			bool m_HasHitFluid;
			cWorld * m_World;

		};

		cCallbacks Callbacks(a_World);
		cLineBlockTracer Tracer(*a_Player->GetWorld(), Callbacks);
		Vector3d Start(a_Player->GetEyePosition() + a_Player->GetLookVector());
		Vector3d End(a_Player->GetEyePosition() + a_Player->GetLookVector() * 5);

		Tracer.Trace(Start.x, Start.y, Start.z, End.x, End.y, End.z);

		if (Callbacks.m_HasHitFluid)
		{
			a_World->SetBlock(Callbacks.m_Pos.x, Callbacks.m_Pos.y, Callbacks.m_Pos.z, E_BLOCK_LILY_PAD, 0);
			if (!a_Player->IsGameModeCreative())
				a_Player->GetInventory().RemoveOneEquippedItem();
			return true;
		}

		return false;
	}
};




