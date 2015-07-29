
#include "Globals.h"

#include "BlockEntities/ChestEntity.h"

#include "Chunk.h"
#include "World.h"
#include "Blocks/GetHandlerCompileTimeTemplate.h"
#include "Blocks/BlockComparator.h"
#include "Blocks/BlockTorch.h"
#include "Blocks/BlockLever.h"
#include "Blocks/BlockButton.h"
#include "Blocks/BlockTripwireHook.h"
#include "Blocks/BlockDoor.h"
#include "Blocks/BlockPiston.h"


#include "IncrementalRedstoneSimulator.h"
#include "BoundingBox.h"
#include "Blocks/ChunkInterface.h"
#include "RedstoneSimulator.h"





void cIncrementalRedstoneSimulator::AddBlock(int a_BlockX, int a_BlockY, int a_BlockZ, cChunk * a_Chunk)
{
	AddBlock(cChunkDef::AbsoluteToRelative({ a_BlockX, a_BlockY, a_BlockZ }, a_Chunk->GetPosX(), a_Chunk->GetPosZ()), a_Chunk, nullptr);
}





void cIncrementalRedstoneSimulator::AddBlock(const Vector3i & a_RelBlockPosition, cChunk * a_OriginalChunk, cChunk * a_NeighborChunk)
{
	if ((a_OriginalChunk == nullptr) || !a_OriginalChunk->IsValid())
	{
		return;
	}
	else if ((a_RelBlockPosition.y < 0) || (a_RelBlockPosition.y >= cChunkDef::Height))
	{
		return;
	}

	// The relative block position is relative to the neighboring chunk should it be passed as an argument

	BLOCKTYPE Block;
	NIBBLETYPE Meta;

	if (a_NeighborChunk != nullptr)
	{
		a_NeighborChunk->UnboundedRelGetBlock(a_RelBlockPosition.x, a_RelBlockPosition.y, a_RelBlockPosition.z, Block, Meta);

		// If a_OtherChunk is passed (not nullptr), it is the neighbouring chunk of a_Chunk, which itself is the chunk with the block change
		// Because said neighbouring chunk does not know of this change but still needs to update its redstone, we set it to dirty
		a_NeighborChunk->SetIsRedstoneDirty(true);
	}
	else
	{
		a_OriginalChunk->GetBlockTypeMeta(a_RelBlockPosition.x, a_RelBlockPosition.y, a_RelBlockPosition.z, Block, Meta);
	}

	// Every time a block is changed (AddBlock called), we want to go through all lists and check to see if the coordiantes stored within are still valid
	// Checking only when a block is changed, as opposed to every tick, also improves performance

	if (
		!IsPotentialSource(Block) ||
		(
			// Changeable sources
			((Block == E_BLOCK_REDSTONE_WIRE) && (Meta == 0)) ||
			((Block == E_BLOCK_LEVER) && !IsLeverOn(Meta)) ||
			((Block == E_BLOCK_DETECTOR_RAIL) && ((Meta & 0x08) == 0)) ||
			(((Block == E_BLOCK_STONE_BUTTON) || (Block == E_BLOCK_WOODEN_BUTTON)) && (!IsButtonOn(Meta))) ||
			((Block == E_BLOCK_TRIPWIRE_HOOK) && ((Meta & 0x08) == 0))
		)
	)
	{
		SetSourceUnpowered(a_RelBlockPosition.x, a_RelBlockPosition.y, a_RelBlockPosition.z, a_NeighborChunk != nullptr ? a_NeighborChunk : a_OriginalChunk);
	}

	if (!IsViableMiddleBlock(Block))
	{
		SetInvalidMiddleBlock(a_RelBlockPosition.x, a_RelBlockPosition.y, a_RelBlockPosition.z, a_NeighborChunk != nullptr ? a_NeighborChunk : a_OriginalChunk);
	}

	if (a_NeighborChunk != nullptr)
	{
		// DO NOT touch our chunk's data structure if we are being called with coordinates from another chunk - this one caused me massive grief :P
		return;
	}
	auto RedstoneSimulatorData = static_cast<cIncrementalRedstoneSimulatorChunkData *>(a_OriginalChunk->GetRedstoneSimulatorData());

	auto & SimulatedPlayerToggleableBlocks = RedstoneSimulatorData->m_SimulatedPlayerToggleableBlocks;
	if (DoesIgnorePlayerToggle(Block))
	{
		// Initialise the toggleable blocks list so that trapdoors etc. aren't reset on restart (#1887)
		SimulatedPlayerToggleableBlocks.emplace(
			a_RelBlockPosition,
			AreCoordsDirectlyPowered(a_RelBlockPosition.x, a_RelBlockPosition.y, a_RelBlockPosition.z, a_OriginalChunk) || AreCoordsLinkedPowered(a_RelBlockPosition.x, a_RelBlockPosition.y, a_RelBlockPosition.z, a_OriginalChunk)
		);  // This map won't insert if key already present, so no need to check
	}
	else
	{
		SimulatedPlayerToggleableBlocks.erase(a_RelBlockPosition);
	}

	if ((Block != E_BLOCK_REDSTONE_REPEATER_ON) && (Block != E_BLOCK_REDSTONE_REPEATER_OFF))
	{
		RedstoneSimulatorData->m_RepeatersDelayList.erase(a_RelBlockPosition);
	}

	auto & RedstoneSimulatorChunkData = RedstoneSimulatorData->m_ChunkData;
	auto Iterator = RedstoneSimulatorChunkData.find(a_RelBlockPosition);
	if (!IsAllowedBlock(Block))
	{
		if (Iterator != RedstoneSimulatorChunkData.end())
		{
			Iterator->second.second = true;  // The new blocktype is not redstone; it must be queued to be removed from this list
		}
		return;
	}
	else
	{
		if (Iterator != RedstoneSimulatorChunkData.end())
		{
			Iterator->second.second = false;  // De-schedule removal from list
			Iterator->second.first = Block;  // Update block information
			return;
		}
	}

	RedstoneSimulatorChunkData.emplace(a_RelBlockPosition, std::make_pair(Block, false));
}





void cIncrementalRedstoneSimulator::SimulateChunk(std::chrono::milliseconds a_Dt, int a_ChunkX, int a_ChunkZ, cChunk * a_Chunk)
{
	m_RedstoneSimulatorChunkData = static_cast<cIncrementalRedstoneSimulatorChunkData *>(a_Chunk->GetRedstoneSimulatorData());
	if (m_RedstoneSimulatorChunkData == nullptr)
	{
		m_RedstoneSimulatorChunkData = new cIncrementalRedstoneSimulator::cIncrementalRedstoneSimulatorChunkData();
		a_Chunk->SetRedstoneSimulatorData(m_RedstoneSimulatorChunkData);
	}
	if (m_RedstoneSimulatorChunkData->m_ChunkData.empty())
	{
		return;
	}

	m_PoweredBlocks = &m_RedstoneSimulatorChunkData->m_PoweredBlocks;
	m_RepeatersDelayList = &m_RedstoneSimulatorChunkData->m_RepeatersDelayList;
	m_SimulatedPlayerToggleableBlocks = &m_RedstoneSimulatorChunkData->m_SimulatedPlayerToggleableBlocks;
	m_LinkedPoweredBlocks = &m_RedstoneSimulatorChunkData->m_LinkedBlocks;
	m_Chunk = a_Chunk;
	bool ShouldUpdateSimulateOnceBlocks = false;

	if (a_Chunk->IsRedstoneDirty())
	{
		// Simulate the majority of devices only if something (blockwise or power-wise) has changed
		// Make sure to allow the chunk to resimulate after the initial run if there was a power change (ShouldUpdateSimulateOnceBlocks helps to do this)
		a_Chunk->SetIsRedstoneDirty(false);
		ShouldUpdateSimulateOnceBlocks = true;
	}

	HandleRedstoneRepeaterDelays();

	for (auto dataitr = m_RedstoneSimulatorChunkData->m_ChunkData.begin(); dataitr != m_RedstoneSimulatorChunkData->m_ChunkData.end();)
	{
		if (dataitr->second.second)
		{
			// Removal was scheduled - do so
			dataitr = m_RedstoneSimulatorChunkData->m_ChunkData.erase(dataitr);
			continue;
		}

		switch (dataitr->second.first)  // Call the appropriate simulator for the entry's block type
		{
			case E_BLOCK_DAYLIGHT_SENSOR: HandleDaylightSensor(dataitr->first.x, dataitr->first.y, dataitr->first.z); break;
			case E_BLOCK_TRIPWIRE:        HandleTripwire(dataitr->first.x, dataitr->first.y, dataitr->first.z);       break;
			case E_BLOCK_TRIPWIRE_HOOK:   HandleTripwireHook(dataitr->first.x, dataitr->first.y, dataitr->first.z);   break;

			case E_BLOCK_WOODEN_PRESSURE_PLATE:
			case E_BLOCK_STONE_PRESSURE_PLATE:
			case E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE:
			case E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE:
			{
				HandlePressurePlate(dataitr->first.x, dataitr->first.y, dataitr->first.z, dataitr->second.first);
				break;
			}
			default: break;
		}

		if (ShouldUpdateSimulateOnceBlocks)
		{
			switch (dataitr->second.first)
			{
				case E_BLOCK_REDSTONE_WIRE:         HandleRedstoneWire(dataitr->first.x, dataitr->first.y, dataitr->first.z);	  break;
				case E_BLOCK_COMMAND_BLOCK:         HandleCommandBlock(dataitr->first.x, dataitr->first.y, dataitr->first.z);   break;
				case E_BLOCK_NOTE_BLOCK:            HandleNoteBlock(dataitr->first.x, dataitr->first.y, dataitr->first.z);      break;
				case E_BLOCK_BLOCK_OF_REDSTONE:     HandleRedstoneBlock(dataitr->first.x, dataitr->first.y, dataitr->first.z);  break;
				case E_BLOCK_LEVER:                 HandleRedstoneLever(dataitr->first.x, dataitr->first.y, dataitr->first.z);  break;
				case E_BLOCK_TNT:                   HandleTNT(dataitr->first.x, dataitr->first.y, dataitr->first.z);            break;
				case E_BLOCK_IRON_TRAPDOOR:         HandleTrapdoor(dataitr->first.x, dataitr->first.y, dataitr->first.z);       break;
				case E_BLOCK_TRAPDOOR:              HandleTrapdoor(dataitr->first.x, dataitr->first.y, dataitr->first.z);       break;
				case E_BLOCK_TRAPPED_CHEST:         HandleTrappedChest(dataitr->first.x, dataitr->first.y, dataitr->first.z);   break;

				case E_BLOCK_ACTIVATOR_RAIL:
				case E_BLOCK_DETECTOR_RAIL:
				case E_BLOCK_POWERED_RAIL:
				{
					HandleRail(dataitr->first.x, dataitr->first.y, dataitr->first.z, dataitr->second.first);
					break;
				}
				case E_BLOCK_ACACIA_DOOR:
				case E_BLOCK_BIRCH_DOOR:
				case E_BLOCK_DARK_OAK_DOOR:
				case E_BLOCK_JUNGLE_DOOR:
				case E_BLOCK_SPRUCE_DOOR:
				case E_BLOCK_OAK_DOOR:
				case E_BLOCK_IRON_DOOR:
				{
					HandleDoor(dataitr->first.x, dataitr->first.y, dataitr->first.z);
					break;
				}
				case E_BLOCK_ACACIA_FENCE_GATE:
				case E_BLOCK_BIRCH_FENCE_GATE:
				case E_BLOCK_DARK_OAK_FENCE_GATE:
				case E_BLOCK_OAK_FENCE_GATE:
				case E_BLOCK_JUNGLE_FENCE_GATE:
				case E_BLOCK_SPRUCE_FENCE_GATE:
				{
					HandleFenceGate(dataitr->first.x, dataitr->first.y, dataitr->first.z);
					break;
				}
				case E_BLOCK_REDSTONE_LAMP_OFF:
				case E_BLOCK_REDSTONE_LAMP_ON:
				{
					HandleRedstoneLamp(dataitr->first.x, dataitr->first.y, dataitr->first.z, dataitr->second.first);
					break;
				}
				case E_BLOCK_DISPENSER:
				case E_BLOCK_DROPPER:
				{
					HandleDropSpenser(dataitr->first.x, dataitr->first.y, dataitr->first.z);
					break;
				}
				case E_BLOCK_PISTON:
				case E_BLOCK_STICKY_PISTON:
				{
					HandlePiston(dataitr->first.x, dataitr->first.y, dataitr->first.z);
					break;
				}
				case E_BLOCK_REDSTONE_REPEATER_OFF:
				case E_BLOCK_REDSTONE_REPEATER_ON:
				{
					HandleRedstoneRepeater(dataitr->first.x, dataitr->first.y, dataitr->first.z, dataitr->second.first);
					break;
				}
				case E_BLOCK_REDSTONE_TORCH_OFF:
				case E_BLOCK_REDSTONE_TORCH_ON:
				{
					HandleRedstoneTorch(dataitr->first.x, dataitr->first.y, dataitr->first.z, dataitr->second.first);
					break;
				}
				case E_BLOCK_STONE_BUTTON:
				case E_BLOCK_WOODEN_BUTTON:
				{
					HandleRedstoneButton(dataitr->first.x, dataitr->first.y, dataitr->first.z);
					break;
				}
				case E_BLOCK_ACTIVE_COMPARATOR:
				case E_BLOCK_INACTIVE_COMPARATOR:
				{
					HandleRedstoneComparator(dataitr->first.x, dataitr->first.y, dataitr->first.z);
					break;
				}
				default: break;
			}
		}
		++dataitr;
	}
}





