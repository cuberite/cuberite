
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "IncrementalRedstoneSimulator.h"
#include "../BlockEntities/DropSpenserEntity.h"
#include "../BlockEntities/NoteEntity.h"
#include "../BlockEntities/CommandBlockEntity.h"
#include "../Entities/TNTEntity.h"
#include "../Entities/Pickup.h"
#include "../Blocks/BlockTorch.h"
#include "../Blocks/BlockDoor.h"
#include "../Piston.h"



	

cIncrementalRedstoneSimulator::cIncrementalRedstoneSimulator(cWorld & a_World)
	: super(a_World)
{
}





cIncrementalRedstoneSimulator::~cIncrementalRedstoneSimulator()
{
}





void cIncrementalRedstoneSimulator::RedstoneAddBlock(int a_BlockX, int a_BlockY, int a_BlockZ, cChunk * a_Chunk, cChunk * a_OtherChunk)
{
	if ((a_Chunk == NULL) || !a_Chunk->IsValid())
	{
		return;
	}
	else if ((a_BlockY < 0) || (a_BlockY > cChunkDef::Height))
	{
		return;
	}

	// We may be called with coordinates in a chunk that is not the first chunk parameter
	// In that case, the actual chunk (which the coordinates are in), will be passed as the second parameter
	// Use that Chunk pointer to get a relative position

	int RelX = 0;
	int RelZ = 0;	
	BLOCKTYPE Block;
	NIBBLETYPE Meta;

	if (a_OtherChunk != NULL)
	{
		RelX = a_BlockX - a_OtherChunk->GetPosX() * cChunkDef::Width;
		RelZ = a_BlockZ - a_OtherChunk->GetPosZ() * cChunkDef::Width;
		a_OtherChunk->GetBlockTypeMeta(RelX, a_BlockY, RelZ, Block, Meta);
	}
	else
	{
		RelX = a_BlockX - a_Chunk->GetPosX() * cChunkDef::Width;
		RelZ = a_BlockZ - a_Chunk->GetPosZ() * cChunkDef::Width;
		a_Chunk->GetBlockTypeMeta(RelX, a_BlockY, RelZ, Block, Meta);
	}

	// Every time a block is changed (AddBlock called), we want to go through all lists and check to see if the coordiantes stored within are still valid
	// Checking only when a block is changed, as opposed to every tick, also improves performance

	PoweredBlocksList * PoweredBlocks = a_Chunk->GetRedstoneSimulatorPoweredBlocksList();
	for (PoweredBlocksList::iterator itr = PoweredBlocks->begin(); itr != PoweredBlocks->end();)
	{
		if (!itr->a_SourcePos.Equals(Vector3i(a_BlockX, a_BlockY, a_BlockZ)))
		{
			++itr;
			continue;
		}

		if (!IsPotentialSource(Block))
		{
			LOGD("cIncrementalRedstoneSimulator: Erased block @ {%i, %i, %i} from powered blocks list as it no longer connected to a source", itr->a_BlockPos.x, itr->a_BlockPos.y, itr->a_BlockPos.z);
			itr = PoweredBlocks->erase(itr);
			continue;
		}
		else if (
			// Changeable sources
			((Block == E_BLOCK_REDSTONE_WIRE) && (Meta == 0)) ||
			((Block == E_BLOCK_LEVER) && !IsLeverOn(Meta)) ||
			((Block == E_BLOCK_DETECTOR_RAIL) && (Meta & 0x08) == 0) ||
			(((Block == E_BLOCK_STONE_BUTTON) || (Block == E_BLOCK_WOODEN_BUTTON)) && (!IsButtonOn(Meta))) ||
			(((Block == E_BLOCK_STONE_PRESSURE_PLATE) || (Block == E_BLOCK_WOODEN_PRESSURE_PLATE)) && (Meta == 0)) ||
			(((Block == E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE) || (Block == E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE)) && (Meta == 0))
			)
		{
			LOGD("cIncrementalRedstoneSimulator: Erased block @ {%i, %i, %i} from powered blocks list due to present/past metadata mismatch", itr->a_BlockPos.x, itr->a_BlockPos.y, itr->a_BlockPos.z);
			itr = PoweredBlocks->erase(itr);
			continue;
		}
		++itr;
	}

	LinkedBlocksList * LinkedPoweredBlocks = a_Chunk->GetRedstoneSimulatorLinkedBlocksList();
	// We loop through all values (insteading of breaking out at the first) to make sure everything is gone, as there can be multiple SourceBlock entries for one AddBlock coordinate
	for (LinkedBlocksList::iterator itr = LinkedPoweredBlocks->begin(); itr != LinkedPoweredBlocks->end();)
	{
		if (itr->a_SourcePos.Equals(Vector3i(a_BlockX, a_BlockY, a_BlockZ)))
		{
			if (!IsPotentialSource(Block))
			{
				LOGD("cIncrementalRedstoneSimulator: Erased block @ {%i, %i, %i} from linked powered blocks list as it is no longer connected to a source", itr->a_BlockPos.x, itr->a_BlockPos.y, itr->a_BlockPos.z);
				itr = LinkedPoweredBlocks->erase(itr);
				continue;
			}
			else if (
				// Things that can send power through a block but which depends on meta
				((Block == E_BLOCK_REDSTONE_WIRE) && (Meta == 0)) ||
				((Block == E_BLOCK_LEVER) && !IsLeverOn(Meta)) ||
				(((Block == E_BLOCK_STONE_BUTTON) || (Block == E_BLOCK_WOODEN_BUTTON)) && (!IsButtonOn(Meta))) ||
				(((Block == E_BLOCK_STONE_PRESSURE_PLATE) || (Block == E_BLOCK_WOODEN_PRESSURE_PLATE)) && (Meta == 0)) ||
				(((Block == E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE) || (Block == E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE)) && (Meta == 0))
				)
			{
				LOGD("cIncrementalRedstoneSimulator: Erased block @ {%i, %i, %i} from linked powered blocks list due to present/past metadata mismatch", itr->a_BlockPos.x, itr->a_BlockPos.y, itr->a_BlockPos.z);
				itr = LinkedPoweredBlocks->erase(itr);
				continue;
			}
		}
		else if (itr->a_MiddlePos.Equals(Vector3i(a_BlockX, a_BlockY, a_BlockZ)))
		{
			if (!IsViableMiddleBlock(Block))
			{
				LOGD("cIncrementalRedstoneSimulator: Erased block @ {%i, %i, %i} from linked powered blocks list as it is no longer powered through a valid middle block", itr->a_BlockPos.x, itr->a_BlockPos.y, itr->a_BlockPos.z);
				itr = LinkedPoweredBlocks->erase(itr);
				continue;
			}
		}
		++itr;
	}

	SimulatedPlayerToggleableList * SimulatedPlayerToggleableBlocks = a_Chunk->GetRedstoneSimulatorSimulatedPlayerToggleableList();
	for (SimulatedPlayerToggleableList::iterator itr = SimulatedPlayerToggleableBlocks->begin(); itr != SimulatedPlayerToggleableBlocks->end(); ++itr)
	{
		if (!itr->a_BlockPos.Equals(Vector3i(a_BlockX, a_BlockY, a_BlockZ)))
		{
			continue;
		}

		if (!IsAllowedBlock(Block))
		{
			LOGD("cIncrementalRedstoneSimulator: Erased block @ {%i, %i, %i} from toggleable simulated list as it is no longer redstone", itr->a_BlockPos.x, itr->a_BlockPos.y, itr->a_BlockPos.z);
			SimulatedPlayerToggleableBlocks->erase(itr);
			break;
		}
	}

	RepeatersDelayList * RepeatersDelayList = a_Chunk->GetRedstoneSimulatorRepeatersDelayList();
	for (RepeatersDelayList::iterator itr = RepeatersDelayList->begin(); itr != RepeatersDelayList->end(); ++itr)
	{
		if (!itr->a_BlockPos.Equals(Vector3i(a_BlockX, a_BlockY, a_BlockZ)))
		{
			continue;
		}

		if ((Block != E_BLOCK_REDSTONE_REPEATER_ON) && (Block != E_BLOCK_REDSTONE_REPEATER_OFF))
		{
			RepeatersDelayList->erase(itr);
			break;
		}
	}

	if (a_OtherChunk != NULL)
	{
		// DO NOT touch our chunk's data structure if we are being called with coordinates from another chunk - this one caused me massive grief :P
		return;
	}

	cRedstoneSimulatorChunkData * RedstoneSimulatorChunkData = a_Chunk->GetRedstoneSimulatorData();
	for (cRedstoneSimulatorChunkData::iterator itr = RedstoneSimulatorChunkData->begin(); itr != RedstoneSimulatorChunkData->end(); ++itr)
	{
		if ((itr->x == RelX) && (itr->y == a_BlockY) && (itr->z == RelZ)) // We are at an entry matching the current (changed) block
		{
			if (!IsAllowedBlock(Block))
			{
				itr->DataTwo = true; // The new blocktype is not redstone; it must be queued to be removed from this list
			}
			else
			{
				itr->Data = Block; // Update block information
			}
			return;
		}
	}

	if (!IsAllowedBlock(Block))
	{
		return;
	}

	RedstoneSimulatorChunkData->push_back(cCoordWithBlockAndBool(RelX, a_BlockY, RelZ, Block, false));
}





