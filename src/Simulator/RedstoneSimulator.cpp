
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "RedstoneSimulator.h"
#include "../BlockEntities/DropSpenserEntity.h"
#include "../BlockEntities/NoteEntity.h"
#include "../Entities/TNTEntity.h"
#include "../Blocks/BlockTorch.h"
#include "../Blocks/BlockDoor.h"
#include "../Piston.h"





cRedstoneSimulator::cRedstoneSimulator(cWorld & a_World)
	: super(a_World)
{
}





cRedstoneSimulator::~cRedstoneSimulator()
{
}





void cRedstoneSimulator::AddBlock(int a_BlockX, int a_BlockY, int a_BlockZ, cChunk * a_Chunk)
{
	if ((a_Chunk == NULL) || !a_Chunk->IsValid())
	{
		return;
	}
	else if ((a_BlockY < 0) || (a_BlockY > cChunkDef::Height))
	{
		return;
	}

	int RelX = a_BlockX - a_Chunk->GetPosX() * cChunkDef::Width;
	int RelZ = a_BlockZ - a_Chunk->GetPosZ() * cChunkDef::Width;

	if (!IsAllowedBlock(a_Chunk->GetBlock(RelX, a_BlockY, RelZ)))
	{
		return;
	}

	// Check for duplicates:
	cRedstoneSimulatorChunkData & ChunkData = a_Chunk->GetRedstoneSimulatorData();
	for (cRedstoneSimulatorChunkData::const_iterator itr = ChunkData.begin(); itr != ChunkData.end(); ++itr)
	{
		if ((itr->x == RelX) && (itr->y == a_BlockY) && (itr->z == RelZ))
		{
			return;
		}
	}

	ChunkData.push_back(cCoordWithInt(RelX, a_BlockY, RelZ));
}