void cIncrementalRedstoneSimulator::WakeUp(int a_BlockX, int a_BlockY, int a_BlockZ, cChunk * a_Chunk)
{
	auto CurrentChunkRelative = cChunkDef::AbsoluteToRelative({a_BlockX, a_BlockY, a_BlockZ}, a_Chunk->GetPosX(), a_Chunk->GetPosZ());
	AddBlock(CurrentChunkRelative, a_Chunk);  // Alert the current chunk which the block is present in

	for (const auto & BoundaryChunk : GetAdjacentChunks(CurrentChunkRelative, a_Chunk))
	{
		// On a chunk boundary, alert all neighbouring chunks which may have a connection with this block
		AddBlock(cChunkDef::AbsoluteToRelative({a_BlockX, a_BlockY, a_BlockZ}, BoundaryChunk->GetPosX(), BoundaryChunk->GetPosZ()), a_Chunk, BoundaryChunk);
	}
}





void cIncrementalRedstoneSimulator::HandleRedstoneTorch(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ, BLOCKTYPE a_MyState)
{
	static const struct  // Define which directions the torch can power
	{
		int x, y, z;
	} gCrossCoords[] =
	{
		{ 1, 0, 0 },
		{ -1, 0, 0 },
		{ 0, 0, 1 },
		{ 0, 0, -1 },
		{ 0, 1, 0 },
	};

	if (a_MyState == E_BLOCK_REDSTONE_TORCH_ON)
	{
		// Check if the block the torch is on is powered
		int X = a_RelBlockX; int Y = a_RelBlockY; int Z = a_RelBlockZ;
		AddFaceDirection(X, Y, Z, GetHandlerCompileTime<E_BLOCK_TORCH>::type::MetaDataToDirection(m_Chunk->GetMeta(a_RelBlockX, a_RelBlockY, a_RelBlockZ)), true);  // Inverse true to get the block torch is on

		cChunk * Neighbour = m_Chunk->GetRelNeighborChunkAdjustCoords(X, Z);
		if ((Neighbour == nullptr) || !Neighbour->IsValid())
		{
			return;
		}

		if (AreCoordsDirectlyPowered(X, Y, Z, Neighbour))
		{
			// There was a match, torch goes off
			m_Chunk->SetBlock(a_RelBlockX, a_RelBlockY, a_RelBlockZ, E_BLOCK_REDSTONE_TORCH_OFF, m_Chunk->GetMeta(a_RelBlockX, a_RelBlockY, a_RelBlockZ));
			return;
		}

		// Torch still on, make all 4(X, Z) + 1(Y) sides powered
		for (size_t i = 0; i < ARRAYCOUNT(gCrossCoords); i++)
		{
			BLOCKTYPE Type = 0;
			if (!m_Chunk->UnboundedRelGetBlockType(a_RelBlockX + gCrossCoords[i].x, a_RelBlockY + gCrossCoords[i].y, a_RelBlockZ + gCrossCoords[i].z, Type))
			{
				continue;
			}
			if (i + 1 < ARRAYCOUNT(gCrossCoords))  // Sides of torch, not top (top is last)
			{
				if (
					IsMechanism(Type) &&  // Is it a mechanism? Not block / other torch etc.
					(!Vector3i(a_RelBlockX + gCrossCoords[i].x, a_RelBlockY + gCrossCoords[i].y, a_RelBlockZ + gCrossCoords[i].z).Equals(Vector3i(X, Y, Z)))  // CAN'T power block is that it is on
					)
				{
					SetBlockPowered(a_RelBlockX + gCrossCoords[i].x, a_RelBlockY + gCrossCoords[i].y, a_RelBlockZ + gCrossCoords[i].z, a_RelBlockX, a_RelBlockY, a_RelBlockZ);
				}
			}
			else
			{
				// Top side, power whatever is there, including blocks
				SetBlockPowered(a_RelBlockX + gCrossCoords[i].x, a_RelBlockY + gCrossCoords[i].y, a_RelBlockZ + gCrossCoords[i].z, a_RelBlockX, a_RelBlockY, a_RelBlockZ);
				// Power all blocks surrounding block above torch
				SetDirectionLinkedPowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ, BLOCK_FACE_YP);
			}
		}

		if (m_Chunk->GetMeta(a_RelBlockX, a_RelBlockY, a_RelBlockZ) != 0x5)  // Is torch standing on ground? If NOT (i.e. on wall), power block beneath
		{
			BLOCKTYPE Type = m_Chunk->GetBlock(a_RelBlockX, a_RelBlockY - 1, a_RelBlockZ);

			if (IsMechanism(Type))  // Still can't make a normal block powered though!
			{
				SetBlockPowered(a_RelBlockX, a_RelBlockY - 1, a_RelBlockZ, a_RelBlockX, a_RelBlockY, a_RelBlockZ);
			}
		}
	}
	else
	{
		// Check if the block the torch is on is powered
		int X = a_RelBlockX; int Y = a_RelBlockY; int Z = a_RelBlockZ;
		AddFaceDirection(X, Y, Z, GetHandlerCompileTime<E_BLOCK_TORCH>::type::MetaDataToDirection(m_Chunk->GetMeta(a_RelBlockX, a_RelBlockY, a_RelBlockZ)), true);  // Inverse true to get the block torch is on

		cChunk * Neighbour = m_Chunk->GetRelNeighborChunkAdjustCoords(X, Z);
		if ((Neighbour == nullptr) || !Neighbour->IsValid())
		{
			return;
		}

		// See if off state torch can be turned on again
		if (AreCoordsDirectlyPowered(X, Y, Z, Neighbour))
		{
			return;  // Something matches, torch still powered
		}

		// Block torch on not powered, can be turned on again!
		m_Chunk->SetBlock(a_RelBlockX, a_RelBlockY, a_RelBlockZ, E_BLOCK_REDSTONE_TORCH_ON, m_Chunk->GetMeta(a_RelBlockX, a_RelBlockY, a_RelBlockZ));
	}
}





void cIncrementalRedstoneSimulator::HandleRedstoneBlock(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ)
{
	SetAllDirsAsPowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ);
	SetBlockPowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ, a_RelBlockX, a_RelBlockY, a_RelBlockZ);  // Set self as powered
}





void cIncrementalRedstoneSimulator::HandleRedstoneLever(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ)
{
	NIBBLETYPE Meta = m_Chunk->GetMeta(a_RelBlockX, a_RelBlockY, a_RelBlockZ);
	if (IsLeverOn(Meta))
	{
		SetAllDirsAsPowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ);

		eBlockFace Dir = GetHandlerCompileTime<E_BLOCK_LEVER>::type::BlockMetaDataToBlockFace(Meta);

		Dir = ReverseBlockFace(Dir);

		SetDirectionLinkedPowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ, Dir);
	}
}





void cIncrementalRedstoneSimulator::HandleFenceGate(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ)
{
	int BlockX = (m_Chunk->GetPosX() * cChunkDef::Width) + a_RelBlockX;
	int BlockZ = (m_Chunk->GetPosZ() * cChunkDef::Width) + a_RelBlockZ;
	NIBBLETYPE MetaData = m_Chunk->GetMeta(a_RelBlockX, a_RelBlockY, a_RelBlockZ);

	if (AreCoordsPowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ))
	{
		if (!AreCoordsSimulated(a_RelBlockX, a_RelBlockY, a_RelBlockZ, true))
		{
			if ((MetaData & 0x4) == 0)
			{
				m_Chunk->SetMeta(a_RelBlockX, a_RelBlockY, a_RelBlockZ, MetaData | 0x4);
				m_Chunk->BroadcastSoundParticleEffect(1003, BlockX, a_RelBlockY, BlockZ, 0);
			}
			SetPlayerToggleableBlockAsSimulated(a_RelBlockX, a_RelBlockY, a_RelBlockZ, true);
		}
	}
	else
	{
		if (!AreCoordsSimulated(a_RelBlockX, a_RelBlockY, a_RelBlockZ, false))
		{
			if ((MetaData & 0x4) != 0)
			{
				m_Chunk->SetMeta(a_RelBlockX, a_RelBlockY, a_RelBlockZ, MetaData & ~0x04);
				m_Chunk->BroadcastSoundParticleEffect(1003, BlockX, a_RelBlockY, BlockZ, 0);
			}
			SetPlayerToggleableBlockAsSimulated(a_RelBlockX, a_RelBlockY, a_RelBlockZ, false);
		}
	}
}





void cIncrementalRedstoneSimulator::HandleRedstoneButton(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ)
{
	NIBBLETYPE Meta = m_Chunk->GetMeta(a_RelBlockX, a_RelBlockY, a_RelBlockZ);
	if (IsButtonOn(Meta))
	{
		SetAllDirsAsPowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ);

		eBlockFace Dir = GetHandlerCompileTime<E_BLOCK_STONE_BUTTON>::type::BlockMetaDataToBlockFace(Meta);
		Dir = ReverseBlockFace(Dir);
		SetDirectionLinkedPowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ, Dir);
	}
}





void cIncrementalRedstoneSimulator::HandleRedstoneWire(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ)
{
	static const eBlockFace BlockFaceOffsets[] =
	{
		BLOCK_FACE_XM,
		BLOCK_FACE_XP,
		BLOCK_FACE_ZM,
		BLOCK_FACE_ZP
	};

	static const Vector3i VectorOffsets[] =
	{
		{ -1, 0, 0 },
		{ 1, 0, 0 },
		{ 0, 0, -1 },
		{ 0, 0, 1 }
	};

	auto RelBlock = Vector3i(a_RelBlockX, a_RelBlockY, a_RelBlockZ);

	// Check to see if directly beside a power source
	unsigned char MyPower = IsWirePowered(RelBlock, m_Chunk);
	m_Chunk->SetMeta(a_RelBlockX, a_RelBlockY, a_RelBlockZ, MyPower);
	if (MyPower == 0)
	{
		SetSourceUnpowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ, m_Chunk);
		return;
	}

	if (MyPower == MAX_POWER_LEVEL)
	{
		std::vector<std::pair<Vector3i, cChunk *>> PotentialWireList = { std::make_pair(RelBlock, m_Chunk) };
		while (!PotentialWireList.empty())
		{
			auto Current = PotentialWireList.back();
			PotentialWireList.pop_back();
			FindAndPowerBorderingWires(PotentialWireList, Current.first, Current.second);
		}
	}
	else if (MyPower == 1)
	{
		return;
	}

	// Wire still powered, power blocks beneath and in direction of facing
	MyPower--;

	SetBlockPowered(a_RelBlockX, a_RelBlockY - 1, a_RelBlockZ, a_RelBlockX, a_RelBlockY, a_RelBlockZ, MyPower);
	SetDirectionLinkedPowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ, BLOCK_FACE_YM, MyPower);

	int BorderingMechanismCount = 0;
	bool RepeaterPresent = false;
	Vector3i OffsetToPower;

	for (const auto & Offset : VectorOffsets)
	{
		BLOCKTYPE Block;
		Vector3i AdjustedOffset = RelBlock + Offset;
		if (m_Chunk->UnboundedRelGetBlockType(AdjustedOffset.x, AdjustedOffset.y, AdjustedOffset.z, Block))
		{
			switch (Block)
			{
				case E_BLOCK_REDSTONE_REPEATER_OFF:
				{
					BorderingMechanismCount++;
					if (!RepeaterPresent)
					{
						// TODO: only if wire is actually connected to repeater (repeater facing right way)
						RepeaterPresent = true;
						OffsetToPower = { -Offset.x, Offset.y, -Offset.z };  // Negate to obtain offset in opposite direction since wire powers that way
					}
					SetBlockPowered(AdjustedOffset, RelBlock, MyPower);
				}
				case E_BLOCK_REDSTONE_TORCH_ON:
				case E_BLOCK_REDSTONE_WIRE:
				{
					BorderingMechanismCount++;
					if (!RepeaterPresent)
					{
						OffsetToPower = { -Offset.x, Offset.y, -Offset.z };
					}
				}
				default: break;
			}
		}
	}

	if (BorderingMechanismCount == 0)
	{
		SetAllDirsAsPowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ);
		for (const auto & BlockFaceOffset : BlockFaceOffsets)
		{
			SetDirectionLinkedPowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ, BlockFaceOffset, MyPower);
		}
	}
	else if (BorderingMechanismCount == 1)
	{
		eBlockFace Face = BlockFaceOffsets[std::distance(VectorOffsets, std::find(VectorOffsets, VectorOffsets + ARRAYCOUNT(VectorOffsets), OffsetToPower))];
		SetBlockPowered(RelBlock + OffsetToPower, RelBlock, MyPower);
		SetDirectionLinkedPowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ, Face, MyPower);
	}
}





