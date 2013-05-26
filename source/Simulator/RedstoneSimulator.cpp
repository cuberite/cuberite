
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "RedstoneSimulator.h"
#include "../DispenserEntity.h"
#include "../Piston.h"
#include "../World.h"
#include "../BlockID.h"
#include "../Torch.h"
#include "../Chunk.h"





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
	int MinY = (a_BlockY > 0) ? -1 : 0;
	int MaxY = (a_BlockY < cChunkDef::Height - 1) ? 1 : 0;
	for (int y = MinY; y <= MaxY; y++) for (int x = -1; x < 2; x++) for (int z = -1; z < 2; z++)
	{
		BLOCKTYPE  BlockType;
		NIBBLETYPE BlockMeta;
		if (!a_Chunk->UnboundedRelGetBlock(RelX + x, a_BlockY + y, RelZ + z, BlockType, BlockMeta))
		{
			continue;
		}
		switch (BlockType)
		{
			case E_BLOCK_REDSTONE_LAMP_OFF:
			case E_BLOCK_REDSTONE_LAMP_ON:
			case E_BLOCK_REDSTONE_REPEATER_OFF:
			case E_BLOCK_REDSTONE_REPEATER_ON:
			case E_BLOCK_REDSTONE_TORCH_OFF:
			case E_BLOCK_REDSTONE_TORCH_ON:
			case E_BLOCK_REDSTONE_WIRE:
			case E_BLOCK_LEVER:
			case E_BLOCK_STONE_BUTTON:
			case E_BLOCK_WOODEN_BUTTON:
			case E_BLOCK_TRIPWIRE_HOOK:
			{
				m_Blocks.push_back(Vector3i(a_BlockX, a_BlockY, a_BlockZ));
				return;
			}
		}  // switch (BlockType)
	}  // for y, x, z - neighbors
}





void cRedstoneSimulator::Simulate(float a_Dt)
{
	// Toggle torches on/off
	while (!m_RefreshTorchesAround.empty())
	{
		Vector3i pos = m_RefreshTorchesAround.front();
		m_RefreshTorchesAround.pop_front();

		RefreshTorchesAround(pos);
	}

	// Set repeaters to correct values, and decrement ticks
	for (RepeaterList::iterator itr = m_SetRepeaters.begin(); itr != m_SetRepeaters.end();)
	{
		if (--itr->Ticks > 0)
		{
			// Not yet, move to next item in the list
			++itr;
			continue;
		}
		BLOCKTYPE BlockType;
		NIBBLETYPE BlockMeta;
		m_World.GetBlockTypeMeta(itr->Position.x, itr->Position.y, itr->Position.z, BlockType, BlockMeta);
		if (itr->bPowerOn && (BlockType == E_BLOCK_REDSTONE_REPEATER_OFF))
		{
			m_World.FastSetBlock(itr->Position.x, itr->Position.y, itr->Position.z, E_BLOCK_REDSTONE_REPEATER_ON, BlockMeta);
			m_Blocks.push_back(itr->Position);
		}
		else if (!itr->bPowerOn && (BlockType == E_BLOCK_REDSTONE_REPEATER_ON))
		{
			m_World.FastSetBlock(itr->Position.x, itr->Position.y, itr->Position.z, E_BLOCK_REDSTONE_REPEATER_OFF, BlockMeta);
			m_Blocks.push_back(itr->Position);
		}

		if (itr->bPowerOffNextTime)
		{
			itr->bPowerOn = false;
			itr->bPowerOffNextTime = false;
			itr->Ticks = 10; // TODO: Look up actual ticks from block metadata
			++itr;
		}
		else
		{
			itr = m_SetRepeaters.erase(itr);
		}
	}

	// Handle changed blocks
	{
		cCSLock Lock(m_CS);
		std::swap(m_Blocks, m_BlocksBuffer);
	}
	for (BlockList::iterator itr = m_BlocksBuffer.begin(); itr != m_BlocksBuffer.end(); ++itr)
	{
		HandleChange(*itr);
	}
	m_BlocksBuffer.clear();
}