void cRedstoneSimulator::SimulateChunk(float a_Dt, int a_ChunkX, int a_ChunkZ, cChunk * a_Chunk)
{
	cRedstoneSimulatorChunkData & ChunkData = a_Chunk->GetRedstoneSimulatorData();
	if (ChunkData.empty())
	{
		return;
	}

	int BaseX = a_Chunk->GetPosX() * cChunkDef::Width;
	int BaseZ = a_Chunk->GetPosZ() * cChunkDef::Width;

	// Check to see if PoweredBlocks have invalid items (source is air or unpowered)
	for (PoweredBlocksList::iterator itr = m_PoweredBlocks.begin(); itr != m_PoweredBlocks.end();)
	{
		int RelX = itr->a_SourcePos.x - a_ChunkX * cChunkDef::Width;
		int RelZ = itr->a_SourcePos.z - a_ChunkZ * cChunkDef::Width;
		int DestRelX = itr->a_BlockPos.x - a_ChunkX * cChunkDef::Width;
		int DestRelZ = itr->a_BlockPos.z - a_ChunkZ * cChunkDef::Width;

		BLOCKTYPE SourceBlockType;
		NIBBLETYPE SourceBlockMeta;
		BLOCKTYPE DestBlockType;
		if (
			!a_Chunk->UnboundedRelGetBlock(RelX, itr->a_SourcePos.y, RelZ, SourceBlockType, SourceBlockMeta) ||
			!a_Chunk->UnboundedRelGetBlockType(DestRelX, itr->a_BlockPos.y, DestRelZ, DestBlockType)
			)
		{
			continue;
		}

		if (SourceBlockType != itr->a_SourceBlock)
		{
			LOGD("cRedstoneSimulator: Erased block %s from powered blocks list due to present/past block type mismatch", ItemToFullString(itr->a_SourceBlock).c_str());
			itr = m_PoweredBlocks.erase(itr);
		}
		else if (
			// Changeable sources
			((SourceBlockType == E_BLOCK_REDSTONE_WIRE) && (SourceBlockMeta == 0)) ||
			((SourceBlockType == E_BLOCK_LEVER) && !IsLeverOn(SourceBlockMeta)) ||
			((SourceBlockType == E_BLOCK_DETECTOR_RAIL) && (SourceBlockMeta & 0x08) == 0x08) ||
			(((SourceBlockType == E_BLOCK_STONE_BUTTON) || (SourceBlockType == E_BLOCK_WOODEN_BUTTON)) && (!IsButtonOn(SourceBlockMeta)))
			)
		{
			LOGD("cRedstoneSimulator: Erased block %s from powered blocks list due to present/past metadata mismatch", ItemToFullString(itr->a_SourceBlock).c_str());
			itr = m_PoweredBlocks.erase(itr);
		}
		else if ((SourceBlockType == E_BLOCK_REDSTONE_WIRE) && (DestBlockType == E_BLOCK_REDSTONE_WIRE))
		{
			// It is simply not allowed that a wire powers another wire, presuming that data here is sane and a dest and source are beside each other
			LOGD("cRedstoneSimulator: Erased redstone wire from powered blocks list because it's source was also wire");
			itr = m_PoweredBlocks.erase(itr);
		}
		else
		{
			itr++;
		}
	}

	// Check to see if LinkedPoweredBlocks have invalid items: source, block powered through, or power destination block has changed
	for (LinkedBlocksList::iterator itr = m_LinkedPoweredBlocks.begin(); itr != m_LinkedPoweredBlocks.end();)
	{
		int RelX = itr->a_SourcePos.x - a_ChunkX * cChunkDef::Width;
		int RelZ = itr->a_SourcePos.z - a_ChunkZ * cChunkDef::Width;
		int MidRelX = itr->a_MiddlePos.x - a_ChunkX * cChunkDef::Width;
		int MidRelZ = itr->a_MiddlePos.z - a_ChunkZ * cChunkDef::Width;

		BLOCKTYPE SourceBlockType;
		NIBBLETYPE SourceBlockMeta;
		BLOCKTYPE MiddleBlockType;
		if (
			!a_Chunk->UnboundedRelGetBlock(RelX, itr->a_SourcePos.y, RelZ, SourceBlockType, SourceBlockMeta) ||
			!a_Chunk->UnboundedRelGetBlockType(MidRelX, itr->a_MiddlePos.y, MidRelZ, MiddleBlockType)
			)
		{
			continue;
		}

		if (SourceBlockType != itr->a_SourceBlock)
		{
			LOGD("cRedstoneSimulator: Erased block %s from linked powered blocks list due to present/past block type mismatch", ItemToFullString(itr->a_SourceBlock).c_str());
			itr = m_LinkedPoweredBlocks.erase(itr);
		}
		else if (MiddleBlockType != itr->a_MiddleBlock)
		{
			LOGD("cRedstoneSimulator: Erased block %s from linked powered blocks list due to present/past middle block mismatch", ItemToFullString(itr->a_SourceBlock).c_str());
			itr = m_LinkedPoweredBlocks.erase(itr);
		}
		else if (
			// Things that can send power through a block but which depends on meta
			((SourceBlockType == E_BLOCK_REDSTONE_WIRE) && (SourceBlockMeta == 0)) ||
			((SourceBlockType == E_BLOCK_LEVER) && !IsLeverOn(SourceBlockMeta)) ||
			(((SourceBlockType == E_BLOCK_STONE_BUTTON) || (SourceBlockType == E_BLOCK_WOODEN_BUTTON)) && (!IsButtonOn(SourceBlockMeta)))
			)
		{
			LOGD("cRedstoneSimulator: Erased block %s from linked powered blocks list due to present/past metadata mismatch", ItemToFullString(itr->a_SourceBlock).c_str());
			itr = m_LinkedPoweredBlocks.erase(itr);
		}
		else
		{
			itr++;
		}
	}

	for (SimulatedPlayerToggleableList::iterator itr = m_SimulatedPlayerToggleableBlocks.begin(); itr != m_SimulatedPlayerToggleableBlocks.end();)
	{
		int RelX = itr->a_BlockPos.x - a_ChunkX * cChunkDef::Width;
		int RelZ = itr->a_BlockPos.z - a_ChunkZ * cChunkDef::Width;

		BLOCKTYPE SourceBlockType;
		if (!a_Chunk->UnboundedRelGetBlockType(RelX, itr->a_BlockPos.y, RelZ, SourceBlockType))
		{
			continue;
		}
		else if (!IsAllowedBlock(SourceBlockType))
		{
			LOGD("cRedstoneSimulator: Erased block %s from toggleable simulated list as block is no longer redstone", ItemToFullString(SourceBlockType).c_str());
			itr = m_SimulatedPlayerToggleableBlocks.erase(itr);
		}
		else
		{
			itr++;
		}
	}

	for (RepeatersDelayList::iterator itr = m_RepeatersDelayList.begin(); itr != m_RepeatersDelayList.end();)
	{
		int RelX = itr->a_BlockPos.x - a_ChunkX * cChunkDef::Width;
		int RelZ = itr->a_BlockPos.z - a_ChunkZ * cChunkDef::Width;

		BLOCKTYPE SourceBlockType;
		if (!a_Chunk->UnboundedRelGetBlockType(RelX, itr->a_BlockPos.y, RelZ, SourceBlockType))
		{
			continue;
		}

		if ((SourceBlockType != E_BLOCK_REDSTONE_REPEATER_ON) && (SourceBlockType != E_BLOCK_REDSTONE_REPEATER_OFF))
		{
			itr = m_RepeatersDelayList.erase(itr);
			continue;
		}

		itr++;
	}

	for (cRedstoneSimulatorChunkData::iterator dataitr = ChunkData.begin(), end = ChunkData.end(); dataitr != end;)
	{
		BLOCKTYPE BlockType = a_Chunk->GetBlock(dataitr->x, dataitr->y, dataitr->z);
		if (!IsAllowedBlock(BlockType))
		{
			dataitr = ChunkData.erase(dataitr);
			continue;
		}

		// PoweredBlock and LinkedPoweredBlock list was fine, now to the actual handling
		int a_X = BaseX + dataitr->x;
		int a_Z = BaseZ + dataitr->z;
		switch (BlockType)
		{
			case E_BLOCK_BLOCK_OF_REDSTONE:     HandleRedstoneBlock(a_X, dataitr->y, a_Z);	break;
			case E_BLOCK_LEVER:                 HandleRedstoneLever(a_X, dataitr->y, a_Z);	break;
			case E_BLOCK_TNT:                   HandleTNT(a_X, dataitr->y, a_Z);			break;
			case E_BLOCK_TRAPDOOR:              HandleTrapdoor(a_X, dataitr->y, a_Z);       break;
			case E_BLOCK_REDSTONE_WIRE:         HandleRedstoneWire(a_X, dataitr->y, a_Z);	break;
			case E_BLOCK_NOTE_BLOCK:            HandleNoteBlock(a_X, dataitr->y, a_Z);      break;
			case E_BLOCK_DAYLIGHT_SENSOR:       HandleDaylightSensor(a_X, dataitr->y, a_Z); break;

			case E_BLOCK_REDSTONE_TORCH_OFF:
			case E_BLOCK_REDSTONE_TORCH_ON:
			{
				HandleRedstoneTorch(a_X, dataitr->y, a_Z, BlockType);
				break;
			}
			case E_BLOCK_STONE_BUTTON:
			case E_BLOCK_WOODEN_BUTTON:
			{
				HandleRedstoneButton(a_X, dataitr->y, a_Z, BlockType);
				break;
			}
			case E_BLOCK_REDSTONE_REPEATER_OFF:
			case E_BLOCK_REDSTONE_REPEATER_ON:
			{
				HandleRedstoneRepeater(a_X, dataitr->y, a_Z, BlockType);
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
				HandleRedstoneLamp(a_X, dataitr->y, a_Z, BlockType);
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
				HandleRail(a_X, dataitr->y, a_Z, BlockType);
				break;
			}
		}

		++dataitr;
	}
}





