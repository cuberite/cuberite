
// NBTChunkSerializer.cpp


#include "Globals.h"
#include "NBTChunkSerializer.h"
#include "EnchantmentSerializer.h"
#include "../ChunkDataCallback.h"
#include "../ItemGrid.h"
#include "../StringCompression.h"
#include "../UUID.h"
#include "FastNBT.h"

#include "../BlockEntities/BeaconEntity.h"
#include "../BlockEntities/BedEntity.h"
#include "../BlockEntities/BrewingstandEntity.h"
#include "../BlockEntities/ChestEntity.h"
#include "../BlockEntities/CommandBlockEntity.h"
#include "../BlockEntities/DispenserEntity.h"
#include "../BlockEntities/DropperEntity.h"
#include "../BlockEntities/EnchantingTableEntity.h"
#include "../BlockEntities/EnderChestEntity.h"
#include "../BlockEntities/EndPortalEntity.h"
#include "../BlockEntities/FurnaceEntity.h"
#include "../BlockEntities/HopperEntity.h"
#include "../BlockEntities/JukeboxEntity.h"
#include "../BlockEntities/MobSpawnerEntity.h"
#include "../BlockEntities/NoteEntity.h"
#include "../BlockEntities/SignEntity.h"
#include "../BlockEntities/MobHeadEntity.h"
#include "../BlockEntities/FlowerPotEntity.h"

#include "../Entities/Entity.h"
#include "../Entities/EnderCrystal.h"
#include "../Entities/FallingBlock.h"
#include "../Entities/Boat.h"
#include "../Entities/Minecart.h"
#include "../Entities/Pickup.h"
#include "../Entities/ArrowEntity.h"
#include "../Entities/SplashPotionEntity.h"
#include "../Entities/TNTEntity.h"
#include "../Entities/ExpOrb.h"
#include "../Entities/HangingEntity.h"
#include "../Entities/ItemFrame.h"
#include "../Entities/LeashKnot.h"
#include "../Entities/Painting.h"

#include "../Mobs/IncludeAllMonsters.h"





