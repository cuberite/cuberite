
// HopperEntity.cpp

// Implements the cHopperEntity representing a hopper block entity

#include "Globals.h"
#include "HopperEntity.h"
#include "../Chunk.h"
#include "../Entities/Player.h"
#include "../Entities/Pickup.h"
#include "../Bindings/PluginManager.h"
#include "../UI/HopperWindow.h"
#include "ChestEntity.h"
#include "FurnaceEntity.h"





// How many ticks at minimum between two item transfers to or from the hopper.
#define TICKS_PER_TRANSFER 8_tick





cHopperEntity::cHopperEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World):
	Super(a_BlockType, a_BlockMeta, a_Pos, ContentsWidth, ContentsHeight, a_World),
	m_LastMoveItemsInTick(0),
	m_LastMoveItemsOutTick(0),
	m_Locked(false)
{
	ASSERT(a_BlockType == E_BLOCK_HOPPER);
}





void cHopperEntity::SetLocked(bool a_Value)
{
	m_Locked = a_Value;
}





std::pair<bool, Vector3i> cHopperEntity::GetOutputBlockPos(NIBBLETYPE a_BlockMeta)
{
	auto pos = GetPos();
	switch (a_BlockMeta)
	{
		case E_META_HOPPER_FACING_XM: return {true, pos.addedX(-1)};
		case E_META_HOPPER_FACING_XP: return {true, pos.addedX( 1)};
		case E_META_HOPPER_FACING_YM: return {true, pos.addedY(-1)};
		case E_META_HOPPER_FACING_ZM: return {true, pos.addedZ(-1)};
		case E_META_HOPPER_FACING_ZP: return {true, pos.addedZ( 1)};
		default:
		{
			// Not attached
			return {false, pos};
		}
	}
}





void cHopperEntity::CopyFrom(const cBlockEntity & a_Src)
{
	Super::CopyFrom(a_Src);
	auto & src = static_cast<const cHopperEntity &>(a_Src);
	m_LastMoveItemsInTick = src.m_LastMoveItemsInTick;
	m_LastMoveItemsOutTick = src.m_LastMoveItemsOutTick;
}





bool cHopperEntity::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	UNUSED(a_Dt);

	bool isDirty = false;
	if (!m_Locked)
	{
		const auto CurrentTick = a_Chunk.GetWorld()->GetWorldAge();
		isDirty = MoveItemsIn(a_Chunk, CurrentTick) || isDirty;
		isDirty = MovePickupsIn(a_Chunk) || isDirty;
		isDirty = MoveItemsOut(a_Chunk, CurrentTick) || isDirty;
	}
	return isDirty;
}





void cHopperEntity::SendTo(cClientHandle & a_Client)
{
	// The hopper entity doesn't need anything sent to the client when it's created / gets in the viewdistance
	// All the actual handling is in the cWindow UI code that gets called when the hopper is right-clicked

	UNUSED(a_Client);
}





bool cHopperEntity::UsedBy(cPlayer * a_Player)
{
	a_Player->GetStatistics().Custom[CustomStatistic::InspectHopper]++;

	// If the window is not created, open it anew:
	cWindow * Window = GetWindow();
	if (Window == nullptr)
	{
		OpenNewWindow();
		Window = GetWindow();
	}

	// Open the window for the player:
	if (Window != nullptr)
	{
		if (a_Player->GetWindow() != Window)
		{
			a_Player->OpenWindow(*Window);
		}
	}

	return true;
}





void cHopperEntity::OpenNewWindow(void)
{
	OpenWindow(new cHopperWindow(this));
}





