
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "RedstoneSimulator.h"
#include "../BlockEntities/DropSpenserEntity.h"
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
	for (cRedstoneSimulatorChunkData::iterator itr = ChunkData.begin(); itr != ChunkData.end(); ++itr)
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

	for (cRedstoneSimulatorChunkData::const_iterator dataitr = ChunkData.begin(), end = ChunkData.end(); dataitr != end;)
	{
		BLOCKTYPE BlockType = a_Chunk->GetBlock(dataitr->x, dataitr->y, dataitr->z);
		if (!IsAllowedBlock(BlockType))
		{
			dataitr = ChunkData.erase(dataitr);
			continue;
		}

		// Check to see if PoweredBlocks have invalid items (source is air or an unpowered source)
		for (PoweredBlocksList::iterator itr = m_PoweredBlocks.begin(); itr != m_PoweredBlocks.end();)
		{
			sPoweredBlocks & Change = *itr;
			BLOCKTYPE SourceBlockType = m_World.GetBlock(Change.a_SourcePos);

			if (SourceBlockType != Change.a_SourceBlock)
			{
				itr = m_PoweredBlocks.erase(itr);
			}
			else if (
				// Changeable sources
				((SourceBlockType == E_BLOCK_REDSTONE_WIRE) && (m_World.GetBlockMeta(Change.a_SourcePos) == 0)) ||
				((SourceBlockType == E_BLOCK_LEVER) && !IsLeverOn(m_World.GetBlockMeta(Change.a_SourcePos))) ||
				((SourceBlockType == E_BLOCK_DETECTOR_RAIL) && (m_World.GetBlockMeta(Change.a_SourcePos) & 0x08) == 0x08) ||
				(((SourceBlockType == E_BLOCK_STONE_BUTTON) || (SourceBlockType == E_BLOCK_WOODEN_BUTTON)) && (!IsButtonOn(m_World.GetBlockMeta(Change.a_SourcePos))))
				)
			{
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
			sLinkedPoweredBlocks & Change = *itr;
			BLOCKTYPE SourceBlockType = m_World.GetBlock(Change.a_SourcePos);
			BLOCKTYPE MiddleBlockType = m_World.GetBlock(Change.a_MiddlePos);

			if (SourceBlockType != Change.a_SourceBlock)
			{
				itr = m_LinkedPoweredBlocks.erase(itr);
			}
			else if (MiddleBlockType != Change.a_MiddleBlock)
			{
				itr = m_LinkedPoweredBlocks.erase(itr);
			}
			else if (
				// Things that can send power through a block but which depends on meta
				((SourceBlockType == E_BLOCK_REDSTONE_WIRE) && (m_World.GetBlockMeta(Change.a_SourcePos) == 0)) ||
				((SourceBlockType == E_BLOCK_LEVER) && !IsLeverOn(m_World.GetBlockMeta(Change.a_SourcePos))) ||
				(((SourceBlockType == E_BLOCK_STONE_BUTTON) || (SourceBlockType == E_BLOCK_WOODEN_BUTTON)) && (!IsButtonOn(m_World.GetBlockMeta(Change.a_SourcePos))))
				)
			{
				itr = m_LinkedPoweredBlocks.erase(itr);
			}
			else
			{
				itr++;
			}
		}

		// PoweredBlock list was fine, now to the actual handling
		int a_X = BaseX + dataitr->x;
		int a_Z = BaseZ + dataitr->z;
		switch (BlockType)
		{
			case E_BLOCK_BLOCK_OF_REDSTONE:		HandleRedstoneBlock(a_X, dataitr->y, a_Z);	break;
			case E_BLOCK_LEVER: 				HandleRedstoneLever(a_X, dataitr->y, a_Z);	break;
			case E_BLOCK_TNT:					HandleTNT(a_X, dataitr->y, a_Z);			break;
			case E_BLOCK_REDSTONE_WIRE:			HandleRedstoneWire(a_X, dataitr->y, a_Z);	break;

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

		if (AreCoordsPowered(X, Y, Z))
		{
			// There was a match, torch goes off
			// FastSetBlock so the server doesn't fail an assert -_-
			m_World.FastSetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_REDSTONE_TORCH_OFF, m_World.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ));
			return;
		}

		// Torch still on, make all 4(X, Z) + 1(Y) sides powered
		for (int i = 0; i < ARRAYCOUNT(gCrossCoords); i++)
		{
			BLOCKTYPE Type = m_World.GetBlock(a_BlockX + gCrossCoords[i].x, a_BlockY + gCrossCoords[i].y, a_BlockZ + gCrossCoords[i].z);
			if (i < ARRAYCOUNT(gCrossCoords) - 1) // Sides of torch, not top (top is last)
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
			}
		}

		if (m_World.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ) != 0x5) // Is torch standing on ground? If not (i.e. on wall), power block beneath
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
		if (AreCoordsPowered(X, Y, Z))
		{
			return; // Something matches, torch still powered
		}

		// Block torch on not powered, can be turned on again!
		// FastSetBlock so the server doesn't fail an assert -_-
		m_World.FastSetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_REDSTONE_TORCH_ON, m_World.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ));
	}
	return;
}