void cIncrementalRedstoneSimulator::SimulateChunk(float a_Dt, int a_ChunkX, int a_ChunkZ, cChunk * a_Chunk)
{
	// We still attempt to simulate all blocks in the chunk every tick, because of outside influence that needs to be taken into account
	// For example, repeaters need to be ticked, pressure plates checked for entities, daylight sensor checked for light changes, etc.
	// The easiest way to make this more efficient is probably just to reduce code within the handlers that put too much strain on server, like getting or setting blocks
	// A marking dirty system might be a TODO for later on, perhaps

	m_RedstoneSimulatorChunkData = a_Chunk->GetRedstoneSimulatorData();
	if (m_RedstoneSimulatorChunkData->empty())
	{
		return;
	}

	m_PoweredBlocks = a_Chunk->GetRedstoneSimulatorPoweredBlocksList();
	m_RepeatersDelayList = a_Chunk->GetRedstoneSimulatorRepeatersDelayList();
	m_SimulatedPlayerToggleableBlocks = a_Chunk->GetRedstoneSimulatorSimulatedPlayerToggleableList();
	m_LinkedPoweredBlocks = a_Chunk->GetRedstoneSimulatorLinkedBlocksList();
	m_Chunk = a_Chunk;

	int BaseX = a_Chunk->GetPosX() * cChunkDef::Width;
	int BaseZ = a_Chunk->GetPosZ() * cChunkDef::Width;

	for (cRedstoneSimulatorChunkData::iterator dataitr = m_RedstoneSimulatorChunkData->begin(); dataitr != m_RedstoneSimulatorChunkData->end();)
	{
		if (dataitr->DataTwo)
		{
			dataitr = m_RedstoneSimulatorChunkData->erase(dataitr);
			continue;
		}

		int a_X = BaseX + dataitr->x;
		int a_Z = BaseZ + dataitr->z;
		switch (dataitr->Data)
		{
			case E_BLOCK_BLOCK_OF_REDSTONE:     HandleRedstoneBlock(a_X, dataitr->y, a_Z);	break;
			case E_BLOCK_LEVER:                 HandleRedstoneLever(a_X, dataitr->y, a_Z);	break;
			case E_BLOCK_FENCE_GATE:            HandleFenceGate(a_X, dataitr->y, a_Z);      break;
			case E_BLOCK_TNT:                   HandleTNT(a_X, dataitr->y, a_Z);            break;
			case E_BLOCK_TRAPDOOR:              HandleTrapdoor(a_X, dataitr->y, a_Z);       break;
			case E_BLOCK_REDSTONE_WIRE:         HandleRedstoneWire(a_X, dataitr->y, a_Z);	break;
			case E_BLOCK_NOTE_BLOCK:            HandleNoteBlock(a_X, dataitr->y, a_Z);      break;
			case E_BLOCK_DAYLIGHT_SENSOR:       HandleDaylightSensor(a_X, dataitr->y, a_Z); break;
			case E_BLOCK_COMMAND_BLOCK:         HandleCommandBlock(a_X, dataitr->y, a_Z);   break;

			case E_BLOCK_REDSTONE_TORCH_OFF:
			case E_BLOCK_REDSTONE_TORCH_ON:
			{
				HandleRedstoneTorch(a_X, dataitr->y, a_Z, dataitr->Data);
				break;
			}
			case E_BLOCK_STONE_BUTTON:
			case E_BLOCK_WOODEN_BUTTON:
			{
				HandleRedstoneButton(a_X, dataitr->y, a_Z, dataitr->Data);
				break;
			}
			case E_BLOCK_REDSTONE_REPEATER_OFF:
			case E_BLOCK_REDSTONE_REPEATER_ON:
			{
				HandleRedstoneRepeater(a_X, dataitr->y, a_Z, dataitr->Data);
				break;
			}
			case E_BLOCK_PISTON:
			case E_BLOCK_STICKY_PISTON:
			{
				HandlePiston(a_X, dataitr->y, a_Z);
				break;
			}
			case E_BLOCK_REDSTONE_LAMP_OFF:
			case E_BLOCK_REDSTONE_LAMP_ON:
			{
				HandleRedstoneLamp(a_X, dataitr->y, a_Z, dataitr->Data);
				break;
			}
			case E_BLOCK_DISPENSER:
			case E_BLOCK_DROPPER:
			{
				HandleDropSpenser(a_X, dataitr->y, a_Z);
				break;
			}
			case E_BLOCK_WOODEN_DOOR:
			case E_BLOCK_IRON_DOOR:
			{
				HandleDoor(a_X, dataitr->y, a_Z);
				break;
			}
			case E_BLOCK_ACTIVATOR_RAIL:
			case E_BLOCK_DETECTOR_RAIL:
			case E_BLOCK_POWERED_RAIL:
			{
				HandleRail(a_X, dataitr->y, a_Z, dataitr->Data);
				break;
			}
			case E_BLOCK_WOODEN_PRESSURE_PLATE:
			case E_BLOCK_STONE_PRESSURE_PLATE:
			case E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE:
			case E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE:
			{
				HandlePressurePlate(a_X, dataitr->y, a_Z, dataitr->Data);
				break;
			}
			default: LOGD("Unhandled block (!) or unimplemented redstone block: %s", ItemToString(dataitr->Data).c_str()); break;
		}
		++dataitr;
	}
}




void cIncrementalRedstoneSimulator::WakeUp(int a_BlockX, int a_BlockY, int a_BlockZ, cChunk * a_Chunk)
{
	if (
		((a_BlockX % cChunkDef::Width) <= 1) ||
		((a_BlockX % cChunkDef::Width) >= 14) ||
		((a_BlockZ % cChunkDef::Width) <= 1) ||
		((a_BlockZ % cChunkDef::Width) >= 14)
		) // Are we on a chunk boundary? +- 2 because of LinkedPowered blocks
	{
		// On a chunk boundary, alert all four sides (i.e. at least one neighbouring chunk)
		AddBlock(a_BlockX, a_BlockY, a_BlockZ, a_Chunk);

		// Pass the original coordinates, because when adding things to our simulator lists, we get the chunk that they are in, and therefore any updates need to preseve their position
		// RedstoneAddBlock to pass both the neighbouring chunk and the chunk which the coordiantes are in and +- 2 in GetNeighbour() to accomodate for LinkedPowered blocks being 2 away from chunk boundaries
		RedstoneAddBlock(a_BlockX, a_BlockY, a_BlockZ, a_Chunk->GetNeighborChunk(a_BlockX - 2, a_BlockZ), a_Chunk);
		RedstoneAddBlock(a_BlockX, a_BlockY, a_BlockZ, a_Chunk->GetNeighborChunk(a_BlockX + 2, a_BlockZ), a_Chunk);
		RedstoneAddBlock(a_BlockX, a_BlockY, a_BlockZ, a_Chunk->GetNeighborChunk(a_BlockX, a_BlockZ - 2), a_Chunk);
		RedstoneAddBlock(a_BlockX, a_BlockY, a_BlockZ, a_Chunk->GetNeighborChunk(a_BlockX, a_BlockZ + 2), a_Chunk);

		return;
	}

	// Not on boundary, just alert this chunk for speed
	AddBlock(a_BlockX, a_BlockY, a_BlockZ, a_Chunk);
}





void cIncrementalRedstoneSimulator::HandleRedstoneTorch(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_MyState)
{
	static const struct // Define which directions the torch can power
	{
		int x, y, z;
	} gCrossCoords[] =
	{
		{ 1, 0,  0},
		{-1, 0,  0},
		{ 0, 0,  1},
		{ 0, 0, -1},
		{ 0, 1,  0},
	} ;

	if (a_MyState == E_BLOCK_REDSTONE_TORCH_ON)
	{
		// Check if the block the torch is on is powered		
		int X = a_BlockX; int Y = a_BlockY; int Z = a_BlockZ;
		AddFaceDirection(X, Y, Z, cBlockTorchHandler::MetaDataToDirection(m_World.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ)), true); // Inverse true to get the block torch is on

		if (AreCoordsDirectlyPowered(X, Y, Z))
		{
			// There was a match, torch goes off
			m_World.SetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_REDSTONE_TORCH_OFF, m_World.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ));
			return;
		}

		// Torch still on, make all 4(X, Z) + 1(Y) sides powered
		for (size_t i = 0; i < ARRAYCOUNT(gCrossCoords); i++)
		{
			BLOCKTYPE Type = m_World.GetBlock(a_BlockX + gCrossCoords[i].x, a_BlockY + gCrossCoords[i].y, a_BlockZ + gCrossCoords[i].z);
			if (i + 1 < ARRAYCOUNT(gCrossCoords)) // Sides of torch, not top (top is last)
			{
				if (
					((IsMechanism(Type)) || (Type == E_BLOCK_REDSTONE_WIRE)) && // Is it a mechanism or wire? Not block/other torch etc.
					(!Vector3i(a_BlockX + gCrossCoords[i].x, a_BlockY + gCrossCoords[i].y, a_BlockZ + gCrossCoords[i].z).Equals(Vector3i(X, Y, Z))) // CAN'T power block is that it is on
					)
				{
					SetBlockPowered(a_BlockX + gCrossCoords[i].x, a_BlockY + gCrossCoords[i].y, a_BlockZ + gCrossCoords[i].z, a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_REDSTONE_TORCH_ON);
				}
			}
			else
			{
				// Top side, power whatever is there, including blocks
				SetBlockPowered(a_BlockX + gCrossCoords[i].x, a_BlockY + gCrossCoords[i].y, a_BlockZ + gCrossCoords[i].z, a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_REDSTONE_TORCH_ON);
				// Power all blocks surrounding block above torch
				SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_YP, E_BLOCK_REDSTONE_TORCH_ON);
			}
		}

		if (m_World.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ) != 0x5) // Is torch standing on ground? If NOT (i.e. on wall), power block beneath
		{
			BLOCKTYPE Type = m_World.GetBlock(a_BlockX, a_BlockY - 1, a_BlockZ);

			if ((IsMechanism(Type)) || (Type == E_BLOCK_REDSTONE_WIRE)) // Still can't make a normal block powered though!
			{
				SetBlockPowered(a_BlockX, a_BlockY - 1, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_REDSTONE_TORCH_ON);
			}
		}
	}
	else
	{
		// Check if the block the torch is on is powered		
		int X = a_BlockX; int Y = a_BlockY; int Z = a_BlockZ;
		AddFaceDirection(X, Y, Z, cBlockTorchHandler::MetaDataToDirection(m_World.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ)), true); // Inverse true to get the block torch is on

		// See if off state torch can be turned on again
		if (AreCoordsDirectlyPowered(X, Y, Z))
		{
			return; // Something matches, torch still powered
		}

		// Block torch on not powered, can be turned on again!
		m_World.SetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_REDSTONE_TORCH_ON, m_World.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ));
	}
}