void cRedstoneSimulator::RefreshTorchesAround(const Vector3i & a_BlockPos)
{
	static Vector3i Surroundings [] = {
		Vector3i(-1, 0, 0),
		Vector3i(1, 0, 0),
		Vector3i(0, 0,-1),
		Vector3i(0, 0, 1),
		Vector3i(0, 1, 0), // Also toggle torch on top
	};
	BLOCKTYPE TargetBlockType = E_BLOCK_REDSTONE_TORCH_ON;
	BLOCKTYPE TargetRepeaterType = E_BLOCK_REDSTONE_REPEATER_OFF;
	if (IsPowered(a_BlockPos, true))
	{
		TargetBlockType = E_BLOCK_REDSTONE_TORCH_OFF;
		TargetRepeaterType = E_BLOCK_REDSTONE_REPEATER_ON;
		//if (m_World.GetBlock(a_BlockPos) == E_BLOCK_DIRT)
		//{
		//	m_World.FastSetBlock(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z, E_BLOCK_STONE, 0);
		//}
	}
	else
	{
		//if (m_World.GetBlock(a_BlockPos) == E_BLOCK_STONE)
		//{
		//	m_World.FastSetBlock(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z, E_BLOCK_DIRT, 0);
		//}
	}

	for (unsigned int i = 0; i < ARRAYCOUNT(Surroundings); ++i)
	{
		Vector3i TorchPos = a_BlockPos + Surroundings[i];
		BLOCKTYPE BlockType;
		NIBBLETYPE BlockMeta;
		m_World.GetBlockTypeMeta(TorchPos.x, TorchPos.y, TorchPos.z, BlockType, BlockMeta);
		switch (BlockType)
		{
			case E_BLOCK_REDSTONE_TORCH_ON:
			case E_BLOCK_REDSTONE_TORCH_OFF:
			{
				if (BlockType != TargetBlockType)
				{
					if (cTorch::IsAttachedTo(TorchPos, BlockMeta, a_BlockPos))
					{
						m_World.FastSetBlock(TorchPos.x, TorchPos.y, TorchPos.z, TargetBlockType, BlockMeta);
						m_Blocks.push_back(TorchPos);
					}
				}
				break;
			}
			case E_BLOCK_REDSTONE_REPEATER_ON:
			case E_BLOCK_REDSTONE_REPEATER_OFF:
			{
				if ((BlockType != TargetRepeaterType) && IsRepeaterPointingAway(TorchPos, BlockMeta, a_BlockPos))
				{
					SetRepeater(TorchPos, 10, (TargetRepeaterType == E_BLOCK_REDSTONE_REPEATER_ON));
				}
				break;
			}
		}  // switch (BlockType)
	}  // for i - Surroundings[]
}





