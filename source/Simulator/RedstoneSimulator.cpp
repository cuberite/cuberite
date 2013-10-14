
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





void cRedstoneSimulator::WakeUp(int a_BlockX, int a_BlockY, int a_BlockZ, cChunk * a_Chunk)
{
	if (a_Chunk == NULL)
	{
		return;
	}

	int RelX = a_BlockX - a_Chunk->GetPosX() * cChunkDef::Width;
	int RelZ = a_BlockZ - a_Chunk->GetPosZ() * cChunkDef::Width;

	// Check if any close neighbor is redstone-related:
	int MinY = (a_BlockY > 0) ? -4 : 0;
	int MaxY = (a_BlockY < cChunkDef::Height - 1) ? 4 : 0;

	for (int y = MinY; y <= MaxY; y++) for (int x = -4; x < 5; x++) for (int z = -4; z < 5; z++)
	{
		BLOCKTYPE BlockType;
		if (!a_Chunk->UnboundedRelGetBlockType(RelX + x, a_BlockY + y, RelZ + z, BlockType))
		{
			continue; // Noo! Bad chunks!
		}
		switch (BlockType)
		{
			// All redstone devices, please alpha sort
			case E_BLOCK_ACTIVATOR_RAIL:
			case E_BLOCK_ACTIVE_COMPARATOR:
			case E_BLOCK_BLOCK_OF_REDSTONE:
			case E_BLOCK_DETECTOR_RAIL:
			case E_BLOCK_DISPENSER:
			case E_BLOCK_DAYLIGHT_SENSOR:
			case E_BLOCK_DROPPER:
			case E_BLOCK_FENCE_GATE:
			case E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE:
			case E_BLOCK_HOPPER:
			case E_BLOCK_INACTIVE_COMPARATOR:
			case E_BLOCK_IRON_DOOR:
			case E_BLOCK_LEVER:
			case E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE:
			case E_BLOCK_NOTE_BLOCK:
			case E_BLOCK_REDSTONE_LAMP_OFF:
			case E_BLOCK_REDSTONE_LAMP_ON:
			case E_BLOCK_REDSTONE_REPEATER_OFF:
			case E_BLOCK_REDSTONE_REPEATER_ON:
			case E_BLOCK_REDSTONE_TORCH_OFF:
			case E_BLOCK_REDSTONE_TORCH_ON:
			case E_BLOCK_REDSTONE_WIRE:
			case E_BLOCK_STICKY_PISTON:
			case E_BLOCK_STONE_BUTTON:
			case E_BLOCK_STONE_PRESSURE_PLATE:
			case E_BLOCK_TNT:
			case E_BLOCK_TRAPDOOR:
			case E_BLOCK_TRIPWIRE_HOOK:
			case E_BLOCK_WOODEN_BUTTON:
			case E_BLOCK_WOODEN_DOOR:
			case E_BLOCK_WOODEN_PRESSURE_PLATE:
			case E_BLOCK_PISTON:
			{
				m_Blocks.push_back(Vector3i(a_BlockX + x, a_BlockY + y, a_BlockZ + z)); // Push back all redstone blocks
			}
		}
	}
	return;
}