void cIncrementalRedstoneSimulator::HandleRedstoneBlock(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	SetAllDirsAsPowered(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_BLOCK_OF_REDSTONE);
	SetBlockPowered(a_BlockX, a_BlockY, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_BLOCK_OF_REDSTONE); // Set self as powered
}





void cIncrementalRedstoneSimulator::HandleRedstoneLever(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	if (IsLeverOn(m_World.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ)))
	{
		SetAllDirsAsPowered(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_LEVER);

		SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_XM, E_BLOCK_LEVER);
		SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_XP, E_BLOCK_LEVER);
		SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_YM, E_BLOCK_LEVER);
		SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_YP, E_BLOCK_LEVER);
		SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_ZM, E_BLOCK_LEVER);
		SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_ZP, E_BLOCK_LEVER);
	}
}





void cIncrementalRedstoneSimulator::HandleFenceGate(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	cChunkInterface ChunkInterface(m_World.GetChunkMap());
	NIBBLETYPE MetaData = ChunkInterface.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);

	if (AreCoordsPowered(a_BlockX, a_BlockY, a_BlockZ))
	{
		if (!AreCoordsSimulated(a_BlockX, a_BlockY, a_BlockZ, true))
		{
			m_World.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, MetaData | 0x4);
			m_World.BroadcastSoundParticleEffect(1003, a_BlockX, a_BlockY, a_BlockZ, 0);
			SetPlayerToggleableBlockAsSimulated(a_BlockX, a_BlockY, a_BlockZ, true);
		}
	}
	else
	{
		if (!AreCoordsSimulated(a_BlockX, a_BlockY, a_BlockZ, false))
		{
			m_World.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, MetaData & 0xFFFFFFFB);
			m_World.BroadcastSoundParticleEffect(1003, a_BlockX, a_BlockY, a_BlockZ, 0);
			SetPlayerToggleableBlockAsSimulated(a_BlockX, a_BlockY, a_BlockZ, false);
		}
	}
}





void cIncrementalRedstoneSimulator::HandleRedstoneButton(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType)
{
	if (IsButtonOn(m_World.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ)))
	{
		SetAllDirsAsPowered(a_BlockX, a_BlockY, a_BlockZ, a_BlockType);

		SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_XM, a_BlockType);
		SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_XP, a_BlockType);
		SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_YM, a_BlockType);
		SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_YP, a_BlockType);
		SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_ZM, a_BlockType);
		SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_ZP, a_BlockType);
	}
}





void cIncrementalRedstoneSimulator::HandleRedstoneWire(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	static const struct // Define which directions the wire can receive power from
	{
		int x, y, z;
	} gCrossCoords[] =
	{
		{ 1, 0,  0}, /* Wires on same level start */
		{-1, 0,  0},
		{ 0, 0,  1},
		{ 0, 0, -1}, /* Wires on same level stop */
		{ 1, 1,  0}, /* Wires one higher, surrounding self start */
		{-1, 1,  0},
		{ 0, 1,  1},
		{ 0, 1, -1}, /* Wires one higher, surrounding self stop */
		{ 1,-1,  0}, /* Wires one lower, surrounding self start */
		{-1,-1,  0},
		{ 0,-1,  1},
		{ 0,-1, -1}, /* Wires one lower, surrounding self stop */
	} ;

	static const struct // Define which directions the wire will check for repeater prescence
	{
		int x, y, z;
	} gSideCoords[] =
	{
		{ 1, 0, 0 },
		{-1, 0, 0 },
		{ 0, 0, 1 },
		{ 0, 0,-1 },
		{ 0, 1, 0 },
	};

	// Check to see if directly beside a power source
	unsigned char MyPower;
	if (!IsWirePowered(a_BlockX, a_BlockY, a_BlockZ, MyPower))
	{
		m_World.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, 0);
		m_World.WakeUpSimulators(a_BlockX, a_BlockY, a_BlockZ);
		return;
	}
	
	m_World.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, MyPower);

	if (MyPower < 1)
	{
		return;
	}

	MyPower--;

	for (size_t i = 0; i < ARRAYCOUNT(gCrossCoords); i++) // Loop through all directions to transfer or receive power
	{
		if ((i >= 4) && (i <= 7)) // If we are currently checking for wire surrounding ourself one block above...
		{
			if (cBlockInfo::IsSolid(m_World.GetBlock(a_BlockX, a_BlockY + 1, a_BlockZ))) // If there is something solid above us (wire cut off)...
			{
				continue; // We don't receive power from that wire
			}
		}
		else if ((i >= 8) && (i <= 11)) // See above, but this is for wire below us
		{
			if (cBlockInfo::IsSolid(m_World.GetBlock(a_BlockX, a_BlockY - 1, a_BlockZ)))
			{
				continue;
			}
		}

		if (m_World.GetBlock(a_BlockX + gCrossCoords[i].x, a_BlockY + gCrossCoords[i].y, a_BlockZ + gCrossCoords[i].z) == E_BLOCK_REDSTONE_WIRE)
		{
			SetBlockPowered(a_BlockX + gCrossCoords[i].x, a_BlockY + gCrossCoords[i].y, a_BlockZ + gCrossCoords[i].z, a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_REDSTONE_WIRE, MyPower);
		}
	}

	for (size_t i = 0; i < ARRAYCOUNT(gSideCoords); i++) // Look for repeaters immediately surrounding self and try to power them
	{
		if (m_World.GetBlock(a_BlockX + gSideCoords[i].x, a_BlockY + gSideCoords[i].y, a_BlockZ + gSideCoords[i].z) == E_BLOCK_REDSTONE_REPEATER_OFF)
		{
			SetBlockPowered(a_BlockX + gSideCoords[i].x, a_BlockY + gSideCoords[i].y, a_BlockZ + gSideCoords[i].z, a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_REDSTONE_WIRE, MyPower);
		}
	}

	// Wire still powered, power blocks beneath
	SetBlockPowered(a_BlockX, a_BlockY - 1, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_REDSTONE_WIRE, MyPower);
	SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_YM, E_BLOCK_REDSTONE_WIRE, MyPower);

	switch (GetWireDirection(a_BlockX, a_BlockY, a_BlockZ))
	{
		case REDSTONE_NONE:
		{
			SetBlockPowered(a_BlockX + 1, a_BlockY, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_REDSTONE_WIRE, MyPower);
			SetBlockPowered(a_BlockX - 1, a_BlockY, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_REDSTONE_WIRE, MyPower);
			SetBlockPowered(a_BlockX, a_BlockY, a_BlockZ + 1, a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_REDSTONE_WIRE, MyPower);
			SetBlockPowered(a_BlockX, a_BlockY, a_BlockZ - 1, a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_REDSTONE_WIRE, MyPower);

			SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_XM, E_BLOCK_REDSTONE_WIRE, MyPower);
			SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_XP, E_BLOCK_REDSTONE_WIRE, MyPower);
			SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_ZM, E_BLOCK_REDSTONE_WIRE, MyPower);
			SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_ZP, E_BLOCK_REDSTONE_WIRE, MyPower);
			break;
		}
		case REDSTONE_X_POS:
		{
			SetBlockPowered(a_BlockX + 1, a_BlockY, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_REDSTONE_WIRE, MyPower);
			SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_XP, E_BLOCK_REDSTONE_WIRE, MyPower);
			break;
		}
		case REDSTONE_X_NEG:
		{
			SetBlockPowered(a_BlockX - 1, a_BlockY, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_REDSTONE_WIRE, MyPower);
			SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_XM, E_BLOCK_REDSTONE_WIRE, MyPower);
			break;
		}
		case REDSTONE_Z_POS:
		{
			SetBlockPowered(a_BlockX, a_BlockY, a_BlockZ + 1, a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_REDSTONE_WIRE, MyPower);
			SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_ZP, E_BLOCK_REDSTONE_WIRE, MyPower);
			break;
		}
		case REDSTONE_Z_NEG:
		{
			SetBlockPowered(a_BlockX, a_BlockY, a_BlockZ - 1, a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_REDSTONE_WIRE, MyPower);
			SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_ZM, E_BLOCK_REDSTONE_WIRE, MyPower);
			break;
		}
	}
}