void cIncrementalRedstoneSimulator::FindAndPowerBorderingWires(std::vector<std::pair<Vector3i, cChunk *>> & a_PotentialWireList, const Vector3i & a_EntryRelBlockPosition, cChunk * a_EntryChunk)
{
	static const Vector3i LevelOffsets[] =  // Wires on same level
	{
		{ 1, 0, 0 },
		{ -1, 0, 0 },
		{ 0, 0, 1 },
		{ 0, 0, -1 },
	};
	static const Vector3i HigherOffsets[] =  // Wires one higher, surrounding self
	{
		{ 1, 1, 0 },
		{ -1, 1, 0 },
		{ 0, 1, 1 },
		{ 0, 1, -1 },
	};
	static const Vector3i LowerOffsets[] =  // Wires one lower, surrounding self
	{
		{ 1, -1, 0 },
		{ -1, -1, 0 },
		{ 0, -1, 1 },
		{ 0, -1, -1 },
	};
	
	for (auto Offset : LevelOffsets)
	{
		auto AdjustedPos = a_EntryRelBlockPosition + Offset;
		auto Neighbour = a_EntryChunk->GetRelNeighborChunkAdjustCoords(AdjustedPos.x, AdjustedPos.z);
		auto MyPower = IsWirePowered(a_EntryRelBlockPosition, a_EntryChunk);

		if ((Neighbour == nullptr) || !Neighbour->IsValid())
		{
			return;
		}

		if ((Neighbour->GetBlock(AdjustedPos) == E_BLOCK_REDSTONE_WIRE) && (MyPower > 1) && (MyPower > IsWirePowered(AdjustedPos, Neighbour)))
		{
			PowerBorderingWires(a_PotentialWireList, a_EntryRelBlockPosition, a_EntryChunk, AdjustedPos, Neighbour, MyPower);
		}
	}

	for (auto Offset : HigherOffsets)
	{
		auto AdjustedPos = a_EntryRelBlockPosition + Offset;
		auto Neighbour = a_EntryChunk->GetRelNeighborChunkAdjustCoords(AdjustedPos.x, AdjustedPos.z);
		auto MyPower = IsWirePowered(a_EntryRelBlockPosition, a_EntryChunk);

		if ((Neighbour == nullptr) || !Neighbour->IsValid())
		{
			return;
		}

		if (
			(Neighbour->GetBlock(AdjustedPos) == E_BLOCK_REDSTONE_WIRE) &&
			(!cBlockInfo::FullyOccupiesVoxel(a_EntryChunk->GetBlock(a_EntryRelBlockPosition.x, a_EntryRelBlockPosition.y + 1, a_EntryRelBlockPosition.z)) ||
			(a_EntryChunk->GetBlock(a_EntryRelBlockPosition.x, a_EntryRelBlockPosition.y + 1, a_EntryRelBlockPosition.z) == E_BLOCK_GLOWSTONE)) &&
			(MyPower > 1) && (MyPower > IsWirePowered(AdjustedPos, Neighbour)))
		{
			PowerBorderingWires(a_PotentialWireList, a_EntryRelBlockPosition, a_EntryChunk, AdjustedPos, Neighbour, MyPower);
		}
	}

	for (auto Offset : LowerOffsets)
	{
		auto AdjustedPos = a_EntryRelBlockPosition + Offset;
		auto Neighbour = a_EntryChunk->GetRelNeighborChunkAdjustCoords(AdjustedPos.x, AdjustedPos.z);
		auto MyPower = IsWirePowered(a_EntryRelBlockPosition, a_EntryChunk);

		if ((Neighbour == nullptr) || !Neighbour->IsValid())
		{
			return;
		}

		if (
			(Neighbour->GetBlock(AdjustedPos) == E_BLOCK_REDSTONE_WIRE) &&
			(!cBlockInfo::FullyOccupiesVoxel(Neighbour->GetBlock(AdjustedPos.x, AdjustedPos.y + 1, AdjustedPos.z)) ||
			(Neighbour->GetBlock(AdjustedPos.x, AdjustedPos.y + 1, AdjustedPos.z) == E_BLOCK_GLOWSTONE)) &&
			(MyPower > 1) && (MyPower > IsWirePowered(AdjustedPos, Neighbour)))
		{
			PowerBorderingWires(a_PotentialWireList, a_EntryRelBlockPosition, a_EntryChunk, AdjustedPos, Neighbour, MyPower);
		}
	}
}





void cIncrementalRedstoneSimulator::PowerBorderingWires(std::vector<std::pair<Vector3i, cChunk *>> & a_PotentialWireList, const Vector3i & a_EntryRelSourcePosition, cChunk * a_EntryChunk, const Vector3i & a_AdjustedPos, cChunk * a_NeighbourChunk, unsigned char a_MyPower)
{
	auto SourcePos = a_EntryRelSourcePosition + Vector3i((a_EntryChunk->GetPosX() - a_NeighbourChunk->GetPosX()) * cChunkDef::Width, 0, (a_EntryChunk->GetPosZ() - a_NeighbourChunk->GetPosZ()) * cChunkDef::Width);
	auto & PoweredBlocks = static_cast<cIncrementalRedstoneSimulatorChunkData *>(a_NeighbourChunk->GetRedstoneSimulatorData())->m_PoweredBlocks;  // We need to insert the value into the chunk who owns the block position
	auto Position = std::find_if(PoweredBlocks.begin(), PoweredBlocks.end(), [a_AdjustedPos, SourcePos, a_MyPower](const sPoweredBlocks & itr) { return ((itr.m_BlockPos == a_AdjustedPos) && (itr.m_SourcePos == SourcePos)); });
	if (Position != PoweredBlocks.end())
	{
		Position->m_PowerLevel = a_MyPower - 1;
	}
	else
	{
		PoweredBlocks.emplace_back(a_AdjustedPos, SourcePos, a_MyPower - 1);

		a_NeighbourChunk->SetIsRedstoneDirty(true);
		m_Chunk->SetIsRedstoneDirty(true);
	}

	a_NeighbourChunk->SetMeta(a_AdjustedPos.x, a_AdjustedPos.y, a_AdjustedPos.z, a_MyPower - 1);
	a_PotentialWireList.emplace_back(std::make_pair(a_AdjustedPos, a_NeighbourChunk));
}





void cIncrementalRedstoneSimulator::HandleRedstoneComparator(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ)
{
	auto Meta = m_Chunk->GetMeta(a_RelBlockX, a_RelBlockY, a_RelBlockZ);
	auto HighestSidePower = std::max(IsWirePowered(AdjustRelativeCoords(cBlockComparatorHandler::GetSideCoordinate(a_RelBlockX, a_RelBlockY, a_RelBlockZ, Meta & 0x3, false)), m_Chunk), IsWirePowered(AdjustRelativeCoords(cBlockComparatorHandler::GetSideCoordinate(a_RelBlockX, a_RelBlockY, a_RelBlockZ, Meta & 0x3, true)), m_Chunk));
	auto FrontCoordinate = AdjustRelativeCoords(cBlockComparatorHandler::GetFrontCoordinate(a_RelBlockX, a_RelBlockY, a_RelBlockZ, Meta & 0x3));
	unsigned char Power = 0;

	class cContainerCallback : public cBlockEntityCallback
	{
	public:
		cContainerCallback() : m_SignalStrength(0)
		{
		}

		virtual bool Item(cBlockEntity * a_BlockEntity) override
		{
			auto & Contents = static_cast<cBlockEntityWithItems *>(a_BlockEntity)->GetContents();
			float Fullness = 0;  // Is a floating-point type to allow later calculation to produce a non-truncated value
			for (int Slot = 0; Slot != Contents.GetNumSlots(); ++Slot)
			{
				Fullness += Contents.GetSlot(Slot).m_ItemCount / Contents.GetSlot(Slot).GetMaxStackSize();
			}

			m_SignalStrength = static_cast<unsigned char>(1 + (Fullness / Contents.GetNumSlots()) * 14);
			return false;
		}

		unsigned char m_SignalStrength;
	} CCB;

	auto AbsoluteComparatorCoords = cChunkDef::RelativeToAbsolute(Vector3i(a_RelBlockX, a_RelBlockY, a_RelBlockZ), m_Chunk->GetPosX(), m_Chunk->GetPosZ());
	auto AbsoluteRearCoords = cBlockComparatorHandler::GetRearCoordinate(AbsoluteComparatorCoords.x, AbsoluteComparatorCoords.y, AbsoluteComparatorCoords.z, Meta & 0x3);

	m_Chunk->DoWithBlockEntityAt(AbsoluteRearCoords.x, AbsoluteRearCoords.y, AbsoluteRearCoords.z, CCB);
	auto RearPower = std::max(CCB.m_SignalStrength, IsWirePowered(AdjustRelativeCoords(cBlockComparatorHandler::GetRearCoordinate(a_RelBlockX, a_RelBlockY, a_RelBlockZ, Meta & 0x3)), m_Chunk));

	if ((Meta & 0x4) == 0x4)
	{
		// Subtraction mode
		Power = std::max(static_cast<unsigned char>(RearPower - HighestSidePower), std::numeric_limits<unsigned char>::min());
	}
	else
	{
		// Comparison mode
		Power = (std::max(HighestSidePower, RearPower) == HighestSidePower) ? 0 : RearPower;
	}

	if (Power > 0)
	{
		m_Chunk->SetMeta(a_RelBlockX, a_RelBlockY, a_RelBlockZ, m_Chunk->GetMeta(a_RelBlockX, a_RelBlockY, a_RelBlockZ) | 0x8);
		SetBlockPowered(FrontCoordinate, Vector3i(a_RelBlockX, a_RelBlockY, a_RelBlockZ), Power);
	}
	else
	{
		m_Chunk->SetMeta(a_RelBlockX, a_RelBlockY, a_RelBlockZ, m_Chunk->GetMeta(a_RelBlockX, a_RelBlockY, a_RelBlockZ) & 0x7);
		SetSourceUnpowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ, m_Chunk);
	}
}





void cIncrementalRedstoneSimulator::HandleRedstoneRepeater(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ, BLOCKTYPE a_MyState)
{
	/* Repeater Orientation Mini Guide:
	===================================

	|
	| Z Axis
	V

	X Axis ---->

	Repeater directions, values from a WorldType::GetBlockMeta(a_RelBlockX, a_RelBlockY, a_RelBlockZ) lookup:

	East (Right) (X+): 0x1
	West (Left) (X-): 0x3
	North (Up) (Z-): 0x2
	South (Down) (Z+): 0x0
	// TODO: Add E_META_XXX enum entries for all meta values and update project with them

	Sun rises from East (X+)

	*/

	// Create a variable holding my meta to avoid multiple lookups.
	NIBBLETYPE Meta = m_Chunk->GetMeta(a_RelBlockX, a_RelBlockY, a_RelBlockZ);
	bool IsOn = (a_MyState == E_BLOCK_REDSTONE_REPEATER_ON);

	if (!IsRepeaterLocked(a_RelBlockX, a_RelBlockY, a_RelBlockZ, Meta))  // If we're locked, change nothing. Otherwise:
	{
		bool IsSelfPowered = IsRepeaterPowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ, Meta);
		if (IsSelfPowered && !IsOn)  // Queue a power change if powered, but not on and not locked.
		{
			QueueRepeaterPowerChange(a_RelBlockX, a_RelBlockY, a_RelBlockZ, Meta, true);
		}
		else if (!IsSelfPowered && IsOn)  // Queue a power change if unpowered, on, and not locked.
		{
			QueueRepeaterPowerChange(a_RelBlockX, a_RelBlockY, a_RelBlockZ, Meta, false);
		}
	}

	if (IsOn)
	{
		switch (Meta & 0x3)  // We only want the direction (bottom) bits
		{
			case 0x0:
			{
				SetBlockPowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ - 1, a_RelBlockX, a_RelBlockY, a_RelBlockZ);
				SetDirectionLinkedPowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ, BLOCK_FACE_ZM);
				break;
			}
			case 0x1:
			{
				SetBlockPowered(a_RelBlockX + 1, a_RelBlockY, a_RelBlockZ, a_RelBlockX, a_RelBlockY, a_RelBlockZ);
				SetDirectionLinkedPowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ, BLOCK_FACE_XP);
				break;
			}
			case 0x2:
			{
				SetBlockPowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ + 1, a_RelBlockX, a_RelBlockY, a_RelBlockZ);
				SetDirectionLinkedPowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ, BLOCK_FACE_ZP);
				break;
			}
			case 0x3:
			{
				SetBlockPowered(a_RelBlockX - 1, a_RelBlockY, a_RelBlockZ, a_RelBlockX, a_RelBlockY, a_RelBlockZ);
				SetDirectionLinkedPowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ, BLOCK_FACE_XM);
				break;
			}
		}
	}
}