void cRedstoneSimulator::Simulate(float a_Dt)
{
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
			((SourceBlockType == E_BLOCK_LEVER) && (!IsLeverOn(m_World.GetBlockMeta(Change.a_SourcePos)))) ||
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
			// The only things that can send power through a block
			((SourceBlockType == E_BLOCK_REDSTONE_WIRE) && (m_World.GetBlockMeta(Change.a_SourcePos) == 0))
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
	for (BlockList::iterator itr = m_Blocks.begin(); itr != m_Blocks.end(); ++itr)
	{
		BLOCKTYPE BlockType = m_World.GetBlock(*itr);

		switch (BlockType)
		{
			case E_BLOCK_REDSTONE_TORCH_OFF:
			case E_BLOCK_REDSTONE_TORCH_ON:
			{
				HandleRedstoneTorch(*itr, BlockType);
				break;
			}
			case E_BLOCK_BLOCK_OF_REDSTONE:
			{
				HandleRedstoneBlock(*itr);
				break;
			}
			case E_BLOCK_LEVER:
			{
				HandleRedstoneLever(*itr);
				break;
			}
			case E_BLOCK_STONE_BUTTON:
			case E_BLOCK_WOODEN_BUTTON:
			{
				HandleRedstoneButton(*itr, BlockType);
				break;
			}
			case E_BLOCK_REDSTONE_WIRE:
			{
				HandleRedstoneWire(*itr);
				break;
			}
			case E_BLOCK_REDSTONE_REPEATER_OFF:
			case E_BLOCK_REDSTONE_REPEATER_ON:
			{
				HandleRedstoneRepeater(*itr);
				break;
			}
			case E_BLOCK_PISTON:
			case E_BLOCK_STICKY_PISTON:
			{
				HandlePiston(*itr);
				break;
			}
			case E_BLOCK_REDSTONE_LAMP_OFF:
			case E_BLOCK_REDSTONE_LAMP_ON:
			{
				HandleRedstoneLamp(*itr, BlockType);
				break;
			}
			case E_BLOCK_DISPENSER:
			case E_BLOCK_DROPPER:
			{
				HandleDropSpenser(*itr);
				break;
			}
			case E_BLOCK_TNT:
			{
				HandleTNT(*itr);
				break;
			}
			case E_BLOCK_WOODEN_DOOR:
			case E_BLOCK_IRON_DOOR:
			{
				HandleDoor(*itr);
				break;
			}
		}
	}

	m_Blocks.clear(); // Clear blocks list as (hopefully :P) everything was handled
	return;
}





void cRedstoneSimulator::HandleRedstoneTorch(const Vector3i & a_BlockPos, BLOCKTYPE a_MyState)
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
		int X = a_BlockPos.x; int Y = a_BlockPos.y; int Z = a_BlockPos.z;
		AddFaceDirection(X, Y, Z, cBlockTorchHandler::MetaDataToDirection(m_World.GetBlockMeta(a_BlockPos)), true); // Inverse true to get the block torch is on

		if (AreCoordsPowered(Vector3i(X, Y, Z)))
		{
			// There was a match, torch goes off
			// FastSetBlock so the server doesn't fail an assert -_-
			m_World.FastSetBlock(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z, E_BLOCK_REDSTONE_TORCH_OFF, m_World.GetBlockMeta(a_BlockPos));
			return;
		}

		// Torch still on, make all 4(X, Z) + 1(Y) sides powered
		for (int i = 0; i < ARRAYCOUNT(gCrossCoords); i++)
		{
			BLOCKTYPE Type = m_World.GetBlock(a_BlockPos.x + gCrossCoords[i].x, a_BlockPos.y + gCrossCoords[i].y, a_BlockPos.z + gCrossCoords[i].z);
			if (i < ARRAYCOUNT(gCrossCoords) - 1) // Sides of torch, not top (top is last)
			{
				if (
					((IsMechanism(Type)) || (Type == E_BLOCK_REDSTONE_WIRE)) && // Is it a mechanism or wire? Not block/other torch etc.
					(!Vector3i(a_BlockPos.x + gCrossCoords[i].x, a_BlockPos.y + gCrossCoords[i].y, a_BlockPos.z + gCrossCoords[i].z).Equals(Vector3i(X, Y, Z))) // CAN'T power block is that it is on
					)
				{
					SetBlockPowered(Vector3i(a_BlockPos.x + gCrossCoords[i].x, a_BlockPos.y + gCrossCoords[i].y, a_BlockPos.z + gCrossCoords[i].z), a_BlockPos, E_BLOCK_REDSTONE_TORCH_ON);
				}
			}
			else
			{
				// Top side, power whatever is there, including blocks
				SetBlockPowered(Vector3i(a_BlockPos.x + gCrossCoords[i].x, a_BlockPos.y + gCrossCoords[i].y, a_BlockPos.z + gCrossCoords[i].z), a_BlockPos, E_BLOCK_REDSTONE_TORCH_ON);
			}
		}

		if (m_World.GetBlockMeta(a_BlockPos) != 0x5) // Is torch standing on ground? If not (on wall), power block beneath
		{
			BLOCKTYPE Type = m_World.GetBlock(a_BlockPos.x, a_BlockPos.y - 1, a_BlockPos.z);

			if ((IsMechanism(Type)) || (Type == E_BLOCK_REDSTONE_WIRE)) // Still can't make a normal block powered though!
			{
				SetBlockPowered(Vector3i(a_BlockPos.x, a_BlockPos.y - 1, a_BlockPos.z), a_BlockPos, E_BLOCK_REDSTONE_TORCH_ON);
			}
		}
	}
	else
	{
		// See if off state torch can be turned on again
		if (AreCoordsPowered(Vector3i(a_BlockPos.x, a_BlockPos.y - 1, a_BlockPos.z)))
		{
			return; // Something matches, torch still powered
		}

		// BlockBelowNotPowered wasn't changed, the torch is no longer powered
		// FastSetBlock so the server doesn't fail an assert -_-
		m_World.FastSetBlock(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z, E_BLOCK_REDSTONE_TORCH_ON, m_World.GetBlockMeta(a_BlockPos));
	}
	return;
}