void cIncrementalRedstoneSimulator::HandleRedstoneRepeater(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_MyState)
{
	/* Repeater Orientation Mini Guide:
	===================================

	|
	| Z Axis
	V

	X Axis ---->

	Repeater directions, values from a cWorld::GetBlockMeta(a_BlockX , a_BlockY, a_BlockZ) lookup:

	East (Right) (X+): 0x1
	West (Left) (X-): 0x3
	North (Up) (Z-): 0x2
	South (Down) (Z+): 0x0
	// TODO: Add E_META_XXX enum entries for all meta values and update project with them

	Sun rises from East (X+)

	*/

	// Create a variable holding my meta to avoid multiple lookups.
	NIBBLETYPE a_Meta = m_World.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);
	bool IsOn = (a_MyState == E_BLOCK_REDSTONE_REPEATER_ON);
    	
	if (!IsRepeaterLocked(a_BlockX, a_BlockY, a_BlockZ, a_Meta)) // If we're locked, change nothing. Otherwise:
	{
		bool IsSelfPowered = IsRepeaterPowered(a_BlockX, a_BlockY, a_BlockZ, a_Meta);
		if (IsSelfPowered && !IsOn) // Queue a power change if powered, but not on and not locked.
		{
			QueueRepeaterPowerChange(a_BlockX, a_BlockY, a_BlockZ, a_Meta, true);
		}
		else if (!IsSelfPowered && IsOn) // Queue a power change if unpowered, on, and not locked.
		{
			QueueRepeaterPowerChange(a_BlockX, a_BlockY, a_BlockZ, a_Meta, false);
		}
	}

	for (RepeatersDelayList::iterator itr = m_RepeatersDelayList->begin(); itr != m_RepeatersDelayList->end(); ++itr)
	{
		if (!itr->a_BlockPos.Equals(Vector3i(a_BlockX, a_BlockY, a_BlockZ)))
		{
			continue;
		}

		if (itr->a_ElapsedTicks >= itr->a_DelayTicks) // Has the elapsed ticks reached the target ticks?
		{
			if (itr->ShouldPowerOn)
			{
				if (!IsOn)
				{
					m_World.SetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_REDSTONE_REPEATER_ON, a_Meta); // For performance
				}

				switch (a_Meta & 0x3) // We only want the direction (bottom) bits
				{
					case 0x0:
					{
						SetBlockPowered(a_BlockX, a_BlockY, a_BlockZ - 1, a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_REDSTONE_REPEATER_ON);
						SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_ZM, E_BLOCK_REDSTONE_REPEATER_ON);
						break;
					}
					case 0x1:
					{
						SetBlockPowered(a_BlockX + 1, a_BlockY, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_REDSTONE_REPEATER_ON);
						SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_XP, E_BLOCK_REDSTONE_REPEATER_ON);
						break;
					}
					case 0x2:
					{
						SetBlockPowered(a_BlockX, a_BlockY, a_BlockZ + 1, a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_REDSTONE_REPEATER_ON);
						SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_ZP, E_BLOCK_REDSTONE_REPEATER_ON);
						break;
					}
					case 0x3:
					{
						SetBlockPowered(a_BlockX - 1, a_BlockY, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_REDSTONE_REPEATER_ON);
						SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_XM, E_BLOCK_REDSTONE_REPEATER_ON);
						break;
					}
				}

				// Removal of the data entry will be handled in SimChunk - we still want to continue trying to power blocks, even if our delay time has reached
				// Otherwise, the power state of blocks in front won't update after we have powered on
				return;
			}
			else
			{
				if (IsOn)
				{
					m_World.SetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_REDSTONE_REPEATER_OFF, a_Meta);
				}
				m_RepeatersDelayList->erase(itr); // We can remove off repeaters which don't need further updating
				return;
			}
		}
		else
		{
			// Apparently, incrementing ticks only works reliably here, and not in SimChunk;
			// With a world with lots of redstone, the repeaters simply do not delay
			// I am confounded to say why. Perhaps optimisation failure.
			LOGD("Incremented a repeater @ {%i %i %i} | Elapsed ticks: %i | Target delay: %i", itr->a_BlockPos.x, itr->a_BlockPos.y, itr->a_BlockPos.z, itr->a_ElapsedTicks, itr->a_DelayTicks);
			itr->a_ElapsedTicks++;
		}
	}
}





void cIncrementalRedstoneSimulator::HandlePiston(int a_BlockX, int a_BlockY, int a_BlockZ)
{	
	cPiston Piston(&m_World);
	if (IsPistonPowered(a_BlockX, a_BlockY, a_BlockZ, m_World.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ) & 0x7)) // We only want the bottom three bits (4th controls extended-ness)
	{
		Piston.ExtendPiston(a_BlockX, a_BlockY, a_BlockZ);
	}
	else
	{
		Piston.RetractPiston(a_BlockX, a_BlockY, a_BlockZ);
	}
}





void cIncrementalRedstoneSimulator::HandleDropSpenser(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	class cSetPowerToDropSpenser :
		public cDropSpenserCallback
	{
		bool m_IsPowered;
	public:
		cSetPowerToDropSpenser(bool a_IsPowered) : m_IsPowered(a_IsPowered) {}
				
		virtual bool Item(cDropSpenserEntity * a_DropSpenser) override
		{
			a_DropSpenser->SetRedstonePower(m_IsPowered);
			return false;
		}
	} DrSpSP (AreCoordsPowered(a_BlockX, a_BlockY, a_BlockZ));

	m_World.DoWithDropSpenserAt(a_BlockX, a_BlockY, a_BlockZ, DrSpSP);
}





void cIncrementalRedstoneSimulator::HandleRedstoneLamp(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_MyState)
{
	if (a_MyState == E_BLOCK_REDSTONE_LAMP_OFF)
	{
		if (AreCoordsPowered(a_BlockX, a_BlockY, a_BlockZ))
		{
			m_World.SetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_REDSTONE_LAMP_ON, 0);
		}
	}
	else
	{
		if (!AreCoordsPowered(a_BlockX, a_BlockY, a_BlockZ))
		{
			m_World.SetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_REDSTONE_LAMP_OFF, 0);
		}
	}
}





void cIncrementalRedstoneSimulator::HandleTNT(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	if (AreCoordsPowered(a_BlockX, a_BlockY, a_BlockZ))
	{
		m_World.BroadcastSoundEffect("game.tnt.primed", a_BlockX * 8, a_BlockY * 8, a_BlockZ * 8, 0.5f, 0.6f);
		m_World.SetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_AIR, 0);
		m_World.SpawnPrimedTNT(a_BlockX + 0.5, a_BlockY + 0.5, a_BlockZ + 0.5);  // 80 ticks to boom
	}
}





void cIncrementalRedstoneSimulator::HandleDoor(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	if (AreCoordsPowered(a_BlockX, a_BlockY, a_BlockZ))
	{
		if (!AreCoordsSimulated(a_BlockX, a_BlockY, a_BlockZ, true))
		{
			cChunkInterface ChunkInterface(m_World.GetChunkMap());
			cBlockDoorHandler::ChangeDoor(ChunkInterface, a_BlockX, a_BlockY, a_BlockZ);
			m_World.BroadcastSoundParticleEffect(1003, a_BlockX, a_BlockY, a_BlockZ, 0);
			SetPlayerToggleableBlockAsSimulated(a_BlockX, a_BlockY, a_BlockZ, true);
		}
	}
	else
	{
		if (!AreCoordsSimulated(a_BlockX, a_BlockY, a_BlockZ, false))
		{
			cChunkInterface ChunkInterface(m_World.GetChunkMap());
			cBlockDoorHandler::ChangeDoor(ChunkInterface, a_BlockX, a_BlockY, a_BlockZ);
			m_World.BroadcastSoundParticleEffect(1003, a_BlockX, a_BlockY, a_BlockZ, 0);
			SetPlayerToggleableBlockAsSimulated(a_BlockX, a_BlockY, a_BlockZ, false);
		}
	}
}





void cIncrementalRedstoneSimulator::HandleCommandBlock(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	class cSetPowerToCommandBlock :
		public cCommandBlockCallback
	{
		bool m_IsPowered;
	public:
		cSetPowerToCommandBlock(bool a_IsPowered) : m_IsPowered(a_IsPowered) {}
				
		virtual bool Item(cCommandBlockEntity * a_CommandBlock) override
		{
			a_CommandBlock->SetRedstonePower(m_IsPowered);
			return false;
		}
	} CmdBlockSP (AreCoordsPowered(a_BlockX, a_BlockY, a_BlockZ));

	m_World.DoWithCommandBlockAt(a_BlockX, a_BlockY, a_BlockZ, CmdBlockSP);
}





void cIncrementalRedstoneSimulator::HandleRail(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_MyType)
{
	switch (a_MyType)
	{
		case E_BLOCK_DETECTOR_RAIL:
		{
			if ((m_World.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ) & 0x08) == 0x08)
			{
				SetAllDirsAsPowered(a_BlockX, a_BlockY, a_BlockZ, a_MyType);
			}
			break;
		}
		case E_BLOCK_ACTIVATOR_RAIL:
		case E_BLOCK_POWERED_RAIL:
		{
			if (AreCoordsPowered(a_BlockX, a_BlockY, a_BlockZ))
			{
				m_World.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, m_World.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ) | 0x08);
			}
			else
			{
				m_World.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, m_World.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ) & 0x07);
			}
			break;
		}
		default: LOGD("Unhandled type of rail in %s", __FUNCTION__);
	}
}