void cIncrementalRedstoneSimulator::HandleRedstoneRepeaterDelays()
{
	for (auto itr = m_RepeatersDelayList->begin(); itr != m_RepeatersDelayList->end();)
	{
		if (itr->second.a_ElapsedTicks >= itr->second.a_DelayTicks)  // Has the elapsed ticks reached the target ticks?
		{
			BLOCKTYPE Block;
			NIBBLETYPE Meta;
			m_Chunk->GetBlockTypeMeta(itr->first.x, itr->first.y, itr->first.z, Block, Meta);
			if (itr->second.ShouldPowerOn)
			{
				if (Block != E_BLOCK_REDSTONE_REPEATER_ON)  // For performance
				{
					m_Chunk->SetBlock(itr->first, E_BLOCK_REDSTONE_REPEATER_ON, Meta);
				}
			}
			else if (Block != E_BLOCK_REDSTONE_REPEATER_OFF)
			{
				m_Chunk->SetBlock(itr->first.x, itr->first.y, itr->first.z, E_BLOCK_REDSTONE_REPEATER_OFF, Meta);
			}
			itr = m_RepeatersDelayList->erase(itr);
		}
		else
		{
			LOGD("Incremented a repeater @ {%i %i %i} | Elapsed ticks: %i | Target delay: %i", itr->first.x, itr->first.y, itr->first.z, itr->second.a_ElapsedTicks, itr->second.a_DelayTicks);
			itr->second.a_ElapsedTicks++;
			++itr;
		}
	}
}





void cIncrementalRedstoneSimulator::HandlePiston(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ)
{
	int BlockX = (m_Chunk->GetPosX() * cChunkDef::Width) + a_RelBlockX;
	int BlockZ = (m_Chunk->GetPosZ() * cChunkDef::Width) + a_RelBlockZ;

	if (IsPistonPowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ, m_Chunk->GetMeta(a_RelBlockX, a_RelBlockY, a_RelBlockZ) & 0x7))  // We only want the bottom three bits (4th controls extended-ness)
	{
		GetHandlerCompileTime<E_BLOCK_PISTON>::type::ExtendPiston(BlockX, a_RelBlockY, BlockZ, &this->m_World);
	}
	else
	{
		GetHandlerCompileTime<E_BLOCK_PISTON>::type::RetractPiston(BlockX, a_RelBlockY, BlockZ, &this->m_World);
	}
}





void cIncrementalRedstoneSimulator::HandleDropSpenser(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ)
{
	class cSetPowerToDropSpenser :
		public cRedstonePoweredCallback
	{
		bool m_IsPowered;
	public:
		cSetPowerToDropSpenser(bool a_IsPowered) : m_IsPowered(a_IsPowered) {}

		virtual bool Item(cRedstonePoweredEntity * a_DropSpenser) override
		{
			a_DropSpenser->SetRedstonePower(m_IsPowered);
			return false;
		}
	} DrSpSP(AreCoordsPowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ));

	int BlockX = (m_Chunk->GetPosX() * cChunkDef::Width) + a_RelBlockX;
	int BlockZ = (m_Chunk->GetPosZ() * cChunkDef::Width) + a_RelBlockZ;
	m_Chunk->DoWithRedstonePoweredEntityAt(BlockX, a_RelBlockY, BlockZ, DrSpSP);
}





void cIncrementalRedstoneSimulator::HandleRedstoneLamp(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ, BLOCKTYPE a_MyState)
{
	if (a_MyState == E_BLOCK_REDSTONE_LAMP_OFF)
	{
		if (AreCoordsPowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ))
		{
			m_Chunk->SetBlock(a_RelBlockX, a_RelBlockY, a_RelBlockZ, E_BLOCK_REDSTONE_LAMP_ON, 0);
		}
	}
	else
	{
		if (!AreCoordsPowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ))
		{
			m_Chunk->SetBlock(a_RelBlockX, a_RelBlockY, a_RelBlockZ, E_BLOCK_REDSTONE_LAMP_OFF, 0);
		}
	}
}





void cIncrementalRedstoneSimulator::HandleTNT(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ)
{
	int BlockX = (m_Chunk->GetPosX() * cChunkDef::Width) + a_RelBlockX;
	int BlockZ = (m_Chunk->GetPosZ() * cChunkDef::Width) + a_RelBlockZ;

	if (AreCoordsPowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ))
	{
		m_Chunk->BroadcastSoundEffect("game.tnt.primed", static_cast<double>(BlockX), static_cast<double>(a_RelBlockY), static_cast<double>(BlockZ), 0.5f, 0.6f);
		m_Chunk->SetBlock(a_RelBlockX, a_RelBlockY, a_RelBlockZ, E_BLOCK_AIR, 0);
		this->m_World.SpawnPrimedTNT(BlockX + 0.5, a_RelBlockY + 0.5, BlockZ + 0.5);  // 80 ticks to boom
	}
}





void cIncrementalRedstoneSimulator::HandleDoor(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ)
{
	int BlockX = (m_Chunk->GetPosX() * cChunkDef::Width) + a_RelBlockX;
	int BlockZ = (m_Chunk->GetPosZ() * cChunkDef::Width) + a_RelBlockZ;

	typedef GetHandlerCompileTime<E_BLOCK_OAK_DOOR>::type DoorHandler;

	if (AreCoordsPowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ))
	{
		if (!AreCoordsSimulated(a_RelBlockX, a_RelBlockY, a_RelBlockZ, true))
		{
			cChunkInterface ChunkInterface(this->m_World.GetChunkMap());
			if (!DoorHandler::IsOpen(ChunkInterface, BlockX, a_RelBlockY, BlockZ))
			{
				DoorHandler::SetOpen(ChunkInterface, BlockX, a_RelBlockY, BlockZ, true);
				m_Chunk->BroadcastSoundParticleEffect(1003, BlockX, a_RelBlockY, BlockZ, 0);
			}
			SetPlayerToggleableBlockAsSimulated(a_RelBlockX, a_RelBlockY, a_RelBlockZ, true);
		}
	}
	else
	{
		if (!AreCoordsSimulated(a_RelBlockX, a_RelBlockY, a_RelBlockZ, false))
		{
			cChunkInterface ChunkInterface(this->m_World.GetChunkMap());
			if (DoorHandler::IsOpen(ChunkInterface, BlockX, a_RelBlockY, BlockZ))
			{
				DoorHandler::SetOpen(ChunkInterface, BlockX, a_RelBlockY, BlockZ, false);
				m_Chunk->BroadcastSoundParticleEffect(1003, BlockX, a_RelBlockY, BlockZ, 0);
			}
			SetPlayerToggleableBlockAsSimulated(a_RelBlockX, a_RelBlockY, a_RelBlockZ, false);
		}
	}
}





void cIncrementalRedstoneSimulator::HandleCommandBlock(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ)
{
	class cSetPowerToCommandBlock :
		public cRedstonePoweredCallback
	{
		bool m_IsPowered;
	public:
		cSetPowerToCommandBlock(bool a_IsPowered) : m_IsPowered(a_IsPowered) {}

		virtual bool Item(cRedstonePoweredEntity * a_CommandBlock) override
		{
			a_CommandBlock->SetRedstonePower(m_IsPowered);
			return false;
		}
	} CmdBlockSP(AreCoordsPowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ));

	int BlockX = (m_Chunk->GetPosX() * cChunkDef::Width) + a_RelBlockX;
	int BlockZ = (m_Chunk->GetPosZ() * cChunkDef::Width) + a_RelBlockZ;
	m_Chunk->DoWithRedstonePoweredEntityAt(BlockX, a_RelBlockY, BlockZ, CmdBlockSP);
}





void cIncrementalRedstoneSimulator::HandleRail(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ, BLOCKTYPE a_MyType)
{
	switch (a_MyType)
	{
		case E_BLOCK_DETECTOR_RAIL:
		{
			if ((m_Chunk->GetMeta(a_RelBlockX, a_RelBlockY, a_RelBlockZ) & 0x08) == 0x08)
			{
				SetAllDirsAsPowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ, a_MyType);
			}
			break;
		}
		case E_BLOCK_ACTIVATOR_RAIL:
		case E_BLOCK_POWERED_RAIL:
		{
			if (AreCoordsPowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ))
			{
				m_Chunk->SetMeta(a_RelBlockX, a_RelBlockY, a_RelBlockZ, m_Chunk->GetMeta(a_RelBlockX, a_RelBlockY, a_RelBlockZ) | 0x08);
			}
			else
			{
				m_Chunk->SetMeta(a_RelBlockX, a_RelBlockY, a_RelBlockZ, m_Chunk->GetMeta(a_RelBlockX, a_RelBlockY, a_RelBlockZ) & 0x07);
			}
			break;
		}
		default: LOGD("Unhandled type of rail in %s", __FUNCTION__);
	}
}





void cIncrementalRedstoneSimulator::HandleTrapdoor(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ)
{
	int BlockX = (m_Chunk->GetPosX() * cChunkDef::Width) + a_RelBlockX;
	int BlockZ = (m_Chunk->GetPosZ() * cChunkDef::Width) + a_RelBlockZ;

	if (AreCoordsPowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ))
	{
		if (!AreCoordsSimulated(a_RelBlockX, a_RelBlockY, a_RelBlockZ, true))
		{
			this->m_World.SetTrapdoorOpen(BlockX, a_RelBlockY, BlockZ, true);
			SetPlayerToggleableBlockAsSimulated(a_RelBlockX, a_RelBlockY, a_RelBlockZ, true);
		}
	}
	else
	{
		if (!AreCoordsSimulated(a_RelBlockX, a_RelBlockY, a_RelBlockZ, false))
		{
			this->m_World.SetTrapdoorOpen(BlockX, a_RelBlockY, BlockZ, false);
			SetPlayerToggleableBlockAsSimulated(a_RelBlockX, a_RelBlockY, a_RelBlockZ, false);
		}
	}
}





void cIncrementalRedstoneSimulator::HandleNoteBlock(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ)
{
	bool m_bAreCoordsPowered = AreCoordsPowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ);

	if (m_bAreCoordsPowered)
	{
		if (!AreCoordsSimulated(a_RelBlockX, a_RelBlockY, a_RelBlockZ, true))
		{
			class cSetPowerToNoteBlock :
				public cRedstonePoweredCallback
			{
			public:
				cSetPowerToNoteBlock() {}

				virtual bool Item(cRedstonePoweredEntity * a_NoteBlock) override
				{
					a_NoteBlock->SetRedstonePower(true);
					return false;
				}
			} NoteBlockSP;

			int BlockX = (m_Chunk->GetPosX() * cChunkDef::Width) + a_RelBlockX;
			int BlockZ = (m_Chunk->GetPosZ() * cChunkDef::Width) + a_RelBlockZ;
			m_Chunk->DoWithRedstonePoweredEntityAt(BlockX, a_RelBlockY, BlockZ, NoteBlockSP);
			SetPlayerToggleableBlockAsSimulated(a_RelBlockX, a_RelBlockY, a_RelBlockZ, true);
		}
	}
	else
	{
		if (!AreCoordsSimulated(a_RelBlockX, a_RelBlockY, a_RelBlockZ, false))
		{
			SetPlayerToggleableBlockAsSimulated(a_RelBlockX, a_RelBlockY, a_RelBlockZ, false);
		}
	}
}





void cIncrementalRedstoneSimulator::HandleDaylightSensor(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ)
{
	int BlockX = (m_Chunk->GetPosX() * cChunkDef::Width) + a_RelBlockX, BlockZ = (m_Chunk->GetPosZ() * cChunkDef::Width) + a_RelBlockZ;
	int ChunkX, ChunkZ;
	cChunkDef::BlockToChunk(BlockX, BlockZ, ChunkX, ChunkZ);

	if (!m_World.IsChunkLighted(ChunkX, ChunkZ))
	{
		m_World.QueueLightChunk(ChunkX, ChunkZ);
	}
	else
	{
		if (m_Chunk->GetTimeAlteredLight(m_Chunk->GetSkyLight(a_RelBlockX, a_RelBlockY + 1, a_RelBlockZ)) > 8)
		{
			SetAllDirsAsPowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ);
		}
		else
		{
			SetSourceUnpowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ, m_Chunk);
		}
	}
}