void cRedstoneSimulator::HandleRedstoneBlock(const Vector3i & a_BlockPos)
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
		SetBlockPowered(Vector3i(a_BlockPos.x + gCrossCoords[i].x, a_BlockPos.y + gCrossCoords[i].y, a_BlockPos.z + gCrossCoords[i].z), a_BlockPos, E_BLOCK_BLOCK_OF_REDSTONE);
	}
	return;
}





void cRedstoneSimulator::HandleRedstoneLever(const Vector3i & a_BlockPos)
{
	if (IsLeverOn(m_World.GetBlockMeta(a_BlockPos)))
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
			SetBlockPowered(Vector3i(a_BlockPos.x + gCrossCoords[i].x, a_BlockPos.y + gCrossCoords[i].y, a_BlockPos.z + gCrossCoords[i].z), a_BlockPos, E_BLOCK_LEVER);
		}
	}
	return;
}





void cRedstoneSimulator::HandleRedstoneButton(const Vector3i & a_BlockPos, BLOCKTYPE a_BlockType)
{
	if (IsButtonOn(m_World.GetBlockMeta(a_BlockPos)))
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
			SetBlockPowered(Vector3i(a_BlockPos.x + gCrossCoords[i].x, a_BlockPos.y + gCrossCoords[i].y, a_BlockPos.z + gCrossCoords[i].z), a_BlockPos, a_BlockType);
		}
	}
}





