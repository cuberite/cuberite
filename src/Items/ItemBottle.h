
#pragma once

#include "ItemHandler.h"
#include "../World.h"





class cItemBottleHandler :
	public cItemHandler
{
public:
	cItemBottleHandler() :
		cItemHandler(E_ITEM_GLASS_BOTTLE)
	{
	}


	bool GetBlockFromTrace(cWorld * a_World, cPlayer * a_Player, Vector3i & a_BlockPos)
	{
		class cCallbacks :
			public cBlockTracer::cCallbacks
		{
		public:
			Vector3i m_Pos;
			bool     m_HasHitFluid;


			cCallbacks(void) :
				m_HasHitFluid(false)
			{
			}

			virtual bool OnNextBlock(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, eBlockFace a_EntryFace) override
			{
				if (IsBlockWater(a_BlockType))
				{
					if (a_BlockMeta != 0)  // we're only looking for source blocks
					{
						return false;
					}
					m_HasHitFluid = true;
					m_Pos.Set(a_BlockX, a_BlockY, a_BlockZ);
					return true;
				}
				return false;
			}
		} Callbacks;

		cLineBlockTracer Tracer(*a_World, Callbacks);
		Vector3d Start(a_Player->GetEyePosition() + a_Player->GetLookVector());
		Vector3d End(a_Player->GetEyePosition() + a_Player->GetLookVector() * 5);

		Tracer.Trace(Start.x, Start.y, Start.z, End.x, End.y, End.z);

		if (!Callbacks.m_HasHitFluid)
		{
			return false;
		}


		a_BlockPos = Callbacks.m_Pos;
		return true;
	}



	virtual bool OnItemUse(
		cWorld * a_World, cPlayer * a_Player, cBlockPluginInterface & a_PluginInterface, const cItem & a_Item,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace
		) override
	{
		if (a_BlockFace != BLOCK_FACE_NONE)
		{
			return false;
		}

		Vector3i BlockPos;
		if (!GetBlockFromTrace(a_World, a_Player, BlockPos))
		{
			return false;  // Nothing in range.
		}

		a_Player->GetInventory().RemoveOneEquippedItem();
		cItem NewItem(E_ITEM_POTION, 1, 0);
		a_Player->GetInventory().AddItem(NewItem);
		return true;
	}
} ;




