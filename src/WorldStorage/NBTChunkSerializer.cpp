
// NBTChunkSerializer.cpp


#include "Globals.h"
#include "NBTChunkSerializer.h"
#include "../BlockID.h"
#include "../BlockEntities/ChestEntity.h"
#include "../BlockEntities/DispenserEntity.h"
#include "../BlockEntities/DropperEntity.h"
#include "../BlockEntities/FurnaceEntity.h"
#include "../BlockEntities/HopperEntity.h"
#include "../BlockEntities/JukeboxEntity.h"
#include "../BlockEntities/NoteEntity.h"
#include "../BlockEntities/SignEntity.h"
#include "../ItemGrid.h"
#include "../StringCompression.h"
#include "../Entities/Entity.h"
#include "FastNBT.h"
#include "../Entities/FallingBlock.h"
#include "../Entities/Boat.h"
#include "../Entities/Minecart.h"
#include "../Mobs/Monster.h"
#include "../Entities/Pickup.h"
#include "../Entities/ProjectileEntity.h"





cNBTChunkSerializer::cNBTChunkSerializer(cFastNBTWriter & a_Writer) :
	m_BiomesAreValid(false),
	m_Writer(a_Writer),
	m_IsTagOpen(false),
	m_HasHadEntity(false),
	m_HasHadBlockEntity(false),
	m_IsLightValid(false)
{
}





void cNBTChunkSerializer::Finish(void)
{
	if (m_IsTagOpen)
	{
		m_Writer.EndList();
	}
	
	// If light not valid, reset it to all zeroes:
	if (!m_IsLightValid)
	{
		memset(m_BlockLight,    0, sizeof(m_BlockLight));
		memset(m_BlockSkyLight, 0, sizeof(m_BlockSkyLight));
	}
}





void cNBTChunkSerializer::AddItem(const cItem & a_Item, int a_Slot, const AString & a_CompoundName)
{
	m_Writer.BeginCompound(a_CompoundName);
	m_Writer.AddShort("id",     (short)(a_Item.m_ItemType));
	m_Writer.AddShort("Damage", a_Item.m_ItemDamage);
	m_Writer.AddByte ("Count",  a_Item.m_ItemCount);
	if (a_Slot >= 0)
	{
		m_Writer.AddByte ("Slot", (unsigned char)a_Slot);
	}
	
	// Write the enchantments:
	if (!a_Item.m_Enchantments.IsEmpty())
	{
		const char * TagName = (a_Item.m_ItemType == E_ITEM_BOOK) ? "StoredEnchantments" : "ench";
		m_Writer.BeginCompound("tag");
			a_Item.m_Enchantments.WriteToNBTCompound(m_Writer, TagName);
		m_Writer.EndCompound();
	}
	
	m_Writer.EndCompound();
}





void cNBTChunkSerializer::AddItemGrid(const cItemGrid & a_Grid, int a_BeginSlotNum)
{
	int NumSlots = a_Grid.GetNumSlots();
	for (int i = 0; i < NumSlots; i++)
	{
		const cItem & Item = a_Grid.GetSlot(i);
		if (Item.IsEmpty())
		{
			continue;
		}
		AddItem(Item, i + a_BeginSlotNum);
	}  // for i - chest slots[]
}





void cNBTChunkSerializer::AddBasicTileEntity(cBlockEntity * a_Entity, const char * a_EntityTypeID)
{
	m_Writer.AddInt   ("x",  a_Entity->GetPosX());
	m_Writer.AddInt   ("y",  a_Entity->GetPosY());
	m_Writer.AddInt   ("z",  a_Entity->GetPosZ());
	m_Writer.AddString("id", a_EntityTypeID);
}





void cNBTChunkSerializer::AddChestEntity(cChestEntity * a_Entity)
{
	m_Writer.BeginCompound("");
		AddBasicTileEntity(a_Entity, "Chest");
		m_Writer.BeginList("Items", TAG_Compound);
			AddItemGrid(a_Entity->GetContents());
		m_Writer.EndList();
	m_Writer.EndCompound();
}





void cNBTChunkSerializer::AddDispenserEntity(cDispenserEntity * a_Entity)
{
	m_Writer.BeginCompound("");
		AddBasicTileEntity(a_Entity, "Trap");
		m_Writer.BeginList("Items", TAG_Compound);
			AddItemGrid(a_Entity->GetContents());
		m_Writer.EndList();
	m_Writer.EndCompound();
}





