
#pragma once

#include "RedstoneHandler.h"
#include "../../BoundingBox.h"





class cPressurePlateHandler : public cRedstoneHandler
{
	typedef cRedstoneHandler super;
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

		unsigned int NumberOfEntities = 0;
		bool FoundPlayer = false;
		a_World.ForEachEntityInBox(cBoundingBox(Vector3d(0.5, 0, 0.5) + a_Position, 0.5, 0.5), [&](cEntity & a_Entity)
			{
				if (a_Entity.IsPlayer())
				{
					FoundPlayer = true;
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

		cIncrementalRedstoneSimulatorChunkData *ChunkData = static_cast<cIncrementalRedstoneSimulator *>(a_World.GetRedstoneSimulator())->GetChunkData();

		// get current states
		auto PreviousPower = ChunkData->GetCachedPowerData(a_Position);
		auto PreviousPlateState = ChunkData->GetCachedPressurePlateState(a_Position);

		// initialise variables that should be realised in the world after this method exits
		auto Power = GetPowerLevel(a_World, a_Position, a_BlockType, a_Meta);  // Get the current power of the plate
		auto NextPressurePlateState = E_PRESSURE_PLATE_RAISED;

		if ((Power != 0) && (PreviousPlateState == E_PRESSURE_PLATE_RAISED))  // Plate is pressed initially
		{
			// Set plate to a "can not release"-state
			NextPressurePlateState = E_PRESSURE_PLATE_INITIALLY_PRESSED;

			// schedule locked state to be released after 1 sec
			/* a_World.ScheduleTask(20, [a_Position](cWorld & a_World)
			{
				static_cast<cIncrementalRedstoneSimulator *>(a_World.GetRedstoneSimulator())->GetChunkData()->SetCachedPressurePlateState(a_Position, E_PRESSURE_PLATE_WANTS_TO_RELEASE);
			}); */

			// manage on-sounds
			AString soundToPlay = "";
			switch (a_BlockType)
			{
				case E_BLOCK_STONE_PRESSURE_PLATE:
					soundToPlay = "block.wood_pressureplate.click_on";
					break;
				case E_BLOCK_WOODEN_PRESSURE_PLATE:
					soundToPlay = "block.wood_pressureplate.click_on";
					break;
				case E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE:
				case E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE:
					soundToPlay = "block.metal_pressureplate.click_on";
					break;
				default:
				{
					ASSERT(!"Unhandled/unimplemented block in pressure plate handler!");
					return {};
				}
			}
			a_World.BroadcastSoundEffect(soundToPlay, a_Position, 0.5f, 0.5f);
		}

		if ((Power != 0) && (PreviousPlateState == E_PRESSURE_PLATE_WANTS_TO_RELEASE))  // Plate is still pressed, even though it COULD release now
		{
			// set state that indicates that power the as soon as it is not pressed anymore, it should last for 0.5 more sec in pressed state
			NextPressurePlateState = E_PRESSURE_PLATE_HELD_DOWN;
		}

		if ((Power == 0) && (PreviousPlateState == E_PRESSURE_PLATE_HELD_DOWN))  // Plate is not pressed anymore, but didn't release yet
		{
			NextPressurePlateState = E_PRESSURE_PLATE_HELD_DOWN;
			Power = PreviousPower.PowerLevel;  // Power should stay the same.

			// schedule release after 0.5 sec
			/* a_World.ScheduleTask(10, [a_Position](cWorld & a_World)
			{
				auto ChunkData = static_cast<cIncrementalRedstoneSimulator *>(a_World.GetRedstoneSimulator())->GetChunkData();
				ChunkData->SetCachedPressurePlateState(a_Position, E_PRESSURE_PLATE_WANTS_TO_RELEASE);
			}); */
		}

		if ((Power == 0) && (PreviousPlateState == E_PRESSURE_PLATE_WANTS_TO_RELEASE))
		{
			NextPressurePlateState = E_PRESSURE_PLATE_RAISED;

			// manage off-sounds
			AString soundToPlay = "";
			switch (a_BlockType)
			{
				case E_BLOCK_STONE_PRESSURE_PLATE:
					soundToPlay = "block.wood_pressureplate.click_off";
					break;
				case E_BLOCK_WOODEN_PRESSURE_PLATE:
					soundToPlay = "block.wood_pressureplate.click_off";
					break;
				case E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE:
				case E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE:
					soundToPlay = "block.metal_pressureplate.click_off";
					break;
				default:
				{
					ASSERT(!"Unhandled/unimplemented block in pressure plate handler!");
					return {};
				}
			}
			a_World.BroadcastSoundEffect(soundToPlay, a_Position, 0.5f, 0.5f);
		}


		if (NextPressurePlateState != PreviousPlateState)
		{
			ChunkData->SetCachedPressurePlateState(a_Position, NextPressurePlateState);
		}

		// Plate appears pressed for every state except E_PRESSURE_PLATE_RAISED
		if ((NextPressurePlateState != E_PRESSURE_PLATE_RAISED) && (a_Meta != E_META_PRESSURE_PLATE_DEPRESSED))
		{
			a_World.SetBlockMeta(a_Position, E_META_PRESSURE_PLATE_DEPRESSED);
		}

		if ((NextPressurePlateState == E_PRESSURE_PLATE_RAISED) && (a_Meta != E_META_PRESSURE_PLATE_RAISED))  // Plate is finally released
		{
			a_World.SetBlockMeta(a_Position, E_META_PRESSURE_PLATE_RAISED);
		}

		if ((Power != PreviousPower.PowerLevel))  // Power output changed
		{
			ChunkData->SetCachedPowerData(a_Position, PoweringData(a_BlockType, Power));
			return GetAdjustedRelatives(a_Position, StaticAppend(GetRelativeLaterals(), cVector3iArray{ OffsetYM() }));
		}
		return {};
	}

	virtual cVector3iArray GetValidSourcePositions(cWorld & a_World, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_World);
		UNUSED(a_Position);
		UNUSED(a_BlockType);
		UNUSED(a_Meta);
		return {};
	}
};