/** Collects and stores the chunk data via the cChunkDataCallback interface */
class SerializerCollector:
	public cChunkDataCopyCollector
{
public:

	// The data collected from the chunk:
	cChunkDef::BiomeMap mBiomes;
	UInt8 mVanillaBiomes[cChunkDef::Width * cChunkDef::Width];
	int mVanillaHeightMap[cChunkDef::Width * cChunkDef::Width];
	bool mBiomesAreValid;

	/** True if a tag has been opened in the callbacks and not yet closed. */
	bool mIsTagOpen;

	/** True if any Entity has already been received and processed. */
	bool mHasHadEntity;

	/** True if any BlockEntity has already been received and processed. */
	bool mHasHadBlockEntity;

	/** True if the chunk lighting is valid. */
	bool mIsLightValid;

	/** The NBT writer used to store the data. */
	cFastNBTWriter & mWriter;





	SerializerCollector(cFastNBTWriter & aWriter):
		mBiomesAreValid(false),
		mIsTagOpen(false),
		mHasHadEntity(false),
		mHasHadBlockEntity(false),
		mIsLightValid(false),
		mWriter(aWriter)
	{
	}





	virtual void LightIsValid(bool a_IsLightValid) override
	{
		mIsLightValid = a_IsLightValid;
	}





	virtual void HeightMap(const cChunkDef::HeightMap * a_HeightMap) override
	{
		for (int RelX = 0; RelX < cChunkDef::Width; RelX++)
		{
			for (int RelZ = 0; RelZ < cChunkDef::Width; RelZ++)
			{
				int Height = cChunkDef::GetHeight(*a_HeightMap, RelX, RelZ);
				mVanillaHeightMap[(RelZ << 4) | RelX] = Height;
			}
		}
	}





	virtual void BiomeData(const cChunkDef::BiomeMap * a_BiomeMap) override
	{
		memcpy(mBiomes, a_BiomeMap, sizeof(mBiomes));
		for (size_t i = 0; i < ARRAYCOUNT(mBiomes); i++)
		{
			if ((*a_BiomeMap)[i] < 255)
			{
				// Normal MC biome, copy as-is:
				mVanillaBiomes[i] = static_cast<Byte>((*a_BiomeMap)[i]);
			}
			else
			{
				// TODO: MCS-specific biome, need to map to some basic MC biome:
				ASSERT(!"Unimplemented MCS-specific biome");
				return;
			}
		}  // for i - mBiomeMap[]
		mBiomesAreValid = true;
	}





	virtual void Entity(cEntity * a_Entity) override
	{
		// Add entity into NBT:
		if (mIsTagOpen)
		{
			if (!mHasHadEntity)
			{
				mWriter.EndList();
				mWriter.BeginList("Entities", TAG_Compound);
			}
		}
		else
		{
			mWriter.BeginList("Entities", TAG_Compound);
		}
		mIsTagOpen = true;
		mHasHadEntity = true;

		switch (a_Entity->GetEntityType())
		{
			case cEntity::etBoat:         AddBoatEntity        (static_cast<cBoat *>            (a_Entity)); break;
			case cEntity::etEnderCrystal: AddEnderCrystalEntity(static_cast<cEnderCrystal *>    (a_Entity)); break;
			case cEntity::etFallingBlock: AddFallingBlockEntity(static_cast<cFallingBlock *>    (a_Entity)); break;
			case cEntity::etMinecart:     AddMinecartEntity    (static_cast<cMinecart *>        (a_Entity)); break;
			case cEntity::etMonster:      AddMonsterEntity     (static_cast<cMonster *>         (a_Entity)); break;
			case cEntity::etPickup:       AddPickupEntity      (static_cast<cPickup *>          (a_Entity)); break;
			case cEntity::etProjectile:   AddProjectileEntity  (static_cast<cProjectileEntity *>(a_Entity)); break;
			case cEntity::etTNT:          AddTNTEntity         (static_cast<cTNTEntity *>       (a_Entity)); break;
			case cEntity::etExpOrb:       AddExpOrbEntity      (static_cast<cExpOrb *>          (a_Entity)); break;
			case cEntity::etItemFrame:    AddItemFrameEntity   (static_cast<cItemFrame *>       (a_Entity)); break;
			case cEntity::etLeashKnot:    AddLeashKnotEntity   (static_cast<cLeashKnot *>       (a_Entity)); break;
			case cEntity::etPainting:     AddPaintingEntity    (static_cast<cPainting *>        (a_Entity)); break;
			case cEntity::etPlayer: return;  // Players aren't saved into the world
			case cEntity::etFloater: return;  // Floaters aren't saved either
			default:
			{
				ASSERT(!"Unhandled entity type is being saved");
				break;
			}
		}
	}





	virtual void BlockEntity(cBlockEntity * a_Entity) override
	{
		if (mIsTagOpen)
		{
			if (!mHasHadBlockEntity)
			{
				mWriter.EndList();
				mWriter.BeginList("TileEntities", TAG_Compound);
			}
		}
		else
		{
			mWriter.BeginList("TileEntities", TAG_Compound);
		}
		mIsTagOpen = true;

		// Add tile-entity into NBT:
		switch (a_Entity->GetBlockType())
		{
			case E_BLOCK_BEACON:            AddBeaconEntity         (static_cast<cBeaconEntity *>         (a_Entity)); break;
			case E_BLOCK_BED:               AddBedEntity            (static_cast<cBedEntity *>            (a_Entity)); break;
			case E_BLOCK_BREWING_STAND:     AddBrewingstandEntity   (static_cast<cBrewingstandEntity *>   (a_Entity)); break;
			case E_BLOCK_CHEST:             AddChestEntity          (static_cast<cChestEntity *>          (a_Entity), a_Entity->GetBlockType()); break;
			case E_BLOCK_COMMAND_BLOCK:     AddCommandBlockEntity   (static_cast<cCommandBlockEntity *>   (a_Entity)); break;
			case E_BLOCK_DISPENSER:         AddDispenserEntity      (static_cast<cDispenserEntity *>      (a_Entity)); break;
			case E_BLOCK_DROPPER:           AddDropperEntity        (static_cast<cDropperEntity *>        (a_Entity)); break;
			case E_BLOCK_ENCHANTMENT_TABLE: AddEnchantingTableEntity(static_cast<cEnchantingTableEntity *>(a_Entity)); break;
			case E_BLOCK_ENDER_CHEST:       AddEnderchestEntity     (static_cast<cEnderChestEntity *>     (a_Entity)); break;
			case E_BLOCK_END_PORTAL:        AddEndPortalEntity      (static_cast<cEndPortalEntity *>      (a_Entity)); break;
			case E_BLOCK_FLOWER_POT:        AddFlowerPotEntity      (static_cast<cFlowerPotEntity *>      (a_Entity)); break;
			case E_BLOCK_FURNACE:           AddFurnaceEntity        (static_cast<cFurnaceEntity *>        (a_Entity)); break;
			case E_BLOCK_HEAD:              AddMobHeadEntity        (static_cast<cMobHeadEntity *>        (a_Entity)); break;
			case E_BLOCK_HOPPER:            AddHopperEntity         (static_cast<cHopperEntity *>         (a_Entity)); break;
			case E_BLOCK_JUKEBOX:           AddJukeboxEntity        (static_cast<cJukeboxEntity *>        (a_Entity)); break;
			case E_BLOCK_LIT_FURNACE:       AddFurnaceEntity        (static_cast<cFurnaceEntity *>        (a_Entity)); break;
			case E_BLOCK_MOB_SPAWNER:       AddMobSpawnerEntity     (static_cast<cMobSpawnerEntity *>     (a_Entity)); break;
			case E_BLOCK_NOTE_BLOCK:        AddNoteEntity           (static_cast<cNoteEntity *>           (a_Entity)); break;
			case E_BLOCK_SIGN_POST:         AddSignEntity           (static_cast<cSignEntity *>           (a_Entity)); break;
			case E_BLOCK_TRAPPED_CHEST:     AddChestEntity          (static_cast<cChestEntity *>          (a_Entity), a_Entity->GetBlockType()); break;
			case E_BLOCK_WALLSIGN:          AddSignEntity           (static_cast<cSignEntity *>           (a_Entity)); break;

			default:
			{
				ASSERT(!"Unhandled block entity saved into Anvil");
			}
		}
		mHasHadBlockEntity = true;
	}





	void Finish(void)
	{
		if (mIsTagOpen)
		{
			mWriter.EndList();
		}

		// If light not valid, reset it to defaults:
		if (!mIsLightValid)
		{
			m_Data.FillBlockLight(0x00);
			m_Data.FillSkyLight(0x0f);
		}

		// Check if "Entity" and "TileEntities" lists exists. MCEdit requires this.
		if (!mHasHadEntity)
		{
			mWriter.BeginList("Entities", TAG_Compound);
			mWriter.EndList();
		}
		if (!mHasHadBlockEntity)
		{
			mWriter.BeginList("TileEntities", TAG_Compound);
			mWriter.EndList();
		}
	}





	/** Writes an item into the writer.
	If aSlot >= 0, adds the Slot tag.
	The compound is named as requested (empty name by default). */
	void AddItem(const cItem & a_Item, int a_Slot, const AString & a_CompoundName = AString())
	{
		mWriter.BeginCompound(a_CompoundName);
		mWriter.AddShort("id",         static_cast<Int16>(a_Item.m_ItemType));
		mWriter.AddShort("Damage",     static_cast<Int16>((a_Item.m_ItemDamage)));
		mWriter.AddByte ("Count",      static_cast<Byte>(a_Item.m_ItemCount));
		if (a_Slot >= 0)
		{
			mWriter.AddByte ("Slot", static_cast<unsigned char>(a_Slot));
		}

		// Write the tag compound (for enchantment, firework, custom name and repair cost):
		if (
			(!a_Item.m_Enchantments.IsEmpty()) ||
			((a_Item.m_ItemType == E_ITEM_FIREWORK_ROCKET) || (a_Item.m_ItemType == E_ITEM_FIREWORK_STAR)) ||
			(a_Item.m_RepairCost > 0) ||
			(a_Item.m_CustomName != "") ||
			(!a_Item.m_LoreTable.empty())
		)
		{
			mWriter.BeginCompound("tag");
				if (a_Item.m_RepairCost > 0)
				{
					mWriter.AddInt("RepairCost", a_Item.m_RepairCost);
				}

				if ((a_Item.m_CustomName != "") || (!a_Item.m_LoreTable.empty()))
				{
					mWriter.BeginCompound("display");
					if (a_Item.m_CustomName != "")
					{
						mWriter.AddString("Name", a_Item.m_CustomName);
					}
					if (!a_Item.m_LoreTable.empty())
					{
						mWriter.BeginList("Lore", TAG_String);

						for (const auto & Line : a_Item.m_LoreTable)
						{
							mWriter.AddString("", Line);
						}

						mWriter.EndList();
					}
					mWriter.EndCompound();
				}

				if ((a_Item.m_ItemType == E_ITEM_FIREWORK_ROCKET) || (a_Item.m_ItemType == E_ITEM_FIREWORK_STAR))
				{
					cFireworkItem::WriteToNBTCompound(a_Item.m_FireworkItem, mWriter, static_cast<ENUM_ITEM_TYPE>(a_Item.m_ItemType));
				}

				if (!a_Item.m_Enchantments.IsEmpty())
				{
					const char * TagName = (a_Item.m_ItemType == E_ITEM_BOOK) ? "StoredEnchantments" : "ench";
					EnchantmentSerializer::WriteToNBTCompound(a_Item.m_Enchantments, mWriter, TagName);
				}
			mWriter.EndCompound();
		}

		mWriter.EndCompound();
	}





	/** Writes an item grid into the writer.
	Begins the stored slot numbers with a_BeginSlotNum.
	Note that it doesn't begin nor end the list tag, so that multiple grids may be concatenated together using this function. */
	void AddItemGrid(const cItemGrid & a_Grid, int a_BeginSlotNum = 0)
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
		}  // for i - slots[]
	}





	void AddBasicTileEntity(cBlockEntity * a_Entity, const char * a_EntityTypeID)
	{
		mWriter.AddInt   ("x",  a_Entity->GetPosX());
		mWriter.AddInt   ("y",  a_Entity->GetPosY());
		mWriter.AddInt   ("z",  a_Entity->GetPosZ());
		mWriter.AddString("id", a_EntityTypeID);
	}





	void AddBeaconEntity(cBeaconEntity * a_Entity)
	{
		mWriter.BeginCompound("");
			AddBasicTileEntity(a_Entity, "Beacon");
			mWriter.AddInt("Levels", a_Entity->GetBeaconLevel());
			mWriter.AddInt("Primary", static_cast<int>(a_Entity->GetPrimaryEffect()));
			mWriter.AddInt("Secondary", static_cast<int>(a_Entity->GetSecondaryEffect()));
			mWriter.BeginList("Items", TAG_Compound);
				AddItemGrid(a_Entity->GetContents());
			mWriter.EndList();
		mWriter.EndCompound();
	}





	void AddBedEntity(cBedEntity * a_Entity)
	{
		mWriter.BeginCompound("");
		AddBasicTileEntity(a_Entity, "Bed");
		mWriter.AddInt("color", a_Entity->GetColor());
		mWriter.EndCompound();
	}





	void AddBrewingstandEntity(cBrewingstandEntity * a_Brewingstand)
	{
		mWriter.BeginCompound("");
			AddBasicTileEntity(a_Brewingstand, "Brewingstand");
			mWriter.BeginList("Items", TAG_Compound);
				AddItemGrid(a_Brewingstand->GetContents());
			mWriter.EndList();
			mWriter.AddShort("BrewTime", a_Brewingstand->GetTimeBrewed());
			mWriter.AddShort("Fuel", a_Brewingstand->GetRemainingFuel());
		mWriter.EndCompound();
	}





	void AddChestEntity(cChestEntity * a_Entity, BLOCKTYPE a_ChestType)
	{
		mWriter.BeginCompound("");
			AddBasicTileEntity(a_Entity, "Chest");
			mWriter.BeginList("Items", TAG_Compound);
				AddItemGrid(a_Entity->GetContents());
			mWriter.EndList();
		mWriter.EndCompound();
	}





	void AddDispenserEntity(cDispenserEntity * a_Entity)
	{
		mWriter.BeginCompound("");
			AddBasicTileEntity(a_Entity, "Trap");
			mWriter.BeginList("Items", TAG_Compound);
				AddItemGrid(a_Entity->GetContents());
			mWriter.EndList();
		mWriter.EndCompound();
	}





	void AddDropperEntity(cDropperEntity * a_Entity)
	{
		mWriter.BeginCompound("");
			AddBasicTileEntity(a_Entity, "Dropper");
			mWriter.BeginList("Items", TAG_Compound);
				AddItemGrid(a_Entity->GetContents());
			mWriter.EndList();
		mWriter.EndCompound();
	}





	void AddEnchantingTableEntity(cEnchantingTableEntity * a_Entity)
	{
		mWriter.BeginCompound("");
			AddBasicTileEntity(a_Entity, "EnchantingTable");
			if (!a_Entity->GetCustomName().empty())
			{
				mWriter.AddString("CustomName", a_Entity->GetCustomName());
			}
		mWriter.EndCompound();
	}




	void AddEnderchestEntity(cEnderChestEntity * a_Entity)
	{
		mWriter.BeginCompound("");
			AddBasicTileEntity(a_Entity, "EnderChest");
		mWriter.EndCompound();
	}




	void AddEndPortalEntity(cEndPortalEntity * a_Entity)
	{
		mWriter.BeginCompound("");
			AddBasicTileEntity(a_Entity, "EndPortal");
		mWriter.EndCompound();
	}





	void AddFurnaceEntity(cFurnaceEntity * a_Furnace)
	{
		mWriter.BeginCompound("");
			AddBasicTileEntity(a_Furnace, "Furnace");
			mWriter.BeginList("Items", TAG_Compound);
				AddItemGrid(a_Furnace->GetContents());
			mWriter.EndList();
			mWriter.AddShort("BurnTime", static_cast<Int16>(a_Furnace->GetFuelBurnTimeLeft()));
			mWriter.AddShort("CookTime", static_cast<Int16>(a_Furnace->GetTimeCooked()));
		mWriter.EndCompound();
	}





	void AddHopperEntity(cHopperEntity * a_Entity)
	{
		mWriter.BeginCompound("");
			AddBasicTileEntity(a_Entity, "Hopper");
			mWriter.BeginList("Items", TAG_Compound);
				AddItemGrid(a_Entity->GetContents());
			mWriter.EndList();
		mWriter.EndCompound();
	}





	void AddJukeboxEntity(cJukeboxEntity * a_Jukebox)
	{
		mWriter.BeginCompound("");
			AddBasicTileEntity(a_Jukebox, "RecordPlayer");
			mWriter.AddInt("Record", a_Jukebox->GetRecord());
		mWriter.EndCompound();
	}





	void AddMobSpawnerEntity(cMobSpawnerEntity * a_MobSpawner)
	{
		mWriter.BeginCompound("");
			AddBasicTileEntity(a_MobSpawner, "MobSpawner");
			mWriter.AddString("EntityId", cMonster::MobTypeToVanillaName(a_MobSpawner->GetEntity()));
			mWriter.AddShort("SpawnCount", a_MobSpawner->GetSpawnCount());
			mWriter.AddShort("SpawnRange", a_MobSpawner->GetSpawnRange());
			mWriter.AddShort("Delay", a_MobSpawner->GetSpawnDelay());
			mWriter.AddShort("MinSpawnDelay", a_MobSpawner->GetMinSpawnDelay());
			mWriter.AddShort("MaxSpawnDelay", a_MobSpawner->GetMaxSpawnDelay());
			mWriter.AddShort("MaxNearbyEntities", a_MobSpawner->GetMaxNearbyEntities());
			mWriter.AddShort("RequiredPlayerRange", a_MobSpawner->GetRequiredPlayerRange());
		mWriter.EndCompound();
	}





	void AddNoteEntity(cNoteEntity * a_Note)
	{
		mWriter.BeginCompound("");
			AddBasicTileEntity(a_Note, "Music");
			mWriter.AddByte("note", static_cast<Byte>(a_Note->GetNote()));
		mWriter.EndCompound();
	}





	void AddCommandBlockEntity(cCommandBlockEntity * a_CmdBlock)
	{
		mWriter.BeginCompound("");
			AddBasicTileEntity(a_CmdBlock, "Control");
			mWriter.AddString("Command",      a_CmdBlock->GetCommand());
			mWriter.AddInt   ("SuccessCount", a_CmdBlock->GetResult());
			mWriter.AddString("LastOutput",   a_CmdBlock->GetLastOutput());
			mWriter.AddByte  ("TrackOutput",  1);  // TODO 2014-01-18 xdot: Figure out what TrackOutput is and save it.
		mWriter.EndCompound();
	}





	void AddSignEntity(cSignEntity * a_Sign)
	{
		mWriter.BeginCompound("");
			AddBasicTileEntity(a_Sign, "Sign");
			mWriter.AddString("Text1",   a_Sign->GetLine(0));
			mWriter.AddString("Text2",   a_Sign->GetLine(1));
			mWriter.AddString("Text3",   a_Sign->GetLine(2));
			mWriter.AddString("Text4",   a_Sign->GetLine(3));
		mWriter.EndCompound();
	}





	void AddMobHeadEntity(cMobHeadEntity * a_MobHead)
	{
		mWriter.BeginCompound("");
			AddBasicTileEntity(a_MobHead, "Skull");
			mWriter.AddByte  ("SkullType", a_MobHead->GetType() & 0xFF);
			mWriter.AddByte  ("Rot",       a_MobHead->GetRotation() & 0xFF);

			// The new Block Entity format for a Mob Head. See: https://minecraft.gamepedia.com/Head#Block_entity
			mWriter.BeginCompound("Owner");
				mWriter.AddString("Id", a_MobHead->GetOwnerUUID().ToShortString());
				mWriter.AddString("Name", a_MobHead->GetOwnerName());
				mWriter.BeginCompound("Properties");
					mWriter.BeginList("textures", TAG_Compound);
						mWriter.BeginCompound("");
							mWriter.AddString("Signature", a_MobHead->GetOwnerTextureSignature());
							mWriter.AddString("Value", a_MobHead->GetOwnerTexture());
						mWriter.EndCompound();
					mWriter.EndList();
				mWriter.EndCompound();
			mWriter.EndCompound();
		mWriter.EndCompound();
	}





	void AddFlowerPotEntity(cFlowerPotEntity * a_FlowerPot)
	{
		mWriter.BeginCompound("");
			AddBasicTileEntity(a_FlowerPot, "FlowerPot");
			mWriter.AddInt   ("Item", static_cast<Int32>(a_FlowerPot->GetItem().m_ItemType));
			mWriter.AddInt   ("Data", static_cast<Int32>(a_FlowerPot->GetItem().m_ItemDamage));
		mWriter.EndCompound();
	}





	void AddBasicEntity(cEntity * a_Entity, const AString & a_ClassName)
	{
		mWriter.AddString("id", a_ClassName);
		mWriter.BeginList("Pos", TAG_Double);
			mWriter.AddDouble("", a_Entity->GetPosX());
			mWriter.AddDouble("", a_Entity->GetPosY());
			mWriter.AddDouble("", a_Entity->GetPosZ());
		mWriter.EndList();
		mWriter.BeginList("Motion", TAG_Double);
			mWriter.AddDouble("", a_Entity->GetSpeedX());
			mWriter.AddDouble("", a_Entity->GetSpeedY());
			mWriter.AddDouble("", a_Entity->GetSpeedZ());
		mWriter.EndList();
		mWriter.BeginList("Rotation", TAG_Double);
			mWriter.AddDouble("", a_Entity->GetYaw());
			mWriter.AddDouble("", a_Entity->GetPitch());
		mWriter.EndList();
		mWriter.AddFloat("Health", a_Entity->GetHealth());
	}





	void AddBoatEntity(cBoat * a_Boat)
	{
		mWriter.BeginCompound("");
			AddBasicEntity(a_Boat, "Boat");
			mWriter.AddString("Type", cBoat::MaterialToString(a_Boat->GetMaterial()));
		mWriter.EndCompound();
	}





	void AddEnderCrystalEntity(cEnderCrystal * a_EnderCrystal)
	{
		mWriter.BeginCompound("");
			AddBasicEntity(a_EnderCrystal, "EnderCrystal");
			mWriter.AddByte("ShowBottom", a_EnderCrystal->ShowsBottom() ? 1 : 0);
			if (a_EnderCrystal->DisplaysBeam())
			{
				mWriter.BeginCompound("BeamTarget");
				const auto & BeamTarget = a_EnderCrystal->GetBeamTarget();
				mWriter.AddInt("X", BeamTarget.x);
				mWriter.AddInt("Y", BeamTarget.y);
				mWriter.AddInt("Z", BeamTarget.z);
				mWriter.EndCompound();
			}
		mWriter.EndCompound();
	}





	void AddFallingBlockEntity(cFallingBlock * a_FallingBlock)
	{
		mWriter.BeginCompound("");
			AddBasicEntity(a_FallingBlock, "FallingSand");
			mWriter.AddInt("TileID", a_FallingBlock->GetBlockType());
			mWriter.AddByte("Data", a_FallingBlock->GetBlockMeta());
			mWriter.AddByte("Time", 1);  // Unused in Cuberite, Vanilla said to need nonzero
			mWriter.AddByte("DropItem", 1);
			mWriter.AddByte("HurtEntities", a_FallingBlock->GetBlockType() == E_BLOCK_ANVIL);
		mWriter.EndCompound();
	}





	void AddMinecartEntity(cMinecart * a_Minecart)
	{
		mWriter.BeginCompound("");

			switch (a_Minecart->GetPayload())
			{
				case cMinecart::mpChest:
				{
					AddBasicEntity(a_Minecart, "MinecartChest");
					// Add chest contents into the Items tag:
					AddMinecartChestContents(static_cast<cMinecartWithChest *>(a_Minecart));
					break;
				}
				case cMinecart::mpFurnace:
				{
					AddBasicEntity(a_Minecart, "MinecartFurnace");
					// TODO: Add "Push" and "Fuel" tags
					break;
				}
				case cMinecart::mpHopper:
				{
					AddBasicEntity(a_Minecart, "MinecartHopper");
					// TODO: Add hopper contents?
					break;
				}
				case cMinecart::mpTNT:
				{
					AddBasicEntity(a_Minecart, "MinecartTNT");
					break;
				}
				case cMinecart::mpNone:
				{
					AddBasicEntity(a_Minecart, "MinecartRideable");
					break;
				}
			}  // switch (Payload)

		mWriter.EndCompound();
	}





	void AddMonsterEntity(cMonster * a_Monster)
	{
		const char * EntityClass = nullptr;
		switch (a_Monster->GetMobType())
		{
			case mtBat:            EntityClass = "Bat";            break;
			case mtBlaze:          EntityClass = "Blaze";          break;
			case mtCaveSpider:     EntityClass = "CaveSpider";     break;
			case mtChicken:        EntityClass = "Chicken";        break;
			case mtCow:            EntityClass = "Cow";            break;
			case mtCreeper:        EntityClass = "Creeper";        break;
			case mtEnderDragon:    EntityClass = "EnderDragon";    break;
			case mtEnderman:       EntityClass = "Enderman";       break;
			case mtGhast:          EntityClass = "Ghast";          break;
			case mtGiant:          EntityClass = "Giant";          break;
			case mtGuardian:       EntityClass = "Guardian";       break;
			case mtHorse:          EntityClass = "Horse";          break;
			case mtIronGolem:      EntityClass = "VillagerGolem";  break;
			case mtMagmaCube:      EntityClass = "LavaSlime";      break;
			case mtMooshroom:      EntityClass = "MushroomCow";    break;
			case mtOcelot:         EntityClass = "Ozelot";         break;
			case mtPig:            EntityClass = "Pig";            break;
			case mtRabbit:         EntityClass = "Rabbit";         break;
			case mtSheep:          EntityClass = "Sheep";          break;
			case mtSilverfish:     EntityClass = "Silverfish";     break;
			case mtSkeleton:       EntityClass = "Skeleton";       break;
			case mtSlime:          EntityClass = "Slime";          break;
			case mtSnowGolem:      EntityClass = "SnowMan";        break;
			case mtSpider:         EntityClass = "Spider";         break;
			case mtSquid:          EntityClass = "Squid";          break;
			case mtVillager:       EntityClass = "Villager";       break;
			case mtWitch:          EntityClass = "Witch";          break;
			case mtWither:         EntityClass = "WitherBoss";     break;
			case mtWitherSkeleton: EntityClass = "WitherSkeleton"; break;
			case mtWolf:           EntityClass = "Wolf";           break;
			case mtZombie:         EntityClass = "Zombie";         break;
			case mtZombiePigman:   EntityClass = "PigZombie";      break;
			case mtZombieVillager: EntityClass = "ZombieVillager"; break;
			default:
			{
				ASSERT(!"Unhandled monster type");
				return;
			}
		}  // switch (payload)

		mWriter.BeginCompound("");
			AddBasicEntity(a_Monster, EntityClass);
			mWriter.BeginList("DropChances", TAG_Float);
				mWriter.AddFloat("", a_Monster->GetDropChanceWeapon());
				mWriter.AddFloat("", a_Monster->GetDropChanceHelmet());
				mWriter.AddFloat("", a_Monster->GetDropChanceChestplate());
				mWriter.AddFloat("", a_Monster->GetDropChanceLeggings());
				mWriter.AddFloat("", a_Monster->GetDropChanceBoots());
			mWriter.EndList();
			mWriter.AddByte("CanPickUpLoot", (a_Monster->CanPickUpLoot())? 1 : 0);
			mWriter.AddString("CustomName", a_Monster->GetCustomName());
			mWriter.AddByte("CustomNameVisible", static_cast<Byte>(a_Monster->IsCustomNameAlwaysVisible()));

			// Mob was leashed
			if (a_Monster->IsLeashed() || (a_Monster->GetLeashToPos() != nullptr))
			{
				mWriter.AddByte("Leashed", 1);

				const Vector3d * LeashedToPos = nullptr;

				if (a_Monster->GetLeashToPos() != nullptr)
				{
					LeashedToPos = a_Monster->GetLeashToPos();
				}
				else if (a_Monster->GetLeashedTo()->IsLeashKnot())
				{
					LeashedToPos = & a_Monster->GetLeashedTo()->GetPosition();
				}

				if (LeashedToPos != nullptr)
				{
					mWriter.BeginCompound("Leash");
					mWriter.AddDouble("X", LeashedToPos->x);
					mWriter.AddDouble("Y", LeashedToPos->y);
					mWriter.AddDouble("Z", LeashedToPos->z);
					mWriter.EndCompound();
				}
			}

			switch (a_Monster->GetMobType())
			{
				case mtBat:
				{
					mWriter.AddByte("BatFlags", static_cast<const cBat *>(a_Monster)->IsHanging());
					break;
				}
				case mtCreeper:
				{
					const cCreeper *Creeper = static_cast<const cCreeper *>(a_Monster);
					mWriter.AddByte("powered", Creeper->IsCharged());
					mWriter.AddByte("ignited", Creeper->IsBlowing());
					break;
				}
				case mtEnderman:
				{
					const cEnderman *Enderman = static_cast<const cEnderman *>(a_Monster);
					mWriter.AddShort("carried",     static_cast<Int16>(Enderman->GetCarriedBlock()));
					mWriter.AddShort("carriedData", static_cast<Int16>(Enderman->GetCarriedMeta()));
					break;
				}
				case mtHorse:
				{
					const cHorse *Horse = static_cast<const cHorse *>(a_Monster);
					mWriter.AddByte("ChestedHorse",   Horse->IsChested()? 1 : 0);
					mWriter.AddByte("EatingHaystack", Horse->IsEating()? 1 : 0);
					mWriter.AddByte("Tame",           Horse->IsTame()? 1: 0);
					mWriter.AddInt ("Type",           Horse->GetHorseType());
					mWriter.AddInt ("Color",          Horse->GetHorseColor());
					mWriter.AddInt ("Style",          Horse->GetHorseStyle());
					mWriter.AddInt ("ArmorType",      Horse->GetHorseArmour());
					mWriter.AddByte("Saddle",         Horse->IsSaddled()? 1 : 0);
					mWriter.AddInt ("Age",            Horse->GetAge());
					break;
				}
				case mtMagmaCube:
				{
					mWriter.AddInt("Size", static_cast<const cMagmaCube *>(a_Monster)->GetSize());
					break;
				}
				case mtOcelot:
				{
					const auto *Ocelot = static_cast<const cOcelot *>(a_Monster);
					if (!Ocelot->GetOwnerName().empty())
					{
						mWriter.AddString("Owner", Ocelot->GetOwnerName());
					}
					if (!Ocelot->GetOwnerUUID().IsNil())
					{
						mWriter.AddString("OwnerUUID", Ocelot->GetOwnerUUID().ToShortString());
					}
					mWriter.AddByte("Sitting", Ocelot->IsSitting() ? 1 : 0);
					mWriter.AddInt("CatType", Ocelot->GetOcelotType());
					mWriter.AddInt("Age", Ocelot->GetAge());
					break;
				}
				case mtPig:
				{
					mWriter.AddInt("Age", static_cast<const cPig *>(a_Monster)->GetAge());
					break;
				}
				case mtRabbit:
				{
					const cRabbit * Rabbit = static_cast<const cRabbit *>(a_Monster);
					mWriter.AddInt("RabbitType", static_cast<Int32>(Rabbit->GetRabbitType()));
					mWriter.AddInt("MoreCarrotTicks", Rabbit->GetMoreCarrotTicks());
					mWriter.AddInt("Age", Rabbit->GetAge());
					break;
				}
				case mtSheep:
				{
					const cSheep *Sheep = static_cast<const cSheep *>(a_Monster);
					mWriter.AddByte("Sheared", Sheep->IsSheared()? 1 : 0);
					mWriter.AddByte("Color",   static_cast<Byte>(Sheep->GetFurColor()));
					mWriter.AddInt ("Age",     Sheep->GetAge());
					break;
				}
				case mtSlime:
				{
					mWriter.AddInt("Size", static_cast<const cSlime *>(a_Monster)->GetSize());
					break;
				}
				case mtVillager:
				{
					const cVillager *Villager = static_cast<const cVillager *>(a_Monster);
					mWriter.AddInt("Profession", Villager->GetVilType());
					mWriter.AddInt("Age",        Villager->GetAge());
					break;
				}
				case mtWither:
				{
					mWriter.AddInt("Invul", static_cast<Int32>(static_cast<const cWither *>(a_Monster)->GetWitherInvulnerableTicks()));
					break;
				}
				case mtWolf:
				{
					const cWolf *Wolf = static_cast<const cWolf *>(a_Monster);
					if (!Wolf->GetOwnerName().empty())
					{
						mWriter.AddString("Owner", Wolf->GetOwnerName());
					}
					if (!Wolf->GetOwnerUUID().IsNil())
					{
						mWriter.AddString("OwnerUUID", Wolf->GetOwnerUUID().ToShortString());
					}
					mWriter.AddByte("Sitting",     Wolf->IsSitting() ? 1 : 0);
					mWriter.AddByte("Angry",       Wolf->IsAngry() ? 1 : 0);
					mWriter.AddByte("CollarColor", static_cast<Byte>(Wolf->GetCollarColor()));
					mWriter.AddInt ("Age",         Wolf->GetAge());
					break;
				}
				case mtZombie:
				{
					mWriter.AddInt("Age", static_cast<const cZombie *>(a_Monster)->GetAge());
					break;
				}
				case mtZombiePigman:
				{
					mWriter.AddInt("Age", static_cast<const cZombiePigman *>(a_Monster)->GetAge());
					break;
				}
				case mtZombieVillager:
				{
					const cZombieVillager *ZombieVillager = reinterpret_cast<const cZombieVillager *>(a_Monster);
					mWriter.AddInt("Profession",     ZombieVillager->GetProfession());
					mWriter.AddInt("ConversionTime", ZombieVillager->ConversionTime());
					mWriter.AddInt("Age",            ZombieVillager->GetAge());
				}
				case mtBlaze:
				case mtCaveSpider:
				case mtChicken:
				case mtCow:
				case mtEnderDragon:
				case mtGhast:
				case mtGiant:
				case mtGuardian:
				case mtIronGolem:
				case mtMooshroom:
				case mtSilverfish:
				case mtSkeleton:
				case mtSnowGolem:
				case mtSpider:
				case mtSquid:
				case mtWitch:
				case mtWitherSkeleton:
				{
					// Other mobs have no special tags.
					break;
				}
				case mtInvalidType:
				{
					ASSERT(!"NBTChunkSerializer::SerializerCollector::AddMonsterEntity: Recieved mob of invalid type");
					break;
				}
			}
		mWriter.EndCompound();
	}





	void AddPickupEntity(cPickup * a_Pickup)
	{
		mWriter.BeginCompound("");
			AddBasicEntity(a_Pickup, "Item");
			AddItem(a_Pickup->GetItem(), -1, "Item");
			mWriter.AddShort("Age",    static_cast<Int16>(a_Pickup->GetAge()));
		mWriter.EndCompound();
	}





	void AddProjectileEntity(cProjectileEntity * a_Projectile)
	{
		mWriter.BeginCompound("");
			AddBasicEntity(a_Projectile, a_Projectile->GetMCAClassName());
			mWriter.AddByte("inGround", a_Projectile->IsInGround() ? 1 : 0);

			switch (a_Projectile->GetProjectileKind())
			{
				case cProjectileEntity::pkArrow:
				{
					cArrowEntity * Arrow = static_cast<cArrowEntity *>(a_Projectile);

					mWriter.AddShort("xTile", static_cast<Int16>(Arrow->GetBlockHit().x));
					mWriter.AddShort("yTile", static_cast<Int16>(Arrow->GetBlockHit().y));
					mWriter.AddShort("zTile", static_cast<Int16>(Arrow->GetBlockHit().z));
					mWriter.AddByte("pickup",   Arrow->GetPickupState());
					mWriter.AddDouble("damage", Arrow->GetDamageCoeff());
					break;
				}
				case cProjectileEntity::pkSplashPotion:
				{
					cSplashPotionEntity * Potion = static_cast<cSplashPotionEntity *>(a_Projectile);

					mWriter.AddInt("EffectType",                static_cast<Int16>(Potion->GetEntityEffectType()));
					mWriter.AddInt("EffectDuration",            static_cast<Int16>(Potion->GetEntityEffect().GetDuration()));
					mWriter.AddShort("EffectIntensity",         Potion->GetEntityEffect().GetIntensity());
					mWriter.AddDouble("EffectDistanceModifier", Potion->GetEntityEffect().GetDistanceModifier());
					mWriter.AddInt("PotionName",                Potion->GetPotionColor());
					break;
				}
				case cProjectileEntity::pkGhastFireball:
				{
					mWriter.AddInt("ExplosionPower", 1);
					break;
				}
				case cProjectileEntity::pkFireCharge:
				case cProjectileEntity::pkWitherSkull:
				case cProjectileEntity::pkEnderPearl:
				{
					break;
				}
				default:
				{
					ASSERT(!"Unsaved projectile entity!");
				}
			}  // switch (ProjectileKind)

			if (!a_Projectile->GetCreatorName().empty())
			{
				mWriter.AddString("ownerName", a_Projectile->GetCreatorName());
			}
		mWriter.EndCompound();
	}





	void AddHangingEntity(cHangingEntity * a_Hanging)
	{
		mWriter.AddInt("TileX", FloorC(a_Hanging->GetPosX()));
		mWriter.AddInt("TileY", FloorC(a_Hanging->GetPosY()));
		mWriter.AddInt("TileZ", FloorC(a_Hanging->GetPosZ()));
		mWriter.AddByte("Facing", a_Hanging->GetProtocolFacing());
	}





	void AddTNTEntity(cTNTEntity * a_TNT)
	{
		mWriter.BeginCompound("");
			AddBasicEntity(a_TNT, "PrimedTnt");
			mWriter.AddByte("Fuse", static_cast<unsigned char>(a_TNT->GetFuseTicks()));
		mWriter.EndCompound();
	}





	void AddExpOrbEntity(cExpOrb * a_ExpOrb)
	{
		mWriter.BeginCompound("");
			AddBasicEntity(a_ExpOrb, "XPOrb");
			mWriter.AddShort("Age", static_cast<Int16>(a_ExpOrb->GetAge()));
			mWriter.AddShort("Value", static_cast<Int16>(a_ExpOrb->GetReward()));
		mWriter.EndCompound();
	}





	void AddItemFrameEntity(cItemFrame * a_ItemFrame)
	{
		mWriter.BeginCompound("");
			AddBasicEntity(a_ItemFrame, "ItemFrame");
			AddHangingEntity(a_ItemFrame);
			AddItem(a_ItemFrame->GetItem(), -1, "Item");
			mWriter.AddByte("ItemRotation", static_cast<Byte>(a_ItemFrame->GetItemRotation()));
			mWriter.AddFloat("ItemDropChance", 1.0F);
		mWriter.EndCompound();
	}





	void AddLeashKnotEntity(cLeashKnot * a_LeashKnot)
	{
		mWriter.BeginCompound("");
			AddBasicEntity(a_LeashKnot, "LeashKnot");
			AddHangingEntity(a_LeashKnot);
		mWriter.EndCompound();
	}





	void AddPaintingEntity(cPainting * a_Painting)
	{
		mWriter.BeginCompound("");
			AddBasicEntity(a_Painting, "Painting");
			AddHangingEntity(a_Painting);
			mWriter.AddString("Motive", a_Painting->GetName());
		mWriter.EndCompound();
	}





	void AddMinecartChestContents(cMinecartWithChest * a_Minecart)
	{
		mWriter.BeginList("Items", TAG_Compound);
			for (int i = 0; i < cMinecartWithChest::ContentsHeight * cMinecartWithChest::ContentsWidth; i++)
			{
				const cItem & Item = a_Minecart->GetSlot(i);
				if (Item.IsEmpty())
				{
					continue;
				}
				AddItem(Item, i);
			}
		mWriter.EndList();
	}
};  // SerializerCollector