void cRedstoneSimulator::HandleRedstoneBlock(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	static const struct // Define which directions the redstone block can power
	{
		int x, y, z;
	} gCrossCoords[] =
	{
		{ 0, 0,  0}, // Oh, anomalous redstone. Only block that powers itself
		{ 1, 0,  0},
		{-1, 0,  0},
		{ 0, 0,  1},
		{ 0, 0, -1},
		{ 0, 1,  0},
		{ 0,-1,  0},
	} ;

	for (int i = 0; i < ARRAYCOUNT(gCrossCoords); i++)
	{
		// Power everything
		SetBlockPowered(a_BlockX + gCrossCoords[i].x, a_BlockY + gCrossCoords[i].y, a_BlockZ + gCrossCoords[i].z, a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_BLOCK_OF_REDSTONE);
	}
	return;
}





void cRedstoneSimulator::HandleRedstoneLever(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	if (IsLeverOn(m_World.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ)))
	{
		static const struct // Define which directions the redstone lever can power (all sides)
		{
			int x, y, z;
		} gCrossCoords[] =
		{
			{ 1, 0,  0},
			{-1, 0,  0},
			{ 0, 0,  1},
			{ 0, 0, -1},
			{ 0, 1,  0},
			{ 0,-1,  0},
		} ;

		for (int i = 0; i < ARRAYCOUNT(gCrossCoords); i++)
		{
			// Power everything
			SetBlockPowered(a_BlockX + gCrossCoords[i].x, a_BlockY + gCrossCoords[i].y, a_BlockZ + gCrossCoords[i].z, a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_LEVER);
		}

		SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_XM, E_BLOCK_LEVER);
		SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_XP, E_BLOCK_LEVER);
		SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_YM, E_BLOCK_LEVER);
		SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_YP, E_BLOCK_LEVER);
		SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_ZM, E_BLOCK_LEVER);
		SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_ZP, E_BLOCK_LEVER);
	}
	return;
}