void cRedstoneSimulator::HandleChange(const Vector3i & a_BlockPos)
{
	std::deque< Vector3i > SpreadStack;

	static const Vector3i Surroundings[] = {
		Vector3i(1, 0, 0),
		Vector3i(1, 1, 0),
		Vector3i(1,-1, 0),
		Vector3i(-1, 0, 0),
		Vector3i(-1, 1, 0),
		Vector3i(-1,-1, 0),
		Vector3i(0, 0, 1),
		Vector3i(0, 1, 1),
		Vector3i(0,-1, 1),
		Vector3i(0, 0,-1),
		Vector3i(0, 1,-1),
		Vector3i(0,-1,-1),
		Vector3i(0,-1, 0),
	};

	BLOCKTYPE BlockType;
	NIBBLETYPE BlockMeta;
	m_World.GetBlockTypeMeta(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z, BlockType, BlockMeta);

	// First check whether torch should be on or off
	switch (BlockType)
	{
		case E_BLOCK_REDSTONE_TORCH_ON:
		case E_BLOCK_REDSTONE_TORCH_OFF:
		{
			static const Vector3i Surroundings [] = {
				Vector3i(-1, 0, 0),
				Vector3i(1, 0, 0),
				Vector3i(0, 0,-1),
				Vector3i(0, 0, 1),
				Vector3i(0,-1, 0),
			};
			for (unsigned int i = 0; i < ARRAYCOUNT(Surroundings); ++i)
			{
				Vector3i pos = a_BlockPos + Surroundings[i];
				BLOCKTYPE OtherBlock = m_World.GetBlock(pos);
				if (
					(OtherBlock != E_BLOCK_AIR) &&
					(OtherBlock != E_BLOCK_REDSTONE_TORCH_ON) &&
					(OtherBlock != E_BLOCK_REDSTONE_TORCH_OFF)
				)
				{
					RefreshTorchesAround(pos);
				}
			}
			m_World.GetBlockTypeMeta(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z, BlockType, BlockMeta);
			break;
		}  // case "torches"
		
		case E_BLOCK_REDSTONE_REPEATER_ON:
		case E_BLOCK_REDSTONE_REPEATER_OFF:
		{
			// Check if repeater is powered by a 'powered block' (not wires/torch)
			Vector3i Direction = GetRepeaterDirection(BlockMeta);
			Vector3i pos = a_BlockPos - Direction; // NOTE: It's minus Direction
			BLOCKTYPE OtherBlock = m_World.GetBlock(pos);
			if (
				(OtherBlock != E_BLOCK_AIR) &&
				(OtherBlock != E_BLOCK_REDSTONE_TORCH_ON) &&
				(OtherBlock != E_BLOCK_REDSTONE_TORCH_OFF) &&
				(OtherBlock != E_BLOCK_REDSTONE_WIRE)
			)
			{
				RefreshTorchesAround(pos);
			}
			else
			{
				SetRepeater(a_BlockPos, 10, IsPowered(a_BlockPos, false));
			}
			m_World.GetBlockTypeMeta(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z, BlockType, BlockMeta);
			break;
		}
	}  // switch (BlockType)

	BlockList Sources;
	switch (BlockType)
	{
		case E_BLOCK_REDSTONE_TORCH_ON:
		{
			// If torch is still on, use it as a source
			Sources.push_back(a_BlockPos);
			break;
		}
		
		case E_BLOCK_REDSTONE_REPEATER_ON:
		{
			// Repeater only spreads charge right in front, and up to one block up:
			static const Vector3i Surroundings [] = {
				Vector3i(0, 0, 0),
				Vector3i(0, 1, 0),
			};
			Vector3i Direction = GetRepeaterDirection(BlockMeta);
			for (unsigned int i = 0; i < ARRAYCOUNT(Surroundings); ++i)
			{
				Vector3i pos = a_BlockPos + Direction + Surroundings[i];
				if (PowerBlock(pos, a_BlockPos, 0xf))
				{
					SpreadStack.push_back(pos);
				}
			}
			break;
		}  // case E_BLOCK_REDSTONE_REPEATER_ON
		
		case E_BLOCK_LEVER:
		{ 
			// Adding lever to the source queue
			if (cRedstoneSimulator::IsLeverOn(BlockMeta))
			{
				Sources.push_back(a_BlockPos);
			}
			break;
		}  // case E_BLOCK_LEVER
	}  // switch (BlockType)

	// Power all blocks legally connected to the sources
	if (BlockType != E_BLOCK_REDSTONE_REPEATER_ON)
	{
		BlockList NewSources = RemoveCurrent(a_BlockPos);
		Sources.insert(Sources.end(), NewSources.begin(), NewSources.end());
		while (!Sources.empty())
		{
			Vector3i SourcePos = Sources.back();
			Sources.pop_back();

			BLOCKTYPE BlockType;
			NIBBLETYPE BlockMeta;
			m_World.GetBlockTypeMeta(SourcePos.x, SourcePos.y, SourcePos.z, BlockType, BlockMeta);
			switch (BlockType)
			{
				case E_BLOCK_LEVER:  // Treating lever as a torch
				case E_BLOCK_REDSTONE_TORCH_OFF:
				case E_BLOCK_REDSTONE_TORCH_ON:
				{
					static Vector3i Surroundings [] = {
						Vector3i(-1, 0, 0),
						Vector3i(1, 0, 0),
						Vector3i(0, 0,-1),
						Vector3i(0, 0, 1),
					};
					for (unsigned int i = 0; i < ARRAYCOUNT(Surroundings); ++i)
					{
						Vector3i OtherPos =  SourcePos + Surroundings[i];
						if (PowerBlock(OtherPos, a_BlockPos, 0xf))
						{
							SpreadStack.push_back(OtherPos); // Changed, so add to stack
						}
					}
					break;
				}

				case E_BLOCK_REDSTONE_REPEATER_OFF:
				case E_BLOCK_REDSTONE_REPEATER_ON:
				{
					static Vector3i Surroundings [] = {
						Vector3i(0, 0, 0),
						Vector3i(0, 1, 0),
					};
					Vector3i Direction = GetRepeaterDirection(BlockMeta);
					for (unsigned int i = 0; i < ARRAYCOUNT(Surroundings); ++i)
					{
						Vector3i pos = SourcePos + Direction + Surroundings[i];
						if (PowerBlock(pos, a_BlockPos, 0xf))
						{
							SpreadStack.push_back(pos);
						}
					}
					break;
				}
			}  // switch (BlockType)
		}  // while (Sources[])
	}  // if (!repeater_on)

	// Do a floodfill
	while (!SpreadStack.empty())
	{
		Vector3i pos = SpreadStack.back();
		SpreadStack.pop_back();
		NIBBLETYPE Meta = m_World.GetBlockMeta(pos);

		for (unsigned int i = 0; i < ARRAYCOUNT(Surroundings); ++i)
		{
			Vector3i OtherPos =  pos + Surroundings[i];
			if (PowerBlock(OtherPos, pos, Meta - 1))
			{
				SpreadStack.push_back(OtherPos);  // Changed, so add to stack
			}
		}
	}

	// Only after a redstone area has been completely simulated the redstone entities can react
	while (!m_RefreshPistons.empty())
	{
		Vector3i pos = m_RefreshPistons.back();
		m_RefreshPistons.pop_back();

		BLOCKTYPE BlockType = m_World.GetBlock(pos);
		switch (BlockType)
		{
			case E_BLOCK_PISTON:
			case E_BLOCK_STICKY_PISTON:
			{
				if (IsPowered(pos))
				{
					cPiston Piston(&m_World);
					Piston.ExtendPiston(pos.x, pos.y, pos.z);
				}
				else
				{
					cPiston Piston(&m_World);
					Piston.RetractPiston(pos.x, pos.y, pos.z);
				}
				break;
			}
		}  // switch (BlockType)
	}  // while (m_RefreshPistons[])

	while (!m_RefreshDropSpensers.empty())
	{
		Vector3i pos = m_RefreshDropSpensers.back();
		m_RefreshDropSpensers.pop_back();

		BLOCKTYPE BlockType = m_World.GetBlock(pos);
		if ((BlockType == E_BLOCK_DISPENSER) || (BlockType == E_BLOCK_DROPPER))
		{
			if (IsPowered(pos))
			{
				class cActivateDropSpenser :
					public cDropSpenserCallback
				{
					virtual bool Item(cDropSpenserEntity * a_DropSpenser) override
					{
						a_DropSpenser->Activate();
						return false;
					}
				} ;
				cActivateDropSpenser DrSpAct;
				m_World.DoWithDropSpenserAt(pos.x, pos.y, pos.z, DrSpAct);
			}
		}
	}
}