void cRedstoneSimulator::HandleRedstoneWire(const Vector3i & a_BlockPos)
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
	if (AreCoordsPowered(a_BlockPos))
	{
		m_World.SetBlockMeta(a_BlockPos, 15); // Maximum power

		for (int i = 0; i < ARRAYCOUNT(gCrossCoords); i++)
		{
			BLOCKTYPE Type;
			NIBBLETYPE Meta;
			m_World.GetBlockTypeMeta(a_BlockPos.x + gCrossCoords[i].x, a_BlockPos.y + gCrossCoords[i].y, a_BlockPos.z + gCrossCoords[i].z, Type, Meta);
			if ((Type == E_BLOCK_REDSTONE_WIRE) && (!AreCoordsPowered(Vector3i(a_BlockPos.x + gCrossCoords[i].x, a_BlockPos.y + gCrossCoords[i].y, a_BlockPos.z + gCrossCoords[i].z))))
			{
				// !AreCoordsPowered prevents infinite loop - powered passing back to powered and so on and so forth
				HandleRedstoneWire(Vector3i(a_BlockPos.x + gCrossCoords[i].x, a_BlockPos.y + gCrossCoords[i].y, a_BlockPos.z + gCrossCoords[i].z));
			}
		}
	}
	else
	{
		int TimesMetaSmaller = 0;
		int TimesFoundAWire = 0;

		for (int i = 0; i < ARRAYCOUNT(gCrossCoords); i++) // Loop through all directions to transfer or receive power
		{
			BLOCKTYPE SurroundType;
			NIBBLETYPE SurroundMeta;
			NIBBLETYPE MyMeta = m_World.GetBlockMeta(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z);
			m_World.GetBlockTypeMeta(a_BlockPos.x + gCrossCoords[i].x, a_BlockPos.y + gCrossCoords[i].y, a_BlockPos.z + gCrossCoords[i].z, SurroundType, SurroundMeta);

			if (SurroundType == E_BLOCK_REDSTONE_WIRE)
			{
				TimesFoundAWire++;

				if (SurroundMeta > 1) // Wires of power 1 or 0 cannot transfer power TO ME, don't bother checking
				{
					if (SurroundMeta > MyMeta) // Does surrounding wire have a higher power level than self?
					{
						m_World.SetBlockMeta(a_BlockPos, SurroundMeta - 1);
					}
				}
				else
				{
					if (MyMeta > 1) // If self is less than 1, I can't transfer power to ANYONE ELSE
					{
						HandleRedstoneWire(Vector3i(a_BlockPos.x + gCrossCoords[i].x, a_BlockPos.y + gCrossCoords[i].y, a_BlockPos.z + gCrossCoords[i].z));
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
			m_World.SetBlockMeta(a_BlockPos, 0);

			for (int i = 0; i < ARRAYCOUNT(gCrossCoords); i++)
			{
				BLOCKTYPE SurroundType;
				NIBBLETYPE SurroundMeta;
				NIBBLETYPE MyMeta = m_World.GetBlockMeta(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z);
				m_World.GetBlockTypeMeta(a_BlockPos.x + gCrossCoords[i].x, a_BlockPos.y + gCrossCoords[i].y, a_BlockPos.z + gCrossCoords[i].z, SurroundType, SurroundMeta);

				if ((SurroundType == E_BLOCK_REDSTONE_WIRE) && (SurroundMeta > 0)) // Propagate lights-out to connecting wires that are still powered
				{
					HandleRedstoneWire(Vector3i(a_BlockPos.x + gCrossCoords[i].x, a_BlockPos.y + gCrossCoords[i].y, a_BlockPos.z + gCrossCoords[i].z));
				}
			}
		}		
	}

	if (m_World.GetBlockMeta(a_BlockPos) != 0) // A powered wire
	{
		//SetBlockPowered(Vector3i(a_BlockPos.x, a_BlockPos.y - 1, a_BlockPos.z), a_BlockPos, E_BLOCK_REDSTONE_WIRE); // No matter what, block underneath gets powered

		switch (GetWireDirection(a_BlockPos))
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
					if (g_BlockIsSolid[m_World.GetBlock(Vector3i(a_BlockPos.x + gCrossCoords[i].x, a_BlockPos.y + gCrossCoords[i].y, a_BlockPos.z + gCrossCoords[i].z))])
					{
						SetBlockPowered(Vector3i(a_BlockPos.x + gCrossCoords[i].x, a_BlockPos.y + gCrossCoords[i].y, a_BlockPos.z + gCrossCoords[i].z), a_BlockPos, E_BLOCK_REDSTONE_WIRE);
					}
				}
				break;
			}
			case REDSTONE_X_POS:
			{
				if (g_BlockIsSolid[m_World.GetBlock(Vector3i(a_BlockPos.x + 1, a_BlockPos.y, a_BlockPos.z))])
				{
					BLOCKTYPE MiddleBlock =  m_World.GetBlock(a_BlockPos.x + 1, a_BlockPos.y, a_BlockPos.z);
					SetBlockPowered(Vector3i(a_BlockPos.x + 1, a_BlockPos.y, a_BlockPos.z), a_BlockPos, E_BLOCK_REDSTONE_WIRE);
					SetBlockLinkedPowered(Vector3i(a_BlockPos.x + 2, a_BlockPos.y, a_BlockPos.z), Vector3i(a_BlockPos.x + 1, a_BlockPos.y, a_BlockPos.z), a_BlockPos, E_BLOCK_REDSTONE_WIRE, MiddleBlock);
					SetBlockLinkedPowered(Vector3i(a_BlockPos.x + 1, a_BlockPos.y, a_BlockPos.z + 1), Vector3i(a_BlockPos.x + 1, a_BlockPos.y, a_BlockPos.z), a_BlockPos, E_BLOCK_REDSTONE_WIRE, MiddleBlock);
					SetBlockLinkedPowered(Vector3i(a_BlockPos.x + 1, a_BlockPos.y, a_BlockPos.z - 1), Vector3i(a_BlockPos.x + 1, a_BlockPos.y, a_BlockPos.z), a_BlockPos, E_BLOCK_REDSTONE_WIRE, MiddleBlock);
				}
				break;
			}
			case REDSTONE_X_NEG:
			{
				if (g_BlockIsSolid[m_World.GetBlock(Vector3i(a_BlockPos.x - 1, a_BlockPos.y, a_BlockPos.z))])
				{					
					BLOCKTYPE MiddleBlock =  m_World.GetBlock(a_BlockPos.x - 1, a_BlockPos.y, a_BlockPos.z);
					SetBlockPowered(Vector3i(a_BlockPos.x - 1, a_BlockPos.y, a_BlockPos.z), a_BlockPos, E_BLOCK_REDSTONE_WIRE);
					SetBlockLinkedPowered(Vector3i(a_BlockPos.x - 2, a_BlockPos.y, a_BlockPos.z), Vector3i(a_BlockPos.x - 1, a_BlockPos.y, a_BlockPos.z), a_BlockPos, E_BLOCK_REDSTONE_WIRE, MiddleBlock);
					SetBlockLinkedPowered(Vector3i(a_BlockPos.x - 1, a_BlockPos.y, a_BlockPos.z + 1), Vector3i(a_BlockPos.x - 1, a_BlockPos.y, a_BlockPos.z), a_BlockPos, E_BLOCK_REDSTONE_WIRE, MiddleBlock);
					SetBlockLinkedPowered(Vector3i(a_BlockPos.x - 1, a_BlockPos.y, a_BlockPos.z - 1), Vector3i(a_BlockPos.x - 1, a_BlockPos.y, a_BlockPos.z), a_BlockPos, E_BLOCK_REDSTONE_WIRE, MiddleBlock);
				}
				break;
			}
			case REDSTONE_Z_POS:
			{
				if (g_BlockIsSolid[m_World.GetBlock(Vector3i(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z + 1))])
				{
					BLOCKTYPE MiddleBlock = m_World.GetBlock(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z + 1);
					SetBlockPowered(Vector3i(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z + 1), a_BlockPos, E_BLOCK_REDSTONE_WIRE);
					SetBlockLinkedPowered(Vector3i(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z + 2), Vector3i(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z + 1), a_BlockPos, E_BLOCK_REDSTONE_WIRE, MiddleBlock);
					SetBlockLinkedPowered(Vector3i(a_BlockPos.x + 1, a_BlockPos.y, a_BlockPos.z + 1), Vector3i(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z + 1), a_BlockPos, E_BLOCK_REDSTONE_WIRE, MiddleBlock);
					SetBlockLinkedPowered(Vector3i(a_BlockPos.x - 1, a_BlockPos.y, a_BlockPos.z + 1), Vector3i(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z + 1), a_BlockPos, E_BLOCK_REDSTONE_WIRE, MiddleBlock);
				}
				break;
			}
			case REDSTONE_Z_NEG:
			{
				if (g_BlockIsSolid[m_World.GetBlock(Vector3i(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z - 1))])
				{
					BLOCKTYPE MiddleBlock = m_World.GetBlock(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z - 1);
					SetBlockPowered(Vector3i(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z - 1), a_BlockPos, E_BLOCK_REDSTONE_WIRE);
					SetBlockLinkedPowered(Vector3i(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z - 2), Vector3i(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z - 1), a_BlockPos, E_BLOCK_REDSTONE_WIRE, MiddleBlock);
					SetBlockLinkedPowered(Vector3i(a_BlockPos.x + 1, a_BlockPos.y, a_BlockPos.z - 1), Vector3i(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z - 1), a_BlockPos, E_BLOCK_REDSTONE_WIRE, MiddleBlock);
					SetBlockLinkedPowered(Vector3i(a_BlockPos.x - 1, a_BlockPos.y, a_BlockPos.z - 1), Vector3i(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z - 1), a_BlockPos,E_BLOCK_REDSTONE_WIRE, MiddleBlock);
				}
				break;
			}
		}
	}
	return;
}





