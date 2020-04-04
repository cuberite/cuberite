
// BlockEntity.cpp

// Implements the cBlockEntity class that is the common ancestor for all block entities

#include "Globals.h"
#include "BeaconEntity.h"
#include "BedEntity.h"
#include "BlockEntity.h"
#include "BrewingstandEntity.h"
#include "ChestEntity.h"
#include "CommandBlockEntity.h"
#include "DispenserEntity.h"
#include "DropperEntity.h"
#include "EnderChestEntity.h"
#include "FlowerPotEntity.h"
#include "FurnaceEntity.h"
#include "HopperEntity.h"
#include "MobHeadEntity.h"
#include "MobSpawnerEntity.h"
#include "JukeboxEntity.h"
#include "NoteEntity.h"
#include "SignEntity.h"





void cBlockEntity::SetPos(Vector3i a_NewPos)
{
	ASSERT(m_World == nullptr);  // Cannot move block entities that represent world blocks (only use this for cBlockArea's BEs)
	m_Pos = a_NewPos;
}





bool cBlockEntity::IsBlockEntityBlockType(BLOCKTYPE a_BlockType)
{
	switch (a_BlockType)
	{
		case E_BLOCK_BEACON:
		case E_BLOCK_BED:
		case E_BLOCK_BREWING_STAND:
		case E_BLOCK_CHEST:
		case E_BLOCK_COMMAND_BLOCK:
		case E_BLOCK_DISPENSER:
		case E_BLOCK_DROPPER:
		case E_BLOCK_ENDER_CHEST:
		case E_BLOCK_FLOWER_POT:
		case E_BLOCK_FURNACE:
		case E_BLOCK_HEAD:
		case E_BLOCK_HOPPER:
		case E_BLOCK_JUKEBOX:
		case E_BLOCK_LIT_FURNACE:
		case E_BLOCK_MOB_SPAWNER:
		case E_BLOCK_NOTE_BLOCK:
		case E_BLOCK_SIGN_POST:
		case E_BLOCK_TRAPPED_CHEST:
		case E_BLOCK_WALLSIGN:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}





OwnedBlockEntity cBlockEntity::CreateByBlockType(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World)
{
	switch (a_BlockType)
	{
		case E_BLOCK_BEACON:        return cpp14::make_unique<cBeaconEntity      >(a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_BED:           return cpp14::make_unique<cBedEntity         >(a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_BREWING_STAND: return cpp14::make_unique<cBrewingstandEntity>(a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_CHEST:         return cpp14::make_unique<cChestEntity       >(a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_COMMAND_BLOCK: return cpp14::make_unique<cCommandBlockEntity>(a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_DISPENSER:     return cpp14::make_unique<cDispenserEntity   >(a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_DROPPER:       return cpp14::make_unique<cDropperEntity     >(a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_ENDER_CHEST:   return cpp14::make_unique<cEnderChestEntity  >(a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_FLOWER_POT:    return cpp14::make_unique<cFlowerPotEntity   >(a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_FURNACE:       return cpp14::make_unique<cFurnaceEntity     >(a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_HEAD:          return cpp14::make_unique<cMobHeadEntity     >(a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_HOPPER:        return cpp14::make_unique<cHopperEntity      >(a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_JUKEBOX:       return cpp14::make_unique<cJukeboxEntity     >(a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_LIT_FURNACE:   return cpp14::make_unique<cFurnaceEntity     >(a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_MOB_SPAWNER:   return cpp14::make_unique<cMobSpawnerEntity  >(a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_NOTE_BLOCK:    return cpp14::make_unique<cNoteEntity        >(a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_SIGN_POST:     return cpp14::make_unique<cSignEntity        >(a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_TRAPPED_CHEST: return cpp14::make_unique<cChestEntity       >(a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_WALLSIGN:      return cpp14::make_unique<cSignEntity        >(a_BlockType, a_BlockMeta, a_Pos, a_World);
		default:
		{
			LOGD("%s: Requesting creation of an unknown block entity - block type %d (%s)",
				__FUNCTION__, a_BlockType, ItemTypeToString(a_BlockType).c_str()
			);
			ASSERT(!"Requesting creation of an unknown block entity");
			return nullptr;
		}
	}
}





OwnedBlockEntity cBlockEntity::Clone(Vector3i a_Pos)
{
	auto res = CreateByBlockType(m_BlockType, m_BlockMeta, a_Pos, nullptr);
	res->CopyFrom(*this);
	return res;
}





void cBlockEntity::CopyFrom(const cBlockEntity & a_Src)
{
	// Nothing to copy, but check that we're copying the right entity:
	ASSERT(m_BlockType == a_Src.m_BlockType);
	ASSERT(m_BlockMeta == a_Src.m_BlockMeta);
}
