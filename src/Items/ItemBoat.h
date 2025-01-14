
#pragma once

#include "../Entities/Boat.h"
#include "../LineBlockTracer.h"





class cItemBoatHandler final:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	using Super::Super;





	virtual bool OnItemUse(
		cWorld * a_World,
		cPlayer * a_Player,
		cBlockPluginInterface & a_PluginInterface,
		const cItem & a_HeldItem,
		const Vector3i a_ClickedBlockPos,
		eBlockFace a_ClickedBlockFace
	) const override
	{
		// Only allow placing blocks on top of blocks, or when not in range of dest block:
		if ((a_ClickedBlockFace != BLOCK_FACE_YP) && (a_ClickedBlockFace != BLOCK_FACE_NONE))
		{
			return false;
		}

		// Find the actual placement position by tracing line of sight until non-air block:
		class cCallbacks:
			public cBlockTracer::cCallbacks
		{
		public:
			Vector3d m_Pos;
			bool m_HasFound;

			cCallbacks():
				m_HasFound(false)
			{
			}

			virtual bool OnNextBlock(Vector3i a_CBBlockPos, BlockState a_Block, eBlockFace a_CBEntryFace) override
			{
				if (a_Block != Block::Air::Air())
				{
					m_Pos = a_CBBlockPos;
					m_HasFound = true;
					return true;
				}
				return false;
			}
		} Callbacks;
		auto Start = a_Player->GetEyePosition() + a_Player->GetLookVector();
		auto End = a_Player->GetEyePosition() + a_Player->GetLookVector() * 5;
		cLineBlockTracer::Trace(*a_World, Callbacks, Start, End);
		if (!Callbacks.m_HasFound)
		{
			return false;
		}

		// Block above must be air to spawn a boat (prevents spawning a boat underwater)
		auto PosAbove = Callbacks.m_Pos.Floor().addedY(1);
		if (!cChunkDef::IsValidHeight(PosAbove))
		{
			return false;
		}
		auto BlockAbove = a_World->GetBlock(PosAbove);
		if (BlockAbove != Block::Air::Air())
		{
			return false;
		}

		// Spawn block at water level
		if (a_World->SpawnBoat(Callbacks.m_Pos + Vector3d(0.5, 1, 0.5), cBoat::ItemToMaterial(a_Player->GetEquippedItem())) == cEntity::INVALID_ID)
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
