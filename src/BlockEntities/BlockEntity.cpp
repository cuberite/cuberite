
// BlockEntity.cpp

// Implements the cBlockEntity class that is the common ancestor for all block entities

#include "Globals.h"
#include "BeaconEntity.h"
#include "BlockEntity.h"
#include "ChestEntity.h"
#include "CommandBlockEntity.h"
#include "DispenserEntity.h"
#include "DropperEntity.h"
#include "EnderChestEntity.h"
#include "FlowerPotEntity.h"
#include "FurnaceEntity.h"
#include "HopperEntity.h"
#include "JukeboxEntity.h"
#include "NoteEntity.h"
#include "SignEntity.h"
#include "MobHeadEntity.h"





cBlockEntity * cBlockEntity::CreateByBlockType(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World)
{
	switch (a_BlockType)
	{
		case E_BLOCK_BEACON:        return new cBeaconEntity      (a_BlockX, a_BlockY, a_BlockZ, a_World);
		case E_BLOCK_CHEST:         return new cChestEntity       (a_BlockX, a_BlockY, a_BlockZ, a_World, a_BlockType);
		case E_BLOCK_COMMAND_BLOCK: return new cCommandBlockEntity(a_BlockX, a_BlockY, a_BlockZ, a_World);
		case E_BLOCK_DISPENSER:     return new cDispenserEntity   (a_BlockX, a_BlockY, a_BlockZ, a_World);
		case E_BLOCK_DROPPER:       return new cDropperEntity     (a_BlockX, a_BlockY, a_BlockZ, a_World);
		case E_BLOCK_ENDER_CHEST:   return new cEnderChestEntity  (a_BlockX, a_BlockY, a_BlockZ, a_World);
		case E_BLOCK_FLOWER_POT:    return new cFlowerPotEntity   (a_BlockX, a_BlockY, a_BlockZ, a_World);
		case E_BLOCK_FURNACE:       return new cFurnaceEntity     (a_BlockX, a_BlockY, a_BlockZ, a_BlockType, a_BlockMeta, a_World);
		case E_BLOCK_HEAD:          return new cMobHeadEntity     (a_BlockX, a_BlockY, a_BlockZ, a_World);
		case E_BLOCK_HOPPER:        return new cHopperEntity      (a_BlockX, a_BlockY, a_BlockZ, a_World);
		case E_BLOCK_JUKEBOX:       return new cJukeboxEntity     (a_BlockX, a_BlockY, a_BlockZ, a_World);
		case E_BLOCK_LIT_FURNACE:   return new cFurnaceEntity     (a_BlockX, a_BlockY, a_BlockZ, a_BlockType, a_BlockMeta, a_World);
		case E_BLOCK_SIGN_POST:     return new cSignEntity        (a_BlockType, a_BlockX, a_BlockY, a_BlockZ, a_World);
		case E_BLOCK_TRAPPED_CHEST: return new cChestEntity       (a_BlockX, a_BlockY, a_BlockZ, a_World, a_BlockType);
		case E_BLOCK_WALLSIGN:      return new cSignEntity        (a_BlockType, a_BlockX, a_BlockY, a_BlockZ, a_World);
		case E_BLOCK_NOTE_BLOCK:    return new cNoteEntity        (a_BlockX, a_BlockY, a_BlockZ, a_World);
	}
	LOGD("%s: Requesting creation of an unknown block entity - block type %d (%s)",
		__FUNCTION__, a_BlockType, ItemTypeToString(a_BlockType).c_str()
	);
	ASSERT(!"Requesting creation of an unknown block entity");
	return NULL;
}