bool cRedstoneSimulator::PowerBlock(const Vector3i & a_BlockPos, const Vector3i & a_FromBlock, char a_Power)
{
	BLOCKTYPE BlockType;
	NIBBLETYPE BlockMeta;
	m_World.GetBlockTypeMeta(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z, BlockType, BlockMeta);
	switch (BlockType)
	{
		case E_BLOCK_REDSTONE_WIRE:
		{
			if (BlockMeta < a_Power)
			{
				m_World.SetBlockMeta(a_BlockPos, a_Power);
				return true;
			}
			break;
		}
		
		case E_BLOCK_PISTON:
		case E_BLOCK_STICKY_PISTON:
		{
			m_RefreshPistons.push_back(a_BlockPos);
			break;
		}

		case E_BLOCK_DISPENSER:
		case E_BLOCK_DROPPER:
		{
			m_RefreshDropSpensers.push_back(a_BlockPos);
			break;
		}
		
		case E_BLOCK_REDSTONE_REPEATER_OFF:
		case E_BLOCK_REDSTONE_REPEATER_ON:
		{
			if (IsRepeaterPointingAway(a_BlockPos, BlockMeta, a_FromBlock))
			{
				SetRepeater(a_BlockPos, 10, true);
			}
			break;
		}
		
		default:
		{
			if (
				(BlockType != E_BLOCK_AIR) &&
				(BlockType != E_BLOCK_REDSTONE_TORCH_ON) &&
				(BlockType != E_BLOCK_REDSTONE_TORCH_OFF) &&
				(BlockType != E_BLOCK_LEVER)  // Treating lever as a torch, for refreshing
			)
			{
				if (IsPowered(a_BlockPos, true))
				{
					m_RefreshTorchesAround.push_back(a_BlockPos);
				}
			}
			break;
		}
	}  // switch (BlockType)

	return false;
}