void cRedstoneSimulator::HandleRedstoneRepeater(const Vector3i & a_BlockPos)
{
	NIBBLETYPE a_Meta = m_World.GetBlockMeta(a_BlockPos);

	if (IsRepeaterPowered(a_BlockPos, a_Meta & 0x3))
	{
		m_World.FastSetBlock(a_BlockPos, E_BLOCK_REDSTONE_REPEATER_ON, a_Meta);
		switch (a_Meta & 0x3) // We only want the direction (bottom) bits
		{
			case 0x0:
			{
				// Power if block is solid, CURRENTLY all mechanisms are solid
				if (g_BlockIsSolid[m_World.GetBlock(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z - 1)])
				{
					// 2-5th LinkedPowered - set blocks around block in front to be powered
					BLOCKTYPE MiddleBlock = m_World.GetBlock(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z - 1);
					SetBlockPowered(Vector3i(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z - 1), a_BlockPos, E_BLOCK_REDSTONE_REPEATER_ON);
					SetBlockLinkedPowered(Vector3i(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z - 2), Vector3i(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z - 1), a_BlockPos, E_BLOCK_REDSTONE_REPEATER_ON, MiddleBlock);
					SetBlockLinkedPowered(Vector3i(a_BlockPos.x + 1, a_BlockPos.y, a_BlockPos.z - 1), Vector3i(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z - 1), a_BlockPos, E_BLOCK_REDSTONE_REPEATER_ON, MiddleBlock);
					SetBlockLinkedPowered(Vector3i(a_BlockPos.x - 1, a_BlockPos.y, a_BlockPos.z - 1), Vector3i(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z - 1), a_BlockPos,E_BLOCK_REDSTONE_REPEATER_ON, MiddleBlock);
				}
				break;
			}
			case 0x1:
			{
				if (g_BlockIsSolid[m_World.GetBlock(a_BlockPos.x + 1, a_BlockPos.y, a_BlockPos.z)])
				{
					BLOCKTYPE MiddleBlock =  m_World.GetBlock(a_BlockPos.x + 1, a_BlockPos.y, a_BlockPos.z);
					SetBlockPowered(Vector3i(a_BlockPos.x + 1, a_BlockPos.y, a_BlockPos.z), a_BlockPos, E_BLOCK_REDSTONE_REPEATER_ON);
					SetBlockLinkedPowered(Vector3i(a_BlockPos.x + 2, a_BlockPos.y, a_BlockPos.z), Vector3i(a_BlockPos.x + 1, a_BlockPos.y, a_BlockPos.z), a_BlockPos, E_BLOCK_REDSTONE_REPEATER_ON, MiddleBlock);
					SetBlockLinkedPowered(Vector3i(a_BlockPos.x + 1, a_BlockPos.y, a_BlockPos.z + 1), Vector3i(a_BlockPos.x + 1, a_BlockPos.y, a_BlockPos.z), a_BlockPos, E_BLOCK_REDSTONE_REPEATER_ON, MiddleBlock);
					SetBlockLinkedPowered(Vector3i(a_BlockPos.x + 1, a_BlockPos.y, a_BlockPos.z - 1), Vector3i(a_BlockPos.x + 1, a_BlockPos.y, a_BlockPos.z), a_BlockPos, E_BLOCK_REDSTONE_REPEATER_ON, MiddleBlock);
				}
				break;
			}
			case 0x2:
			{
				if (g_BlockIsSolid[m_World.GetBlock(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z + 1)])
				{
					BLOCKTYPE MiddleBlock = m_World.GetBlock(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z + 1);
					SetBlockPowered(Vector3i(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z + 1), a_BlockPos, E_BLOCK_REDSTONE_REPEATER_ON);
					SetBlockLinkedPowered(Vector3i(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z + 2), Vector3i(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z + 1), a_BlockPos, E_BLOCK_REDSTONE_REPEATER_ON, MiddleBlock);
					SetBlockLinkedPowered(Vector3i(a_BlockPos.x + 1, a_BlockPos.y, a_BlockPos.z + 1), Vector3i(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z + 1), a_BlockPos, E_BLOCK_REDSTONE_REPEATER_ON, MiddleBlock);
					SetBlockLinkedPowered(Vector3i(a_BlockPos.x - 1, a_BlockPos.y, a_BlockPos.z + 1), Vector3i(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z + 1), a_BlockPos, E_BLOCK_REDSTONE_REPEATER_ON, MiddleBlock);
				}
				break;
			}
			case 0x3:
			{
				if (g_BlockIsSolid[m_World.GetBlock(a_BlockPos.x - 1, a_BlockPos.y, a_BlockPos.z)])
				{
					BLOCKTYPE MiddleBlock =  m_World.GetBlock(a_BlockPos.x - 1, a_BlockPos.y, a_BlockPos.z);
					SetBlockPowered(Vector3i(a_BlockPos.x - 1, a_BlockPos.y, a_BlockPos.z), a_BlockPos, E_BLOCK_REDSTONE_REPEATER_ON);
					SetBlockLinkedPowered(Vector3i(a_BlockPos.x - 2, a_BlockPos.y, a_BlockPos.z), Vector3i(a_BlockPos.x - 1, a_BlockPos.y, a_BlockPos.z), a_BlockPos, E_BLOCK_REDSTONE_REPEATER_ON, MiddleBlock);
					SetBlockLinkedPowered(Vector3i(a_BlockPos.x - 1, a_BlockPos.y, a_BlockPos.z + 1), Vector3i(a_BlockPos.x - 1, a_BlockPos.y, a_BlockPos.z), a_BlockPos, E_BLOCK_REDSTONE_REPEATER_ON, MiddleBlock);
					SetBlockLinkedPowered(Vector3i(a_BlockPos.x - 1, a_BlockPos.y, a_BlockPos.z - 1), Vector3i(a_BlockPos.x - 1, a_BlockPos.y, a_BlockPos.z), a_BlockPos, E_BLOCK_REDSTONE_REPEATER_ON, MiddleBlock);
				}
				break;
			}
		}
	}
	else
	{
		m_World.FastSetBlock(a_BlockPos, E_BLOCK_REDSTONE_REPEATER_OFF, a_Meta);
	}
	return;
}