bool cHopperEntity::MoveItemsIn(cChunk & a_Chunk, const cTickTimeLong a_CurrentTick)
{
	if (m_Pos.y >= cChunkDef::Height)
	{
		// This hopper is at the top of the world, no more blocks above
		return false;
	}

	if ((a_CurrentTick - m_LastMoveItemsInTick) < TICKS_PER_TRANSFER)
	{
		// Too early after the previous transfer
		return false;
	}

	// Try moving an item in:
	bool res = false;
	switch (a_Chunk.GetBlock(GetRelPos().addedY(1)))
	{
		case E_BLOCK_CHEST:
		case E_BLOCK_TRAPPED_CHEST:
		{
			// Chests have special handling because of double-chests
			res = MoveItemsFromChest(a_Chunk);
			break;
		}
		case E_BLOCK_FURNACE:
		case E_BLOCK_LIT_FURNACE:
		{
			// Furnaces have special handling because only the output and leftover fuel buckets shall be moved
			res = MoveItemsFromFurnace(a_Chunk);
			break;
		}
		case E_BLOCK_DISPENSER:
		case E_BLOCK_DROPPER:
		case E_BLOCK_HOPPER:
		{
			res = MoveItemsFromGrid(*static_cast<cBlockEntityWithItems *>(a_Chunk.GetBlockEntity(this->GetPos().addedY(1))));
			break;
		}
	}

	// If the item has been moved, reset the last tick:
	if (res)
	{
		m_LastMoveItemsInTick = a_CurrentTick;
	}

	return res;
}





bool cHopperEntity::MovePickupsIn(cChunk & a_Chunk)
{
	class cHopperPickupSearchCallback
	{
	public:
		cHopperPickupSearchCallback(Vector3i a_Pos, cItemGrid & a_Contents) :
			m_Pos(a_Pos),
			m_bFoundPickupsAbove(false),
			m_Contents(a_Contents)
		{
		}

		bool operator () (cEntity & a_Entity)
		{
			if (!a_Entity.IsPickup())
			{
				return false;
			}

			Vector3f EntityPos = a_Entity.GetPosition();
			Vector3f BlockPos(m_Pos.x + 0.5f, static_cast<float>(m_Pos.y) + 1, m_Pos.z + 0.5f);  // One block above hopper, and search from center outwards
			double Distance = (EntityPos - BlockPos).Length();

			if (Distance < 0.5)
			{
				if (TrySuckPickupIn(static_cast<cPickup &>(a_Entity)))
				{
					return false;
				}
			}

			return false;
		}

		bool TrySuckPickupIn(cPickup & a_Pickup)
		{
			cItem & Item = a_Pickup.GetItem();

			for (std::size_t i = 0; i < ContentsWidth * ContentsHeight; i++)
			{
				if (m_Contents.IsSlotEmpty(i))
				{
					m_bFoundPickupsAbove = true;
					m_Contents.SetSlot(i, Item);
					a_Pickup.Destroy();  // Kill pickup

					return true;
				}
				else if (m_Contents.GetSlot(i).IsEqual(Item) && !m_Contents.GetSlot(i).IsFullStack())
				{
					m_bFoundPickupsAbove = true;

					auto PreviousCount = m_Contents.GetSlot(i).m_ItemCount;

					Item.m_ItemCount -= m_Contents.ChangeSlotCount(i, Item.m_ItemCount) - PreviousCount;  // Set count to however many items were added

					if (Item.IsEmpty())
					{
						a_Pickup.Destroy();  // Kill pickup if all items were added
					}
					return true;
				}
			}
			return false;
		}

		bool FoundPickupsAbove(void) const
		{
			return m_bFoundPickupsAbove;
		}

	protected:
		Vector3i m_Pos;
		bool m_bFoundPickupsAbove;
		cItemGrid & m_Contents;
	};

	cHopperPickupSearchCallback HopperPickupSearchCallback(Vector3i(GetPosX(), GetPosY(), GetPosZ()), m_Contents);
	a_Chunk.ForEachEntity(HopperPickupSearchCallback);

	return HopperPickupSearchCallback.FoundPickupsAbove();
}





