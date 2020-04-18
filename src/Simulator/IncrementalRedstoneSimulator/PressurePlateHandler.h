
#pragma once

#include "RedstoneHandler.h"
#include "../../BoundingBox.h"
#include "../../Entities/Pickup.h"





class cPressurePlateHandler:
	public cRedstoneHandler
{
	using Super = cRedstoneHandler;

public:

	virtual unsigned char GetPowerDeliveredToPosition(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, Vector3i a_QueryPosition, BLOCKTYPE a_QueryBlockType) const override
	{
		UNUSED(a_BlockType);
		UNUSED(a_Meta);
		UNUSED(a_QueryPosition);
		UNUSED(a_QueryBlockType);

		return static_cast<cIncrementalRedstoneSimulator *>(a_World.GetRedstoneSimulator())->GetChunkData()->GetCachedPowerData(a_Position).PowerLevel;
	}

	virtual unsigned char GetPowerLevel(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);

		int NumberOfEntities = 0;
		bool FoundPlayer = false;
		a_World.ForEachEntityInBox(cBoundingBox(Vector3d(0.5, 0, 0.5) + a_Position, 0.5, 0.5), [&](cEntity & a_Entity)
			{
				if (a_Entity.IsPlayer())
				{
					FoundPlayer = true;
				}

				if (a_Entity.IsPickup())
				{
					NumberOfEntities += static_cast<cPickup &>(a_Entity).GetItem().m_ItemCount;
					return false;
				}
				NumberOfEntities++;
				return false;
			}
		);

		switch (a_BlockType)
		{
			case E_BLOCK_STONE_PRESSURE_PLATE:
			{
				return (FoundPlayer ? 15 : 0);
			}
			case E_BLOCK_WOODEN_PRESSURE_PLATE:
			{
				return (NumberOfEntities != 0 ? 15 : 0);
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

	virtual cVector3iArray Update(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, PoweringData a_PoweringData) const override
	{
		UNUSED(a_PoweringData.PowerLevel);
		// LOGD("Evaluating clicky the pressure plate (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);

		auto ChunkData = static_cast<cIncrementalRedstoneSimulator *>(a_World.GetRedstoneSimulator())->GetChunkData();

		const auto PreviousPower = ChunkData->GetCachedPowerData(a_Position);
		auto Power = GetPowerLevel(a_World, a_Position, a_BlockType, a_Meta);  // Get the current power of the platey

		const auto PlateUpdates = GetAdjustedRelatives(a_Position, StaticAppend(GetRelativeLaterals(), cVector3iArray{ OffsetYM() }));
		auto DelayInfo = ChunkData->GetMechanismDelayInfo(a_Position);

		// Resting state?
		if (DelayInfo == nullptr)
		{
			if (Power == 0)
			{
				// Nothing happened, back to rest
				return {};
			}

			// From rest, a player stepped on us
			// Schedule a minimum 0.5 second delay before even thinking about releasing
			ChunkData->m_MechanismDelays[a_Position] = std::make_pair(5, true);

			auto soundToPlay = GetClickOnSound(a_BlockType);
			a_World.BroadcastSoundEffect(soundToPlay, a_Position, 0.5f, 0.6f);

			// Update power
			ChunkData->SetCachedPowerData(a_Position, PoweringData(a_BlockType, Power));

			// Immediately depress plate
			a_World.SetBlockMeta(a_Position, E_META_PRESSURE_PLATE_DEPRESSED);
			return PlateUpdates;
		}

		// Not a resting state

		int DelayTicks;
		bool HasExitedMinimumOnDelayPhase;
		std::tie(DelayTicks, HasExitedMinimumOnDelayPhase) = *DelayInfo;

		// Are we waiting for the initial delay or subsequent release delay?
		if (DelayTicks > 0)
		{
			// Nothing changes, if there is nothing on it anymore, because the state is locked.
			if (Power == 0)
			{
				return {};
			}

			// Yes. Are we waiting to release, and found that the player stepped on it again?
			if (!HasExitedMinimumOnDelayPhase)
			{
				// Reset delay
				*DelayInfo = std::make_pair(0, true);
			}

			// Did the power level change and is still above zero?
			if (Power != PreviousPower.PowerLevel)
			{
				// Yes. Update power
				ChunkData->SetCachedPowerData(a_Position, PoweringData(a_BlockType, Power));
				return PlateUpdates;
			}

			return {};
		}

		// Not waiting for anything. Has the initial delay elapsed?
		if (HasExitedMinimumOnDelayPhase)
		{
			// Yep, initial delay elapsed. Has the player gotten off?
			if (Power == 0)
			{
				// Yes. Go into subsequent release delay, for a further 0.5 seconds
				*DelayInfo = std::make_pair(5, false);
				return {};
			}

			// Did the power level change and is still above zero?
			if (Power != PreviousPower.PowerLevel)
			{
				// Yes. Update power
				ChunkData->SetCachedPowerData(a_Position, PoweringData(a_BlockType, Power));
				return PlateUpdates;
			}

			// Yes, but player's still on the plate, do nothing
			return {};
		}

		// Just got out of the subsequent release phase, reset everything and raise the plate
		ChunkData->m_MechanismDelays.erase(a_Position);

		auto soundToPlay = GetClickOffSound(a_BlockType);
		a_World.BroadcastSoundEffect(soundToPlay, a_Position, 0.5f, 0.5f);
		ChunkData->SetCachedPowerData(a_Position, PoweringData(a_BlockType, Power));

		a_World.SetBlockMeta(a_Position, E_META_PRESSURE_PLATE_RAISED);
		return PlateUpdates;
	}

	virtual cVector3iArray GetValidSourcePositions(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_World);
		UNUSED(a_Position);
		UNUSED(a_BlockType);
		UNUSED(a_Meta);
		return {};
	}

private:
	static AString GetClickOnSound(BLOCKTYPE a_BlockType)
	{
		// manage on-sound
		switch (a_BlockType)
		{
			case E_BLOCK_STONE_PRESSURE_PLATE:
				return "block.stone_pressureplate.click_on";
			case E_BLOCK_WOODEN_PRESSURE_PLATE:
				return "block.wood_pressureplate.click_on";
			case E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE:
			case E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE:
				return "block.metal_pressureplate.click_on";
			default:
			{
				ASSERT(!"No on sound for this one!");
				return "";
			}
		}
	}

	static AString GetClickOffSound(BLOCKTYPE a_BlockType)
	{
		// manage off-sound
		switch (a_BlockType)
		{
			case E_BLOCK_STONE_PRESSURE_PLATE:
				return "block.stone_pressureplate.click_off";
			case E_BLOCK_WOODEN_PRESSURE_PLATE:
				return "block.wood_pressureplate.click_off";
			case E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE:
			case E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE:
				return "block.metal_pressureplate.click_off";
			default:
			{
				ASSERT(!"No off sound for this one!");
				return "";
			}
		}
	}
};
