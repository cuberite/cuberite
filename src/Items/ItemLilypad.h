
#pragma once

#include "ItemHandler.h"
#include "../Entities/Player.h"
#include "../LineBlockTracer.h"





class cItemLilypadHandler :
	public cItemHandler
{
	typedef cItemHandler super;

public:
	cItemLilypadHandler(int a_ItemType):
		super(a_ItemType)
	{

	}


	virtual bool IsPlaceable(void) override
	{
		return false;  // Set as not placeable so OnItemUse is called
	}



	virtual bool OnItemUse(
		cWorld * a_World, cPlayer * a_Player, cBlockPluginInterface & a_PluginInterface, const cItem & a_Item,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace
	) override
	{
		if (a_BlockFace > BLOCK_FACE_NONE)
		{
			// Clicked on the side of a submerged block; vanilla allows placement, so should we
			AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace);
			a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_LILY_PAD, 0);
			if (!a_Player->IsGameModeCreative())
			{
				a_Player->GetInventory().RemoveOneEquippedItem();
			}
			return true;
		}

		class cCallbacks :
			public cBlockTracer::cCallbacks
		{
		public:

			cCallbacks(void) :
				m_HasHitFluid(false)
			{
			}

			virtual bool OnNextBlock(int a_CBBlockX, int a_CBBlockY, int a_CBBlockZ, BLOCKTYPE a_CBBlockType, NIBBLETYPE a_CBBlockMeta, eBlockFace a_CBEntryFace) override
			{
				if (IsBlockWater(a_CBBlockType))
				{
					if ((a_CBBlockMeta != 0) || (a_CBEntryFace == BLOCK_FACE_NONE))  // The hit block should be a source. The FACE_NONE check is clicking whilst submerged
					{
						return false;
					}
					AddFaceDirection(a_CBBlockX, a_CBBlockY, a_CBBlockZ, BLOCK_FACE_YP);  // Always place pad at top of water block
					if (
						!IsBlockWater(a_CBBlockType) &&
						cBlockInfo::FullyOccupiesVoxel(a_CBBlockType)
						)
					{
						// Can't place lilypad on air / in another block!
						return true;
					}
					m_HasHitFluid = true;
					m_Pos.Set(a_CBBlockX, a_CBBlockY, a_CBBlockZ);
					return true;
				}
				return false;
			}

			Vector3i m_Pos;
			bool m_HasHitFluid;

		};

		cCallbacks Callbacks;
		cLineBlockTracer Tracer(*a_Player->GetWorld(), Callbacks);
		Vector3d Start(a_Player->GetEyePosition() + a_Player->GetLookVector());
		Vector3d End(a_Player->GetEyePosition() + a_Player->GetLookVector() * 5);

		Tracer.Trace(Start.x, Start.y, Start.z, End.x, End.y, End.z);

		if (Callbacks.m_HasHitFluid)
		{
			a_World->SetBlock(Callbacks.m_Pos.x, Callbacks.m_Pos.y, Callbacks.m_Pos.z, E_BLOCK_LILY_PAD, 0);
			if (!a_Player->IsGameModeCreative())
			{
				a_Player->GetInventory().RemoveOneEquippedItem();
			}
			return true;
		}

		return false;
	}
};