void cIncrementalRedstoneSimulator::HandleTrapdoor(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	if (AreCoordsPowered(a_BlockX, a_BlockY, a_BlockZ))
	{
		if (!AreCoordsSimulated(a_BlockX, a_BlockY, a_BlockZ, true))
		{
			m_World.SetTrapdoorOpen(a_BlockX, a_BlockY, a_BlockZ, true);
			SetPlayerToggleableBlockAsSimulated(a_BlockX, a_BlockY, a_BlockZ, true);
		}
	}
	else
	{
		if (!AreCoordsSimulated(a_BlockX, a_BlockY, a_BlockZ, false))
		{
			m_World.SetTrapdoorOpen(a_BlockX, a_BlockY, a_BlockZ, false);
			SetPlayerToggleableBlockAsSimulated(a_BlockX, a_BlockY, a_BlockZ, false);
		}
	}
}





void cIncrementalRedstoneSimulator::HandleNoteBlock(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	bool m_bAreCoordsPowered = AreCoordsPowered(a_BlockX, a_BlockY, a_BlockZ);

	if (m_bAreCoordsPowered)
	{
		if (!AreCoordsSimulated(a_BlockX, a_BlockY, a_BlockZ, true))
		{
			class cSetPowerToNoteBlock :
				public cNoteBlockCallback
			{
				bool m_IsPowered;
			public:
				cSetPowerToNoteBlock(bool a_IsPowered) : m_IsPowered(a_IsPowered) {}

				virtual bool Item(cNoteEntity * a_NoteBlock) override
				{
					if (m_IsPowered)
					{
						a_NoteBlock->MakeSound();
					}
					return false;
				}
			} NoteBlockSP(m_bAreCoordsPowered);

			m_World.DoWithNoteBlockAt(a_BlockX, a_BlockY, a_BlockZ, NoteBlockSP);
			SetPlayerToggleableBlockAsSimulated(a_BlockX, a_BlockY, a_BlockZ, true);
		}
	}
	else
	{
		if (!AreCoordsSimulated(a_BlockX, a_BlockY, a_BlockZ, false))
		{
			SetPlayerToggleableBlockAsSimulated(a_BlockX, a_BlockY, a_BlockZ, false);
		}
	}
}





void cIncrementalRedstoneSimulator::HandleDaylightSensor(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	int a_ChunkX, a_ChunkZ;
	cChunkDef::BlockToChunk(a_BlockX, a_BlockZ, a_ChunkX, a_ChunkZ);

	if (!m_World.IsChunkLighted(a_ChunkX, a_ChunkZ))
	{
		m_World.QueueLightChunk(a_ChunkX, a_ChunkZ);
	}
	else
	{
		NIBBLETYPE SkyLight = m_World.GetBlockSkyLight(a_BlockX, a_BlockY + 1, a_BlockZ) - m_World.GetSkyDarkness();
		if (SkyLight > 8)
		{
			SetAllDirsAsPowered(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_DAYLIGHT_SENSOR);
		}
	}
}





void cIncrementalRedstoneSimulator::HandlePressurePlate(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_MyType)
{
	switch (a_MyType)
	{
		case E_BLOCK_STONE_PRESSURE_PLATE:
		{
			// MCS feature - stone pressure plates can only be triggered by players :D
			cPlayer * a_Player = m_World.FindClosestPlayer(Vector3f(a_BlockX + 0.5f, (float)a_BlockY, a_BlockZ + 0.5f), 0.7f, false);

			if (a_Player != NULL)
			{
				m_World.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, 0x1);
				SetAllDirsAsPowered(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_STONE_PRESSURE_PLATE);
				SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_YM, a_MyType);
			}
			else
			{
				m_World.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, 0x0);
				m_World.WakeUpSimulators(a_BlockX, a_BlockY, a_BlockZ);
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
					Vector3f BlockPos(m_X + 0.5f, (float)m_Y, m_Z + 0.5f);
					double Distance = (EntityPos - BlockPos).Length();

					if (Distance <= 0.7)
					{
						m_NumberOfEntities++;
					}
					return false;
				}

				bool GetPowerLevel(unsigned char & a_PowerLevel) const
				{
					a_PowerLevel = std::min(m_NumberOfEntities, MAX_POWER_LEVEL);
					return (a_PowerLevel > 0);
				}

			protected:
				int m_NumberOfEntities;

				int m_X;
				int m_Y;
				int m_Z;
			};

			cPressurePlateCallback PressurePlateCallback(a_BlockX, a_BlockY, a_BlockZ);
			m_World.ForEachEntity(PressurePlateCallback);

			unsigned char Power;
			NIBBLETYPE Meta = m_World.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);
			if (PressurePlateCallback.GetPowerLevel(Power))
			{
				if (Meta == E_META_PRESSURE_PLATE_RAISED)
				{
					m_World.BroadcastSoundEffect("random.click", (int)((a_BlockX + 0.5) * 8.0), (int)((a_BlockY + 0.1) * 8.0), (int)((a_BlockZ + 0.5) * 8.0), 0.3F, 0.5F);
				}
				m_World.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, E_META_PRESSURE_PLATE_DEPRESSED);
				SetAllDirsAsPowered(a_BlockX, a_BlockY, a_BlockZ, a_MyType, Power);
				SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_YM, a_MyType);
			}
			else
			{
				if (Meta == E_META_PRESSURE_PLATE_DEPRESSED)
				{
					m_World.BroadcastSoundEffect("random.click", (int)((a_BlockX + 0.5) * 8.0), (int)((a_BlockY + 0.1) * 8.0), (int)((a_BlockZ + 0.5) * 8.0), 0.3F, 0.6F);
				}
				m_World.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, E_META_PRESSURE_PLATE_RAISED);
				m_World.WakeUpSimulators(a_BlockX, a_BlockY, a_BlockZ);
			}

			break;
		}
		case E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE:
		{class cPressurePlateCallback :
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
				Vector3f BlockPos(m_X + 0.5f, (float)m_Y, m_Z + 0.5f);
				double Distance = (EntityPos - BlockPos).Length();

				if (Distance <= 0.7)
				{
					m_NumberOfEntities++;
				}
				return false;
			}

			bool GetPowerLevel(unsigned char & a_PowerLevel) const
			{
				a_PowerLevel = std::min((int)ceil(m_NumberOfEntities / (float)10), MAX_POWER_LEVEL);
				return (a_PowerLevel > 0);
			}

		protected:
			int m_NumberOfEntities;

			int m_X;
			int m_Y;
			int m_Z;
		};

		cPressurePlateCallback PressurePlateCallback(a_BlockX, a_BlockY, a_BlockZ);
		m_World.ForEachEntity(PressurePlateCallback);

		unsigned char Power;
		NIBBLETYPE Meta = m_World.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);
		if (PressurePlateCallback.GetPowerLevel(Power))
		{
			if (Meta == E_META_PRESSURE_PLATE_RAISED)
			{
				m_World.BroadcastSoundEffect("random.click", (int)((a_BlockX + 0.5) * 8.0), (int)((a_BlockY + 0.1) * 8.0), (int)((a_BlockZ + 0.5) * 8.0), 0.3F, 0.5F);
			}
			m_World.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, E_META_PRESSURE_PLATE_DEPRESSED);
			SetAllDirsAsPowered(a_BlockX, a_BlockY, a_BlockZ, a_MyType, Power);
			SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_YM, a_MyType);
		}
		else
		{
			if (Meta == E_META_PRESSURE_PLATE_DEPRESSED)
			{
				m_World.BroadcastSoundEffect("random.click", (int)((a_BlockX + 0.5) * 8.0), (int)((a_BlockY + 0.1) * 8.0), (int)((a_BlockZ + 0.5) * 8.0), 0.3F, 0.6F);
			}
			m_World.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, E_META_PRESSURE_PLATE_RAISED);
			m_World.WakeUpSimulators(a_BlockX, a_BlockY, a_BlockZ);
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
					Vector3f BlockPos(m_X + 0.5f, (float)m_Y, m_Z + 0.5f);
					double Distance = (EntityPos - BlockPos).Length();

					if (Distance <= 0.7)
					{
						m_FoundEntity = true;
						return true; // Break out, we only need to know for plates that at least one entity is on top
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
			} ;

			cPressurePlateCallback PressurePlateCallback(a_BlockX, a_BlockY, a_BlockZ);
			m_World.ForEachEntity(PressurePlateCallback);

			NIBBLETYPE Meta = m_World.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);
			if (PressurePlateCallback.FoundEntity())
			{
				if (Meta == E_META_PRESSURE_PLATE_RAISED)
				{
					m_World.BroadcastSoundEffect("random.click", (int) ((a_BlockX + 0.5) * 8.0), (int) ((a_BlockY + 0.1) * 8.0), (int) ((a_BlockZ + 0.5) * 8.0), 0.3F, 0.5F);
				}
				m_World.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, E_META_PRESSURE_PLATE_DEPRESSED);
				SetAllDirsAsPowered(a_BlockX, a_BlockY, a_BlockZ, a_MyType);
				SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_YM, a_MyType);
			}
			else
			{
				if (Meta == E_META_PRESSURE_PLATE_DEPRESSED)
				{
					m_World.BroadcastSoundEffect("random.click", (int) ((a_BlockX + 0.5) * 8.0), (int) ((a_BlockY + 0.1) * 8.0), (int) ((a_BlockZ + 0.5) * 8.0), 0.3F, 0.6F);
				}
				m_World.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, E_META_PRESSURE_PLATE_RAISED);
				m_World.WakeUpSimulators(a_BlockX, a_BlockY, a_BlockZ);
			}
			break;
		}
		default:
		{
			LOGD("Unimplemented pressure plate type %s in cRedstoneSimulator", ItemToFullString(a_MyType).c_str());
			break;
		}
	}
}