int cRedstoneSimulator::UnPowerBlock(const Vector3i & a_BlockPos, const Vector3i & a_FromBlock)
{
	BLOCKTYPE BlockType;
	NIBBLETYPE BlockMeta;
	if ((a_BlockPos.y < 0) || (a_BlockPos.y >= cChunkDef::Height))
	{
		return 0;
	}
	m_World.GetBlockTypeMeta(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z, BlockType, BlockMeta);
	switch (BlockType)
	{
		case E_BLOCK_REDSTONE_WIRE:
		{
			if (BlockMeta > 0)
			{
				m_World.SetBlockMeta(a_BlockPos, 0);
				return 1;
			}
			break;
		}
		
		case E_BLOCK_PISTON:
		case E_BLOCK_STICKY_PISTON:
		{
			m_RefreshPistons.push_back(a_BlockPos);
			break;
		}

		case E_BLOCK_REDSTONE_TORCH_ON:
		{
			return 2;
			break;
		}
		
		case E_BLOCK_LEVER:
		{
			// Check if lever is ON. If it is, report it back as a source
			if (cRedstoneSimulator::IsLeverOn(BlockMeta))
			{
				return 2;
			}
			break;
		}
		
		case E_BLOCK_REDSTONE_REPEATER_ON:
		{
			if (
				IsRepeaterPointingTo(a_BlockPos, BlockMeta, a_FromBlock) ||  // Repeater is next to wire
				IsRepeaterPointingTo(a_BlockPos, BlockMeta, a_FromBlock - Vector3i(0, 1, 0))  // Repeater is below wire
			)
			{
				return 2;
			}
			else if (IsRepeaterPointingAway(a_BlockPos, BlockMeta, a_FromBlock))
			{
				SetRepeater(a_BlockPos, 10, false);
			}
			// fall-through:
		}
		
		case E_BLOCK_REDSTONE_REPEATER_OFF:
		{
			if (IsRepeaterPointingAway(a_BlockPos, BlockMeta, a_FromBlock))
			{
				SetRepeater(a_BlockPos, 10, false);
			}
			break;
		}
		
		default:
		{
			if (
				(BlockType != E_BLOCK_AIR) &&
				(BlockType != E_BLOCK_REDSTONE_TORCH_ON) &&
				(BlockType != E_BLOCK_REDSTONE_TORCH_OFF) &&
				(BlockType != E_BLOCK_LEVER)
			)
			{
				if (!IsPowered(a_BlockPos, true))
				{
					m_RefreshTorchesAround.push_back(a_BlockPos);
				}
			}
			break;
		}
	}  // switch (BlockType)

	return 0;
}