void cRedstoneSimulator::HandleRedstoneTorch(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_MyState)
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





void cRedstoneSimulator::HandleRedstoneBlock(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	SetAllDirsAsPowered(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_BLOCK_OF_REDSTONE);
	SetBlockPowered(a_BlockX, a_BlockY, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_BLOCK_OF_REDSTONE); // Set self as powered
}





void cRedstoneSimulator::HandleRedstoneLever(int a_BlockX, int a_BlockY, int a_BlockZ)
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





void cRedstoneSimulator::HandleRedstoneButton(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType)
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





void cRedstoneSimulator::HandleRedstoneWire(int a_BlockX, int a_BlockY, int a_BlockZ)
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
	if (AreCoordsPowered(a_BlockX, a_BlockY, a_BlockZ))
	{
		m_World.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, 15); // Maximum power
	}
	else
	{
		NIBBLETYPE MetaToSet = 0;
		NIBBLETYPE MyMeta = m_World.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);
		int TimesMetaSmaller = 0, TimesFoundAWire = 0;

		for (size_t i = 0; i < ARRAYCOUNT(gCrossCoords); i++) // Loop through all directions to transfer or receive power
		{
			if ((i >= 4) && (i <= 7)) // If we are currently checking for wire surrounding ourself one block above...
			{
				if (g_BlockIsSolid[m_World.GetBlock(a_BlockX, a_BlockY + 1, a_BlockZ)]) // If there is something solid above us (wire cut off)...
				{
					continue; // We don't receive power from that wire
				}
			}
			else if ((i >= 8) && (i <= 11)) // See above, but this is for wire below us
			{
				if (g_BlockIsSolid[m_World.GetBlock(a_BlockX + gCrossCoords[i].x, a_BlockY + gCrossCoords[i].y + 1, a_BlockZ + gCrossCoords[i].z)])
				{
					continue;
				}
			}

			BLOCKTYPE SurroundType;
			NIBBLETYPE SurroundMeta;
			m_World.GetBlockTypeMeta(a_BlockX + gCrossCoords[i].x, a_BlockY + gCrossCoords[i].y, a_BlockZ + gCrossCoords[i].z, SurroundType, SurroundMeta);

			if (SurroundType == E_BLOCK_REDSTONE_WIRE)
			{
				TimesFoundAWire++;

				if (SurroundMeta > 1) // Wires of power 1 or 0 cannot transfer power TO ME, don't bother checking
				{
					// Does surrounding wire have a higher power level than self?
					// >= to fix a bug where wires bordering each other with the same power level will appear (in terms of meta) to power each other, when they aren't actually in the powered list
					if (SurroundMeta >= MyMeta)
					{
						MetaToSet = SurroundMeta - 1; // To improve performance
					}
				}
				
				if (SurroundMeta < MyMeta) // Go through all surroundings to see if self power is larger than everyone else's
				{
					TimesMetaSmaller++;
				}
			}			
		}

		if (TimesMetaSmaller == TimesFoundAWire)
		{
			// All surrounding metas were smaller - self must have been a wire that was
			// transferring power to other wires around.
			// However, self not directly powered anymore, so source must have been removed,
			// therefore, self must be set to meta zero
			m_World.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, 0);
			return; // No need to process block power sets because self not powered
		}
		else
		{
			m_World.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, MetaToSet);
		}
	}

	if (m_World.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ) != 0) // A powered wire
	{
		for (size_t i = 0; i < ARRAYCOUNT(gSideCoords); i++) // Look for repeaters immediately surrounding self and try to power them
		{
			if (m_World.GetBlock(a_BlockX + gSideCoords[i].x, a_BlockY + gSideCoords[i].y, a_BlockZ + gSideCoords[i].z) == E_BLOCK_REDSTONE_REPEATER_OFF)
			{
				SetBlockPowered(a_BlockX + gSideCoords[i].x, a_BlockY + gSideCoords[i].y, a_BlockZ + gSideCoords[i].z, a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_REDSTONE_WIRE);
			}
		}

		// Wire still powered, power blocks beneath
		SetBlockPowered(a_BlockX, a_BlockY - 1, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_REDSTONE_WIRE);
		SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_YM, E_BLOCK_REDSTONE_WIRE);

		switch (GetWireDirection(a_BlockX, a_BlockY, a_BlockZ))
		{
			case REDSTONE_NONE:
			{
				SetAllDirsAsPowered(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_REDSTONE_WIRE);

				SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_XM, E_BLOCK_REDSTONE_WIRE);
				SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_XP, E_BLOCK_REDSTONE_WIRE);
				SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_YM, E_BLOCK_REDSTONE_WIRE);
				SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_YP, E_BLOCK_REDSTONE_WIRE);
				SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_ZM, E_BLOCK_REDSTONE_WIRE);
				SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_ZP, E_BLOCK_REDSTONE_WIRE);
				break;
			}
			case REDSTONE_X_POS:
			{
				SetBlockPowered(a_BlockX + 1, a_BlockY, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_REDSTONE_WIRE);
				SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_XP, E_BLOCK_REDSTONE_WIRE);
				break;
			}
			case REDSTONE_X_NEG:
			{
				SetBlockPowered(a_BlockX - 1, a_BlockY, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_REDSTONE_WIRE);
				SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_XM, E_BLOCK_REDSTONE_WIRE);
				break;
			}
			case REDSTONE_Z_POS:
			{
				SetBlockPowered(a_BlockX, a_BlockY, a_BlockZ + 1, a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_REDSTONE_WIRE);
				SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_ZP, E_BLOCK_REDSTONE_WIRE);
				break;
			}
			case REDSTONE_Z_NEG:
			{
				SetBlockPowered(a_BlockX, a_BlockY, a_BlockZ - 1, a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_REDSTONE_WIRE);
				SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_ZM, E_BLOCK_REDSTONE_WIRE);
				break;
			}
		}
	}
}