void cNBTChunkSerializer::AddDropperEntity(cDropperEntity * a_Entity)
{
	m_Writer.BeginCompound("");
		AddBasicTileEntity(a_Entity, "Dropper");
		m_Writer.BeginList("Items", TAG_Compound);
			AddItemGrid(a_Entity->GetContents());
		m_Writer.EndList();
	m_Writer.EndCompound();
}





void cNBTChunkSerializer::AddFurnaceEntity(cFurnaceEntity * a_Furnace)
{
	m_Writer.BeginCompound("");
		AddBasicTileEntity(a_Furnace, "Furnace");
		m_Writer.BeginList("Items", TAG_Compound);
			AddItemGrid(a_Furnace->GetContents());
		m_Writer.EndList();
		m_Writer.AddShort("BurnTime", a_Furnace->GetFuelBurnTimeLeft());
		m_Writer.AddShort("CookTime", a_Furnace->GetTimeCooked());
	m_Writer.EndCompound();
}





void cNBTChunkSerializer::AddHopperEntity(cHopperEntity * a_Entity)
{
	m_Writer.BeginCompound("");
		AddBasicTileEntity(a_Entity, "Hopper");
		m_Writer.BeginList("Items", TAG_Compound);
			AddItemGrid(a_Entity->GetContents());
		m_Writer.EndList();
	m_Writer.EndCompound();
}





void cNBTChunkSerializer::AddJukeboxEntity(cJukeboxEntity * a_Jukebox)
{
	m_Writer.BeginCompound("");
		AddBasicTileEntity(a_Jukebox, "RecordPlayer");
		m_Writer.AddInt("Record", a_Jukebox->GetRecord());
	m_Writer.EndCompound();
}





void cNBTChunkSerializer::AddNoteEntity(cNoteEntity * a_Note)
{
	m_Writer.BeginCompound("");
	AddBasicTileEntity(a_Note, "Music");
	m_Writer.AddByte("note", a_Note->GetPitch());
	m_Writer.EndCompound();
}





void cNBTChunkSerializer::AddSignEntity(cSignEntity * a_Sign)
{
	m_Writer.BeginCompound("");
	AddBasicTileEntity(a_Sign, "Sign");
	m_Writer.AddString("Text1",   a_Sign->GetLine(0));
	m_Writer.AddString("Text2",   a_Sign->GetLine(1));
	m_Writer.AddString("Text3",   a_Sign->GetLine(2));
	m_Writer.AddString("Text4",   a_Sign->GetLine(3));
	m_Writer.EndCompound();
}





void cNBTChunkSerializer::AddBasicEntity(cEntity * a_Entity, const AString & a_ClassName)
{
	m_Writer.AddString("id", a_ClassName);
	m_Writer.BeginList("Pos", TAG_Double);
		m_Writer.AddDouble("", a_Entity->GetPosX());
		m_Writer.AddDouble("", a_Entity->GetPosY());
		m_Writer.AddDouble("", a_Entity->GetPosZ());
	m_Writer.EndList();
	m_Writer.BeginList("Motion", TAG_Double);
		m_Writer.AddDouble("", a_Entity->GetSpeedX());
		m_Writer.AddDouble("", a_Entity->GetSpeedY());
		m_Writer.AddDouble("", a_Entity->GetSpeedZ());
	m_Writer.EndList();
	m_Writer.BeginList("Rotation", TAG_Double);
		m_Writer.AddDouble("", a_Entity->GetRotation());
		m_Writer.AddDouble("", a_Entity->GetPitch());
	m_Writer.EndList();
}





void cNBTChunkSerializer::AddBoatEntity(cBoat * a_Boat)
{
	m_Writer.BeginCompound("");
		AddBasicEntity(a_Boat, "Boat");
	m_Writer.EndCompound();
}





void cNBTChunkSerializer::AddFallingBlockEntity(cFallingBlock * a_FallingBlock)
{
	m_Writer.BeginCompound("");
		AddBasicEntity(a_FallingBlock, "FallingSand");
		m_Writer.AddInt("TileID", a_FallingBlock->GetBlockType());
		m_Writer.AddByte("Data", a_FallingBlock->GetBlockMeta());
		m_Writer.AddByte("Time", 1);  // Unused in MCServer, Vanilla said to need nonzero
		m_Writer.AddByte("DropItem", 1);
		m_Writer.AddByte("HurtEntities", a_FallingBlock->GetBlockType() == E_BLOCK_ANVIL);
	m_Writer.EndCompound();
}