// Removes current from all powered redstone wires until it reaches an energy source.
// Also returns all energy sources it encountered
cRedstoneSimulator::BlockList cRedstoneSimulator::RemoveCurrent(const Vector3i & a_BlockPos)
{


	std::deque< Vector3i > SpreadStack;
	std::deque< Vector3i > FoundSources;

	Vector3i Surroundings[] = {
		Vector3i(1, 0, 0),
		Vector3i(1, 1, 0),
		Vector3i(1,-1, 0),
		Vector3i(-1, 0, 0),
		Vector3i(-1, 1, 0),
		Vector3i(-1,-1, 0),
		Vector3i(0, 0, 1),
		Vector3i(0, 1, 1),
		Vector3i(0,-1, 1),
		Vector3i(0, 0,-1),
		Vector3i(0, 1,-1),
		Vector3i(0,-1,-1),
		Vector3i(0,-1, 0),
	};

	BLOCKTYPE BlockType;
	NIBBLETYPE BlockMeta;
	m_World.GetBlockTypeMeta(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z, BlockType, BlockMeta);
	switch (BlockType)
	{
		case E_BLOCK_REDSTONE_REPEATER_ON:
		case E_BLOCK_REDSTONE_REPEATER_OFF:
		{
			// Repeaters only spread to their front front and 0 or 1 block up
			static Vector3i Surroundings [] = {
				Vector3i(0, 0, 0),
				Vector3i(0, 1, 0),
			};
			Vector3i Direction = GetRepeaterDirection(BlockMeta);
			for (unsigned int i = 0; i < ARRAYCOUNT(Surroundings); ++i)
			{
				Vector3i pos = a_BlockPos + Direction + Surroundings[i];
				int RetVal = UnPowerBlock(pos, a_BlockPos);
				if (RetVal == 1)
				{
					// Changed, so add to stack
					SpreadStack.push_back(pos);
				}
				else if (RetVal == 2)
				{
					FoundSources.push_back(pos);
				}
			}
			break;
		}
		
		case E_BLOCK_REDSTONE_TORCH_OFF:
		case E_BLOCK_REDSTONE_TORCH_ON:
		case E_BLOCK_LEVER:
		{
			static Vector3i Surroundings [] = {	// Torches only spread on the same level
				Vector3i(-1, 0, 0),
				Vector3i(1, 0, 0),
				Vector3i(0, 0,-1),
				Vector3i(0, 0, 1),
			};

			for (unsigned int i = 0; i < ARRAYCOUNT(Surroundings); ++i)
			{
				Vector3i pos = Vector3i(a_BlockPos) + Surroundings[i];
				int RetVal = UnPowerBlock(pos, a_BlockPos);
				if (RetVal == 1)
				{
					SpreadStack.push_back(pos); // Changed, so add to stack
				}
				else if (RetVal == 2)
				{
					FoundSources.push_back(pos);
				}
			}
			break;
		}
			
		default:
		{
			SpreadStack.push_back(a_BlockPos);
			break;
		}
	}  // switch (BlockType)


	while (!SpreadStack.empty())
	{
		Vector3i pos = SpreadStack.back();
		SpreadStack.pop_back();

		for (unsigned int i = 0; i < ARRAYCOUNT(Surroundings); ++i)
		{
			Vector3i OtherPos =  pos + Surroundings[i];
			int RetVal = UnPowerBlock(OtherPos, pos);
			if (RetVal == 1)
			{
				SpreadStack.push_back(OtherPos); // Changed, so add to stack
			}
			else if (RetVal == 2)
			{
				FoundSources.push_back(OtherPos);
			}
		}
	}

	return FoundSources;
}





bool cRedstoneSimulator::IsPowering(const Vector3i & a_PowerPos, const Vector3i & a_BlockPos, eRedstoneDirection a_WireDirection, bool a_bOnlyByWire)
{
	BLOCKTYPE PowerBlock;
	NIBBLETYPE PowerMeta;
	m_World.GetBlockTypeMeta(a_PowerPos.x, a_PowerPos.y, a_PowerPos.z, PowerBlock, PowerMeta);
	
	// Filter out powering blocks for a_bOnlyByWire
	if (
		!a_bOnlyByWire && (
			(PowerBlock == E_BLOCK_REDSTONE_TORCH_ON) ||
			(PowerBlock == E_BLOCK_LEVER)
		)
	)
	{
		return true;
	}
	
	switch (PowerBlock)
	{
		case E_BLOCK_REDSTONE_REPEATER_ON:
		{
			// A repeater pointing towards block is regarded as wire
			if (IsRepeaterPointingTo(a_PowerPos, PowerMeta, a_BlockPos))
			{
				return true;
			}
			break;
		}
		
		case E_BLOCK_REDSTONE_WIRE:
		{
			if (PowerMeta > 0)
			{
				if (GetWireDirection(a_PowerPos) == a_WireDirection)
				{
					return true;
				}
			}
			break;
		}
	}  // switch (PowerBlock)

	return false;
}