void cRedstoneSimulator::HandleRedstoneRepeater(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_MyState)
{
	NIBBLETYPE a_Meta = m_World.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);

	bool IsOn = ((a_MyState == E_BLOCK_REDSTONE_REPEATER_ON) ? true : false); // Cache if repeater is on
	bool IsSelfPowered = IsRepeaterPowered(a_BlockX, a_BlockY, a_BlockZ, a_Meta & 0x3); // Cache if repeater is pwoered

	if (IsSelfPowered && !IsOn) // Queue a power change if I am receiving power but not on
	{
		QueueRepeaterPowerChange(a_BlockX, a_BlockY, a_BlockZ, a_Meta, 0, true);
	}
	else if (!IsSelfPowered && IsOn) // Queue a power change if I am not receiving power but on
	{
		QueueRepeaterPowerChange(a_BlockX, a_BlockY, a_BlockZ, a_Meta, 0, false);
	}

	for (RepeatersDelayList::iterator itr = m_RepeatersDelayList.begin(); itr != m_RepeatersDelayList.end(); itr++)
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
				m_RepeatersDelayList.erase(itr); // We can remove off repeaters which don't need further updating
				return;
			}
		}
		else
		{
			// Apparently, incrementing ticks only works reliably here, and not in SimChunk;
			// With a world with lots of redstone, the repeaters simply do not delay
			// I am confounded to say why. Perhaps optimisation failure.
			LOGD("Incremented a repeater @ %i %i %i | Elapsed ticks: %i | Target delay: %i", itr->a_BlockPos.x, itr->a_BlockPos.y, itr->a_BlockPos.z, itr->a_ElapsedTicks, itr->a_DelayTicks);
			itr->a_ElapsedTicks++;
		}
	}
}





void cRedstoneSimulator::HandlePiston(int a_BlockX, int a_BlockY, int a_BlockZ)
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





void cRedstoneSimulator::HandleDropSpenser(int a_BlockX, int a_BlockY, int a_BlockZ)
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





void cRedstoneSimulator::HandleRedstoneLamp(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_MyState)
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





void cRedstoneSimulator::HandleTNT(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	if (AreCoordsPowered(a_BlockX, a_BlockY, a_BlockZ))
	{
		m_World.BroadcastSoundEffect("random.fuse", a_BlockX * 8, a_BlockY * 8, a_BlockZ * 8, 0.5f, 0.6f);
		m_World.SpawnPrimedTNT(a_BlockX + 0.5, a_BlockY + 0.5, a_BlockZ + 0.5, 4);  // 4 seconds to boom
		m_World.SetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_AIR, 0);
	}
}