void cIncrementalRedstoneSimulator::HandlePressurePlate(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ, BLOCKTYPE a_MyType)
{
	int BlockX = (m_Chunk->GetPosX() * cChunkDef::Width) + a_RelBlockX;
	int BlockZ = (m_Chunk->GetPosZ() * cChunkDef::Width) + a_RelBlockZ;

	switch (a_MyType)
	{
		case E_BLOCK_STONE_PRESSURE_PLATE:
		{
			// MCS feature - stone pressure plates can only be triggered by players :D
			cPlayer * a_Player = this->m_World.FindClosestPlayer(Vector3f(BlockX + 0.5f, static_cast<float>(a_RelBlockY), BlockZ + 0.5f), 0.5f, false);

			if (a_Player != nullptr)
			{
				m_Chunk->SetMeta(a_RelBlockX, a_RelBlockY, a_RelBlockZ, 0x1);
				SetAllDirsAsPowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ);
				SetDirectionLinkedPowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ, BLOCK_FACE_YM, a_MyType);
			}
			else
			{
				m_Chunk->SetMeta(a_RelBlockX, a_RelBlockY, a_RelBlockZ, 0x0);
				SetSourceUnpowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ, m_Chunk);
			}
			break;
		}
		case E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE:
		{
			class cPressurePlateCallback :
				public cEntityCallback
			{
			public:
				cPressurePlateCallback(int a_BlockX, int a_BlockY, int a_BlockZ) :
					m_NumberOfEntities(0),
					m_X(a_BlockX),
					m_Y(a_BlockY),
					m_Z(a_BlockZ)
				{
				}

				virtual bool Item(cEntity * a_Entity) override
				{
					Vector3f EntityPos = a_Entity->GetPosition();
					Vector3f BlockPos(m_X + 0.5f, static_cast<float>(m_Y), m_Z + 0.5f);
					double Distance = (EntityPos - BlockPos).Length();

					if (Distance <= 0.5)
					{
						m_NumberOfEntities++;
					}
					return false;
				}

				bool GetPowerLevel(unsigned char & a_PowerLevel) const
				{
					a_PowerLevel = static_cast<unsigned char>(std::min(m_NumberOfEntities, MAX_POWER_LEVEL));
					return (a_PowerLevel > 0);
				}

			protected:
				int m_NumberOfEntities;

				int m_X;
				int m_Y;
				int m_Z;
			};

			cPressurePlateCallback PressurePlateCallback(BlockX, a_RelBlockY, BlockZ);
			this->m_World.ForEachEntityInChunk(m_Chunk->GetPosX(), m_Chunk->GetPosZ(), PressurePlateCallback);

			unsigned char Power;
			NIBBLETYPE Meta = m_Chunk->GetMeta(a_RelBlockX, a_RelBlockY, a_RelBlockZ);
			if (PressurePlateCallback.GetPowerLevel(Power))
			{
				if (Meta == E_META_PRESSURE_PLATE_RAISED)
				{
					m_Chunk->BroadcastSoundEffect("random.click", static_cast<double>(BlockX) + 0.5, static_cast<double>(a_RelBlockY) + 0.1, static_cast<double>(BlockZ) + 0.5, 0.3F, 0.5F);
				}
				m_Chunk->SetMeta(a_RelBlockX, a_RelBlockY, a_RelBlockZ, E_META_PRESSURE_PLATE_DEPRESSED);
				SetAllDirsAsPowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ, Power);
				SetDirectionLinkedPowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ, BLOCK_FACE_YM, a_MyType);
			}
			else
			{
				if (Meta == E_META_PRESSURE_PLATE_DEPRESSED)
				{
					m_Chunk->BroadcastSoundEffect("random.click", static_cast<double>(BlockX) + 0.5, static_cast<double>(a_RelBlockY) + 0.1, static_cast<double>(BlockZ) + 0.5, 0.3F, 0.6F);
				}
				m_Chunk->SetMeta(a_RelBlockX, a_RelBlockY, a_RelBlockZ, E_META_PRESSURE_PLATE_RAISED);
				SetSourceUnpowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ, m_Chunk);
			}

			break;
		}
		case E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE:
		{
			class cPressurePlateCallback :
				public cEntityCallback
			{
			public:
				cPressurePlateCallback(int a_BlockX, int a_BlockY, int a_BlockZ) :
					m_NumberOfEntities(0),
					m_X(a_BlockX),
					m_Y(a_BlockY),
					m_Z(a_BlockZ)
				{
				}

				virtual bool Item(cEntity * a_Entity) override
				{
					Vector3f EntityPos = a_Entity->GetPosition();
					Vector3f BlockPos(m_X + 0.5f, static_cast<float>(m_Y), m_Z + 0.5f);
					double Distance = (EntityPos - BlockPos).Length();

					if (Distance <= 0.5)
					{
						m_NumberOfEntities++;
					}
					return false;
				}

				bool GetPowerLevel(unsigned char & a_PowerLevel) const
				{
					a_PowerLevel = static_cast<Byte>(std::min(static_cast<int>(ceil(m_NumberOfEntities / 10.f)), MAX_POWER_LEVEL));
					return (a_PowerLevel > 0);
				}

			protected:
				int m_NumberOfEntities;

				int m_X;
				int m_Y;
				int m_Z;
			};

			cPressurePlateCallback PressurePlateCallback(BlockX, a_RelBlockY, BlockZ);
			this->m_World.ForEachEntityInChunk(m_Chunk->GetPosX(), m_Chunk->GetPosZ(), PressurePlateCallback);

			unsigned char Power;
			NIBBLETYPE Meta = m_Chunk->GetMeta(a_RelBlockX, a_RelBlockY, a_RelBlockZ);
			if (PressurePlateCallback.GetPowerLevel(Power))
			{
				if (Meta == E_META_PRESSURE_PLATE_RAISED)
				{
					m_Chunk->BroadcastSoundEffect("random.click", static_cast<double>(BlockX) + 0.5, static_cast<double>(a_RelBlockY) + 0.1, static_cast<double>(BlockZ) + 0.5, 0.3F, 0.5F);
				}
				m_Chunk->SetMeta(a_RelBlockX, a_RelBlockY, a_RelBlockZ, E_META_PRESSURE_PLATE_DEPRESSED);
				SetAllDirsAsPowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ, Power);
				SetDirectionLinkedPowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ, BLOCK_FACE_YM, a_MyType);
			}
			else
			{
				if (Meta == E_META_PRESSURE_PLATE_DEPRESSED)
				{
					m_Chunk->BroadcastSoundEffect("random.click", static_cast<double>(BlockX) + 0.5, static_cast<double>(a_RelBlockY) + 0.1, static_cast<double>(BlockZ) + 0.5, 0.3F, 0.6F);
				}
				m_Chunk->SetMeta(a_RelBlockX, a_RelBlockY, a_RelBlockZ, E_META_PRESSURE_PLATE_RAISED);
				SetSourceUnpowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ, m_Chunk);
			}

			break;
		}
		case E_BLOCK_WOODEN_PRESSURE_PLATE:
		{
			class cPressurePlateCallback :
				public cEntityCallback
			{
			public:
				cPressurePlateCallback(int a_BlockX, int a_BlockY, int a_BlockZ) :
					m_FoundEntity(false),
					m_X(a_BlockX),
					m_Y(a_BlockY),
					m_Z(a_BlockZ)
				{
				}

				virtual bool Item(cEntity * a_Entity) override
				{
					Vector3f EntityPos = a_Entity->GetPosition();
					Vector3f BlockPos(m_X + 0.5f, static_cast<float>(m_Y), m_Z + 0.5f);
					double Distance = (EntityPos - BlockPos).Length();

					if (Distance <= 0.5)
					{
						m_FoundEntity = true;
						return true;  // Break out, we only need to know for plates that at least one entity is on top
					}
					return false;
				}

				bool FoundEntity(void) const
				{
					return m_FoundEntity;
				}

			protected:
				bool m_FoundEntity;

				int m_X;
				int m_Y;
				int m_Z;
			};

			cPressurePlateCallback PressurePlateCallback(BlockX, a_RelBlockY, BlockZ);
			this->m_World.ForEachEntityInChunk(m_Chunk->GetPosX(), m_Chunk->GetPosZ(), PressurePlateCallback);

			NIBBLETYPE Meta = m_Chunk->GetMeta(a_RelBlockX, a_RelBlockY, a_RelBlockZ);
			if (PressurePlateCallback.FoundEntity())
			{
				if (Meta == E_META_PRESSURE_PLATE_RAISED)
				{
					m_Chunk->BroadcastSoundEffect("random.click", static_cast<double>(BlockX) + 0.5, static_cast<double>(a_RelBlockY) + 0.1, static_cast<double>(BlockZ) + 0.5, 0.3F, 0.5F);
				}
				m_Chunk->SetMeta(a_RelBlockX, a_RelBlockY, a_RelBlockZ, E_META_PRESSURE_PLATE_DEPRESSED);
				SetAllDirsAsPowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ);
				SetDirectionLinkedPowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ, BLOCK_FACE_YM, a_MyType);
			}
			else
			{
				if (Meta == E_META_PRESSURE_PLATE_DEPRESSED)
				{
					m_Chunk->BroadcastSoundEffect("random.click", static_cast<double>(BlockX) + 0.5, static_cast<double>(a_RelBlockY) + 0.1, static_cast<double>(BlockZ) + 0.5, 0.3F, 0.6F);
				}
				m_Chunk->SetMeta(a_RelBlockX, a_RelBlockY, a_RelBlockZ, E_META_PRESSURE_PLATE_RAISED);
				SetSourceUnpowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ, m_Chunk);
			}
			break;
		}
		default:
		{
			LOGD("Unimplemented pressure plate type %s in cRedstoneSimulator", ItemToFullString(cItem(a_MyType)).c_str());
			break;
		}
	}
}





void cIncrementalRedstoneSimulator::HandleTripwireHook(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ)
{
	int RelX = a_RelBlockX, RelZ = a_RelBlockZ;
	bool FoundActivated = false;
	eBlockFace FaceToGoTowards = GetHandlerCompileTime<E_BLOCK_TRIPWIRE_HOOK>::type::MetadataToDirection(m_Chunk->GetMeta(a_RelBlockX, a_RelBlockY, a_RelBlockZ));

	for (int i = 0; i < 40; ++i)  // Tripwires can be connected up to 40 blocks
	{
		BLOCKTYPE Type;
		NIBBLETYPE Meta;

		AddFaceDirection(RelX, a_RelBlockY, RelZ, FaceToGoTowards);
		m_Chunk->UnboundedRelGetBlock(RelX, a_RelBlockY, RelZ, Type, Meta);

		if (Type == E_BLOCK_TRIPWIRE)
		{
			if (Meta == 0x1)
			{
				FoundActivated = true;
			}
		}
		else if (Type == E_BLOCK_TRIPWIRE_HOOK)
		{
			if (ReverseBlockFace(GetHandlerCompileTime<E_BLOCK_TRIPWIRE_HOOK>::type::MetadataToDirection(Meta)) == FaceToGoTowards)
			{
				// Other hook facing in opposite direction - circuit completed!
				break;
			}
			else
			{
				// Tripwire hook not connected at all, AND away all the power state bits
				m_Chunk->SetMeta(a_RelBlockX, a_RelBlockY, a_RelBlockZ, m_Chunk->GetMeta(a_RelBlockX, a_RelBlockY, a_RelBlockZ) & 0x3);
				SetSourceUnpowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ, m_Chunk);
				return;
			}
		}
		else
		{
			// Tripwire hook not connected at all, AND away all the power state bits
			m_Chunk->SetMeta(a_RelBlockX, a_RelBlockY, a_RelBlockZ, m_Chunk->GetMeta(a_RelBlockX, a_RelBlockY, a_RelBlockZ) & 0x3);
			SetSourceUnpowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ, m_Chunk);
			return;
		}
	}

	if (FoundActivated)
	{
		// Connected and activated, set the 3rd and 4th highest bits
		m_Chunk->SetMeta(a_RelBlockX, a_RelBlockY, a_RelBlockZ, m_Chunk->GetMeta(a_RelBlockX, a_RelBlockY, a_RelBlockZ) | 0xC);
		SetAllDirsAsPowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ);
	}
	else
	{
		// Connected but not activated, AND away the highest bit
		m_Chunk->SetMeta(a_RelBlockX, a_RelBlockY, a_RelBlockZ, (m_Chunk->GetMeta(a_RelBlockX, a_RelBlockY, a_RelBlockZ) & 0x7) | 0x4);
		SetSourceUnpowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ, m_Chunk);
	}
}





void cIncrementalRedstoneSimulator::HandleTrappedChest(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ)
{
	class cGetTrappedChestPlayers :
		public cItemCallback<cChestEntity>
	{
	public:
		cGetTrappedChestPlayers(void) :
			m_NumberOfPlayers(0)
		{
		}

		virtual ~cGetTrappedChestPlayers()
		{
		}

		virtual bool Item(cChestEntity * a_Chest) override
		{
			ASSERT(a_Chest->GetBlockType() == E_BLOCK_TRAPPED_CHEST);
			m_NumberOfPlayers = a_Chest->GetNumberOfPlayers();
			return (m_NumberOfPlayers <= 0);
		}

		unsigned char GetPowerLevel(void) const
		{
			return static_cast<unsigned char>(std::min(m_NumberOfPlayers, MAX_POWER_LEVEL));
		}

	private:
		int m_NumberOfPlayers;

	} GTCP;

	int BlockX = m_Chunk->GetPosX() * cChunkDef::Width + a_RelBlockX;
	int BlockZ = m_Chunk->GetPosZ() * cChunkDef::Width + a_RelBlockZ;
	if (m_Chunk->DoWithChestAt(BlockX, a_RelBlockY, BlockZ, GTCP))
	{
		SetAllDirsAsPowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ, GTCP.GetPowerLevel());
	}
	else
	{
		SetSourceUnpowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ, m_Chunk);
	}
}