bool cRedstoneSimulator::IsPowered(const Vector3i & a_BlockPos, bool a_bOnlyByWire /* = false */)
{
	BLOCKTYPE BlockType;
	NIBBLETYPE BlockMeta;
	m_World.GetBlockTypeMeta(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z, BlockType, BlockMeta);
	if ((BlockType == E_BLOCK_REDSTONE_REPEATER_OFF) || (BlockType == E_BLOCK_REDSTONE_REPEATER_ON))
	{
		Vector3i Behind = a_BlockPos - GetRepeaterDirection(BlockMeta);
		BLOCKTYPE BehindBlock;
		NIBBLETYPE BehindMeta;
		m_World.GetBlockTypeMeta(Behind.x, Behind.y, Behind.z, BehindBlock, BehindMeta);
		switch (BehindBlock)
		{
			case E_BLOCK_REDSTONE_TORCH_ON:
			case E_BLOCK_LEVER:
			{
				// _X: TODO: Shouldn't a lever be checked if it is switched on?
				return true;
			}
			case E_BLOCK_REDSTONE_WIRE:
			{
				return (BehindMeta > 0);
			}
			case E_BLOCK_REDSTONE_REPEATER_ON:
			{
				return IsRepeaterPointingTo(Behind, BehindMeta, a_BlockPos);
			}
		}  // switch (BehindBlock)
		return false;
	}

	if (IsPowering(Vector3i(a_BlockPos.x - 1, a_BlockPos.y, a_BlockPos.z), a_BlockPos, REDSTONE_X_POS, a_bOnlyByWire))
	{
		return true;
	}
	if (IsPowering(Vector3i(a_BlockPos.x + 1, a_BlockPos.y, a_BlockPos.z), a_BlockPos, REDSTONE_X_NEG, a_bOnlyByWire))
	{
		return true;
	}
	if (IsPowering(Vector3i(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z - 1), a_BlockPos, REDSTONE_Z_POS, a_bOnlyByWire))
	{
		return true;
	}
	if (IsPowering(Vector3i(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z + 1), a_BlockPos, REDSTONE_Z_NEG, a_bOnlyByWire))
	{
		return true;
	}
	
	// Only wires can power the bottom block
	BLOCKTYPE PosYType;
	NIBBLETYPE PosYMeta;
	m_World.GetBlockTypeMeta(a_BlockPos.x, a_BlockPos.y + 1, a_BlockPos.z, PosYType, PosYMeta);
	if (PosYType == E_BLOCK_REDSTONE_WIRE)
	{
		return (PosYMeta > 0);
	}

	return false;
}