void cNBTChunkSerializer::AddMinecartEntity(cMinecart * a_Minecart)
{
	const char * EntityClass = NULL;
	switch (a_Minecart->GetPayload())
	{
		case cMinecart::mpNone:    EntityClass = "MinecartRideable"; break;
		case cMinecart::mpChest:   EntityClass = "MinecartChest";    break;
		case cMinecart::mpFurnace: EntityClass = "MinecartFurnace";  break;
		case cMinecart::mpTNT:     EntityClass = "MinecartTNT";      break;
		case cMinecart::mpHopper:  EntityClass = "MinecartHopper";   break;
		default:
		{
			ASSERT(!"Unhandled minecart payload type");
			return;
		}
	}  // switch (payload)
	
	m_Writer.BeginCompound("");
		AddBasicEntity(a_Minecart, EntityClass);
		switch (a_Minecart->GetPayload())
		{
			case cMinecart::mpChest:
			{
				// Add chest contents into the Items tag:
				AddMinecartChestContents((cMinecartWithChest *)a_Minecart);
				break;
			}
			
			case cMinecart::mpFurnace:
			{
				// TODO: Add "Push" and "Fuel" tags
				break;
			}
		}  // switch (Payload)
	m_Writer.EndCompound();
}





void cNBTChunkSerializer::AddMonsterEntity(cMonster * a_Monster)
{
	// TODO
}





void cNBTChunkSerializer::AddPickupEntity(cPickup * a_Pickup)
{
	m_Writer.BeginCompound("");
		AddBasicEntity(a_Pickup, "Item");
		AddItem(a_Pickup->GetItem(), -1, "Item");
		m_Writer.AddShort("Health", a_Pickup->GetHealth());
		m_Writer.AddShort("Age",    a_Pickup->GetAge());
	m_Writer.EndCompound();
}





void cNBTChunkSerializer::AddProjectileEntity(cProjectileEntity * a_Projectile)
{
	m_Writer.BeginCompound("");
		AddBasicEntity(a_Projectile, a_Projectile->GetMCAClassName());
		Vector3d Pos = a_Projectile->GetPosition();
		m_Writer.AddShort("xTile",  (Int16)floor(Pos.x));
		m_Writer.AddShort("yTile",  (Int16)floor(Pos.y));
		m_Writer.AddShort("zTile",  (Int16)floor(Pos.z));
		m_Writer.AddShort("inTile", 0);  // TODO: Query the block type
		m_Writer.AddShort("shake",  0);  // TODO: Any shake?
		m_Writer.AddByte ("inGround", a_Projectile->IsInGround() ? 1 : 0);
		
		switch (a_Projectile->GetProjectileKind())
		{
			case cProjectileEntity::pkArrow:
			{
				m_Writer.AddByte("inData",   0);  // TODO: Query the block meta (is it needed?)
				m_Writer.AddByte("pickup",   ((cArrowEntity *)a_Projectile)->GetPickupState());
				m_Writer.AddDouble("damage", ((cArrowEntity *)a_Projectile)->GetDamageCoeff());
				break;
			}
			case cProjectileEntity::pkGhastFireball:
			{
				m_Writer.AddInt("ExplosionPower", 1);
				// fall-through:
			}
			case cProjectileEntity::pkFireCharge:
			case cProjectileEntity::pkWitherSkull:
			case cProjectileEntity::pkEnderPearl:
			{
				m_Writer.BeginList("Motion", TAG_Double);
					m_Writer.AddDouble("", a_Projectile->GetSpeedX());
					m_Writer.AddDouble("", a_Projectile->GetSpeedY());
					m_Writer.AddDouble("", a_Projectile->GetSpeedZ());
				m_Writer.EndList();
				break;
			}
			default:
			{
				ASSERT(!"Unsaved projectile entity!");
			}
		}  // switch (ProjectileKind)
		cEntity * Creator = a_Projectile->GetCreator();
		if (Creator != NULL)
		{
			if (Creator->GetEntityType() == cEntity::etPlayer)
			{
				m_Writer.AddString("ownerName", ((cPlayer *)Creator)->GetName());
			}
		}
	m_Writer.EndCompound();
}