void cRedstoneSimulator::HandleRedstoneButton(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType)
{
	if (IsButtonOn(m_World.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ)))
	{
		static const struct // Define which directions the redstone button can power (all sides)
		{
			int x, y, z;
		} gCrossCoords[] =
		{
			{ 1, 0,  0},
			{-1, 0,  0},
			{ 0, 0,  1},
			{ 0, 0, -1},
			{ 0, 1,  0},
			{ 0,-1,  0},
		} ;

		for (int i = 0; i < ARRAYCOUNT(gCrossCoords); i++)
		{
			// Power everything
			SetBlockPowered(a_BlockX + gCrossCoords[i].x, a_BlockY + gCrossCoords[i].y, a_BlockZ + gCrossCoords[i].z, a_BlockX, a_BlockY, a_BlockZ, a_BlockType);
		}

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
		{ 1, 0,  0},
		{-1, 0,  0},
		{ 0, 0,  1},
		{ 0, 0, -1},
		{ 1, 1,  0}, // From here to end, check for wire placed on sides of blocks
		{-1, 1,  0},
		{ 0, 1,  1},
		{ 0, 1, -1},
		{ 1,-1,  0},
		{-1,-1,  0},
		{ 0,-1,  1},
		{ 0,-1, -1},
	} ;

	// Check to see if directly beside a power source
	if (AreCoordsPowered(a_BlockX, a_BlockY, a_BlockZ))
	{
		m_World.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, 15); // Maximum power
	}
	else
	{
		NIBBLETYPE MyMeta = m_World.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);
		int TimesMetaSmaller = 0, TimesFoundAWire = 0;

		for (int i = 0; i < ARRAYCOUNT(gCrossCoords); i++) // Loop through all directions to transfer or receive power
		{
			BLOCKTYPE SurroundType;
			NIBBLETYPE SurroundMeta;
			m_World.GetBlockTypeMeta(a_BlockX + gCrossCoords[i].x, a_BlockY + gCrossCoords[i].y, a_BlockZ + gCrossCoords[i].z, SurroundType, SurroundMeta);

			if (SurroundType == E_BLOCK_REDSTONE_WIRE)
			{
				TimesFoundAWire++;

				if (SurroundMeta > 1) // Wires of power 1 or 0 cannot transfer power TO ME, don't bother checking
				{
					if (SurroundMeta > MyMeta) // Does surrounding wire have a higher power level than self?
					{
						m_World.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, SurroundMeta - 1);
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
		}		
	}

	if (m_World.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ) != 0) // A powered wire
	{
		//SetBlockPowered(a_BlockX, a_BlockY - 1, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_REDSTONE_WIRE); // No matter what, block underneath gets powered

		switch (GetWireDirection(a_BlockX, a_BlockY, a_BlockZ))
		{
			case REDSTONE_NONE:
			{
				static const struct // Define which directions the redstone wire can power
				{
					int x, y, z;
				} gCrossCoords[] =
				{
					{ 1, 0,  0}, // Power block in front
					{ 2, 0,  0}, // Power block in front of that (strongly power)
					{-1, 0,  0},
					{-2, 0,  0},
					{ 0, 0,  1},
					{ 0, 0,  2},
					{ 0, 0, -1},
					{ 0, 0, -2},
					{ 0, 1,  0},
					{ 0, 2,  0},
					{ 0,-1,  0},
					{ 0,-2,  0},
				} ;

				for (int i = 0; i < ARRAYCOUNT(gCrossCoords); i++)
				{
					// Power if block is solid, CURRENTLY all mechanisms are solid
					if (g_BlockIsSolid[m_World.GetBlock(a_BlockX + gCrossCoords[i].x, a_BlockY + gCrossCoords[i].y, a_BlockZ + gCrossCoords[i].z)])
					{
						SetBlockPowered(a_BlockX + gCrossCoords[i].x, a_BlockY + gCrossCoords[i].y, a_BlockZ + gCrossCoords[i].z, a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_REDSTONE_WIRE);
					}
				}
				break;
			}
			case REDSTONE_X_POS:
			{
				if (m_World.GetBlock(a_BlockX + 1, a_BlockY, a_BlockZ) != E_BLOCK_REDSTONE_WIRE)
				{
					SetBlockPowered(a_BlockX + 1, a_BlockY, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_REDSTONE_WIRE);
				}
				SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_XP, E_BLOCK_REDSTONE_WIRE);
				break;
			}
			case REDSTONE_X_NEG:
			{
				if (m_World.GetBlock(a_BlockX - 1, a_BlockY, a_BlockZ) != E_BLOCK_REDSTONE_WIRE)
				{
					SetBlockPowered(a_BlockX - 1, a_BlockY, a_BlockZ, a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_REDSTONE_WIRE);
				}
				SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_XM, E_BLOCK_REDSTONE_WIRE);
				break;
			}
			case REDSTONE_Z_POS:
			{
				if (m_World.GetBlock(a_BlockX, a_BlockY, a_BlockZ + 1) != E_BLOCK_REDSTONE_WIRE)
				{
					SetBlockPowered(a_BlockX, a_BlockY, a_BlockZ + 1, a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_REDSTONE_WIRE);
				}
				SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_ZP, E_BLOCK_REDSTONE_WIRE);
				break;
			}
			case REDSTONE_Z_NEG:
			{
				if (m_World.GetBlock(a_BlockX, a_BlockY, a_BlockZ - 1) != E_BLOCK_REDSTONE_WIRE)
				{
					SetBlockPowered(a_BlockX, a_BlockY, a_BlockZ - 1, a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_REDSTONE_WIRE);
				}
				SetDirectionLinkedPowered(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_ZM, E_BLOCK_REDSTONE_WIRE);
				break;
			}
		}
	}
	return;
}