// Believe me, it works!! TODO: Add repeaters and low/high wires
cRedstoneSimulator::eRedstoneDirection cRedstoneSimulator::GetWireDirection(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	int Dir = REDSTONE_NONE;

	BLOCKTYPE NegX = m_World.GetBlock(a_BlockX - 1, a_BlockY, a_BlockZ);
	if (
		(NegX == E_BLOCK_REDSTONE_WIRE) ||
		(NegX == E_BLOCK_REDSTONE_TORCH_ON) ||
		(NegX == E_BLOCK_LEVER)
	)
	{
		Dir |= (REDSTONE_X_POS);
	}
	
	BLOCKTYPE PosX = m_World.GetBlock(a_BlockX + 1, a_BlockY, a_BlockZ);
	if (
		(PosX == E_BLOCK_REDSTONE_WIRE) ||
		(PosX == E_BLOCK_REDSTONE_TORCH_ON) ||
		(PosX == E_BLOCK_LEVER)
	)
	{
		Dir |= (REDSTONE_X_NEG);
	}
	
	BLOCKTYPE NegZ = m_World.GetBlock(a_BlockX, a_BlockY, a_BlockZ - 1);
	if (
		(NegZ == E_BLOCK_REDSTONE_WIRE) ||
		(NegZ == E_BLOCK_REDSTONE_TORCH_ON) ||
		(NegZ == E_BLOCK_LEVER)
	)
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
	if (
		(PosZ == E_BLOCK_REDSTONE_WIRE) ||
		(PosZ == E_BLOCK_REDSTONE_TORCH_ON) ||
		(PosZ == E_BLOCK_LEVER)
	)
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





bool cRedstoneSimulator::IsRepeaterPointingTo(const Vector3i & a_RepeaterPos, char a_MetaData, const Vector3i & a_BlockPos)
{
	switch (a_MetaData & 0x3)
	{
		case 0x0:
		{
			if ((a_RepeaterPos - a_BlockPos).Equals(Vector3i(0, 0, 1)))
			{
				return true;
			}
			break;
		}
		
		case 0x1:
		{
			if ((a_RepeaterPos - a_BlockPos).Equals(Vector3i(-1, 0, 0)))
			{
				return true;
			}
			break;
		}
		
		case 0x2:
		{
			if ((a_RepeaterPos - a_BlockPos).Equals(Vector3i(0, 0,-1)))
			{
				return true;
			}
			break;
		}
		
		case 0x3:
		{
			if ((a_RepeaterPos - a_BlockPos).Equals(Vector3i(1, 0, 0)))
			{
				return true;
			}
			break;
		}
	}
	return false;
}





bool cRedstoneSimulator::IsRepeaterPointingAway(const Vector3i & a_RepeaterPos, char a_MetaData, const Vector3i & a_BlockPos)
{
	switch (a_MetaData & 0x3)
	{
		case 0x0:
		{
			if ((a_RepeaterPos - a_BlockPos).Equals(Vector3i(0, 0,-1)))
			{
				return true;
			}
			break;
		}
		
		case 0x1:
		{
			if ((a_RepeaterPos - a_BlockPos).Equals(Vector3i(1, 0, 0)))
			{
				return true;
			}
			break;
		}
		
		case 0x2:
		{
			if ((a_RepeaterPos - a_BlockPos).Equals(Vector3i(0, 0, 1)))
			{
				return true;
			}
			break;
		}
		
		case 0x3:
		{
			if ((a_RepeaterPos - a_BlockPos).Equals(Vector3i(-1, 0, 0)))
			{
				return true;
			}
			break;
		}
	}
	return false;
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





Vector3i cRedstoneSimulator::GetRepeaterDirection(NIBBLETYPE a_MetaData)
{
	switch (a_MetaData & 0x3)
	{
		case 0x0: return Vector3i(0, 0,-1);
		case 0x1: return Vector3i(1, 0, 0);
		case 0x2: return Vector3i(0, 0, 1);
		case 0x3: return Vector3i(-1, 0, 0);
	}
	return Vector3i();
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





bool cRedstoneSimulator::IsLeverOn(cWorld * a_World, const Vector3i & a_BlockPos)
{
	// Extract the metadata and ask the lower level:
	return IsLeverOn(a_World->GetBlockMeta(a_BlockPos));
}





bool cRedstoneSimulator::IsLeverOn(NIBBLETYPE a_BlockMeta)
{
	// Extract the ON bit from metadata and return if true if it is set:
	return ((a_BlockMeta & 0x8) == 0x8);
}





void cRedstoneSimulator::SetRepeater(const Vector3i & a_Position, int a_Ticks, bool a_bPowerOn)
{
	for (RepeaterList::iterator itr = m_SetRepeaters.begin(); itr != m_SetRepeaters.end(); ++itr)
	{
		sRepeaterChange & Change = *itr;
		if (Change.Position.Equals(a_Position))
		{
			if (Change.bPowerOn && !a_bPowerOn)
			{
				Change.bPowerOffNextTime = true;
			}
			if (a_bPowerOn)
			{
				Change.bPowerOffNextTime = false;
			}
			Change.bPowerOn |= a_bPowerOn;
			return;
		}
	}

	sRepeaterChange RC;
	RC.Position = a_Position;
	RC.Ticks = a_Ticks;
	RC.bPowerOn = a_bPowerOn;
	RC.bPowerOffNextTime = false;
	m_SetRepeaters.push_back(RC);
}




