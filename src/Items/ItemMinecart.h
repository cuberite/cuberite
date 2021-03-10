
#pragma once





class cItemMinecartHandler final:
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
		// Must be used on a block
		if (a_ClickedBlockFace < 0)
		{
			return false;
		}

		// Check that there's rail in there:
		auto Block = a_World->GetBlock(a_ClickedBlockPos);
		switch (Block.Type())
		{
			case BlockType::Rail:
			case BlockType::PoweredRail:
			case BlockType::DetectorRail:
			case BlockType::ActivatorRail:
			{
				// These are allowed
				break;
			}
			default:
			{
				LOGD("Used minecart on an unsuitable block %d (%s)", "MISSING FUNCTION TO CONVERT BLOCK TO STRING!!", "Missing function for BlockState -> String");
				return false;
			}
		}

		// Spawn the minecart:
		auto SpawnPos = Vector3d(a_ClickedBlockPos) + Vector3d(0.5, 0.5, 0.5);
		if (a_World->SpawnMinecart(SpawnPos, m_ItemType) == cEntity::INVALID_ID)
		{
			return false;
		}

		// Remove the item from inventory:
		if (!a_Player->IsGameModeCreative())
		{
			a_Player->GetInventory().RemoveOneEquippedItem();
		}
		return true;
	}

} ;