void cIncrementalRedstoneSimulator::HandleTripwire(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ)
{
	int BlockX = m_Chunk->GetPosX() * cChunkDef::Width + a_RelBlockX;
	int BlockZ = m_Chunk->GetPosZ() * cChunkDef::Width + a_RelBlockZ;

	class cTripwireCallback :
		public cEntityCallback
	{
	public:
		cTripwireCallback(int a_BlockX, int a_BlockY, int a_BlockZ) :
			m_FoundEntity(false),
			m_X(a_BlockX),
			m_Y(a_BlockY),
			m_Z(a_BlockZ)
		{
		}

		virtual bool Item(cEntity * a_Entity) override
		{
			cBoundingBox bbWire(m_X, m_X + 1, m_Y, m_Y + 0.1, m_Z, m_Z + 1);
			cBoundingBox bbEntity(a_Entity->GetPosition(), a_Entity->GetWidth() / 2, a_Entity->GetHeight());

			if (bbEntity.DoesIntersect(bbWire))
			{
				m_FoundEntity = true;
				return true;  // One entity is sufficient to trigger the wire
			}
			return false;
		}

		bool FoundEntity(void) const
		{
			return m_FoundEntity;
		}

	protected:
		bool m_FoundEntity;

		int m_X;
		int m_Y;
		int m_Z;
	};

	cTripwireCallback TripwireCallback(BlockX, a_RelBlockY, BlockZ);
	this->m_World.ForEachEntityInChunk(m_Chunk->GetPosX(), m_Chunk->GetPosZ(), TripwireCallback);

	if (TripwireCallback.FoundEntity())
	{
		m_Chunk->SetMeta(a_RelBlockX, a_RelBlockY, a_RelBlockZ, 0x1);
	}
	else
	{
		m_Chunk->SetMeta(a_RelBlockX, a_RelBlockY, a_RelBlockZ, 0x0);
	}
}





bool cIncrementalRedstoneSimulator::AreCoordsDirectlyPowered(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ, cChunk * a_Chunk)
{
	// Torches want to access neighbour's data when on a wall, hence the extra chunk parameter

	const auto & Data = static_cast<cIncrementalRedstoneSimulatorChunkData *>(a_Chunk->GetRedstoneSimulatorData())->m_PoweredBlocks;
	return std::find_if(Data.begin(), Data.end(), [a_RelBlockX, a_RelBlockY, a_RelBlockZ](const sPoweredBlocks & itr) { return itr.m_BlockPos == Vector3i(a_RelBlockX, a_RelBlockY, a_RelBlockZ); }) != Data.end();
}





bool cIncrementalRedstoneSimulator::AreCoordsLinkedPowered(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ, cChunk * a_Chunk)
{
	const auto & Data = static_cast<cIncrementalRedstoneSimulatorChunkData *>(a_Chunk->GetRedstoneSimulatorData())->m_LinkedBlocks;
	return std::find_if(Data.begin(), Data.end(), [a_RelBlockX, a_RelBlockY, a_RelBlockZ](const sLinkedPoweredBlocks & itr) { return itr.a_BlockPos == Vector3i(a_RelBlockX, a_RelBlockY, a_RelBlockZ); }) != Data.end();
}





bool cIncrementalRedstoneSimulator::IsRepeaterPowered(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ, NIBBLETYPE a_Meta)
{
	// Repeaters cannot be powered by any face except their back; verify that this is true for a source

	for (const auto & itr : *m_PoweredBlocks)
	{
		if (!itr.m_BlockPos.Equals(Vector3i(a_RelBlockX, a_RelBlockY, a_RelBlockZ)))
		{
			continue;
		}

		switch (a_Meta & 0x3)
		{
			case 0x0:
			{
				// Flip the coords to check the back of the repeater
				if (itr.m_SourcePos.Equals(Vector3i(a_RelBlockX, a_RelBlockY, a_RelBlockZ + 1)))
				{
					return true;
				}
				break;
			}
			case 0x1:
			{
				if (itr.m_SourcePos.Equals(Vector3i(a_RelBlockX - 1, a_RelBlockY, a_RelBlockZ)))
				{
					return true;
				}
				break;
			}
			case 0x2:
			{
				if (itr.m_SourcePos.Equals(Vector3i(a_RelBlockX, a_RelBlockY, a_RelBlockZ - 1)))
				{
					return true;
				}
				break;
			}
			case 0x3:
			{
				if (itr.m_SourcePos.Equals(Vector3i(a_RelBlockX + 1, a_RelBlockY, a_RelBlockZ)))
				{
					return true;
				}
				break;
			}
		}
	}  // for itr - m_PoweredBlocks[]

	for (const auto & itr : *m_LinkedPoweredBlocks)
	{
		if (!itr.a_BlockPos.Equals(Vector3i(a_RelBlockX, a_RelBlockY, a_RelBlockZ)))
		{
			continue;
		}

		switch (a_Meta & 0x3)
		{
			case 0x0:
			{
				if (itr.a_MiddlePos.Equals(Vector3i(a_RelBlockX, a_RelBlockY, a_RelBlockZ + 1)))
				{
					return true;
				}
				break;
			}
			case 0x1:
			{
				if (itr.a_MiddlePos.Equals(Vector3i(a_RelBlockX - 1, a_RelBlockY, a_RelBlockZ)))
				{
					return true;
				}
				break;
			}
			case 0x2:
			{
				if (itr.a_MiddlePos.Equals(Vector3i(a_RelBlockX, a_RelBlockY, a_RelBlockZ - 1)))
				{
					return true;
				}
				break;
			}
			case 0x3:
			{
				if (itr.a_MiddlePos.Equals(Vector3i(a_RelBlockX + 1, a_RelBlockY, a_RelBlockZ)))
				{
					return true;
				}
				break;
			}
		}
	}  // for itr - m_LinkedPoweredBlocks[]
	return false;  // Couldn't find power source behind repeater
}





bool cIncrementalRedstoneSimulator::IsRepeaterLocked(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ, NIBBLETYPE a_Meta)
{
	switch (a_Meta & 0x3)  // We only want the 'direction' part of our metadata
	{
		// If the repeater is looking up or down (If parallel to the Z axis)
		case 0x0:
		case 0x2:
		{
			// Check if eastern (right) neighbor is a powered on repeater who is facing us
			BLOCKTYPE Block = 0;
			NIBBLETYPE OtherRepeaterDir = 0;
			if (
				m_Chunk->UnboundedRelGetBlock(a_RelBlockX + 1, a_RelBlockY, a_RelBlockZ, Block, OtherRepeaterDir) &&
				(Block == E_BLOCK_REDSTONE_REPEATER_ON)
			)
			{
				if ((OtherRepeaterDir & 0x03) == 0x3)
				{
					return true;
				}  // If so, I am latched / locked
			}

			// Check if western(left) neighbor is a powered on repeater who is facing us
			if (
				m_Chunk->UnboundedRelGetBlock(a_RelBlockX - 1, a_RelBlockY, a_RelBlockZ, Block, OtherRepeaterDir) &&
				(Block == E_BLOCK_REDSTONE_REPEATER_ON)
			)
			{
				if ((OtherRepeaterDir & 0x03) == 0x1)
				{
					return true;
				}  // If so, I am latched / locked
			}

			break;
		}

		// If the repeater is looking left or right (If parallel to the x axis)
		case 0x1:
		case 0x3:
		{
			// Check if southern(down) neighbor is a powered on repeater who is facing us
			BLOCKTYPE Block = 0;
			NIBBLETYPE OtherRepeaterDir = 0;

			if (
				m_Chunk->UnboundedRelGetBlock(a_RelBlockX, a_RelBlockY, a_RelBlockZ + 1, Block, OtherRepeaterDir) &&
				(Block == E_BLOCK_REDSTONE_REPEATER_ON)
			)
			{
				if ((OtherRepeaterDir & 0x30) == 0x00)
				{
					return true;
				}  // If so, I am latched / locked
			}

			// Check if northern(up) neighbor is a powered on repeater who is facing us
			if (
				m_Chunk->UnboundedRelGetBlock(a_RelBlockX, a_RelBlockY, a_RelBlockZ - 1, Block, OtherRepeaterDir) &&
				(Block == E_BLOCK_REDSTONE_REPEATER_ON)
			)
			{
				if ((OtherRepeaterDir & 0x03) == 0x02)
				{
					return true;
				}  // If so, I am latched / locked
			}

			break;
		}
	}

	return false;  // None of the checks succeeded, I am not a locked repeater
}




bool cIncrementalRedstoneSimulator::IsPistonPowered(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ, NIBBLETYPE a_Meta)
{
	// Pistons cannot be powered through their front face; this function verifies that a source meets this requirement

	eBlockFace Face = GetHandlerCompileTime<E_BLOCK_PISTON>::type::MetaDataToDirection(a_Meta);

	for (const auto & itr : *m_PoweredBlocks)
	{
		if (!itr.m_BlockPos.Equals(Vector3i(a_RelBlockX, a_RelBlockY, a_RelBlockZ)))
		{
			continue;
		}

		int X = a_RelBlockX, Z = a_RelBlockZ;
		AddFaceDirection(X, a_RelBlockY, Z, Face);

		if (!itr.m_SourcePos.Equals(AdjustRelativeCoords(Vector3i(X, a_RelBlockY, Z))))
		{
			return true;
		}
	}

	for (const auto & itr : *m_LinkedPoweredBlocks)
	{
		if (!itr.a_BlockPos.Equals(Vector3i(a_RelBlockX, a_RelBlockY, a_RelBlockZ)))
		{
			continue;
		}

		int X = a_RelBlockX, Z = a_RelBlockZ;
		AddFaceDirection(X, a_RelBlockY, Z, Face);

		if (!itr.a_MiddlePos.Equals(AdjustRelativeCoords(Vector3i(X, a_RelBlockY, Z))))
		{
			return true;
		}
	}
	return false;  // Source was in front of the piston's front face
}




unsigned char cIncrementalRedstoneSimulator::IsWirePowered(Vector3i a_RelBlockPosition, cChunk * a_Chunk)
{
	unsigned char PowerLevel = 0;

	for (const auto & itr : static_cast<cIncrementalRedstoneSimulatorChunkData *>(a_Chunk->GetRedstoneSimulatorData())->m_PoweredBlocks)  // Check powered list
	{
		if (itr.m_BlockPos != a_RelBlockPosition)
		{
			continue;
		}
		PowerLevel = std::max(itr.m_PowerLevel, PowerLevel);  // Get the highest power level (a_PowerLevel is initialised already and there CAN be multiple levels for one block)
	}

	for (const auto & itr : static_cast<cIncrementalRedstoneSimulatorChunkData *>(a_Chunk->GetRedstoneSimulatorData())->m_LinkedBlocks)  // Check linked powered list
	{
		if (itr.a_BlockPos != a_RelBlockPosition)
		{
			continue;
		}

		BLOCKTYPE Type = E_BLOCK_AIR;
		if (!a_Chunk->UnboundedRelGetBlockType(itr.a_SourcePos.x, itr.a_SourcePos.y, itr.a_SourcePos.z, Type) || (Type == E_BLOCK_REDSTONE_WIRE))
		{
			continue;
		}
		PowerLevel = std::max(itr.a_PowerLevel, PowerLevel);
	}

	return PowerLevel;
}





bool cIncrementalRedstoneSimulator::AreCoordsSimulated(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ, bool IsCurrentStatePowered)
{
	for (const auto & itr : *m_SimulatedPlayerToggleableBlocks)
	{
		if (itr.first.Equals(Vector3i(a_RelBlockX, a_RelBlockY, a_RelBlockZ)))
		{
			if (itr.second != IsCurrentStatePowered)  // Was the last power state different to the current?
			{
				return false;  // It was, coordinates are no longer simulated
			}
			else
			{
				return true;  // It wasn't, don't resimulate block, and allow players to toggle
			}
		}
	}
	return false;  // Block wasn't even in the list, not simulated
}