bool cIncrementalRedstoneSimulator::AreCoordsDirectlyPowered(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	for (PoweredBlocksList::const_iterator itr = m_PoweredBlocks->begin(); itr != m_PoweredBlocks->end(); ++itr) // Check powered list
	{
		if (itr->a_BlockPos.Equals(Vector3i(a_BlockX, a_BlockY, a_BlockZ)))
		{
			return true;
		}
	}
	return false;	
}





bool cIncrementalRedstoneSimulator::AreCoordsLinkedPowered(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	for (LinkedBlocksList::const_iterator itr = m_LinkedPoweredBlocks->begin(); itr != m_LinkedPoweredBlocks->end(); ++itr) // Check linked powered list
	{
		if (itr->a_BlockPos.Equals(Vector3i(a_BlockX, a_BlockY, a_BlockZ)))
		{
			return true;
		}
	}
	return false;
}




// IsRepeaterPowered tests if a repeater should be powered by testing for power sources behind the repeater.
// It takes the coordinates of the repeater the the meta value.
bool cIncrementalRedstoneSimulator::IsRepeaterPowered(int a_BlockX, int a_BlockY, int a_BlockZ, NIBBLETYPE a_Meta)
{
	// Repeaters cannot be powered by any face except their back; verify that this is true for a source

	for (PoweredBlocksList::const_iterator itr = m_PoweredBlocks->begin(); itr != m_PoweredBlocks->end(); ++itr)
	{
		if (!itr->a_BlockPos.Equals(Vector3i(a_BlockX, a_BlockY, a_BlockZ))) { continue; }

		switch (a_Meta & 0x3)
		{
			case 0x0:
			{
				// Flip the coords to check the back of the repeater
				if (itr->a_SourcePos.Equals(Vector3i(a_BlockX, a_BlockY, a_BlockZ + 1))) { return true; }
				break;
			}
			case 0x1:
			{
				if (itr->a_SourcePos.Equals(Vector3i(a_BlockX - 1, a_BlockY, a_BlockZ))) { return true; }
				break;
			}
			case 0x2:
			{
				if (itr->a_SourcePos.Equals(Vector3i(a_BlockX, a_BlockY, a_BlockZ - 1))) { return true; }
				break;
			}
			case 0x3:
			{
				if (itr->a_SourcePos.Equals(Vector3i(a_BlockX + 1, a_BlockY, a_BlockZ))) { return true; }
				break;
			}
		}
	}

	for (LinkedBlocksList::const_iterator itr = m_LinkedPoweredBlocks->begin(); itr != m_LinkedPoweredBlocks->end(); ++itr)
	{
		if (!itr->a_BlockPos.Equals(Vector3i(a_BlockX, a_BlockY, a_BlockZ))) { continue; }

		switch (a_Meta & 0x3)
		{
			case 0x0:
			{
				if (itr->a_MiddlePos.Equals(Vector3i(a_BlockX, a_BlockY, a_BlockZ + 1))) { return true; }
				break;
			}
			case 0x1:
			{
				if (itr->a_MiddlePos.Equals(Vector3i(a_BlockX - 1, a_BlockY, a_BlockZ))) { return true; }
				break;
			}
			case 0x2:
			{
				if (itr->a_MiddlePos.Equals(Vector3i(a_BlockX, a_BlockY, a_BlockZ - 1))) { return true; }
				break;
			}
			case 0x3:
			{
				if (itr->a_MiddlePos.Equals(Vector3i(a_BlockX + 1, a_BlockY, a_BlockZ))) { return true; }
				break;
			}
		}
	}
	return false; // Couldn't find power source behind repeater
}





bool cIncrementalRedstoneSimulator::IsRepeaterLocked(int a_BlockX, int a_BlockY, int a_BlockZ, NIBBLETYPE a_Meta)
{	
	switch (a_Meta & 0x3) // We only want the 'direction' part of our metadata
	{
		// If the repeater is looking up or down (If parallel to the Z axis)
		case 0x0:
		case 0x2:
		{
			// Check if eastern(right) neighbor is a powered on repeater who is facing us.
			if (m_World.GetBlock(a_BlockX + 1, a_BlockY, a_BlockZ) == E_BLOCK_REDSTONE_REPEATER_ON) // Is right neighbor a powered repeater?
			{
				NIBBLETYPE OtherRepeaterDir = m_World.GetBlockMeta(a_BlockX + 1, a_BlockY, a_BlockZ) & 0x3;
				if (OtherRepeaterDir == 0x3) { return true; } // If so, I am latched/locked.
			}

			// Check if western(left) neighbor is a powered on repeater who is facing us.
	  		if (m_World.GetBlock(a_BlockX - 1, a_BlockY, a_BlockZ) == E_BLOCK_REDSTONE_REPEATER_ON) 
			{
				NIBBLETYPE OtherRepeaterDir = m_World.GetBlockMeta(a_BlockX -1, a_BlockY, a_BlockZ) & 0x3;
				if (OtherRepeaterDir == 0x1) { return true; } // If so, I am latched/locked.
			}

			break;
		}

		// If the repeater is looking left or right (If parallel to the x axis)
		case 0x1:
		case 0x3:
		{
			// Check if southern(down) neighbor is a powered on repeater who is facing us.
			if (m_World.GetBlock(a_BlockX, a_BlockY, a_BlockZ + 1) == E_BLOCK_REDSTONE_REPEATER_ON) 
			{ 
				NIBBLETYPE OtherRepeaterDir = m_World.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ + 1) & 0x3;
				if (OtherRepeaterDir == 0x0) { return true; } // If so,  am latched/locked.
			}
			
			// Check if northern(up) neighbor is a powered on repeater who is facing us.
			if (m_World.GetBlock(a_BlockX, a_BlockY, a_BlockZ -1) == E_BLOCK_REDSTONE_REPEATER_ON) 
			{ 
				NIBBLETYPE OtherRepeaterDir = m_World.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ - 1) & 0x3;
				if (OtherRepeaterDir == 0x2) { return true; } // If so, I am latched/locked.
			}

			break;
		}
	}

	return false; //  None of the checks succeeded, I am not a locked repeater.
}




bool cIncrementalRedstoneSimulator::IsPistonPowered(int a_BlockX, int a_BlockY, int a_BlockZ, NIBBLETYPE a_Meta)
{
	// Pistons cannot be powered through their front face; this function verifies that a source meets this requirement

	int OldX = a_BlockX, OldY = a_BlockY, OldZ = a_BlockZ;
	eBlockFace Face = cPiston::MetaDataToDirection(a_Meta);

	for (PoweredBlocksList::const_iterator itr = m_PoweredBlocks->begin(); itr != m_PoweredBlocks->end(); ++itr)
	{
		if (!itr->a_BlockPos.Equals(Vector3i(a_BlockX, a_BlockY, a_BlockZ))) { continue; }

		AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, Face);

		if (!itr->a_SourcePos.Equals(Vector3i(a_BlockX, a_BlockY, a_BlockZ)))
		{
			return true;
		}

		a_BlockX = OldX;
		a_BlockY = OldY;
		a_BlockZ = OldZ;
	}

	for (LinkedBlocksList::const_iterator itr = m_LinkedPoweredBlocks->begin(); itr != m_LinkedPoweredBlocks->end(); ++itr)
	{
		if (!itr->a_BlockPos.Equals(Vector3i(a_BlockX, a_BlockY, a_BlockZ))) { continue; }

		AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, Face);

		if (!itr->a_MiddlePos.Equals(Vector3i(a_BlockX, a_BlockY, a_BlockZ)))
		{
			return true;
		}

		a_BlockX = OldX;
		a_BlockY = OldY;
		a_BlockZ = OldZ;
	}
	return false; // Source was in front of the piston's front face
}




bool cIncrementalRedstoneSimulator::IsWirePowered(int a_BlockX, int a_BlockY, int a_BlockZ, unsigned char & a_PowerLevel)
{
	a_PowerLevel = 0;

	for (PoweredBlocksList::const_iterator itr = m_PoweredBlocks->begin(); itr != m_PoweredBlocks->end(); ++itr) // Check powered list
	{
		if (!itr->a_BlockPos.Equals(Vector3i(a_BlockX, a_BlockY, a_BlockZ)))
		{
			continue;
		}
		a_PowerLevel = std::max(a_PowerLevel, itr->a_PowerLevel);
	}

	for (LinkedBlocksList::const_iterator itr = m_LinkedPoweredBlocks->begin(); itr != m_LinkedPoweredBlocks->end(); ++itr) // Check linked powered list
	{
		if (!itr->a_BlockPos.Equals(Vector3i(a_BlockX, a_BlockY, a_BlockZ)))
		{
			continue;
		}
		a_PowerLevel = std::max(a_PowerLevel, itr->a_PowerLevel);
	}

	return (a_PowerLevel != 0); // Source was in front of the piston's front face
}