void cNBTChunkSerializer::AddMinecartChestContents(cMinecartWithChest * a_Minecart)
{
	m_Writer.BeginList("Items", TAG_Compound);
		for (int i = 0; i < cMinecartWithChest::NumSlots; i++)
		{
			const cItem & Item = a_Minecart->GetSlot(i);
			if (Item.IsEmpty())
			{
				continue;
			}
			AddItem(Item, i);
		}
	m_Writer.EndList();
}





bool cNBTChunkSerializer::LightIsValid(bool a_IsLightValid)
{
	m_IsLightValid = a_IsLightValid;
	return a_IsLightValid;  // We want lighting only if it's valid, otherwise don't bother
}





void cNBTChunkSerializer::BiomeData(const cChunkDef::BiomeMap * a_BiomeMap)
{
	memcpy(m_Biomes, a_BiomeMap, sizeof(m_Biomes));
	for (size_t i = 0; i < ARRAYCOUNT(m_Biomes); i++)
	{
		if ((*a_BiomeMap)[i] < 255)
		{
			// Normal MC biome, copy as-is:
			m_VanillaBiomes[i] = (unsigned char)((*a_BiomeMap)[i]);
		}
		else
		{
			// TODO: MCS-specific biome, need to map to some basic MC biome:
			ASSERT(!"Unimplemented MCS-specific biome");
			return;
		}
	}  // for i - m_BiomeMap[]
	m_BiomesAreValid = true;
}





void cNBTChunkSerializer::Entity(cEntity * a_Entity)
{
	// Add entity into NBT:
	if (m_IsTagOpen)
	{
		if (!m_HasHadEntity)
		{
			m_Writer.EndList();
			m_Writer.BeginList("Entities", TAG_Compound);
		}
	}
	else
	{
		m_Writer.BeginList("Entities", TAG_Compound);
	}
	m_IsTagOpen = true;
	m_HasHadEntity = true;
	
	switch (a_Entity->GetEntityType())
	{
		case cEntity::etBoat:         AddBoatEntity        ((cBoat *)            a_Entity); break;
		case cEntity::etFallingBlock: AddFallingBlockEntity((cFallingBlock *)    a_Entity); break;
		case cEntity::etMinecart:     AddMinecartEntity    ((cMinecart *)        a_Entity); break;
		case cEntity::etMonster:      AddMonsterEntity     ((cMonster *)         a_Entity); break;
		case cEntity::etPickup:       AddPickupEntity      ((cPickup *)          a_Entity); break;
		case cEntity::etProjectile:   AddProjectileEntity  ((cProjectileEntity *)a_Entity); break;
		case cEntity::etPlayer: return;  // Players aren't saved into the world
		default:
		{
			ASSERT(!"Unhandled entity type is being saved");
			break;
		}
	}
}





void cNBTChunkSerializer::BlockEntity(cBlockEntity * a_Entity)
{
	if (m_IsTagOpen)
	{
		if (!m_HasHadBlockEntity)
		{
			m_Writer.EndList();
			m_Writer.BeginList("TileEntities", TAG_Compound);
		}
	}
	else
	{
		m_Writer.BeginList("TileEntities", TAG_Compound);
	}
	m_IsTagOpen = true;
	
	// Add tile-entity into NBT:
	switch (a_Entity->GetBlockType())
	{
		case E_BLOCK_CHEST:      AddChestEntity     ((cChestEntity *)     a_Entity); break;
		case E_BLOCK_DISPENSER:  AddDispenserEntity ((cDispenserEntity *) a_Entity); break;
		case E_BLOCK_DROPPER:    AddDropperEntity   ((cDropperEntity *)   a_Entity); break;
		case E_BLOCK_FURNACE:    AddFurnaceEntity   ((cFurnaceEntity *)   a_Entity); break;
		case E_BLOCK_HOPPER:     AddHopperEntity    ((cHopperEntity *)    a_Entity); break;
		case E_BLOCK_SIGN_POST:
		case E_BLOCK_WALLSIGN:   AddSignEntity      ((cSignEntity *)      a_Entity); break;
		case E_BLOCK_NOTE_BLOCK: AddNoteEntity      ((cNoteEntity *)      a_Entity); break;
		case E_BLOCK_JUKEBOX:    AddJukeboxEntity   ((cJukeboxEntity *)   a_Entity); break;
		default:
		{
			ASSERT(!"Unhandled block entity saved into Anvil");
		}
	}
	m_HasHadBlockEntity = true;
}