void cIncrementalRedstoneSimulator::SetDirectionLinkedPowered(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ, char a_Direction, unsigned char a_PowerLevel)
{
	BLOCKTYPE MiddleBlock = 0;
	switch (a_Direction)
	{
		case BLOCK_FACE_XM:
		{
			if (!m_Chunk->UnboundedRelGetBlockType(a_RelBlockX - 1, a_RelBlockY, a_RelBlockZ, MiddleBlock))
			{
				return;
			}

			SetBlockLinkedPowered(a_RelBlockX - 2, a_RelBlockY, a_RelBlockZ, a_RelBlockX - 1, a_RelBlockY, a_RelBlockZ, a_RelBlockX, a_RelBlockY, a_RelBlockZ, MiddleBlock, a_PowerLevel);
			SetBlockLinkedPowered(a_RelBlockX - 1, a_RelBlockY + 1, a_RelBlockZ, a_RelBlockX - 1, a_RelBlockY, a_RelBlockZ, a_RelBlockX, a_RelBlockY, a_RelBlockZ, MiddleBlock, a_PowerLevel);
			SetBlockLinkedPowered(a_RelBlockX - 1, a_RelBlockY - 1, a_RelBlockZ, a_RelBlockX - 1, a_RelBlockY, a_RelBlockZ, a_RelBlockX, a_RelBlockY, a_RelBlockZ, MiddleBlock, a_PowerLevel);
			SetBlockLinkedPowered(a_RelBlockX - 1, a_RelBlockY, a_RelBlockZ + 1, a_RelBlockX - 1, a_RelBlockY, a_RelBlockZ, a_RelBlockX, a_RelBlockY, a_RelBlockZ, MiddleBlock, a_PowerLevel);
			SetBlockLinkedPowered(a_RelBlockX - 1, a_RelBlockY, a_RelBlockZ - 1, a_RelBlockX - 1, a_RelBlockY, a_RelBlockZ, a_RelBlockX, a_RelBlockY, a_RelBlockZ, MiddleBlock, a_PowerLevel);

			break;
		}
		case BLOCK_FACE_XP:
		{
			if (!m_Chunk->UnboundedRelGetBlockType(a_RelBlockX + 1, a_RelBlockY, a_RelBlockZ, MiddleBlock))
			{
				return;
			}

			SetBlockLinkedPowered(a_RelBlockX + 2, a_RelBlockY, a_RelBlockZ, a_RelBlockX + 1, a_RelBlockY, a_RelBlockZ, a_RelBlockX, a_RelBlockY, a_RelBlockZ, MiddleBlock, a_PowerLevel);
			SetBlockLinkedPowered(a_RelBlockX + 1, a_RelBlockY + 1, a_RelBlockZ, a_RelBlockX + 1, a_RelBlockY, a_RelBlockZ, a_RelBlockX, a_RelBlockY, a_RelBlockZ, MiddleBlock, a_PowerLevel);
			SetBlockLinkedPowered(a_RelBlockX + 1, a_RelBlockY - 1, a_RelBlockZ, a_RelBlockX + 1, a_RelBlockY, a_RelBlockZ, a_RelBlockX, a_RelBlockY, a_RelBlockZ, MiddleBlock, a_PowerLevel);
			SetBlockLinkedPowered(a_RelBlockX + 1, a_RelBlockY, a_RelBlockZ + 1, a_RelBlockX + 1, a_RelBlockY, a_RelBlockZ, a_RelBlockX, a_RelBlockY, a_RelBlockZ, MiddleBlock, a_PowerLevel);
			SetBlockLinkedPowered(a_RelBlockX + 1, a_RelBlockY, a_RelBlockZ - 1, a_RelBlockX + 1, a_RelBlockY, a_RelBlockZ, a_RelBlockX, a_RelBlockY, a_RelBlockZ, MiddleBlock, a_PowerLevel);

			break;
		}
		case BLOCK_FACE_YM:
		{
			if (!m_Chunk->UnboundedRelGetBlockType(a_RelBlockX, a_RelBlockY - 1, a_RelBlockZ, MiddleBlock))
			{
				return;
			}

			SetBlockLinkedPowered(a_RelBlockX, a_RelBlockY - 2, a_RelBlockZ, a_RelBlockX, a_RelBlockY - 1, a_RelBlockZ, a_RelBlockX, a_RelBlockY, a_RelBlockZ, MiddleBlock, a_PowerLevel);
			SetBlockLinkedPowered(a_RelBlockX + 1, a_RelBlockY - 1, a_RelBlockZ, a_RelBlockX, a_RelBlockY - 1, a_RelBlockZ, a_RelBlockX, a_RelBlockY, a_RelBlockZ, MiddleBlock, a_PowerLevel);
			SetBlockLinkedPowered(a_RelBlockX - 1, a_RelBlockY - 1, a_RelBlockZ, a_RelBlockX, a_RelBlockY - 1, a_RelBlockZ, a_RelBlockX, a_RelBlockY, a_RelBlockZ, MiddleBlock, a_PowerLevel);
			SetBlockLinkedPowered(a_RelBlockX, a_RelBlockY - 1, a_RelBlockZ + 1, a_RelBlockX, a_RelBlockY - 1, a_RelBlockZ, a_RelBlockX, a_RelBlockY, a_RelBlockZ, MiddleBlock, a_PowerLevel);
			SetBlockLinkedPowered(a_RelBlockX, a_RelBlockY - 1, a_RelBlockZ - 1, a_RelBlockX, a_RelBlockY - 1, a_RelBlockZ, a_RelBlockX, a_RelBlockY, a_RelBlockZ, MiddleBlock, a_PowerLevel);

			break;
		}
		case BLOCK_FACE_YP:
		{
			if (!m_Chunk->UnboundedRelGetBlockType(a_RelBlockX, a_RelBlockY + 1, a_RelBlockZ, MiddleBlock))
			{
				return;
			}

			SetBlockLinkedPowered(a_RelBlockX, a_RelBlockY + 2, a_RelBlockZ, a_RelBlockX, a_RelBlockY + 1, a_RelBlockZ, a_RelBlockX, a_RelBlockY, a_RelBlockZ, MiddleBlock, a_PowerLevel);
			SetBlockLinkedPowered(a_RelBlockX + 1, a_RelBlockY + 1, a_RelBlockZ, a_RelBlockX, a_RelBlockY + 1, a_RelBlockZ, a_RelBlockX, a_RelBlockY, a_RelBlockZ, MiddleBlock, a_PowerLevel);
			SetBlockLinkedPowered(a_RelBlockX - 1, a_RelBlockY + 1, a_RelBlockZ, a_RelBlockX, a_RelBlockY + 1, a_RelBlockZ, a_RelBlockX, a_RelBlockY, a_RelBlockZ, MiddleBlock, a_PowerLevel);
			SetBlockLinkedPowered(a_RelBlockX, a_RelBlockY + 1, a_RelBlockZ + 1, a_RelBlockX, a_RelBlockY + 1, a_RelBlockZ, a_RelBlockX, a_RelBlockY, a_RelBlockZ, MiddleBlock, a_PowerLevel);
			SetBlockLinkedPowered(a_RelBlockX, a_RelBlockY + 1, a_RelBlockZ - 1, a_RelBlockX, a_RelBlockY + 1, a_RelBlockZ, a_RelBlockX, a_RelBlockY, a_RelBlockZ, MiddleBlock, a_PowerLevel);

			break;
		}
		case BLOCK_FACE_ZM:
		{
			if (!m_Chunk->UnboundedRelGetBlockType(a_RelBlockX, a_RelBlockY, a_RelBlockZ - 1, MiddleBlock))
			{
				return;
			}

			SetBlockLinkedPowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ - 2, a_RelBlockX, a_RelBlockY, a_RelBlockZ - 1, a_RelBlockX, a_RelBlockY, a_RelBlockZ, MiddleBlock, a_PowerLevel);
			SetBlockLinkedPowered(a_RelBlockX + 1, a_RelBlockY, a_RelBlockZ - 1, a_RelBlockX, a_RelBlockY, a_RelBlockZ - 1, a_RelBlockX, a_RelBlockY, a_RelBlockZ, MiddleBlock, a_PowerLevel);
			SetBlockLinkedPowered(a_RelBlockX - 1, a_RelBlockY, a_RelBlockZ - 1, a_RelBlockX, a_RelBlockY, a_RelBlockZ - 1, a_RelBlockX, a_RelBlockY, a_RelBlockZ, MiddleBlock, a_PowerLevel);
			SetBlockLinkedPowered(a_RelBlockX, a_RelBlockY + 1, a_RelBlockZ - 1, a_RelBlockX, a_RelBlockY, a_RelBlockZ - 1, a_RelBlockX, a_RelBlockY, a_RelBlockZ, MiddleBlock, a_PowerLevel);
			SetBlockLinkedPowered(a_RelBlockX, a_RelBlockY - 1, a_RelBlockZ - 1, a_RelBlockX, a_RelBlockY, a_RelBlockZ - 1, a_RelBlockX, a_RelBlockY, a_RelBlockZ, MiddleBlock, a_PowerLevel);

			break;
		}
		case BLOCK_FACE_ZP:
		{
			if (!m_Chunk->UnboundedRelGetBlockType(a_RelBlockX, a_RelBlockY, a_RelBlockZ + 1, MiddleBlock))
			{
				return;
			}

			SetBlockLinkedPowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ + 2, a_RelBlockX, a_RelBlockY, a_RelBlockZ + 1, a_RelBlockX, a_RelBlockY, a_RelBlockZ, MiddleBlock, a_PowerLevel);
			SetBlockLinkedPowered(a_RelBlockX + 1, a_RelBlockY, a_RelBlockZ + 1, a_RelBlockX, a_RelBlockY, a_RelBlockZ + 1, a_RelBlockX, a_RelBlockY, a_RelBlockZ, MiddleBlock, a_PowerLevel);
			SetBlockLinkedPowered(a_RelBlockX - 1, a_RelBlockY, a_RelBlockZ + 1, a_RelBlockX, a_RelBlockY, a_RelBlockZ + 1, a_RelBlockX, a_RelBlockY, a_RelBlockZ, MiddleBlock, a_PowerLevel);
			SetBlockLinkedPowered(a_RelBlockX, a_RelBlockY + 1, a_RelBlockZ + 1, a_RelBlockX, a_RelBlockY, a_RelBlockZ + 1, a_RelBlockX, a_RelBlockY, a_RelBlockZ, MiddleBlock, a_PowerLevel);
			SetBlockLinkedPowered(a_RelBlockX, a_RelBlockY - 1, a_RelBlockZ + 1, a_RelBlockX, a_RelBlockY, a_RelBlockZ + 1, a_RelBlockX, a_RelBlockY, a_RelBlockZ, MiddleBlock, a_PowerLevel);

			break;
		}
		default:
		{
			ASSERT(!"Unhandled face direction when attempting to set blocks as linked powered!");  // Zombies, that wasn't supposed to happen...
			break;
		}
	}
}





void cIncrementalRedstoneSimulator::SetAllDirsAsPowered(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ, unsigned char a_PowerLevel)
{
	static const Vector3i Offsets[] =
	{
		{1, 0, 0},
		{ -1, 0, 0},
		{0, 0, 1},
		{0, 0, -1},
		{0, 1, 0},
		{0, -1, 0}
	};

	for (auto Offset : Offsets)  // Loop through struct to power all directions
	{
		SetBlockPowered(Vector3i(a_RelBlockX, a_RelBlockY, a_RelBlockZ) + Offset, Vector3i(a_RelBlockX, a_RelBlockY, a_RelBlockZ), a_PowerLevel);
	}
}





void cIncrementalRedstoneSimulator::SetBlockPowered(Vector3i a_RelBlockPosition, Vector3i a_RelSourcePosition, unsigned char a_PowerLevel)
{
	cChunk * Neighbour = m_Chunk->GetRelNeighborChunkAdjustCoords(a_RelBlockPosition.x, a_RelBlockPosition.z);  // Adjust coordinates for the later call using these values
	if ((Neighbour == nullptr) || !Neighbour->IsValid())
	{
		return;
	}
	a_RelSourcePosition.x += (m_Chunk->GetPosX() - Neighbour->GetPosX()) * cChunkDef::Width;
	a_RelSourcePosition.z += (m_Chunk->GetPosZ() - Neighbour->GetPosZ()) * cChunkDef::Width;

	auto & Powered = static_cast<cIncrementalRedstoneSimulatorChunkData *>(Neighbour->GetRedstoneSimulatorData())->m_PoweredBlocks;  // We need to insert the value into the chunk who owns the block position
	for (auto & itr : Powered)
	{
		if ((itr.m_BlockPos == a_RelBlockPosition) && (itr.m_SourcePos == a_RelSourcePosition))
		{
			if (itr.m_PowerLevel != a_PowerLevel)
			{
				// Update power level, don't add a new listing
				Neighbour->SetIsRedstoneDirty(true);
				m_Chunk->SetIsRedstoneDirty(true);
				itr.m_PowerLevel = a_PowerLevel;
			}
			return;
		}
	}

	Powered.emplace_back(a_RelBlockPosition, a_RelSourcePosition, a_PowerLevel);
	Neighbour->SetIsRedstoneDirty(true);
	m_Chunk->SetIsRedstoneDirty(true);
}