bool cIncrementalRedstoneSimulator::AreCoordsSimulated(int a_BlockX, int a_BlockY, int a_BlockZ, bool IsCurrentStatePowered)
{
	for (SimulatedPlayerToggleableList::const_iterator itr = m_SimulatedPlayerToggleableBlocks->begin(); itr != m_SimulatedPlayerToggleableBlocks->end(); ++itr)
	{
		if (itr->a_BlockPos.Equals(Vector3i(a_BlockX, a_BlockY, a_BlockZ)))
		{
			if (itr->WasLastStatePowered != IsCurrentStatePowered) // Was the last power state different to the current?
			{
				return false; // It was, coordinates are no longer simulated
			}
			else
			{
				return true; // It wasn't, don't resimulate block, and allow players to toggle
			}
		}
	}
	return false; // Block wasn't even in the list, not simulated
}





void cIncrementalRedstoneSimulator::SetDirectionLinkedPowered(int a_BlockX, int a_BlockY, int a_BlockZ, char a_Direction, BLOCKTYPE a_SourceType, unsigned char a_PowerLevel)
{
	switch (a_Direction)
	{
		case BLOCK_FACE_XM:
		{
			BLOCKTYPE MiddleBlock = m_World.GetBlock(a_BlockX - 1, a_BlockY, a_BlockZ);

			SetBlockLinkedPowered(a_BlockX - 2, a_BlockY, a_BlockZ, a_BlockX - 1, a_BlockY, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock, a_PowerLevel);
			SetBlockLinkedPowered(a_BlockX - 1, a_BlockY + 1, a_BlockZ, a_BlockX - 1, a_BlockY, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock, a_PowerLevel);
			SetBlockLinkedPowered(a_BlockX - 1, a_BlockY - 1, a_BlockZ, a_BlockX - 1, a_BlockY, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock, a_PowerLevel);
			SetBlockLinkedPowered(a_BlockX - 1, a_BlockY, a_BlockZ + 1, a_BlockX - 1, a_BlockY, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock, a_PowerLevel);
			SetBlockLinkedPowered(a_BlockX - 1, a_BlockY, a_BlockZ - 1, a_BlockX - 1, a_BlockY, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock, a_PowerLevel);

			break;
		}
		case BLOCK_FACE_XP:
		{
			BLOCKTYPE MiddleBlock = m_World.GetBlock(a_BlockX + 1, a_BlockY, a_BlockZ);

			SetBlockLinkedPowered(a_BlockX + 2, a_BlockY, a_BlockZ, a_BlockX + 1, a_BlockY, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock, a_PowerLevel);
			SetBlockLinkedPowered(a_BlockX + 1, a_BlockY + 1, a_BlockZ, a_BlockX + 1, a_BlockY, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock, a_PowerLevel);
			SetBlockLinkedPowered(a_BlockX + 1, a_BlockY - 1, a_BlockZ, a_BlockX + 1, a_BlockY, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock, a_PowerLevel);
			SetBlockLinkedPowered(a_BlockX + 1, a_BlockY, a_BlockZ + 1, a_BlockX + 1, a_BlockY, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock, a_PowerLevel);
			SetBlockLinkedPowered(a_BlockX + 1, a_BlockY, a_BlockZ - 1, a_BlockX + 1, a_BlockY, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock, a_PowerLevel);

			break;
		}
		case BLOCK_FACE_YM:
		{
			BLOCKTYPE MiddleBlock = m_World.GetBlock(a_BlockX, a_BlockY - 1, a_BlockZ);

			SetBlockLinkedPowered(a_BlockX, a_BlockY - 2, a_BlockZ, a_BlockX, a_BlockY - 1, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock, a_PowerLevel);
			SetBlockLinkedPowered(a_BlockX + 1, a_BlockY - 1, a_BlockZ, a_BlockX, a_BlockY - 1, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock, a_PowerLevel);
			SetBlockLinkedPowered(a_BlockX - 1, a_BlockY - 1, a_BlockZ, a_BlockX, a_BlockY - 1, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock, a_PowerLevel);
			SetBlockLinkedPowered(a_BlockX, a_BlockY - 1, a_BlockZ + 1, a_BlockX, a_BlockY - 1, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock, a_PowerLevel);
			SetBlockLinkedPowered(a_BlockX, a_BlockY - 1, a_BlockZ - 1, a_BlockX, a_BlockY - 1, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock, a_PowerLevel);

			break;
		}
		case BLOCK_FACE_YP:
		{
			BLOCKTYPE MiddleBlock = m_World.GetBlock(a_BlockX, a_BlockY + 1, a_BlockZ);

			SetBlockLinkedPowered(a_BlockX, a_BlockY + 2, a_BlockZ, a_BlockX, a_BlockY + 1, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock, a_PowerLevel);
			SetBlockLinkedPowered(a_BlockX + 1, a_BlockY + 1, a_BlockZ, a_BlockX, a_BlockY + 1, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock, a_PowerLevel);
			SetBlockLinkedPowered(a_BlockX - 1, a_BlockY + 1, a_BlockZ, a_BlockX, a_BlockY + 1, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock, a_PowerLevel);
			SetBlockLinkedPowered(a_BlockX, a_BlockY + 1, a_BlockZ + 1, a_BlockX, a_BlockY + 1, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock, a_PowerLevel);
			SetBlockLinkedPowered(a_BlockX, a_BlockY + 1, a_BlockZ - 1, a_BlockX, a_BlockY + 1, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock, a_PowerLevel);

			break;
		}
		case BLOCK_FACE_ZM:
		{
			BLOCKTYPE MiddleBlock = m_World.GetBlock(a_BlockX, a_BlockY, a_BlockZ - 1);
						
			SetBlockLinkedPowered(a_BlockX, a_BlockY, a_BlockZ - 2, a_BlockX, a_BlockY, a_BlockZ - 1, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock, a_PowerLevel);
			SetBlockLinkedPowered(a_BlockX + 1, a_BlockY, a_BlockZ - 1, a_BlockX, a_BlockY, a_BlockZ - 1, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock, a_PowerLevel);
			SetBlockLinkedPowered(a_BlockX - 1, a_BlockY, a_BlockZ - 1, a_BlockX, a_BlockY, a_BlockZ - 1, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock, a_PowerLevel);
			SetBlockLinkedPowered(a_BlockX, a_BlockY + 1, a_BlockZ - 1, a_BlockX, a_BlockY, a_BlockZ - 1, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock, a_PowerLevel);
			SetBlockLinkedPowered(a_BlockX, a_BlockY - 1, a_BlockZ - 1, a_BlockX, a_BlockY, a_BlockZ - 1, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock, a_PowerLevel);

			break;
		}
		case BLOCK_FACE_ZP:
		{
			BLOCKTYPE MiddleBlock = m_World.GetBlock(a_BlockX, a_BlockY, a_BlockZ + 1);
			
			SetBlockLinkedPowered(a_BlockX, a_BlockY, a_BlockZ + 2, a_BlockX, a_BlockY, a_BlockZ + 1, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock, a_PowerLevel);
			SetBlockLinkedPowered(a_BlockX + 1, a_BlockY, a_BlockZ + 1, a_BlockX, a_BlockY, a_BlockZ + 1, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock, a_PowerLevel);
			SetBlockLinkedPowered(a_BlockX - 1, a_BlockY, a_BlockZ + 1, a_BlockX, a_BlockY, a_BlockZ + 1, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock, a_PowerLevel);
			SetBlockLinkedPowered(a_BlockX, a_BlockY + 1, a_BlockZ + 1, a_BlockX, a_BlockY, a_BlockZ + 1, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock, a_PowerLevel);
			SetBlockLinkedPowered(a_BlockX, a_BlockY - 1, a_BlockZ + 1, a_BlockX, a_BlockY, a_BlockZ + 1, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock, a_PowerLevel);

			break;
		}
		default:
		{
			ASSERT(!"Unhandled face direction when attempting to set blocks as linked powered!"); // Zombies, that wasn't supposed to happen...
			break;
		}
	}
}





void cIncrementalRedstoneSimulator::SetAllDirsAsPowered(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_SourceBlock, unsigned char a_PowerLevel)
{
	static const struct
	{
		int x, y, z;
	} gCrossCoords[] =
	{
		{ 1, 0, 0 },
		{-1, 0, 0 },
		{ 0, 0, 1 },
		{ 0, 0,-1 },
		{ 0, 1, 0 },
		{ 0,-1, 0 }
	};

	for (size_t i = 0; i < ARRAYCOUNT(gCrossCoords); i++) // Loop through struct to power all directions
	{
		SetBlockPowered(a_BlockX + gCrossCoords[i].x, a_BlockY + gCrossCoords[i].y, a_BlockZ + gCrossCoords[i].z, a_BlockX, a_BlockY, a_BlockZ, a_SourceBlock, a_PowerLevel);
	}
}