bool cHopperEntity::MoveItemsOut(cChunk & a_Chunk, const cTickTimeLong a_CurrentTick)
{
	if ((a_CurrentTick - m_LastMoveItemsOutTick) < TICKS_PER_TRANSFER)
	{
		// Too early after the previous transfer
		return false;
	}

	// Get the coords of the block where to output items:
	auto meta = a_Chunk.GetMeta(GetRelPos());
	auto out = GetOutputBlockPos(meta);
	if (!out.first)
	{
		// Not attached to another container
		return false;
	}
	if (out.second.y < 0)
	{
		// Cannot output below the zero-th block level
		return false;
	}

	// Convert coords to relative:
	auto relCoord = cChunkDef::AbsoluteToRelative(out.second);
	auto destChunk = a_Chunk.GetRelNeighborChunkAdjustCoords(relCoord);
	if (destChunk == nullptr)
	{
		// The destination chunk has been unloaded, don't tick
		return false;
	}

	// Call proper moving function, based on the blocktype present at the coords:
	bool res = false;
	auto absCoord = destChunk->RelativeToAbsolute(relCoord);
	switch (destChunk->GetBlock(relCoord))
	{
		case E_BLOCK_CHEST:
		case E_BLOCK_TRAPPED_CHEST:
		{
			// Chests have special handling because of double-chests
			res = MoveItemsToChest(*destChunk, absCoord);
			break;
		}
		case E_BLOCK_FURNACE:
		case E_BLOCK_LIT_FURNACE:
		{
			// Furnaces have special handling because of the direction-to-slot relation
			res = MoveItemsToFurnace(*destChunk, absCoord, meta);
			break;
		}
		case E_BLOCK_DISPENSER:
		case E_BLOCK_DROPPER:
		case E_BLOCK_HOPPER:
		{
			auto blockEntity = static_cast<cBlockEntityWithItems *>(destChunk->GetBlockEntity(absCoord));
			if (blockEntity == nullptr)
			{
				FLOGWARNING("{0}: A block entity was not found where expected at {1}", __FUNCTION__, absCoord);
				return false;
			}
			res = MoveItemsToGrid(*blockEntity);
			break;
		}
	}

	// If the item has been moved, reset the last tick:
	if (res)
	{
		m_LastMoveItemsOutTick = a_CurrentTick;
	}

	return res;
}





bool cHopperEntity::MoveItemsFromChest(cChunk & a_Chunk)
{
	const auto ConnectedBlockEntity = a_Chunk.GetBlockEntityRel(GetRelPos().addedY(1));

	if (ConnectedBlockEntity == nullptr)
	{
		return false;
	}

	const auto ConnectedChest = static_cast<cChestEntity *>(ConnectedBlockEntity);

	if (MoveItemsFromGrid(ConnectedChest->GetPrimaryChest()))
	{
		return true;
	}

	const auto SecondaryChest = ConnectedChest->GetSecondaryChest();
	return (SecondaryChest != nullptr) && MoveItemsFromGrid(*SecondaryChest);
}





bool cHopperEntity::MoveItemsFromFurnace(cChunk & a_Chunk)
{
	auto furnace = static_cast<cFurnaceEntity *>(a_Chunk.GetBlockEntity(m_Pos.addedY(1)));
	if (furnace == nullptr)
	{
		FLOGWARNING("{0}: A furnace entity was not found where expected, at {1}", __FUNCTION__, m_Pos.addedY(1));
		return false;
	}

	// Try move from the output slot:
	if (MoveItemsFromSlot(*furnace, cFurnaceEntity::fsOutput))
	{
		cItem NewOutput(furnace->GetOutputSlot());
		furnace->SetOutputSlot(NewOutput.AddCount(-1));
		return true;
	}

	// No output moved, check if we can move an empty bucket out of the fuel slot:
	if (furnace->GetFuelSlot().m_ItemType == E_ITEM_BUCKET)
	{
		if (MoveItemsFromSlot(*furnace, cFurnaceEntity::fsFuel))
		{
			furnace->SetFuelSlot(cItem());
			return true;
		}
	}

	// Nothing can be moved
	return false;
}





bool cHopperEntity::MoveItemsFromGrid(cBlockEntityWithItems & a_Entity)
{
	auto & Grid = a_Entity.GetContents();
	size_t NumSlots = Grid.GetNumSlots();

	for (size_t i = 0; i < NumSlots; i++)
	{
		if (Grid.IsSlotEmpty(i))
		{
			continue;
		}
		if (MoveItemsFromSlot(a_Entity, i))
		{
			Grid.ChangeSlotCount(i, -1);
			return true;
		}
	}
	return false;
}