void cRedstoneSimulator::HandleDoor(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	if ((m_World.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ) & 0x08) == 0x08)
	{
		// Block position is located at top half of door
		// Is Y - 1 both within world boundaries, a door block, and the bottom half of a door?
		// The bottom half stores the open/closed information
		if (
			(a_BlockY - 1 >= 0) &&
			((m_World.GetBlock(a_BlockX, a_BlockY, a_BlockZ) == E_BLOCK_WOODEN_DOOR) || (m_World.GetBlock(a_BlockX, a_BlockY, a_BlockZ) == E_BLOCK_IRON_DOOR)) &&
			(m_World.GetBlockMeta(a_BlockX, a_BlockY - 1, a_BlockZ & 0x08) == 0)
			)
		{
			if ((m_World.GetBlockMeta(a_BlockX, a_BlockY - 1, a_BlockZ) & 0x04) == 0) // Closed door?
			{
				if (AreCoordsPowered(a_BlockX, a_BlockY, a_BlockZ)) // Powered? If so, toggle open
				{
					cBlockDoorHandler::ChangeDoor(&m_World, a_BlockX, a_BlockY, a_BlockZ);
				}
			}
			else // Opened door
			{
				if (!AreCoordsPowered(a_BlockX, a_BlockY, a_BlockZ)) // Unpowered? Close if so
				{
					cBlockDoorHandler::ChangeDoor(&m_World, a_BlockX, a_BlockY, a_BlockZ);
				}
			}
		}
	}
	else
	{
		if ((m_World.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ) & 0x04) == 0) // Closed door?
		{
			if (AreCoordsPowered(a_BlockX, a_BlockY, a_BlockZ)) // Powered? If so, toggle open
			{
				cBlockDoorHandler::ChangeDoor(&m_World, a_BlockX, a_BlockY, a_BlockZ);
			}
		}
		else // Opened door
		{
			if (!AreCoordsPowered(a_BlockX, a_BlockY, a_BlockZ)) // Unpowered? Close if so
			{
				cBlockDoorHandler::ChangeDoor(&m_World, a_BlockX, a_BlockY, a_BlockZ);
			}
		}
	}
}





void cRedstoneSimulator::HandleRail(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_MyType)
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





void cRedstoneSimulator::HandleTrapdoor(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	if (AreCoordsPowered(a_BlockX, a_BlockY, a_BlockZ))
	{
		if (!AreCoordsSimulated(a_BlockX, a_BlockY, a_BlockZ, true))
		{
			m_World.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, m_World.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ) | 0x4);
			SetPlayerToggleableBlockAsSimulated(a_BlockX, a_BlockY, a_BlockZ, true);
		}		
	}
	else
	{
		if (!AreCoordsSimulated(a_BlockX, a_BlockY, a_BlockZ, false))
		{
			m_World.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, m_World.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ) & 0xB); // Take into account that the fourth bit is needed for trapdoors too
			SetPlayerToggleableBlockAsSimulated(a_BlockX, a_BlockY, a_BlockZ, false);
		}
	}
}





void cRedstoneSimulator::HandleNoteBlock(int a_BlockX, int a_BlockY, int a_BlockZ)
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





void cRedstoneSimulator::HandleDaylightSensor(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	for (int Y = a_BlockY + 1; Y < 10; Y++)
	{
		if (!g_BlockTransparent[m_World.GetBlock(a_BlockX, Y, a_BlockZ)])
		{
			return;
		}
	}

	LOG("%i", m_World.GetSkyDarkness());
	if (m_World.GetSkyDarkness() > 10)
	{
		SetAllDirsAsPowered(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_DAYLIGHT_SENSOR);
	}
}





bool cRedstoneSimulator::AreCoordsDirectlyPowered(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	for (PoweredBlocksList::const_iterator itr = m_PoweredBlocks.begin(); itr != m_PoweredBlocks.end(); ++itr) // Check powered list
	{
		if (itr->a_BlockPos.Equals(Vector3i(a_BlockX, a_BlockY, a_BlockZ)))
		{
			return true;
		}
	}
	return false;	
}





bool cRedstoneSimulator::AreCoordsLinkedPowered(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	for (LinkedBlocksList::const_iterator itr = m_LinkedPoweredBlocks.begin(); itr != m_LinkedPoweredBlocks.end(); ++itr) // Check linked powered list
	{
		if (itr->a_BlockPos.Equals(Vector3i(a_BlockX, a_BlockY, a_BlockZ)))
		{
			return true;
		}
	}
	return false;
}