void cIncrementalRedstoneSimulator::SetBlockPowered(int a_BlockX, int a_BlockY, int a_BlockZ, int a_SourceX, int a_SourceY, int a_SourceZ, BLOCKTYPE a_SourceBlock, unsigned char a_PowerLevel)
{
	BLOCKTYPE Block = m_World.GetBlock(a_BlockX, a_BlockY, a_BlockZ);
	if (Block == E_BLOCK_AIR)
	{
		// Don't set air, fixes some bugs (wires powering themselves)
		return;
	}

	PoweredBlocksList * Powered = m_Chunk->GetNeighborChunk(a_BlockX, a_BlockZ)->GetRedstoneSimulatorPoweredBlocksList();
	for (PoweredBlocksList::iterator itr = Powered->begin(); itr != Powered->end(); ++itr)  // Check powered list
	{
		if (
			itr->a_BlockPos.Equals(Vector3i(a_BlockX, a_BlockY, a_BlockZ)) &&
			itr->a_SourcePos.Equals(Vector3i(a_SourceX, a_SourceY, a_SourceZ))
		)
		{
			// Check for duplicates, update power level if everything else the same but either way, don't add a new listing
			if (itr->a_PowerLevel != a_PowerLevel)
			{
				itr->a_PowerLevel = a_PowerLevel;
			}
			return;
		}
	}

	PoweredBlocksList * OtherPowered = m_Chunk->GetNeighborChunk(a_SourceX, a_SourceZ)->GetRedstoneSimulatorPoweredBlocksList();
	for (PoweredBlocksList::const_iterator itr = OtherPowered->begin(); itr != OtherPowered->end(); ++itr)  // Check powered list
	{
		if (
			itr->a_BlockPos.Equals(Vector3i(a_SourceX, a_SourceY, a_SourceZ)) &&
			itr->a_SourcePos.Equals(Vector3i(a_BlockX, a_BlockY, a_BlockZ))
		)
		{
			// Powered wires try to power their source - don't let them!
			return;
		}
	}

	sPoweredBlocks RC;
	RC.a_BlockPos = Vector3i(a_BlockX, a_BlockY, a_BlockZ);
	RC.a_SourcePos = Vector3i(a_SourceX, a_SourceY, a_SourceZ);
	RC.a_PowerLevel = a_PowerLevel;
	Powered->push_back(RC);
}





void cIncrementalRedstoneSimulator::SetBlockLinkedPowered(
	int a_BlockX,  int a_BlockY,  int a_BlockZ,
	int a_MiddleX, int a_MiddleY, int a_MiddleZ,
	int a_SourceX, int a_SourceY, int a_SourceZ,
	BLOCKTYPE a_SourceBlock, BLOCKTYPE a_MiddleBlock, unsigned char a_PowerLevel
)
{
	BLOCKTYPE DestBlock = m_World.GetBlock(a_BlockX, a_BlockY, a_BlockZ);
	if (DestBlock == E_BLOCK_AIR)
	{
		// Don't set air, fixes some bugs (wires powering themselves)
		return;
	}
	if (!IsViableMiddleBlock(a_MiddleBlock))
	{
		return;
	}

	LinkedBlocksList * Linked = m_Chunk->GetNeighborChunk(a_BlockX, a_BlockZ)->GetRedstoneSimulatorLinkedBlocksList();
	for (LinkedBlocksList::iterator itr = Linked->begin(); itr != Linked->end(); ++itr)  // Check linked powered list
	{
		if (
			itr->a_BlockPos.Equals(Vector3i(a_BlockX, a_BlockY, a_BlockZ)) &&
			itr->a_MiddlePos.Equals(Vector3i(a_MiddleX, a_MiddleY, a_MiddleZ)) &&
			itr->a_SourcePos.Equals(Vector3i(a_SourceX, a_SourceY, a_SourceZ))
		)
		{
			// Check for duplicates, update power level if everything else the same but either way, don't add a new listing
			if (itr->a_PowerLevel != a_PowerLevel)
			{
				itr->a_PowerLevel = a_PowerLevel;
			}
			return;
		}
	}

	sLinkedPoweredBlocks RC;
	RC.a_BlockPos = Vector3i(a_BlockX, a_BlockY, a_BlockZ);
	RC.a_MiddlePos = Vector3i(a_MiddleX, a_MiddleY, a_MiddleZ);
	RC.a_SourcePos = Vector3i(a_SourceX, a_SourceY, a_SourceZ);
	RC.a_PowerLevel = a_PowerLevel;
	Linked->push_back(RC);
}





void cIncrementalRedstoneSimulator::SetPlayerToggleableBlockAsSimulated(int a_BlockX, int a_BlockY, int a_BlockZ, bool WasLastStatePowered)
{
	for (SimulatedPlayerToggleableList::iterator itr = m_SimulatedPlayerToggleableBlocks->begin(); itr != m_SimulatedPlayerToggleableBlocks->end(); ++itr)
	{
		if (!itr->a_BlockPos.Equals(Vector3i(a_BlockX, a_BlockY, a_BlockZ)))
		{
			continue;
		}

		if (itr->WasLastStatePowered != WasLastStatePowered)
		{
			// If power states different, update listing
			itr->WasLastStatePowered = WasLastStatePowered;
			return;
		}
		else
		{
			// If states the same, just ignore
			return;
		}
	}

	// We have arrive here; no block must be in list - add one
	sSimulatedPlayerToggleableList RC;
	RC.a_BlockPos = Vector3i(a_BlockX, a_BlockY, a_BlockZ);
	RC.WasLastStatePowered = WasLastStatePowered;
	m_SimulatedPlayerToggleableBlocks->push_back(RC);
}





void cIncrementalRedstoneSimulator::QueueRepeaterPowerChange(int a_BlockX, int a_BlockY, int a_BlockZ, NIBBLETYPE a_Meta, bool ShouldPowerOn)
{
	for (RepeatersDelayList::iterator itr = m_RepeatersDelayList->begin(); itr != m_RepeatersDelayList->end(); ++itr)
	{
		if (itr->a_BlockPos.Equals(Vector3i(a_BlockX, a_BlockY, a_BlockZ)))
		{
			if (ShouldPowerOn == itr->ShouldPowerOn) // We are queued already for the same thing, don't replace entry
			{
				return;
			}

			// Already in here (normal to allow repeater to continue on powering and updating blocks in front) - just update info and quit
			itr->a_DelayTicks = (((a_Meta & 0xC) >> 0x2) + 1) * 2; // See below for description
			itr->a_ElapsedTicks = 0;
			itr->ShouldPowerOn = ShouldPowerOn;
			return;
		}
	}

	// Self not in list, add self to list
	sRepeatersDelayList RC;	
	RC.a_BlockPos = Vector3i(a_BlockX, a_BlockY, a_BlockZ);
	
	// Gets the top two bits (delay time), shifts them into the lower two bits, and adds one (meta 0 = 1 tick; 1 = 2 etc.)
	// * 2 because in MCS, 1 redstone tick = 1 world tick, but in Vanilla, 1 redstone tick = 2 world ticks, and we need to maintain compatibility
	RC.a_DelayTicks = (((a_Meta & 0xC) >> 0x2) + 1) * 2;

	RC.a_ElapsedTicks = 0;
	RC.ShouldPowerOn = ShouldPowerOn;
	m_RepeatersDelayList->push_back(RC);
	return;
}





cIncrementalRedstoneSimulator::eRedstoneDirection cIncrementalRedstoneSimulator::GetWireDirection(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	int Dir = REDSTONE_NONE;

	BLOCKTYPE NegX = m_World.GetBlock(a_BlockX - 1, a_BlockY, a_BlockZ);
	if (IsPotentialSource(NegX))
	{
		Dir |= (REDSTONE_X_POS);
	}
	
	BLOCKTYPE PosX = m_World.GetBlock(a_BlockX + 1, a_BlockY, a_BlockZ);
	if (IsPotentialSource(PosX))
	{
		Dir |= (REDSTONE_X_NEG);
	}
	
	BLOCKTYPE NegZ = m_World.GetBlock(a_BlockX, a_BlockY, a_BlockZ - 1);
	if (IsPotentialSource(NegZ))
	{
		if ((Dir & REDSTONE_X_POS) && !(Dir & REDSTONE_X_NEG))  // corner
		{
			Dir ^= REDSTONE_X_POS;
			Dir |= REDSTONE_X_NEG;
		}
		if ((Dir & REDSTONE_X_NEG) && !(Dir & REDSTONE_X_POS))  // corner
		{
			Dir ^= REDSTONE_X_NEG;
			Dir |= REDSTONE_X_POS;
		}
		Dir |= REDSTONE_Z_POS;
	}
	
	BLOCKTYPE PosZ = m_World.GetBlock(a_BlockX, a_BlockY, a_BlockZ + 1);
	if (IsPotentialSource(PosZ))
	{
		if ((Dir & REDSTONE_X_POS) && !(Dir & REDSTONE_X_NEG))  // corner
		{
			Dir ^= REDSTONE_X_POS;
			Dir |= REDSTONE_X_NEG;
		}
		if ((Dir & REDSTONE_X_NEG) && !(Dir & REDSTONE_X_POS))  // corner
		{
			Dir ^= REDSTONE_X_NEG;
			Dir |= REDSTONE_X_POS;
		}
		Dir |= REDSTONE_Z_NEG;
	}
	return (eRedstoneDirection)Dir;
}





bool cIncrementalRedstoneSimulator::IsLeverOn(NIBBLETYPE a_BlockMeta)
{
	// Extract the ON bit from metadata and return if true if it is set:
	return ((a_BlockMeta & 0x8) == 0x8);
}




