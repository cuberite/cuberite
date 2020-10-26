
#pragma once

#include "../../BoundingBox.h"
#include "../../Entities/Pickup.h"





namespace PressurePlateHandler
{
	inline unsigned char GetPowerLevel(const cChunk & Chunk, const Vector3i Position, const BLOCKTYPE BlockType)
	{
		Int64 NumberOfEntities = 0;
		bool FoundPlayer = false;

		Chunk.ForEachEntityInBox(cBoundingBox(Vector3d(0.5, 0, 0.5) + Position, 0.5, 0.5), [&](cEntity & Entity)
		{
			if (Entity.IsPlayer())
			{
				FoundPlayer = true;
			}

			if (Entity.IsPickup())
			{
				NumberOfEntities += static_cast<cPickup &>(Entity).GetItem().m_ItemCount;
				return false;
			}
			NumberOfEntities++;
			return false;
		});

		switch (BlockType)
		{
			case E_BLOCK_STONE_PRESSURE_PLATE:
			{
				return (FoundPlayer ? 15 : 0);
			}
			case E_BLOCK_WOODEN_PRESSURE_PLATE:
			{
				return (NumberOfEntities > 0 ? 15 : 0);
			}
			case E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE:
			{
				return std::min(static_cast<unsigned char>(CeilC(NumberOfEntities / 10.f)), static_cast<unsigned char>(15));
			}
			case E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE:
			{
				return std::min(static_cast<unsigned char>(NumberOfEntities), static_cast<unsigned char>(15));
			}
			default:
			{
				ASSERT(!"Unhandled/unimplemented block in pressure plate handler!");
				return 0;
			}
		}
	}

	inline const char * GetClickOnSound(BLOCKTYPE a_BlockType)
	{
		// manage on-sound
		switch (a_BlockType)
		{
			case E_BLOCK_STONE_PRESSURE_PLATE: return "block.stone_pressureplate.click_on";
			case E_BLOCK_WOODEN_PRESSURE_PLATE: return "block.wood_pressureplate.click_on";
			case E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE:
			case E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE: return "block.metal_pressureplate.click_on";
			default:
			{
				ASSERT(!"No on sound for this one!");
				return "";
			}
		}
	}

	inline const char * GetClickOffSound(BLOCKTYPE a_BlockType)
	{
		// manage off-sound
		switch (a_BlockType)
		{
			case E_BLOCK_STONE_PRESSURE_PLATE: return "block.stone_pressureplate.click_off";
			case E_BLOCK_WOODEN_PRESSURE_PLATE: return "block.wood_pressureplate.click_off";
			case E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE:
			case E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE: return "block.metal_pressureplate.click_off";
			default:
			{
				ASSERT(!"No off sound for this one!");
				return "";
			}
		}
	}

	inline PowerLevel GetPowerDeliveredToPosition(const cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, Vector3i a_QueryPosition, BLOCKTYPE a_QueryBlockType, bool IsLinked)
	{
		UNUSED(a_BlockType);
		UNUSED(a_QueryPosition);
		UNUSED(a_QueryBlockType);

		// Plates only link power blocks below
		// Retrieve and return the cached power calculated by Update for performance:
		return (IsLinked && (a_QueryPosition != (a_Position + OffsetYM))) ? 0 : DataForChunk(a_Chunk).GetCachedPowerData(a_Position);
	}

	inline void Update(cChunk & a_Chunk, cChunk & CurrentlyTicking, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, const PowerLevel Power)
	{
		// LOGD("Evaluating clicky the pressure plate (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);

		auto & ChunkData = DataForChunk(a_Chunk);

		const auto PreviousPower = ChunkData.GetCachedPowerData(a_Position);
		const auto Absolute = cChunkDef::RelativeToAbsolute(a_Position, a_Chunk.GetPos());
		const auto PowerLevel = GetPowerLevel(a_Chunk, Absolute, a_BlockType);  // Get the current power of the platey
		const auto DelayInfo = ChunkData.GetMechanismDelayInfo(a_Position);

		// Resting state?
		if (DelayInfo == nullptr)
		{
			if (PowerLevel == 0)
			{
				// Nothing happened, back to rest
				return;
			}

			// From rest, a player stepped on us
			// Schedule a minimum 0.5 second delay before even thinking about releasing
			ChunkData.m_MechanismDelays[a_Position] = std::make_pair(5, true);

			a_Chunk.GetWorld()->BroadcastSoundEffect(GetClickOnSound(a_BlockType), Absolute, 0.5f, 0.6f);

			// Update power
			ChunkData.SetCachedPowerData(a_Position, PowerLevel);

			// Immediately depress plate
			a_Chunk.SetMeta(a_Position, E_META_PRESSURE_PLATE_DEPRESSED);

			UpdateAdjustedRelatives(a_Chunk, CurrentlyTicking, a_Position, RelativeAdjacents);
			return;
		}

		// Not a resting state

		int DelayTicks;
		bool HasExitedMinimumOnDelayPhase;
		std::tie(DelayTicks, HasExitedMinimumOnDelayPhase) = *DelayInfo;

		// Are we waiting for the initial delay or subsequent release delay?
		if (DelayTicks > 0)
		{
			// Nothing changes, if there is nothing on it anymore, because the state is locked.
			if (PowerLevel == 0)
			{
				return;
			}

			// Yes. Are we waiting to release, and found that the player stepped on it again?
			if (!HasExitedMinimumOnDelayPhase)
			{
				// Reset delay
				*DelayInfo = std::make_pair(0, true);
			}

			// Did the power level change and is still above zero?
			if (PowerLevel != PreviousPower)
			{
				// Yes. Update power
				ChunkData.SetCachedPowerData(a_Position, PowerLevel);
				UpdateAdjustedRelatives(a_Chunk, CurrentlyTicking, a_Position, RelativeAdjacents);
			}

			return;
		}

		// Not waiting for anything. Has the initial delay elapsed?
		if (HasExitedMinimumOnDelayPhase)
		{
			// Yep, initial delay elapsed. Has the player gotten off?
			if (PowerLevel == 0)
			{
				// Yes. Go into subsequent release delay, for a further 0.5 seconds
				*DelayInfo = std::make_pair(5, false);
				return;
			}

			// Did the power level change and is still above zero?
			if (PowerLevel != PreviousPower)
			{
				// Yes. Update power
				ChunkData.SetCachedPowerData(a_Position, PowerLevel);
				UpdateAdjustedRelatives(a_Chunk, CurrentlyTicking, a_Position, RelativeAdjacents);
			}

			// Yes, but player's still on the plate, do nothing
			return;
		}

		// Just got out of the subsequent release phase, reset everything and raise the plate
		ChunkData.m_MechanismDelays.erase(a_Position);

		a_Chunk.GetWorld()->BroadcastSoundEffect(GetClickOffSound(a_BlockType), Absolute, 0.5f, 0.5f);
		ChunkData.SetCachedPowerData(a_Position, PowerLevel);

		a_Chunk.SetMeta(a_Position, E_META_PRESSURE_PLATE_RAISED);
		UpdateAdjustedRelatives(a_Chunk, CurrentlyTicking, a_Position, RelativeAdjacents);
	}

	inline void ForValidSourcePositions(const cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, ForEachSourceCallback & Callback)
	{
		UNUSED(a_Chunk);
		UNUSED(a_Position);
		UNUSED(a_BlockType);
		UNUSED(a_Meta);
		UNUSED(Callback);
	}
};