void cRedstoneSimulator::HandleRedstoneRepeater(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_MyState)
{
	NIBBLETYPE a_Meta = m_World.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);
	if (a_MyState == E_BLOCK_REDSTONE_REPEATER_OFF)
	{
		if (IsRepeaterPowered(a_BlockX, a_BlockY, a_BlockZ, a_Meta & 0x3))
		{
			m_World.FastSetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_REDSTONE_REPEATER_ON, a_Meta);
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
		}
	}
	else
	{
		if (!IsRepeaterPowered(a_BlockX, a_BlockY, a_BlockZ, a_Meta & 0x3))
		{
			m_World.FastSetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_REDSTONE_REPEATER_OFF, a_Meta);
		}
	}
	return;
}





void cRedstoneSimulator::HandlePiston(int a_BlockX, int a_BlockY, int a_BlockZ)
{	
	if (AreCoordsPowered(a_BlockX, a_BlockY, a_BlockZ))
	{
		cPiston Piston(&m_World);
		Piston.ExtendPiston(a_BlockX, a_BlockY, a_BlockZ);
	}
	else
	{
		cPiston Piston(&m_World);
		Piston.RetractPiston(a_BlockX, a_BlockY, a_BlockZ);
	}
	return;
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
	return;
}





void cRedstoneSimulator::HandleRedstoneLamp(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_MyState)
{
	if (a_MyState == E_BLOCK_REDSTONE_LAMP_OFF)
	{
		if (AreCoordsPowered(a_BlockX, a_BlockY, a_BlockZ))
		{
			m_World.FastSetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_REDSTONE_LAMP_ON, 0);
		}
	}
	else
	{
		if (!AreCoordsPowered(a_BlockX, a_BlockY, a_BlockZ))
		{
			m_World.FastSetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_REDSTONE_LAMP_OFF, 0);
		}
	}
	return;
}





void cRedstoneSimulator::HandleTNT(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	if (AreCoordsPowered(a_BlockX, a_BlockY, a_BlockZ))
	{
		m_World.BroadcastSoundEffect("random.fuse", a_BlockX * 8, a_BlockY * 8, a_BlockZ * 8, 0.5f, 0.6f);
		m_World.SpawnPrimedTNT(a_BlockX + 0.5, a_BlockY + 0.5, a_BlockZ + 0.5, 4);  // 4 seconds to boom
		m_World.FastSetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_AIR, 0);
	}
	return;
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
	return;
}