bool cHopperEntity::MoveItemsFromSlot(cBlockEntityWithItems & a_Entity, std::size_t a_SlotNum)
{
	cItem One(a_Entity.GetSlot(a_SlotNum).CopyOne());
	for (std::size_t i = 0; i < ContentsWidth * ContentsHeight; i++)
	{
		if (m_Contents.IsSlotEmpty(i))
		{
			if (cPluginManager::Get()->CallHookHopperPullingItem(*m_World, *this, i, a_Entity, a_SlotNum))
			{
				// Plugin disagrees with the move
				continue;
			}

			m_Contents.SetSlot(i, One);
			return true;
		}
		else if (m_Contents.GetSlot(i).IsEqual(One))
		{
			if (cPluginManager::Get()->CallHookHopperPullingItem(*m_World, *this, i, a_Entity, a_SlotNum))
			{
				// Plugin disagrees with the move
				continue;
			}

			auto PreviousCount = m_Contents.GetSlot(i).m_ItemCount;
			m_Contents.ChangeSlotCount(i, 1);

			if (PreviousCount + 1 == m_Contents.GetSlot(i).m_ItemCount)
			{
				// Successfully added a new item. (Failure condition consistutes: stack full)
				return true;
			}
		}
	}
	return false;
}





bool cHopperEntity::MoveItemsToChest(cChunk & a_Chunk, Vector3i a_Coords)
{
	const auto ConnectedBlockEntity = a_Chunk.GetBlockEntity(a_Coords);

	if (ConnectedBlockEntity == nullptr)
	{
		return false;
	}

	const auto ConnectedChest = static_cast<cChestEntity *>(ConnectedBlockEntity);

	if (MoveItemsToGrid(ConnectedChest->GetPrimaryChest()))
	{
		return true;
	}

	const auto SecondaryChest = ConnectedChest->GetSecondaryChest();
	return (SecondaryChest != nullptr) && MoveItemsToGrid(*SecondaryChest);
}





bool cHopperEntity::MoveItemsToFurnace(cChunk & a_Chunk, Vector3i a_Coords, NIBBLETYPE a_HopperMeta)
{
	auto furnace = static_cast<cFurnaceEntity *>(a_Chunk.GetBlockEntity(a_Coords));
	if (a_HopperMeta == E_META_HOPPER_FACING_YM)
	{
		// Feed the input slot of the furnace
		return MoveItemsToSlot(*furnace, cFurnaceEntity::fsInput);
	}
	else
	{
		// Feed the fuel slot of the furnace
		return MoveItemsToSlot(*furnace, cFurnaceEntity::fsFuel);
	}
}





bool cHopperEntity::MoveItemsToGrid(cBlockEntityWithItems & a_Entity)
{
	// Iterate through our slots, try to move from each one:
	auto NumSlots = a_Entity.GetContents().GetNumSlots();
	for (std::size_t i = 0; i < NumSlots; i++)
	{
		if (MoveItemsToSlot(a_Entity, i))
		{
			return true;
		}
	}
	return false;
}





bool cHopperEntity::MoveItemsToSlot(cBlockEntityWithItems & a_Entity, std::size_t a_DstSlotNum)
{
	cItemGrid & Grid = a_Entity.GetContents();
	if (Grid.IsSlotEmpty(a_DstSlotNum))
	{
		// The slot is empty, move the first non-empty slot from our contents:
		for (std::size_t i = 0; i < ContentsWidth * ContentsHeight; i++)
		{
			if (!m_Contents.IsSlotEmpty(i))
			{
				if (cPluginManager::Get()->CallHookHopperPushingItem(*m_World, *this, i, a_Entity, a_DstSlotNum))
				{
					// A plugin disagrees with the move
					continue;
				}
				Grid.SetSlot(a_DstSlotNum, m_Contents.GetSlot(i).CopyOne());
				m_Contents.ChangeSlotCount(i, -1);
				return true;
			}
		}
		return false;
	}
	else
	{
		// The slot is taken, try to top it up:
		const cItem & DestSlot = Grid.GetSlot(a_DstSlotNum);
		if (DestSlot.IsFullStack())
		{
			return false;
		}
		for (std::size_t i = 0; i < ContentsWidth * ContentsHeight; i++)
		{
			if (m_Contents.GetSlot(i).IsEqual(DestSlot))
			{
				if (cPluginManager::Get()->CallHookHopperPushingItem(*m_World, *this, i, a_Entity, a_DstSlotNum))
				{
					// A plugin disagrees with the move
					continue;
				}
				Grid.ChangeSlotCount(a_DstSlotNum, 1);
				m_Contents.ChangeSlotCount(i, -1);
				return true;
			}
		}
		return false;
	}
}
