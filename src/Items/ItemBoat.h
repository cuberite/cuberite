// ItemBoat.h

// Declares the various boat ItemHandlers





#pragma once

#include "../Entities/Boat.h"
#include "../LineBlockTracer.h"





class cItemBoatHandler :
	public cItemHandler
{
	typedef cItemHandler super;

public:
	cItemBoatHandler(int a_ItemType) :
		super(a_ItemType)
	{
	}



	virtual bool OnItemUse(
		cWorld * a_World, cPlayer * a_Player, cBlockPluginInterface & a_PluginInterface, const cItem & a_Item,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace
	) override
	{
		if ((a_BlockFace != BLOCK_FACE_YM) && (a_BlockFace != BLOCK_FACE_NONE))
		{
			return false;
		}

		class cCallbacks :
			public cBlockTracer::cCallbacks
		{
		public:
			Vector3d m_Pos;
			bool m_HasFound;

			cCallbacks(void) :
				m_HasFound(false)
			{
			}

			virtual bool OnNextBlock(int a_CBBlockX, int a_CBBlockY, int a_CBBlockZ, BLOCKTYPE a_CBBlockType, NIBBLETYPE a_CBBlockMeta, eBlockFace a_CBEntryFace) override
			{
				if (a_CBBlockType != E_BLOCK_AIR)
				{
					m_Pos.Set(a_CBBlockX, a_CBBlockY, a_CBBlockZ);
					m_HasFound = true;
					return true;
				}
				return false;
			}
		} Callbacks;

		cLineBlockTracer Tracer(*a_World, Callbacks);
		Vector3d Start(a_Player->GetEyePosition() + a_Player->GetLookVector());
		Vector3d End(a_Player->GetEyePosition() + a_Player->GetLookVector() * 5);

		Tracer.Trace(Start.x, Start.y, Start.z, End.x, End.y, End.z);

		if (!Callbacks.m_HasFound)
		{
			return false;
		}

		auto x = Callbacks.m_Pos.x;
		auto y = Callbacks.m_Pos.y;
		auto z = Callbacks.m_Pos.z;
		auto bx = FloorC(x);
		auto by = FloorC(y);
		auto bz = FloorC(z);

		// Verify that block type for spawn point is water
		BLOCKTYPE SpawnBlock = a_World->GetBlock(bx, by, bz);
		if (!IsBlockWater(SpawnBlock))
		{
			return false;
		}

		// Block above must be air to spawn a boat (prevents spawning a boat underwater)
		BLOCKTYPE BlockAbove = a_World->GetBlock(bx, by + 1, bz);
		if (BlockAbove != E_BLOCK_AIR)
		{
			return false;
		}

		// Spawn block at water level
		if (a_World->SpawnBoat(x + 0.5, y + 0.5, z + 0.5, cBoat::ItemToMaterial(a_Player->GetEquippedItem())) == cEntity::INVALID_ID)
		{
			return false;
		}

		// Remove boat from players hand
		if (!a_Player->IsGameModeCreative())
		{
			a_Player->GetInventory().RemoveOneEquippedItem();
		}

		return true;
	}
} ;