void cRedstoneSimulator::HandleRail(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_MyType)
{
	switch (a_MyType)
	{
		case E_BLOCK_DETECTOR_RAIL:
		{
			if ((m_World.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ) & 0x08) == 0x08)
			{
				static const struct // Define which directions the rail can power (all sides)
				{
					int x, y, z;
				} gCrossCoords[] =
				{
					{ 1, 0,  0},
					{-1, 0,  0},
					{ 0, 0,  1},
					{ 0, 0, -1},
					{ 0, 1,  0},
					{ 0,-1,  0},
				} ;

				for (int i = 0; i < ARRAYCOUNT(gCrossCoords); i++)
				{
					// Power everything
					SetBlockPowered(a_BlockX + gCrossCoords[i].x, a_BlockY + gCrossCoords[i].y, a_BlockZ + gCrossCoords[i].z, a_BlockX, a_BlockY, a_BlockZ, a_MyType);
				}
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
	}
}





bool cRedstoneSimulator::AreCoordsPowered(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	for (PoweredBlocksList::iterator itr = m_PoweredBlocks.begin(); itr != m_PoweredBlocks.end(); ++itr) // Check powered list
	{
		sPoweredBlocks & Change = *itr;

		if (Change.a_BlockPos.Equals(Vector3i(a_BlockX, a_BlockY, a_BlockZ)))
		{
			return true;
		}
	}

	for (LinkedBlocksList::iterator itr = m_LinkedPoweredBlocks.begin(); itr != m_LinkedPoweredBlocks.end(); ++itr) // Check linked powered list
	{
		sLinkedPoweredBlocks & Change = *itr;

		if (Change.a_BlockPos.Equals(Vector3i(a_BlockX, a_BlockY, a_BlockZ)))
		{
			return true;
		}
	}
	return false;
}





bool cRedstoneSimulator::IsRepeaterPowered(int a_BlockX, int a_BlockY, int a_BlockZ, NIBBLETYPE a_Meta)
{
	// Check through powered blocks list
	for (PoweredBlocksList::iterator itr = m_PoweredBlocks.begin(); itr != m_PoweredBlocks.end(); ++itr)
	{
		sPoweredBlocks & Change = *itr;

		switch (a_Meta)
		{
			case 0x0:
			{
				// Flip the coords to check the back of the repeater
				if (Change.a_SourcePos.Equals(Vector3i(a_BlockX, a_BlockY, a_BlockZ + 1))) { return true; }
				break;
			}
			case 0x1:
			{
				if (Change.a_SourcePos.Equals(Vector3i(a_BlockX - 1, a_BlockY, a_BlockZ))) { return true; }
				break;
			}
			case 0x2:
			{
				if (Change.a_SourcePos.Equals(Vector3i(a_BlockX, a_BlockY, a_BlockZ - 1))) { return true; }
				break;
			}
			case 0x3:
			{
				if (Change.a_SourcePos.Equals(Vector3i(a_BlockX + 1, a_BlockY, a_BlockZ))) { return true; }
				break;
			}
		}
	}

	// Check linked powered list, 'middle' blocks
	for (LinkedBlocksList::iterator itr = m_LinkedPoweredBlocks.begin(); itr != m_LinkedPoweredBlocks.end(); ++itr)
	{
		sLinkedPoweredBlocks & Change = *itr;

		switch (a_Meta)
		{
			case 0x0:
			{
				if (Change.a_MiddlePos.Equals(Vector3i(a_BlockX, a_BlockY, a_BlockZ + 1))) { return true; }
				break;
			}
			case 0x1:
			{
				if (Change.a_MiddlePos.Equals(Vector3i(a_BlockX - 1, a_BlockY, a_BlockZ))) { return true; }
				break;
			}
			case 0x2:
			{
				if (Change.a_MiddlePos.Equals(Vector3i(a_BlockX, a_BlockY, a_BlockZ - 1))) { return true; }
				break;
			}
			case 0x3:
			{
				if (Change.a_MiddlePos.Equals(Vector3i(a_BlockX + 1, a_BlockY, a_BlockZ))) { return true; }
				break;
			}
		}
	}
	return false; // Couldn't find power source behind repeater
}





void cRedstoneSimulator::SetDirectionLinkedPowered(int a_BlockX, int a_BlockY, int a_BlockZ, char a_Direction, BLOCKTYPE a_SourceType)
{
	switch (a_Direction)
	{
		case BLOCK_FACE_XM:
		{
			BLOCKTYPE MiddleBlock = m_World.GetBlock(a_BlockX - 1, a_BlockY, a_BlockZ);
			if (!g_BlockIsSolid[MiddleBlock]) { return; }

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
			if (!g_BlockIsSolid[MiddleBlock]) { return; }

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
			if (!g_BlockIsSolid[MiddleBlock]) { return; }

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
			if (!g_BlockIsSolid[MiddleBlock]) { return; }

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
			if (!g_BlockIsSolid[MiddleBlock]) { return; }

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
			if (!g_BlockIsSolid[MiddleBlock]) { return; }

			SetBlockLinkedPowered(a_BlockX, a_BlockY, a_BlockZ + 2, a_BlockX, a_BlockY, a_BlockZ + 1, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock);
			SetBlockLinkedPowered(a_BlockX + 1, a_BlockY, a_BlockZ + 1, a_BlockX, a_BlockY, a_BlockZ + 1, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock);
			SetBlockLinkedPowered(a_BlockX - 1, a_BlockY, a_BlockZ + 1, a_BlockX, a_BlockY, a_BlockZ + 1, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock);
			SetBlockLinkedPowered(a_BlockX, a_BlockY + 1, a_BlockZ + 1, a_BlockX, a_BlockY, a_BlockZ + 1, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock);
			SetBlockLinkedPowered(a_BlockX, a_BlockY - 1, a_BlockZ + 1, a_BlockX, a_BlockY, a_BlockZ + 1, a_BlockX, a_BlockY, a_BlockZ, a_SourceType, MiddleBlock);
			break;
		}
		default:
		{
			ASSERT(!"Unhandled face direction when attempting to set blocks as linked powered!");
			break;
		}
	}
	return;
}





void cRedstoneSimulator::SetBlockPowered(int a_BlockX, int a_BlockY, int a_BlockZ, int a_SourceX, int a_SourceY, int a_SourceZ, BLOCKTYPE a_SourceBlock)
{
	if (AreCoordsPowered(a_BlockX, a_BlockY, a_BlockZ)) { return; } // Check for duplicates

	sPoweredBlocks RC;
	RC.a_BlockPos = Vector3i(a_BlockX, a_BlockY, a_BlockZ);
	RC.a_SourcePos = Vector3i(a_SourceX, a_SourceY, a_SourceZ);
	RC.a_SourceBlock = a_SourceBlock;
	m_PoweredBlocks.push_back(RC);
	return;
}





void cRedstoneSimulator::SetBlockLinkedPowered(int a_BlockX, int a_BlockY, int a_BlockZ,
	int a_MiddleX, int a_MiddleY, int a_MiddleZ,
	int a_SourceX, int a_SourceY, int a_SourceZ,
	BLOCKTYPE a_SourceBlock, BLOCKTYPE a_MiddleBlock
	)
{
	if (AreCoordsPowered(a_BlockX, a_BlockY, a_BlockZ)) { return; } // Check for duplicates

	sLinkedPoweredBlocks RC;
	RC.a_BlockPos = Vector3i(a_BlockX, a_BlockY, a_BlockZ);
	RC.a_MiddlePos = Vector3i(a_MiddleX, a_MiddleY, a_MiddleZ);
	RC.a_SourcePos = Vector3i(a_SourceX, a_SourceY, a_SourceZ);
	RC.a_SourceBlock = a_SourceBlock;
	RC.a_MiddleBlock = a_MiddleBlock;
	m_LinkedPoweredBlocks.push_back(RC);
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