void cRedstoneSimulator::HandlePiston(const Vector3i & a_BlockPos)
{	
	if (AreCoordsPowered(a_BlockPos))
	{
		cPiston Piston(&m_World);
		Piston.ExtendPiston(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z);
	}
	else
	{
		cPiston Piston(&m_World);
		Piston.RetractPiston(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z);
	}
	return;
}





void cRedstoneSimulator::HandleDropSpenser(const Vector3i & a_BlockPos)
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
	} DrSpSP (AreCoordsPowered(a_BlockPos));

	m_World.DoWithDropSpenserAt(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z, DrSpSP);
	return;
}





void cRedstoneSimulator::HandleRedstoneLamp(const Vector3i & a_BlockPos, BLOCKTYPE a_MyState)
{
	if (a_MyState == E_BLOCK_REDSTONE_LAMP_OFF)
	{
		if (AreCoordsPowered(a_BlockPos))
		{
			m_World.FastSetBlock(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z, E_BLOCK_REDSTONE_LAMP_ON, 0);
		}
	}
	else
	{
		if (!AreCoordsPowered(a_BlockPos))
		{
			m_World.FastSetBlock(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z, E_BLOCK_REDSTONE_LAMP_OFF, 0);
		}
	}
	return;
}





void cRedstoneSimulator::HandleTNT(const Vector3i & a_BlockPos)
{
	if (AreCoordsPowered(a_BlockPos))
	{
		m_World.BroadcastSoundEffect("random.fuse", a_BlockPos.x * 8, a_BlockPos.y * 8, a_BlockPos.z * 8, 0.5f, 0.6f);
		m_World.SpawnPrimedTNT(a_BlockPos.x + 0.5, a_BlockPos.y + 0.5, a_BlockPos.z + 0.5, 4);  // 4 seconds to boom
		m_World.FastSetBlock(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z, E_BLOCK_AIR, 0);
	}
	return;
}