////////////////////////////////////////////////////////////////////////////////
// NBTChunkSerializer:

bool NBTChunkSerializer::serialize(const cWorld & aWorld, cChunkCoords aCoords, cFastNBTWriter & aWriter)
{
	SerializerCollector serializer(aWriter);
	aWriter.BeginCompound("Level");
	aWriter.AddInt("xPos", aCoords.m_ChunkX);
	aWriter.AddInt("zPos", aCoords.m_ChunkZ);
	if (!aWorld.GetChunkData(aCoords, serializer))
	{
		aWriter.EndCompound();  // "Level"
		return false;
	}
	serializer.Finish();  // Close NBT tags

	// Save biomes, both MCS (IntArray) and MC-vanilla (ByteArray):
	if (serializer.mBiomesAreValid)
	{
		aWriter.AddByteArray("Biomes",    reinterpret_cast<const char *>(serializer.mVanillaBiomes), ARRAYCOUNT(serializer.mVanillaBiomes));
		aWriter.AddIntArray ("MCSBiomes", reinterpret_cast<const int *>(serializer.mBiomes),         ARRAYCOUNT(serializer.mBiomes));
	}

	// Save heightmap (Vanilla require this):
	aWriter.AddIntArray("HeightMap", reinterpret_cast<const int *>(serializer.mVanillaHeightMap), ARRAYCOUNT(serializer.mVanillaHeightMap));

	// Save blockdata:
	aWriter.BeginList("Sections", TAG_Compound);
	for (size_t Y = 0; Y != cChunkData::NumSections; ++Y)
	{
		auto section = serializer.m_Data.GetSection(Y);
		if (section == nullptr)
		{
			continue;
		}

		aWriter.BeginCompound("");
		aWriter.AddByteArray("Blocks", reinterpret_cast<const char *>(section->m_BlockTypes), ARRAYCOUNT(section->m_BlockTypes));
		aWriter.AddByteArray("Data",   reinterpret_cast<const char *>(section->m_BlockMetas), ARRAYCOUNT(section->m_BlockMetas));

		#ifdef DEBUG_SKYLIGHT
			aWriter.AddByteArray("BlockLight", reinterpret_cast<const char *>(section->m_BlockSkyLight), ARRAYCOUNT(section->m_BlockSkyLight));
		#else
			aWriter.AddByteArray("BlockLight", reinterpret_cast<const char *>(section->m_BlockLight),    ARRAYCOUNT(section->m_BlockLight));
		#endif

		aWriter.AddByteArray("SkyLight", reinterpret_cast<const char *>(section->m_BlockSkyLight), ARRAYCOUNT(section->m_BlockSkyLight));
		aWriter.AddByte("Y", static_cast<unsigned char>(Y));
		aWriter.EndCompound();
	}
	aWriter.EndList();  // "Sections"

	// Store the information that the lighting is valid.
	// For compatibility reason, the default is "invalid" (missing) - this means older data is re-lighted upon loading.
	if (serializer.mIsLightValid)
	{
		aWriter.AddByte("MCSIsLightValid", 1);
	}

	// Save the world age to the chunk data. Required by vanilla and mcedit.
	aWriter.AddLong("LastUpdate", aWorld.GetWorldAge());

	// Store the flag that the chunk has all the ores, trees, dungeons etc. Cuberite chunks are always complete.
	aWriter.AddByte("TerrainPopulated", 1);

	aWriter.EndCompound();  // "Level"
	return true;
}