void cIncrementalRedstoneSimulator::SetBlockLinkedPowered(
	int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ,
	int a_RelMiddleX, int a_RelMiddleY, int a_RelMiddleZ,
	int a_RelSourceX, int a_RelSourceY, int a_RelSourceZ,
	BLOCKTYPE a_MiddleBlock, unsigned char a_PowerLevel
	)
{
	if (!IsViableMiddleBlock(a_MiddleBlock))
	{
		return;
	}

	cChunk * Neighbour = m_Chunk->GetRelNeighborChunkAdjustCoords(a_RelBlockX, a_RelBlockZ);
	if ((Neighbour == nullptr) || !Neighbour->IsValid())
	{
		return;
	}
	a_RelMiddleX += (m_Chunk->GetPosX() - Neighbour->GetPosX()) * cChunkDef::Width;
	a_RelMiddleZ += (m_Chunk->GetPosZ() - Neighbour->GetPosZ()) * cChunkDef::Width;
	a_RelSourceX += (m_Chunk->GetPosX() - Neighbour->GetPosX()) * cChunkDef::Width;
	a_RelSourceZ += (m_Chunk->GetPosZ() - Neighbour->GetPosZ()) * cChunkDef::Width;

	auto & Linked = static_cast<cIncrementalRedstoneSimulatorChunkData *>(Neighbour->GetRedstoneSimulatorData())->m_LinkedBlocks;
	for (auto & itr : Linked)  // Check linked powered list
	{
		if (
			itr.a_BlockPos.Equals(Vector3i(a_RelBlockX, a_RelBlockY, a_RelBlockZ)) &&
			itr.a_MiddlePos.Equals(Vector3i(a_RelMiddleX, a_RelMiddleY, a_RelMiddleZ)) &&
			itr.a_SourcePos.Equals(Vector3i(a_RelSourceX, a_RelSourceY, a_RelSourceZ))
			)
		{
			if (itr.a_PowerLevel != a_PowerLevel)
			{
				// Update power level, don't add a new listing
				Neighbour->SetIsRedstoneDirty(true);
				m_Chunk->SetIsRedstoneDirty(true);
				itr.a_PowerLevel = a_PowerLevel;
			}
			return;
		}
	}

	sLinkedPoweredBlocks RC;
	RC.a_BlockPos = Vector3i(a_RelBlockX, a_RelBlockY, a_RelBlockZ);
	RC.a_MiddlePos = Vector3i(a_RelMiddleX, a_RelMiddleY, a_RelMiddleZ);
	RC.a_SourcePos = Vector3i(a_RelSourceX, a_RelSourceY, a_RelSourceZ);
	RC.a_PowerLevel = a_PowerLevel;
	Linked.emplace_back(RC);
	Neighbour->SetIsRedstoneDirty(true);
	m_Chunk->SetIsRedstoneDirty(true);
}





void cIncrementalRedstoneSimulator::SetPlayerToggleableBlockAsSimulated(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ, bool a_WasLastStatePowered)
{
	m_SimulatedPlayerToggleableBlocks->operator[](Vector3i(a_RelBlockX, a_RelBlockY, a_RelBlockZ)) = a_WasLastStatePowered;
}





void cIncrementalRedstoneSimulator::QueueRepeaterPowerChange(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ, NIBBLETYPE a_Meta, bool a_ShouldPowerOn)
{
	sRepeatersDelayList RC;

	// Gets the top two bits (delay time), shifts them into the lower two bits, and adds one (meta 0 = 1 tick; 1 = 2 etc.)
	// Multiply by 2 because in MCS, 1 redstone tick = 1 world tick, but in Vanilla, 1 redstone tick = 2 world ticks, and we need to maintain compatibility
	RC.a_DelayTicks = (((a_Meta & 0xC) >> 0x2) + 1) * 2;
	RC.a_ElapsedTicks = 0;
	RC.ShouldPowerOn = a_ShouldPowerOn;

	auto Result = m_RepeatersDelayList->emplace(Vector3i(a_RelBlockX, a_RelBlockY, a_RelBlockZ), RC);
	if (!Result.second)
	{
		// Key exists
		if (a_ShouldPowerOn == Result.first->second.ShouldPowerOn)
		{
			// We are queued already for the same thing, don't replace entry
			return;
		}

		Result.first->second.a_DelayTicks = RC.a_DelayTicks;
		Result.first->second.a_ElapsedTicks = 0;
		Result.first->second.ShouldPowerOn = a_ShouldPowerOn;
	}
}





void cIncrementalRedstoneSimulator::SetSourceUnpowered(int a_RelSourceX, int a_RelSourceY, int a_RelSourceZ, cChunk * a_Chunk)
{
	if ((a_Chunk == nullptr) || !a_Chunk->IsValid())
	{
		return;
	}
	std::vector<std::pair<Vector3i, cChunk *>> BlocksPotentiallyUnpowered = { std::make_pair(Vector3i(a_RelSourceX, a_RelSourceY, a_RelSourceZ), a_Chunk) };

	auto UnpoweringFunction = [&BlocksPotentiallyUnpowered](cChunk * a_LambdaChunk, const Vector3i & a_RelSource)
	{
		BLOCKTYPE RepeaterType;
		if (a_LambdaChunk->UnboundedRelGetBlockType(a_RelSource.x, a_RelSource.y, a_RelSource.z, RepeaterType) && (RepeaterType == E_BLOCK_REDSTONE_REPEATER_ON))
		{
			return;
		}

		auto Data = static_cast<cIncrementalRedstoneSimulatorChunkData *>(a_LambdaChunk->GetRedstoneSimulatorData());
		Data->m_PoweredBlocks.erase(std::remove_if(Data->m_PoweredBlocks.begin(), Data->m_PoweredBlocks.end(), [&BlocksPotentiallyUnpowered, a_LambdaChunk, a_RelSource](const sPoweredBlocks & itr)
			{
				if (itr.m_SourcePos != a_RelSource)
				{
					return false;
				}

				BlocksPotentiallyUnpowered.emplace_back(std::make_pair(itr.m_BlockPos, a_LambdaChunk));
				a_LambdaChunk->SetIsRedstoneDirty(true);
				return true;
			}
		), Data->m_PoweredBlocks.end());
		
		Data->m_LinkedBlocks.erase(std::remove_if(Data->m_LinkedBlocks.begin(), Data->m_LinkedBlocks.end(), [&BlocksPotentiallyUnpowered, a_LambdaChunk, a_RelSource](const sLinkedPoweredBlocks & itr)
			{
				if (itr.a_SourcePos != a_RelSource)
				{
					return false;
				}

				BlocksPotentiallyUnpowered.emplace_back(std::make_pair(itr.a_BlockPos, a_LambdaChunk));
				a_LambdaChunk->SetIsRedstoneDirty(true);
				return true;
			}
		), Data->m_LinkedBlocks.end());

		for (const auto & BoundaryChunk : GetAdjacentChunks(a_RelSource, a_LambdaChunk))
		{
			auto BoundaryData = static_cast<cIncrementalRedstoneSimulatorChunkData *>(BoundaryChunk->GetRedstoneSimulatorData());
			Vector3i ChunkAdjustedSource = a_RelSource;
			ChunkAdjustedSource.x += (a_LambdaChunk->GetPosX() - BoundaryChunk->GetPosX()) * cChunkDef::Width;
			ChunkAdjustedSource.z += (a_LambdaChunk->GetPosZ() - BoundaryChunk->GetPosZ()) * cChunkDef::Width;

			if (
				(std::find_if(BoundaryData->m_PoweredBlocks.begin(), BoundaryData->m_PoweredBlocks.end(), [ChunkAdjustedSource](const sPoweredBlocks & itr) { return (itr.m_SourcePos == ChunkAdjustedSource); }) != BoundaryData->m_PoweredBlocks.end()) ||
				(std::find_if(BoundaryData->m_LinkedBlocks.begin(), BoundaryData->m_LinkedBlocks.end(), [ChunkAdjustedSource](const sLinkedPoweredBlocks & itr) { return (itr.a_SourcePos == ChunkAdjustedSource); }) != BoundaryData->m_LinkedBlocks.end())
				)
			{
				BlocksPotentiallyUnpowered.emplace_back(std::make_pair(ChunkAdjustedSource, BoundaryChunk));
			}
		}
	};

	while (!BlocksPotentiallyUnpowered.empty())
	{
		auto End = BlocksPotentiallyUnpowered.back();
		BlocksPotentiallyUnpowered.pop_back();
		UnpoweringFunction(End.second, End.first);
	}
}





void cIncrementalRedstoneSimulator::SetInvalidMiddleBlock(int a_RelMiddleX, int a_RelMiddleY, int a_RelMiddleZ, cChunk * a_Chunk)
{
	std::vector<std::pair<Vector3i, cChunk *>> BlocksPotentiallyUnpowered;

	BLOCKTYPE RepeaterType;
	if (a_Chunk->UnboundedRelGetBlockType(a_RelMiddleX, a_RelMiddleY, a_RelMiddleZ, RepeaterType) && (RepeaterType == E_BLOCK_REDSTONE_REPEATER_ON))
	{
		return;
	}

	auto MiddleBlockUnpoweringFunction = [&BlocksPotentiallyUnpowered](cChunk * a_LambdaChunk, const Vector3i & a_RelMiddle)
	{
		auto Data = static_cast<cIncrementalRedstoneSimulatorChunkData *>(a_LambdaChunk->GetRedstoneSimulatorData());
		Data->m_LinkedBlocks.erase(std::remove_if(Data->m_LinkedBlocks.begin(), Data->m_LinkedBlocks.end(), [&BlocksPotentiallyUnpowered, a_LambdaChunk, a_RelMiddle](const sLinkedPoweredBlocks & itr)
			{
				if (itr.a_MiddlePos != a_RelMiddle)
				{
					return false;
				}

				BlocksPotentiallyUnpowered.emplace_back(std::make_pair(itr.a_BlockPos, a_LambdaChunk));
				a_LambdaChunk->SetIsRedstoneDirty(true);
				return true;
			}
		), Data->m_LinkedBlocks.end());
	};

	MiddleBlockUnpoweringFunction(a_Chunk, Vector3i(a_RelMiddleX, a_RelMiddleY, a_RelMiddleZ));
	for (const auto & BoundaryChunk : GetAdjacentChunks(Vector3i(a_RelMiddleX, a_RelMiddleY, a_RelMiddleZ), a_Chunk))
	{
		Vector3i ChunkAdjustedMiddlePos = Vector3i(a_RelMiddleX, a_RelMiddleY, a_RelMiddleZ);
		ChunkAdjustedMiddlePos.x += (a_Chunk->GetPosX() - BoundaryChunk->GetPosX()) * cChunkDef::Width;
		ChunkAdjustedMiddlePos.z += (a_Chunk->GetPosZ() - BoundaryChunk->GetPosZ()) * cChunkDef::Width;

		MiddleBlockUnpoweringFunction(a_Chunk, Vector3i(a_RelMiddleX, a_RelMiddleY, a_RelMiddleZ));
	}

	for (const auto & itr : BlocksPotentiallyUnpowered)
	{
		if (!AreCoordsPowered(itr.first.x, itr.first.y, itr.first.z))
		{
			SetSourceUnpowered(itr.first.x, itr.first.y, itr.first.z, itr.second);
		}
	}
}





bool cIncrementalRedstoneSimulator::IsLeverOn(NIBBLETYPE a_BlockMeta)
{
	// Extract the ON bit from metadata and return if true if it is set:
	return ((a_BlockMeta & 0x8) == 0x8);
}





std::vector<cChunk *> cIncrementalRedstoneSimulator::GetAdjacentChunks(const Vector3i & a_RelBlockPosition, cChunk * a_Chunk)
{
	std::vector<cChunk *> AdjacentChunks;
	AdjacentChunks.reserve(2);  // At most bordering two chunks; reserve that many

	auto CheckAndEmplace = [&AdjacentChunks](cChunk * a_LambdaChunk)
	{
		if ((a_LambdaChunk != nullptr) && a_LambdaChunk->IsValid())
		{
			AdjacentChunks.emplace_back(a_LambdaChunk);
		}
	};

	// Are we on a chunk boundary? +- 2 because of LinkedPowered blocks
	if (a_RelBlockPosition.x <= 1)
	{
		CheckAndEmplace(a_Chunk->GetRelNeighborChunk(a_RelBlockPosition.x - 2, a_RelBlockPosition.z));
	}
	if (a_RelBlockPosition.x >= 14)
	{
		CheckAndEmplace(a_Chunk->GetRelNeighborChunk(a_RelBlockPosition.x + 2, a_RelBlockPosition.z));
	}
	if (a_RelBlockPosition.z <= 1)
	{
		CheckAndEmplace(a_Chunk->GetRelNeighborChunk(a_RelBlockPosition.x, a_RelBlockPosition.z - 2));
	}
	if (a_RelBlockPosition.z >= 14)
	{
		CheckAndEmplace(a_Chunk->GetRelNeighborChunk(a_RelBlockPosition.x, a_RelBlockPosition.z + 2));
	}

	return AdjacentChunks;
}