void cRedstoneSimulator::HandleDoor(const Vector3i & a_BlockPos)
{
	cBlockDoorHandler::ChangeDoor(&m_World, a_BlockPos.x, a_BlockPos.y, a_BlockPos.z);
	return;
}





bool cRedstoneSimulator::AreCoordsPowered(const Vector3i & a_BlockPos)
{
	for (PoweredBlocksList::iterator itr = m_PoweredBlocks.begin(); itr != m_PoweredBlocks.end(); ++itr) // Check powered list
	{
		sPoweredBlocks & Change = *itr;

		if (Change.a_BlockPos.Equals(a_BlockPos))
		{
			return true;
		}
	}

	for (LinkedBlocksList::iterator itr = m_LinkedPoweredBlocks.begin(); itr != m_LinkedPoweredBlocks.end(); ++itr) // Check linked powered list
	{
		sLinkedPoweredBlocks & Change = *itr;

		if (Change.a_BlockPos.Equals(a_BlockPos))
		{
			return true;
		}
	}
	return false;
}





bool cRedstoneSimulator::IsRepeaterPowered(const Vector3i & a_BlockPos, NIBBLETYPE a_Meta)
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
				if (Change.a_SourcePos.Equals(Vector3i(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z + 1))) { return true; }
				break;
			}
			case 0x1:
			{
				if (Change.a_SourcePos.Equals(Vector3i(a_BlockPos.x - 1, a_BlockPos.y, a_BlockPos.z))) { return true; }
				break;
			}
			case 0x2:
			{
				if (Change.a_SourcePos.Equals(Vector3i(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z - 1))) { return true; }
				break;
			}
			case 0x3:
			{
				if (Change.a_SourcePos.Equals(Vector3i(a_BlockPos.x + 1, a_BlockPos.y, a_BlockPos.z))) { return true; }
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
				if (Change.a_MiddlePos.Equals(Vector3i(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z + 1))) { return true; }
				break;
			}
			case 0x1:
			{
				if (Change.a_MiddlePos.Equals(Vector3i(a_BlockPos.x - 1, a_BlockPos.y, a_BlockPos.z))) { return true; }
				break;
			}
			case 0x2:
			{
				if (Change.a_MiddlePos.Equals(Vector3i(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z - 1))) { return true; }
				break;
			}
			case 0x3:
			{
				if (Change.a_MiddlePos.Equals(Vector3i(a_BlockPos.x + 1, a_BlockPos.y, a_BlockPos.z))) { return true; }
				break;
			}
		}
	}
	return false; // Couldn't find power source behind repeater
}





