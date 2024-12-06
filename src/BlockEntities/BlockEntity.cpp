
// BlockEntity.cpp

// Implements the cBlockEntity class that is the common ancestor for all block entities

#include "Globals.h"
#include "BannerEntity.h"
#include "BeaconEntity.h"
#include "BedEntity.h"
#include "BlockEntity.h"
#include "EnchantingTableEntity.h"
#include "BrewingstandEntity.h"
#include "ChestEntity.h"
#include "CommandBlockEntity.h"
#include "DispenserEntity.h"
#include "DropperEntity.h"
#include "EnderChestEntity.h"
#include "EndPortalEntity.h"
#include "FlowerPotEntity.h"
#include "FurnaceEntity.h"
#include "HopperEntity.h"
#include "MobHeadEntity.h"
#include "MobSpawnerEntity.h"
#include "JukeboxEntity.h"
#include "NoteEntity.h"
#include "ShulkerBoxEntity.h"
#include "SignEntity.h"





cBlockEntity::cBlockEntity(const BLOCKTYPE a_BlockType, const NIBBLETYPE a_BlockMeta, const Vector3i a_Pos, cWorld * const a_World) :
	m_Pos(a_Pos),
	m_RelX(a_Pos.x - cChunkDef::Width * FAST_FLOOR_DIV(a_Pos.x, cChunkDef::Width)),
	m_RelZ(a_Pos.z - cChunkDef::Width * FAST_FLOOR_DIV(a_Pos.z, cChunkDef::Width)),
	m_BlockType(a_BlockType),
	m_BlockMeta(a_BlockMeta),
	m_World(a_World)
{
}





OwnedBlockEntity cBlockEntity::Clone(const Vector3i a_Pos)
{
	auto res = CreateByBlockType(m_BlockType, m_BlockMeta, a_Pos, nullptr);
	res->CopyFrom(*this);
	return res;
}





cItems cBlockEntity::ConvertToPickups() const
{
	return {};
}





void cBlockEntity::CopyFrom(const cBlockEntity & a_Src)
{
	// Nothing to copy, but check that we're copying the right entity:
	ASSERT(m_BlockType == a_Src.m_BlockType);
	ASSERT(m_BlockMeta == a_Src.m_BlockMeta);
}





