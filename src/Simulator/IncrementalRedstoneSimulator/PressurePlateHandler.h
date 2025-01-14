
#pragma once

#include "../../BoundingBox.h"
#include "../../Entities/Pickup.h"
#include "../../Blocks/BlockPressurePlate.h"





namespace PressurePlateHandler
{
	static unsigned char GetPowerLevel(const cChunk & a_Chunk, const Vector3i a_Position, const BlockState a_Block)
	{
		size_t NumberOfEntities = 0;
		bool FoundPlayer = false;

		a_Chunk.ForEachEntityInBox(cBoundingBox(Vector3d(0.5, 0, 0.5) + a_Position, 0.5, 0.5), [&](cEntity & Entity)
		{
			if (Entity.GetHealth() <= 0)
			{
				return false;
			}

			if (Entity.IsPlayer())
			{
				const auto & Player = static_cast<cPlayer &>(Entity);

				if (Player.IsGameModeSpectator())
				{
					return false;
				}

				FoundPlayer = true;
			}
			else if (Entity.IsPickup())
			{
				const auto & Pickup = static_cast<cPickup &>(Entity);
				NumberOfEntities += static_cast<size_t>(Pickup.GetItem().m_ItemCount);
				return false;
			}

			NumberOfEntities++;
			return false;
		});

		switch (a_Block.Type())
		{
			case BlockType::StonePressurePlate:
			{
				return FoundPlayer ? 15 : 0;
			}
			case BlockType::AcaciaPressurePlate:
			case BlockType::BirchPressurePlate:
			case BlockType::CrimsonPressurePlate:
			case BlockType::DarkOakPressurePlate:
			case BlockType::JunglePressurePlate:
			case BlockType::OakPressurePlate:
			case BlockType::SprucePressurePlate:
			case BlockType::WarpedPressurePlate:
			case BlockType::PolishedBlackstonePressurePlate:
			{
				return (NumberOfEntities != 0) ? 15 : 0;
			}
			case BlockType::HeavyWeightedPressurePlate:
			{
				return std::min(static_cast<unsigned char>(CeilC(NumberOfEntities / 10.f)), static_cast<unsigned char>(15));
			}
			case BlockType::LightWeightedPressurePlate:
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

	static const char * GetClickOnSound(BlockState a_Block)
	{
		// manage on-sound
		switch (a_Block.Type())
		{
			case BlockType::PolishedBlackstonePressurePlate:
			case BlockType::StonePressurePlate: return "block.stone_pressureplate.click_on";
			case BlockType::AcaciaPressurePlate:
			case BlockType::BirchPressurePlate:
			case BlockType::CrimsonPressurePlate:
			case BlockType::DarkOakPressurePlate:
			case BlockType::JunglePressurePlate:
			case BlockType::OakPressurePlate:
			case BlockType::SprucePressurePlate:
			case BlockType::WarpedPressurePlate: return "block.wood_pressureplate.click_on";
			case BlockType::HeavyWeightedPressurePlate:
			case BlockType::LightWeightedPressurePlate: return "block.metal_pressureplate.click_on";
			default:
			{
				ASSERT(!"No on sound for this one!");
				return "";
			}
		}
	}

	static const char * GetClickOffSound(BlockState a_Block)
	{
		// manage off-sound
		switch (a_Block.Type())
		{
			case BlockType::PolishedBlackstonePressurePlate:
			case BlockType::StonePressurePlate: return "block.stone_pressureplate.click_off";
			case BlockType::AcaciaPressurePlate:
			case BlockType::BirchPressurePlate:
			case BlockType::CrimsonPressurePlate:
			case BlockType::DarkOakPressurePlate:
			case BlockType::JunglePressurePlate:
			case BlockType::OakPressurePlate:
			case BlockType::SprucePressurePlate:
			case BlockType::WarpedPressurePlate: return "block.wood_pressureplate.click_off";
			case BlockType::HeavyWeightedPressurePlate:
			case BlockType::LightWeightedPressurePlate: return "block.metal_pressureplate.click_off";
			default:
			{
				ASSERT(!"No off sound for this one!");
				return "";
			}
		}
	}

	static PowerLevel GetPowerDeliveredToPosition(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Block, Vector3i a_QueryPosition, BlockState a_QueryBlock, bool IsLinked)
	{
		UNUSED(a_Block);
		UNUSED(a_QueryPosition);
		UNUSED(a_QueryBlock);

		// Plates only link power blocks below
		// Retrieve and return the cached power calculated by Update for performance:
		return (IsLinked && (a_QueryPosition != (a_Position + OffsetYM))) ? 0 : DataForChunk(a_Chunk).GetCachedPowerData(a_Position);
	}

	static void Update(cChunk & a_Chunk, cChunk & CurrentlyTicking, Vector3i a_Position, BlockState a_Block, const PowerLevel Power)
	{
		LOGREDSTONE("Evaluating clicky the pressure plate (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);

		auto & ChunkData = DataForChunk(a_Chunk);

		const auto PreviousPower = ChunkData.GetCachedPowerData(a_Position);
		const auto Absolute = cChunkDef::RelativeToAbsolute(a_Position, a_Chunk.GetPos());
		const auto PowerLevel = GetPowerLevel(a_Chunk, Absolute, a_Block);  // Get the current power of the platey
		const auto DelayInfo = ChunkData.GetMechanismDelayInfo(a_Position);
		cChunkInterface ChunkInterface(a_Chunk.GetWorld()->GetChunkMap());

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

			a_Chunk.GetWorld()->BroadcastSoundEffect(GetClickOnSound(a_Block), Absolute, 0.5f, 0.6f);

			// Update power
			ChunkData.SetCachedPowerData(a_Position, PowerLevel);

			// Immediately depress plate
			cBlockPressurePlateHandler::SetPressuredState(ChunkInterface, Absolute, true);

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

		a_Chunk.GetWorld()->BroadcastSoundEffect(GetClickOffSound(a_Block), Absolute, 0.5f, 0.5f);
		ChunkData.SetCachedPowerData(a_Position, PowerLevel);

		cBlockPressurePlateHandler::SetPressuredState(ChunkInterface, Absolute, false);
		UpdateAdjustedRelatives(a_Chunk, CurrentlyTicking, a_Position, RelativeAdjacents);
	}

	static void ForValidSourcePositions(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Block, ForEachSourceCallback & Callback)
	{
		UNUSED(a_Chunk);
		UNUSED(a_Position);
		UNUSED(a_Block);
		UNUSED(Callback);
	}
};