void cRedstoneSimulator::SetBlockPowered(const Vector3i & a_BlockPos, const Vector3i & a_SourcePos, BLOCKTYPE a_SourceBlock)
{
	sPoweredBlocks RC;
	RC.a_BlockPos = a_BlockPos;
	RC.a_SourcePos = a_SourcePos;
	RC.a_SourceBlock = a_SourceBlock;
	m_PoweredBlocks.push_back(RC);
	return;
}





void cRedstoneSimulator::SetBlockLinkedPowered(const Vector3i & a_BlockPos, const Vector3i & a_MiddlePos, const Vector3i & a_SourcePos,
											   BLOCKTYPE a_SourceBlock, BLOCKTYPE a_MiddleBlock)
{
	sLinkedPoweredBlocks RC;
	RC.a_BlockPos = a_BlockPos;
	RC.a_MiddlePos = a_MiddlePos;
	RC.a_SourcePos = a_SourcePos;
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





NIBBLETYPE cRedstoneSimulator::RepeaterRotationToMetaData(double a_Rotation)
{
	a_Rotation += 90 + 45; // So its not aligned with axis
	if (a_Rotation > 360)
	{
		a_Rotation -= 360;
	}
	
	if ((a_Rotation >= 0) && (a_Rotation < 90))
	{
		return 0x1;
	}
	else if ((a_Rotation >= 180) && (a_Rotation < 270))
	{
		return 0x3;
	}
	else if ((a_Rotation >= 90) && (a_Rotation < 180))
	{
		return 0x2;
	}
	else
	{
		return 0x0;
	}
}





NIBBLETYPE cRedstoneSimulator::LeverDirectionToMetaData(char a_Dir)
{
	// Determine lever direction:
	switch (a_Dir)
	{
		case BLOCK_FACE_TOP:    return 0x6;
		case BLOCK_FACE_EAST:   return 0x1;
		case BLOCK_FACE_WEST:   return 0x2;
		case BLOCK_FACE_SOUTH:  return 0x3;
		case BLOCK_FACE_NORTH:  return 0x4;
		case BLOCK_FACE_BOTTOM: return 0x0;
		default:                return 0x6;
	}
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