OwnedBlockEntity cBlockEntity::CreateByBlockType(const BLOCKTYPE a_BlockType, const NIBBLETYPE a_BlockMeta, const Vector3i a_Pos, cWorld * const a_World)
{
	switch (a_BlockType)
	{
		// Banners:
		case E_BLOCK_STANDING_BANNER:
		case E_BLOCK_WALL_BANNER:       return std::make_unique<cBannerEntity         >(a_BlockType, a_BlockMeta, a_Pos, a_World);

		// Others:
		case E_BLOCK_BEACON:            return std::make_unique<cBeaconEntity         >(a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_BED:               return std::make_unique<cBedEntity            >(a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_BREWING_STAND:     return std::make_unique<cBrewingstandEntity   >(a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_CHEST:             return std::make_unique<cChestEntity          >(a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_COMMAND_BLOCK:     return std::make_unique<cCommandBlockEntity   >(a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_DISPENSER:         return std::make_unique<cDispenserEntity      >(a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_DROPPER:           return std::make_unique<cDropperEntity        >(a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_ENCHANTMENT_TABLE: return std::make_unique<cEnchantingTableEntity>(a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_ENDER_CHEST:       return std::make_unique<cEnderChestEntity     >(a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_END_PORTAL:        return std::make_unique<cEndPortalEntity      >(a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_FLOWER_POT:        return std::make_unique<cFlowerPotEntity      >(a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_FURNACE:           return std::make_unique<cFurnaceEntity        >(a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_HEAD:              return std::make_unique<cMobHeadEntity        >(a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_HOPPER:            return std::make_unique<cHopperEntity         >(a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_JUKEBOX:           return std::make_unique<cJukeboxEntity        >(a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_LIT_FURNACE:       return std::make_unique<cFurnaceEntity        >(a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_MOB_SPAWNER:       return std::make_unique<cMobSpawnerEntity     >(a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_NOTE_BLOCK:        return std::make_unique<cNoteEntity           >(a_BlockType, a_BlockMeta, a_Pos, a_World);

		case E_BLOCK_WHITE_SHULKER_BOX:
		case E_BLOCK_ORANGE_SHULKER_BOX:
		case E_BLOCK_MAGENTA_SHULKER_BOX:
		case E_BLOCK_LIGHT_BLUE_SHULKER_BOX:
		case E_BLOCK_YELLOW_SHULKER_BOX:
		case E_BLOCK_LIME_SHULKER_BOX:
		case E_BLOCK_PINK_SHULKER_BOX:
		case E_BLOCK_GRAY_SHULKER_BOX:
		case E_BLOCK_LIGHT_GRAY_SHULKER_BOX:
		case E_BLOCK_CYAN_SHULKER_BOX:
		case E_BLOCK_PURPLE_SHULKER_BOX:
		case E_BLOCK_BLUE_SHULKER_BOX:
		case E_BLOCK_BROWN_SHULKER_BOX:
		case E_BLOCK_GREEN_SHULKER_BOX:
		case E_BLOCK_RED_SHULKER_BOX:
		case E_BLOCK_BLACK_SHULKER_BOX: return std::make_unique<cShulkerBoxEntity     >(a_BlockType, a_BlockMeta, a_Pos, a_World);

		case E_BLOCK_SIGN_POST:         return std::make_unique<cSignEntity           >(a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_TRAPPED_CHEST:     return std::make_unique<cChestEntity          >(a_BlockType, a_BlockMeta, a_Pos, a_World);
		case E_BLOCK_WALLSIGN:          return std::make_unique<cSignEntity           >(a_BlockType, a_BlockMeta, a_Pos, a_World);
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





void cBlockEntity::Destroy()
{
}





bool cBlockEntity::IsBlockEntityBlockType(const BLOCKTYPE a_BlockType)
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
		case E_BLOCK_ENCHANTMENT_TABLE:
		case E_BLOCK_ENDER_CHEST:
		case E_BLOCK_END_PORTAL:
		case E_BLOCK_FLOWER_POT:
		case E_BLOCK_FURNACE:
		case E_BLOCK_HEAD:
		case E_BLOCK_HOPPER:
		case E_BLOCK_JUKEBOX:
		case E_BLOCK_LIT_FURNACE:
		case E_BLOCK_MOB_SPAWNER:
		case E_BLOCK_NOTE_BLOCK:

		case E_BLOCK_WHITE_SHULKER_BOX:
		case E_BLOCK_ORANGE_SHULKER_BOX:
		case E_BLOCK_MAGENTA_SHULKER_BOX:
		case E_BLOCK_LIGHT_BLUE_SHULKER_BOX:
		case E_BLOCK_YELLOW_SHULKER_BOX:
		case E_BLOCK_LIME_SHULKER_BOX:
		case E_BLOCK_PINK_SHULKER_BOX:
		case E_BLOCK_GRAY_SHULKER_BOX:
		case E_BLOCK_LIGHT_GRAY_SHULKER_BOX:
		case E_BLOCK_CYAN_SHULKER_BOX:
		case E_BLOCK_PURPLE_SHULKER_BOX:
		case E_BLOCK_BLUE_SHULKER_BOX:
		case E_BLOCK_BROWN_SHULKER_BOX:
		case E_BLOCK_GREEN_SHULKER_BOX:
		case E_BLOCK_RED_SHULKER_BOX:
		case E_BLOCK_BLACK_SHULKER_BOX:

		case E_BLOCK_SIGN_POST:
		case E_BLOCK_STANDING_BANNER:
		case E_BLOCK_TRAPPED_CHEST:
		case E_BLOCK_WALL_BANNER:
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





void cBlockEntity::OnAddToWorld(cWorld & a_World, cChunk & a_Chunk)
{
	m_World = &a_World;
}





void cBlockEntity::OnRemoveFromWorld()
{
}





void cBlockEntity::SetPos(const Vector3i a_NewPos)
{
	ASSERT(m_World == nullptr);  // Cannot move block entities that represent world blocks (only use this for cBlockArea's BEs)
	m_Pos = a_NewPos;
}





void cBlockEntity::SetWorld(cWorld * const a_World)
{
	m_World = a_World;
}





bool cBlockEntity::Tick(const std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	UNUSED(a_Dt);
	return false;
}