bool cRedstoneSimulator::IsRepeaterPowered(int a_BlockX, int a_BlockY, int a_BlockZ, NIBBLETYPE a_Meta)
{
	// Repeaters cannot be powered by any face except their back; verify that this is true for a source

	for (PoweredBlocksList::const_iterator itr = m_PoweredBlocks.begin(); itr != m_PoweredBlocks.end(); ++itr)
	{
		if (!itr->a_BlockPos.Equals(Vector3i(a_BlockX, a_BlockY, a_BlockZ))) { continue; }

		switch (a_Meta)
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

	for (LinkedBlocksList::const_iterator itr = m_LinkedPoweredBlocks.begin(); itr != m_LinkedPoweredBlocks.end(); ++itr)
	{
		if (!itr->a_BlockPos.Equals(Vector3i(a_BlockX, a_BlockY, a_BlockZ))) { continue; }

		switch (a_Meta)
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




bool cRedstoneSimulator::IsPistonPowered(int a_BlockX, int a_BlockY, int a_BlockZ, NIBBLETYPE a_Meta)
{
	// Pistons cannot be powered through their front face; this function verifies that a source meets this requirement

	int OldX = a_BlockX, OldY = a_BlockY, OldZ = a_BlockZ;

	for (PoweredBlocksList::const_iterator itr = m_PoweredBlocks.begin(); itr != m_PoweredBlocks.end(); ++itr)
	{
		if (!itr->a_BlockPos.Equals(Vector3i(a_BlockX, a_BlockY, a_BlockZ))) { continue; }

		AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, a_Meta); // Piston meta is based on what direction they face, so we can do this

		if (!itr->a_SourcePos.Equals(Vector3i(a_BlockX, a_BlockY, a_BlockZ)))
		{
			return true;
		}

		a_BlockX = OldX;
		a_BlockY = OldY;
		a_BlockZ = OldZ;
	}

	for (LinkedBlocksList::const_iterator itr = m_LinkedPoweredBlocks.begin(); itr != m_LinkedPoweredBlocks.end(); ++itr)
	{
		if (!itr->a_BlockPos.Equals(Vector3i(a_BlockX, a_BlockY, a_BlockZ))) { continue; }

		AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, a_Meta);

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





bool cRedstoneSimulator::AreCoordsSimulated(int a_BlockX, int a_BlockY, int a_BlockZ, bool IsCurrentStatePowered)
{
	for (SimulatedPlayerToggleableList::const_iterator itr = m_SimulatedPlayerToggleableBlocks.begin(); itr != m_SimulatedPlayerToggleableBlocks.end(); ++itr)
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





void cRedstoneSimulator::SetDirectionLinkedPowered(int a_BlockX, int a_BlockY, int a_BlockZ, char a_Direction, BLOCKTYPE a_SourceType)
{
	switch (a_Direction)
	{
		case BLOCK_FACE_XM:
		{
			BLOCKTYPE MiddleBlock = m_World.GetBlock(a_BlockX - 1, a_BlockY, a_BlockZ);

			SetBlockLinkedPowered(a_BlockX - 2, a_BlockY, a_BlockZ, a_BlockX - 1, a_BlockY, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock);
			SetBlockLinkedPowered(a_BlockX - 1, a_BlockY + 1, a_BlockZ, a_BlockX - 1, a_BlockY, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock);
			SetBlockLinkedPowered(a_BlockX - 1, a_BlockY - 1, a_BlockZ, a_BlockX - 1, a_BlockY, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock);
			SetBlockLinkedPowered(a_BlockX - 1, a_BlockY, a_BlockZ + 1, a_BlockX - 1, a_BlockY, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock);
			SetBlockLinkedPowered(a_BlockX - 1, a_BlockY, a_BlockZ - 1, a_BlockX - 1, a_BlockY, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock);

			break;
		}
		case BLOCK_FACE_XP:
		{
			BLOCKTYPE MiddleBlock = m_World.GetBlock(a_BlockX + 1, a_BlockY, a_BlockZ);

			SetBlockLinkedPowered(a_BlockX + 2, a_BlockY, a_BlockZ, a_BlockX + 1, a_BlockY, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock);
			SetBlockLinkedPowered(a_BlockX + 1, a_BlockY + 1, a_BlockZ, a_BlockX + 1, a_BlockY, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock);
			SetBlockLinkedPowered(a_BlockX + 1, a_BlockY - 1, a_BlockZ, a_BlockX + 1, a_BlockY, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock);
			SetBlockLinkedPowered(a_BlockX + 1, a_BlockY, a_BlockZ + 1, a_BlockX + 1, a_BlockY, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock);
			SetBlockLinkedPowered(a_BlockX + 1, a_BlockY, a_BlockZ - 1, a_BlockX + 1, a_BlockY, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock);

			break;
		}
		case BLOCK_FACE_YM:
		{
			BLOCKTYPE MiddleBlock = m_World.GetBlock(a_BlockX, a_BlockY - 1, a_BlockZ);

			SetBlockLinkedPowered(a_BlockX, a_BlockY - 2, a_BlockZ, a_BlockX, a_BlockY - 1, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock);
			SetBlockLinkedPowered(a_BlockX + 1, a_BlockY - 1, a_BlockZ, a_BlockX, a_BlockY - 1, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock);
			SetBlockLinkedPowered(a_BlockX - 1, a_BlockY - 1, a_BlockZ, a_BlockX, a_BlockY - 1, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock);
			SetBlockLinkedPowered(a_BlockX, a_BlockY - 1, a_BlockZ + 1, a_BlockX, a_BlockY - 1, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock);
			SetBlockLinkedPowered(a_BlockX, a_BlockY - 1, a_BlockZ - 1, a_BlockX, a_BlockY - 1, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock);

			break;
		}
		case BLOCK_FACE_YP:
		{
			BLOCKTYPE MiddleBlock = m_World.GetBlock(a_BlockX, a_BlockY + 1, a_BlockZ);

			SetBlockLinkedPowered(a_BlockX, a_BlockY + 2, a_BlockZ, a_BlockX, a_BlockY + 1, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock);
			SetBlockLinkedPowered(a_BlockX + 1, a_BlockY + 1, a_BlockZ, a_BlockX, a_BlockY + 1, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock);
			SetBlockLinkedPowered(a_BlockX - 1, a_BlockY + 1, a_BlockZ, a_BlockX, a_BlockY + 1, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock);
			SetBlockLinkedPowered(a_BlockX, a_BlockY + 1, a_BlockZ + 1, a_BlockX, a_BlockY + 1, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock);
			SetBlockLinkedPowered(a_BlockX, a_BlockY + 1, a_BlockZ - 1, a_BlockX, a_BlockY + 1, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock);

			break;
		}
		case BLOCK_FACE_ZM:
		{
			BLOCKTYPE MiddleBlock = m_World.GetBlock(a_BlockX, a_BlockY, a_BlockZ - 1);
						
			SetBlockLinkedPowered(a_BlockX, a_BlockY, a_BlockZ - 2, a_BlockX, a_BlockY, a_BlockZ - 1, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock);
			SetBlockLinkedPowered(a_BlockX + 1, a_BlockY, a_BlockZ - 1, a_BlockX, a_BlockY, a_BlockZ - 1, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock);
			SetBlockLinkedPowered(a_BlockX - 1, a_BlockY, a_BlockZ - 1, a_BlockX, a_BlockY, a_BlockZ - 1, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock);
			SetBlockLinkedPowered(a_BlockX, a_BlockY + 1, a_BlockZ - 1, a_BlockX, a_BlockY, a_BlockZ - 1, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock);
			SetBlockLinkedPowered(a_BlockX, a_BlockY - 1, a_BlockZ - 1, a_BlockX, a_BlockY, a_BlockZ - 1, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock);

			break;
		}
		case BLOCK_FACE_ZP:
		{
			BLOCKTYPE MiddleBlock = m_World.GetBlock(a_BlockX, a_BlockY, a_BlockZ + 1);
			
			SetBlockLinkedPowered(a_BlockX, a_BlockY, a_BlockZ + 2, a_BlockX, a_BlockY, a_BlockZ + 1, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock);
			SetBlockLinkedPowered(a_BlockX + 1, a_BlockY, a_BlockZ + 1, a_BlockX, a_BlockY, a_BlockZ + 1, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock);
			SetBlockLinkedPowered(a_BlockX - 1, a_BlockY, a_BlockZ + 1, a_BlockX, a_BlockY, a_BlockZ + 1, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock);
			SetBlockLinkedPowered(a_BlockX, a_BlockY + 1, a_BlockZ + 1, a_BlockX, a_BlockY, a_BlockZ + 1, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock);
			SetBlockLinkedPowered(a_BlockX, a_BlockY - 1, a_BlockZ + 1, a_BlockX, a_BlockY, a_BlockZ + 1, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock);

			break;
		}
		default:
		{
			ASSERT(!"Unhandled face direction when attempting to set blocks as linked powered!"); // Zombies, that wasn't supposed to happen...
			break;
		}
	}
}





void cRedstoneSimulator::SetAllDirsAsPowered(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_SourceBlock)
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
		SetBlockPowered(a_BlockX + gCrossCoords[i].x, a_BlockY + gCrossCoords[i].y, a_BlockZ + gCrossCoords[i].z, a_BlockX, a_BlockY, a_BlockZ, a_SourceBlock);
	}
}





void cRedstoneSimulator::SetBlockPowered(int a_BlockX, int a_BlockY, int a_BlockZ, int a_SourceX, int a_SourceY, int a_SourceZ, BLOCKTYPE a_SourceBlock)
{
	BLOCKTYPE Block = m_World.GetBlock(a_BlockX, a_BlockY, a_BlockZ);
	if (Block == E_BLOCK_AIR)
	{
		// Don't set air, fixes some bugs (wires powering themselves)
		return;
	}
	if ((Block == E_BLOCK_REDSTONE_WIRE) && (a_SourceBlock == E_BLOCK_REDSTONE_WIRE))
	{
		// Wires cannot power themselves normally, instead, the wire handler will manually set meta
		return;
	}

	for (PoweredBlocksList::const_iterator itr = m_PoweredBlocks.begin(); itr != m_PoweredBlocks.end(); ++itr) // Check powered list
	{
		if (
			itr->a_BlockPos.Equals(Vector3i(a_BlockX, a_BlockY, a_BlockZ)) &&
			itr->a_SourcePos.Equals(Vector3i(a_SourceX, a_SourceY, a_SourceZ))
			)
		{
			// Check for duplicates
			return;
		}
	}

	sPoweredBlocks RC;
	RC.a_BlockPos = Vector3i(a_BlockX, a_BlockY, a_BlockZ);
	RC.a_SourcePos = Vector3i(a_SourceX, a_SourceY, a_SourceZ);
	RC.a_SourceBlock = a_SourceBlock;
	m_PoweredBlocks.push_back(RC);
}





void cRedstoneSimulator::SetBlockLinkedPowered(
	int a_BlockX,  int a_BlockY,  int a_BlockZ,
	int a_MiddleX, int a_MiddleY, int a_MiddleZ,
	int a_SourceX, int a_SourceY, int a_SourceZ,
	BLOCKTYPE a_SourceBlock, BLOCKTYPE a_MiddleBlock
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
	if ((a_SourceBlock == E_BLOCK_REDSTONE_WIRE) && (DestBlock == E_BLOCK_REDSTONE_WIRE))
	{
		// Wires cannot power another wire through a block
		return;
	}

	for (LinkedBlocksList::const_iterator itr = m_LinkedPoweredBlocks.begin(); itr != m_LinkedPoweredBlocks.end(); ++itr) // Check linked powered list
	{
		if (
			itr->a_BlockPos.Equals(Vector3i(a_BlockX, a_BlockY, a_BlockZ)) &&
			itr->a_MiddlePos.Equals(Vector3i(a_MiddleX, a_MiddleY, a_MiddleZ)) &&
			itr->a_SourcePos.Equals(Vector3i(a_SourceX, a_SourceY, a_SourceZ))
			)
		{
			// Check for duplicates
			return;
		}
	}

	sLinkedPoweredBlocks RC;
	RC.a_BlockPos = Vector3i(a_BlockX, a_BlockY, a_BlockZ);
	RC.a_MiddlePos = Vector3i(a_MiddleX, a_MiddleY, a_MiddleZ);
	RC.a_SourcePos = Vector3i(a_SourceX, a_SourceY, a_SourceZ);
	RC.a_SourceBlock = a_SourceBlock;
	RC.a_MiddleBlock = a_MiddleBlock;
	m_LinkedPoweredBlocks.push_back(RC);
}





void cRedstoneSimulator::SetPlayerToggleableBlockAsSimulated(int a_BlockX, int a_BlockY, int a_BlockZ, bool WasLastStatePowered)
{
	for (SimulatedPlayerToggleableList::iterator itr = m_SimulatedPlayerToggleableBlocks.begin(); itr != m_SimulatedPlayerToggleableBlocks.end(); ++itr)
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
	m_SimulatedPlayerToggleableBlocks.push_back(RC);
}





void cRedstoneSimulator::QueueRepeaterPowerChange(int a_BlockX, int a_BlockY, int a_BlockZ, NIBBLETYPE a_Meta, short a_ElapsedTicks, bool ShouldPowerOn)
{
	for (RepeatersDelayList::iterator itr = m_RepeatersDelayList.begin(); itr != m_RepeatersDelayList.end(); itr++)
	{
		if (itr->a_BlockPos.Equals(Vector3i(a_BlockX, a_BlockY, a_BlockZ)))
		{
			if (ShouldPowerOn == itr->ShouldPowerOn) // We are queued already for the same thing, don't replace entry
			{
				return;
			}

			// Already in here (normal to allow repeater to continue on powering and updating blocks in front) - just update info and quit
			itr->a_DelayTicks = (((a_Meta & 0xC) >> 0x2) + (ShouldPowerOn ? 1 : 0)) * 2; // See below for description
			itr->a_ElapsedTicks = 0;
			itr->ShouldPowerOn = ShouldPowerOn;
			return;
		}
	}

	// Self not in list, add self to list
	sRepeatersDelayList RC;
	RC.a_BlockPos = Vector3i(a_BlockX, a_BlockY, a_BlockZ);
	
	// Gets the top two bits (delay time), shifts them into the lower two bits, and adds one (meta 0 = 1 tick; 1 = 2 etc.)
	// * 2 because apparently, MCS ticks are way faster than vanilla ticks, so repeater aren't noticeably delayed
	// We don't +1 when powering off because everything seems to already delay a tick when powering off, why? No idea :P
	RC.a_DelayTicks = (((a_Meta & 0xC) >> 0x2) + (ShouldPowerOn ? 1 : 0)) * 2;


	RC.a_ElapsedTicks = 0;
	RC.ShouldPowerOn = ShouldPowerOn;
	m_RepeatersDelayList.push_back(RC);
	return;
}





cRedstoneSimulator::eRedstoneDirection cRedstoneSimulator::GetWireDirection(int a_BlockX, int a_BlockY, int a_BlockZ)
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





bool cRedstoneSimulator::IsLeverOn(NIBBLETYPE a_BlockMeta)
{
	// Extract the ON bit from metadata and return if true if it is set:
	return ((a_BlockMeta & 0x8) == 0x8);
}





bool cRedstoneSimulator::IsButtonOn(NIBBLETYPE a_BlockMeta)
{
	return IsLeverOn(a_BlockMeta);
}




