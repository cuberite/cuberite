
// NBTChunkSerializer.cpp


#include "Globals.h"
#include "NBTChunkSerializer.h"
#include "EnchantmentSerializer.h"
#include "NamespaceSerializer.h"
#include "../ChunkDataCallback.h"
#include "../ItemGrid.h"
#include "../StringCompression.h"
#include "../UUID.h"
#include "../BlockInfo.h"
#include "FastNBT.h"
#include "../Blocks/BlockAnvil.h"

#include "../BlockEntities/BannerEntity.h"
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

#include "../Protocol/Palettes/Upgrade.h"
#include <Protocol/Palettes/BlockMap.h>

#include "JsonUtils.h"


/** Collects and stores the chunk data via the cChunkDataCallback interface */
class SerializerCollector final :
	public cChunkDataCopyCollector
{
public:

	// The data collected from the chunk:
	UInt8 Biomes[cChunkDef::Width * cChunkDef::Width];
	int Heights[cChunkDef::Width * cChunkDef::Width];

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





	virtual void HeightMap(const cChunkDef::HeightMap & a_HeightMap) override
	{
		for (int RelZ = 0; RelZ < cChunkDef::Width; RelZ++)
		{
			for (int RelX = 0; RelX < cChunkDef::Width; RelX++)
			{
				Heights[RelX + RelZ * cChunkDef::Width] = cChunkDef::GetHeight(a_HeightMap, RelX, RelZ);
			}
		}
	}





	virtual void BiomeMap(const cChunkDef::BiomeMap & a_BiomeMap) override
	{
		for (size_t i = 0; i < ARRAYCOUNT(Biomes); i++)
		{
			if (a_BiomeMap[i] < 255)
			{
				// Normal MC biome, copy as-is:
				Biomes[i] = static_cast<Byte>(a_BiomeMap[i]);
			}
			else
			{
				// TODO: MCS-specific biome, need to map to some basic MC biome:
				ASSERT(!"Unimplemented MCS-specific biome");
				return;
			}
		}  // for i - mBiomeMap[]
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
				mWriter.BeginList("block_entities", TAG_Compound);
			}
		}
		else
		{
			mWriter.BeginList("block_entities", TAG_Compound);
		}
		mIsTagOpen = true;

		// Add tile-entity into NBT:
		auto type = a_Entity->GetBlockType();
		switch (type)
		{
			case BlockType::BlackBanner:
			case BlockType::BlueBanner:
			case BlockType::BrownBanner:
			case BlockType::CyanBanner:
			case BlockType::GrayBanner:
			case BlockType::GreenBanner:
			case BlockType::LightBlueBanner:
			case BlockType::LightGrayBanner:
			case BlockType::LimeBanner:
			case BlockType::MagentaBanner:
			case BlockType::OrangeBanner:
			case BlockType::PinkBanner:
			case BlockType::PurpleBanner:
			case BlockType::RedBanner:
			case BlockType::WhiteBanner:
			case BlockType::YellowBanner:

			case BlockType::BlackWallBanner:
			case BlockType::BlueWallBanner:
			case BlockType::BrownWallBanner:
			case BlockType::CyanWallBanner:
			case BlockType::GrayWallBanner:
			case BlockType::GreenWallBanner:
			case BlockType::LightBlueWallBanner:
			case BlockType::LightGrayWallBanner:
			case BlockType::LimeWallBanner:
			case BlockType::MagentaWallBanner:
			case BlockType::OrangeWallBanner:
			case BlockType::PinkWallBanner:
			case BlockType::PurpleWallBanner:
			case BlockType::RedWallBanner:
			case BlockType::WhiteWallBanner:
			case BlockType::YellowWallBanner: AddBannerEntity         (static_cast<cBannerEntity *>         (a_Entity)); break;

			case BlockType::Beacon:         AddBeaconEntity         (static_cast<cBeaconEntity *>         (a_Entity)); break;
			case BlockType::BlackBed:
			case BlockType::BlueBed:
			case BlockType::BrownBed:
			case BlockType::CyanBed:
			case BlockType::GrayBed:
			case BlockType::GreenBed:
			case BlockType::LightBlueBed:
			case BlockType::LightGrayBed:
			case BlockType::LimeBed:
			case BlockType::MagentaBed:
			case BlockType::OrangeBed:
			case BlockType::PinkBed:
			case BlockType::PurpleBed:
			case BlockType::RedBed:
			case BlockType::WhiteBed:
			case BlockType::YellowBed:       AddBedEntity            (static_cast<cBedEntity *>            (a_Entity)); break;


			case BlockType::BrewingStand:    AddBrewingstandEntity   (static_cast<cBrewingstandEntity *>   (a_Entity)); break;
			case BlockType::Chest:           AddChestEntity          (static_cast<cChestEntity *>          (a_Entity), a_Entity->GetBlockType()); break;
			case BlockType::RepeatingCommandBlock:
			case BlockType::ChainCommandBlock:
			case BlockType::CommandBlock:    AddCommandBlockEntity   (static_cast<cCommandBlockEntity *>   (a_Entity)); break;
			case BlockType::Dispenser:       AddDispenserEntity      (static_cast<cDispenserEntity *>      (a_Entity)); break;
			case BlockType::Dropper:         AddDropperEntity        (static_cast<cDropperEntity *>        (a_Entity)); break;
			case BlockType::EnchantingTable: AddEnchantingTableEntity(static_cast<cEnchantingTableEntity *>(a_Entity)); break;
			case BlockType::EnderChest:      AddEnderchestEntity     (static_cast<cEnderChestEntity *>     (a_Entity)); break;
			case BlockType::EndPortal:       AddEndPortalEntity      (static_cast<cEndPortalEntity *>      (a_Entity)); break;
			case BlockType::FlowerPot:       AddFlowerPotEntity      (static_cast<cFlowerPotEntity *>      (a_Entity)); break;
			case BlockType::Furnace:         AddFurnaceEntity        (static_cast<cFurnaceEntity *>        (a_Entity)); break;

			case BlockType::CreeperHead:
			case BlockType::CreeperWallHead:
			case BlockType::DragonHead:
			case BlockType::DragonWallHead:
			case BlockType::PlayerHead:
			case BlockType::PlayerWallHead:
			case BlockType::ZombieHead:
			case BlockType::ZombieWallHead:
			case BlockType::SkeletonSkull:
			case BlockType::SkeletonWallSkull:
			case BlockType::WitherSkeletonSkull:
			case BlockType::WitherSkeletonWallSkull: AddMobHeadEntity        (static_cast<cMobHeadEntity *>        (a_Entity)); break;

			case BlockType::Hopper:          AddHopperEntity         (static_cast<cHopperEntity *>         (a_Entity)); break;
			case BlockType::Jukebox:         AddJukeboxEntity        (static_cast<cJukeboxEntity *>        (a_Entity)); break;
			case BlockType::Spawner:         AddMobSpawnerEntity     (static_cast<cMobSpawnerEntity *>     (a_Entity)); break;
			case BlockType::NoteBlock:       AddNoteEntity           (static_cast<cNoteEntity *>           (a_Entity)); break;

			case BlockType::AcaciaSign:
			case BlockType::AcaciaWallSign:
			case BlockType::BirchSign:
			case BlockType::BirchWallSign:
			case BlockType::CrimsonSign:
			case BlockType::CrimsonWallSign:
			case BlockType::DarkOakSign:
			case BlockType::DarkOakWallSign:
			case BlockType::JungleSign:
			case BlockType::JungleWallSign:
			case BlockType::OakSign:
			case BlockType::OakWallSign:
			case BlockType::SpruceSign:
			case BlockType::SpruceWallSign:
			case BlockType::WarpedSign:
			case BlockType::WarpedWallSign:  AddSignEntity           (static_cast<cSignEntity *>           (a_Entity)); break;

			case BlockType::TrappedChest:    AddChestEntity          (static_cast<cChestEntity *>          (a_Entity), a_Entity->GetBlockType()); break;

			case BlockType::PottedAcaciaSapling:
			case BlockType::PottedAzureBluet:
			case BlockType::PottedBamboo:
			case BlockType::PottedBirchSapling:
			case BlockType::PottedBlueOrchid:
			case BlockType::PottedBrownMushroom:
			case BlockType::PottedCactus:
			case BlockType::PottedCornflower:
			case BlockType::PottedCrimsonRoots:
			case BlockType::PottedCrimsonFungus:
			case BlockType::PottedDandelion:
			case BlockType::PottedDarkOakSapling:
			case BlockType::PottedDeadBush:
			case BlockType::PottedFern:
			case BlockType::PottedJungleSapling:
			case BlockType::PottedLilyOfTheValley:
			case BlockType::PottedOakSapling:
			case BlockType::PottedOrangeTulip:
			case BlockType::PottedOxeyeDaisy:
			case BlockType::PottedPinkTulip:
			case BlockType::PottedPoppy:
			case BlockType::PottedRedMushroom:
			case BlockType::PottedRedTulip:
			case BlockType::PottedSpruceSapling:
			case BlockType::PottedWarpedFungus:
			case BlockType::PottedWarpedRoots:
			case BlockType::PottedWhiteTulip:
			case BlockType::PottedWitherRose:
			case BlockType::PottedAllium:
			case BlockType::PottedAzaleaBush:
			case BlockType::PottedCherrySapling:
			case BlockType::PottedFloweringAzaleaBush:
			case BlockType::PottedMangrovePropagule:
			case BlockType::PottedPaleOakSapling:
			case BlockType::PottedTorchflower:  /* Flower pots aren't block entities anymore however, but old code thinks they are, so it tries to save them. This is to prevent crashes */
				FLOGWARNING("Tried to save a flower pot {} as block entity ignoring. TODO: fix flower pots", a_Entity->GetBlockType()); break;
			default:
			{
				FLOGERROR("{}: Got unintended block entity to save: {}", __FUNCTION__, a_Entity->GetBlockType());
				UNREACHABLE("Unhandled block entity saved into Anvil");
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

		// Check if "Entity" and "TileEntities" lists exists. MCEdit requires this.
		if (!mHasHadEntity)
		{
			mWriter.BeginList("Entities", TAG_Compound);
			mWriter.EndList();
		}
		if (!mHasHadBlockEntity)
		{
			mWriter.BeginList("block_entities", TAG_Compound);
			mWriter.EndList();
		}
	}





	/** Writes an item into the writer.
	If aSlot >= 0, adds the Slot tag.
	The compound is named as requested (empty name by default). */
	void AddItem(const cItem & a_Item, int a_Slot, const AString & a_CompoundName = AString())
	{
		mWriter.BeginCompound(a_CompoundName);
		mWriter.AddString("id",         NamespaceSerializer::From(a_Item.m_ItemType));
		mWriter.AddInt ("Count",      a_Item.m_ItemCount);
		if (a_Slot >= 0)
		{
			mWriter.AddByte ("Slot", static_cast<unsigned char>(a_Slot));
		}

		mWriter.BeginCompound("components");
		// TODO: item components
		mWriter.EndCompound();

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





	void AddBannerEntity(cBannerEntity * a_Entity)
	{
		mWriter.BeginCompound("");
			AddBasicTileEntity(a_Entity,"minecraft:banner");
			if (!a_Entity->GetCustomName().empty())
			{
				mWriter.AddString("CustomName", a_Entity->GetCustomName());
			}
		mWriter.EndCompound();
	}





	void AddBeaconEntity(cBeaconEntity * a_Entity)
	{
		mWriter.BeginCompound("");
			AddBasicTileEntity(a_Entity, "minecraft:beacon");
			if (a_Entity->GetPrimaryEffect() != cEntityEffect::effNoEffect)
			{
				mWriter.AddString("primary_effect", "minecraft:" + AString(NamespaceSerializer::From(a_Entity->GetPrimaryEffect())));
			}
			if (a_Entity->GetSecondaryEffect() != cEntityEffect::effNoEffect)
			{
				mWriter.AddString("secondary_effect", "minecraft:" + AString(NamespaceSerializer::From(a_Entity->GetSecondaryEffect())));
			}
			mWriter.BeginList("Items", TAG_Compound);
				AddItemGrid(a_Entity->GetContents());
			mWriter.EndList();
		mWriter.EndCompound();
	}





	void AddBedEntity(cBedEntity * a_Entity)
	{
		mWriter.BeginCompound("");
		AddBasicTileEntity(a_Entity, "minecraft:bed");
		mWriter.EndCompound();
	}





	void AddBrewingstandEntity(cBrewingstandEntity * a_Brewingstand)
	{
		mWriter.BeginCompound("");
			AddBasicTileEntity(a_Brewingstand, "minecraft:brewing_stand");
			mWriter.BeginList("Items", TAG_Compound);
				AddItemGrid(a_Brewingstand->GetContents());
			mWriter.EndList();
			mWriter.AddShort("BrewTime", a_Brewingstand->GetTimeBrewed());
			mWriter.AddByte("Fuel", static_cast<unsigned char>(a_Brewingstand->GetRemainingFuel()));
		mWriter.EndCompound();
	}





	void AddChestEntity(cChestEntity * a_Entity, BlockType a_ChestType)
	{
		mWriter.BeginCompound("");
			AddBasicTileEntity(a_Entity, "minecraft:chest");
			mWriter.BeginList("Items", TAG_Compound);
				AddItemGrid(a_Entity->GetContents());
			mWriter.EndList();
		mWriter.EndCompound();
	}





	void AddDispenserEntity(cDispenserEntity * a_Entity)
	{
		mWriter.BeginCompound("");
			AddBasicTileEntity(a_Entity, "minecraft:dispenser");
			mWriter.BeginList("Items", TAG_Compound);
				AddItemGrid(a_Entity->GetContents());
			mWriter.EndList();
		mWriter.EndCompound();
	}





	void AddDropperEntity(cDropperEntity * a_Entity)
	{
		mWriter.BeginCompound("");
			AddBasicTileEntity(a_Entity, "minecraft:dropper");
			mWriter.BeginList("Items", TAG_Compound);
				AddItemGrid(a_Entity->GetContents());
			mWriter.EndList();
		mWriter.EndCompound();
	}





	void AddEnchantingTableEntity(cEnchantingTableEntity * a_Entity)
	{
		mWriter.BeginCompound("");
			AddBasicTileEntity(a_Entity, "minecraft:enchanting_table");
			if (!a_Entity->GetCustomName().empty())
			{
				mWriter.AddString("CustomName", a_Entity->GetCustomName());
			}
		mWriter.EndCompound();
	}




	void AddEnderchestEntity(cEnderChestEntity * a_Entity)
	{
		mWriter.BeginCompound("");
			AddBasicTileEntity(a_Entity, "minecraft:ender_chest");
		mWriter.EndCompound();
	}




	void AddEndPortalEntity(cEndPortalEntity * a_Entity)
	{
		mWriter.BeginCompound("");
			AddBasicTileEntity(a_Entity, "minecraft:end_portal");
		mWriter.EndCompound();
	}





	void AddFurnaceEntity(cFurnaceEntity * a_Furnace)
	{
		mWriter.BeginCompound("");
			AddBasicTileEntity(a_Furnace, "minecraft:furnace");
			mWriter.BeginList("Items", TAG_Compound);
				AddItemGrid(a_Furnace->GetContents());
			mWriter.EndList();
			mWriter.AddShort("lit_time_remaining", static_cast<Int16>(a_Furnace->GetFuelBurnTimeLeft()));
			mWriter.AddShort("cooking_time_spent", static_cast<Int16>(a_Furnace->GetTimeCooked()));
		mWriter.EndCompound();
	}





	void AddHopperEntity(cHopperEntity * a_Entity)
	{
		mWriter.BeginCompound("");
			AddBasicTileEntity(a_Entity, "minecraft:hopper");
			mWriter.BeginList("Items", TAG_Compound);
				AddItemGrid(a_Entity->GetContents());
			mWriter.EndList();
		mWriter.EndCompound();
	}





	void AddJukeboxEntity(cJukeboxEntity * a_Jukebox)
	{
		mWriter.BeginCompound("");
			AddBasicTileEntity(a_Jukebox, "minecraft:jukebox");
			AddItem(a_Jukebox->GetRecord(), -1);
		mWriter.EndCompound();
	}





	void AddMobSpawnerEntity(cMobSpawnerEntity * a_MobSpawner)
	{
		mWriter.BeginCompound("");
			AddBasicTileEntity(a_MobSpawner, "minecraft:mob_spawner");
			mWriter.AddString("EntityId", NamespaceSerializer::From(a_MobSpawner->GetEntity()));
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
			AddBasicTileEntity(a_Note, "minecraft:noteblock");
			mWriter.AddByte("note", static_cast<Byte>(a_Note->GetNote()));
		mWriter.EndCompound();
	}





	void AddCommandBlockEntity(cCommandBlockEntity * a_CmdBlock)
	{
		mWriter.BeginCompound("");
			AddBasicTileEntity(a_CmdBlock, "minecraft:command_block");
			mWriter.AddString("Command",      a_CmdBlock->GetCommand());
			mWriter.AddInt   ("SuccessCount", a_CmdBlock->GetResult());
			mWriter.AddString("LastOutput",   a_CmdBlock->GetLastOutput());
			mWriter.AddByte  ("TrackOutput",  1);  // TODO 2014-01-18 xdot: Figure out what TrackOutput is and save it.
		mWriter.EndCompound();
	}





	void AddSignEntity(cSignEntity * a_Sign)
	{
		mWriter.BeginCompound("");
			AddBasicTileEntity(a_Sign, "minecraft:sign");
			mWriter.BeginCompound("front_text");
				mWriter.AddString("color", "black");
				mWriter.AddByte("has_glowing_text", false);
				mWriter.BeginList("messages", eTagType::TAG_String);
					mWriter.AddString("", a_Sign->GetLine(0));
					mWriter.AddString("", a_Sign->GetLine(1));
					mWriter.AddString("", a_Sign->GetLine(2));
					mWriter.AddString("", a_Sign->GetLine(3));
				mWriter.EndList();
			mWriter.EndCompound();
			mWriter.AddByte("is_waxed", false);
		mWriter.EndCompound();
	}





	void AddMobHeadEntity(cMobHeadEntity * a_MobHead)
	{
		mWriter.BeginCompound("");
		AddBasicTileEntity(a_MobHead, "minecraft:skull");
		if (a_MobHead->GetType() == Item::PlayerHead)
		{
			mWriter.BeginCompound("profile");
				mWriter.AddIntArray("id", reinterpret_cast<const Int32 *>(a_MobHead->GetOwnerUUID().ToRaw().data()), 4);
				mWriter.AddString("name", a_MobHead->GetOwnerName());
				mWriter.BeginList("properties", TAG_Compound);
					mWriter.BeginCompound("");
						mWriter.AddString("name", "textures");
						if (!a_MobHead->GetOwnerTextureSignature().empty())
						{
							mWriter.AddString("signature", a_MobHead->GetOwnerTextureSignature());
						}
						mWriter.AddString("value", a_MobHead->GetOwnerTexture());
					mWriter.EndCompound();
				mWriter.EndList();
			mWriter.EndCompound();
		}
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





	void AddBasicEntity(cEntity * a_Entity, const std::string_view a_ClassName)
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
			auto BlockData = PaletteUpgrade::ToBlock(a_FallingBlock->GetBlock());
			mWriter.AddInt("TileID", BlockData.first);
			mWriter.AddByte("Data", static_cast<unsigned char>(BlockData.second));
			mWriter.AddByte("Time", 1);  // Unused in Cuberite, Vanilla said to need nonzero
			mWriter.AddByte("DropItem", 1);
			mWriter.AddByte("HurtEntities", IsBlockAnvil(a_FallingBlock->GetBlock()));
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
		mWriter.BeginCompound("");
			AddBasicEntity(a_Monster, NamespaceSerializer::From(a_Monster->GetMobType()));
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
					auto NumericBlock = PaletteUpgrade::ToBlock(Enderman->GetCarriedBlock());
					mWriter.AddShort("carried",     static_cast<Int16>(NumericBlock.first));
					mWriter.AddShort("carriedData", static_cast<Int16>(NumericBlock.second));
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
					mWriter.BeginList("Inventory", TAG_Compound);
						AddItemGrid(Villager->GetInventory());
					mWriter.EndList();
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
					break;
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
				case mtCat:
				case mtCod:
				case mtDolphin:
				case mtDonkey:
				case mtDrowned:
				case mtElderGuardian:
				case mtEndermite:
				case mtEvoker:
				case mtFox:
				case mtHoglin:
				case mtHusk:
				case mtIllusioner:
				case mtLlama:
				case mtMule:
				case mtPanda:
				case mtParrot:
				case mtPhantom:
				case mtPiglin:
				case mtPiglinBrute:
				case mtPillager:
				case mtPolarBear:
				case mtPufferfish:
				case mtRavager:
				case mtSalmon:
				case mtShulker:
				case mtSkeletonHorse:
				case mtStray:
				case mtStrider:
				case mtTraderLlama:
				case mtTropicalFish:
				case mtTurtle:
				case mtVex:
				case mtVindicator:
				case mtWanderingTrader:
				case mtZoglin:
				case mtZombieHorse:
				case mtBee:
				{
					// All the entities not added
					LOGD("Saving unimplemented entity type: %d", NamespaceSerializer::From(a_Monster->GetMobType()));
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
					mWriter.AddByte("pickup", static_cast<unsigned char>(Arrow->GetPickupState()));
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
				case cProjectileEntity::pkSnowball:
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

void NBTChunkSerializer::Serialize(const cWorld & aWorld, cChunkCoords aCoords, cFastNBTWriter & aWriter)
{
	SerializerCollector serializer(aWriter);
	// set to 1.21.1
	aWriter.AddInt("DataVersion", 3955);  // to which game version does this save correspond to

	aWriter.AddInt("xPos", aCoords.m_ChunkX);
	aWriter.AddInt("zPos", aCoords.m_ChunkZ);
	[[maybe_unused]] const bool Result = aWorld.GetChunkData(aCoords, serializer);  // Chunk must be present in order to save
	ASSERT(Result);
	serializer.Finish();  // Close NBT tags

	aWriter.BeginList("sections", TAG_Compound);

	const bool use_padding = true;  // used in 1.16+
	for (size_t Y = 0; Y < cChunkDef::NumSections; Y++)
	{
		const auto Blocks = serializer.m_BlockData.GetSection(Y);
		const auto BlockLights = serializer.m_LightData.GetBlockLightSection(Y);
		const auto SkyLights = serializer.m_LightData.GetSkyLightSection(Y);
		aWriter.BeginCompound("");
		aWriter.AddInt("Y", static_cast<Int32>(Y));
		if (Blocks != nullptr)
		{
			ChunkBlockData::BlockArray temparr;
			std::copy(Blocks->begin(), Blocks->end(), temparr.begin());
			std::sort(temparr.begin(), temparr.end());
			auto newlistend = std::unique(temparr.begin(), temparr.end());
			int newsize = static_cast<int>(newlistend - temparr.begin());
			int bitused = Clamp(CeilC(log2(newsize)), 4, 16);

			int longarrsize = -1;

			if (use_padding)
			{
				int blocks_per_long = FloorC(64.0 / bitused);
				int longs_needed = CeilC(4096.0 / blocks_per_long);
				longarrsize = longs_needed;
			}
			else
			{
				longarrsize = CeilC((bitused * 4096) / 8 / 8);
			}

			aWriter.BeginCompound("block_states");
			aWriter.BeginList("palette", eTagType::TAG_Compound);

			for (size_t i = 0; i < static_cast<size_t>(newsize); i++)
			{
				bool hasblockstats = false;
				aWriter.BeginCompound("");
				auto val = temparr[i];
				auto strval = AString(NamespaceSerializer::From(val.Type()));
				auto splitpos = std::find(strval.begin(), strval.end(), ' ');
				auto id_end_index = static_cast<UInt64>(std::distance(strval.begin(), splitpos));
				AString stringid = strval.substr(0, id_end_index);
				AString blockstates;
				AStringVector blockstatesstrings;
				if (splitpos != strval.end())
				{
					hasblockstats = true;
					auto blockstates2 = strval.substr(id_end_index + 1, std::string::npos);
					blockstatesstrings = StringSplit(blockstates2, " ");
				}

				aWriter.AddString("Name", "minecraft:"+stringid);
				if (hasblockstats)
				{
					aWriter.BeginCompound("Properties");
					for (size_t j = 0; j < blockstatesstrings.size(); j +=2)
					{
						// AString str = blockstatesstrings[j];
						// auto nameendindex = static_cast<int>(std::distance(strval.begin(), std::find(strval.begin(), strval.end(), ':')));
						AString val2;
						if ((j + 2) >= blockstatesstrings.size())
						{
							val2 = blockstatesstrings[j + 1];
						}
						else
						{
							val2 = blockstatesstrings[j + 1].substr(0, blockstatesstrings[j + 1].length() - 1);
						}
						aWriter.AddString(blockstatesstrings[j].substr(0, blockstatesstrings[j].length() - 1), val2);
					}
					aWriter.EndCompound();
				}
				/*
				auto bls = PaletteUpgrade::GetSaveStrings(val);
				aWriter.AddString("Name", "minecraft:" + bls[0].second);
				if (bls[1].first != "")
				{
					aWriter.BeginCompound("Properties");
					for (size_t j = 1; j < bls.size(); j++)
					{
						if (bls[j].first == "")
						{
							break;
						}
						aWriter.AddString(bls[j].first, bls[j].second);
					}
					aWriter.EndCompound();
				} */
				aWriter.EndCompound();
			}
			aWriter.EndList();



			Int64 * arr = new Int64[static_cast<UInt64>(longarrsize)];

			UInt64 tbuf = 0;
			int BitIndex = 0;
			int longindex = 0;
			auto toloop = Blocks->size();
			// int bitswritten = 0;
			// std::vector<int> bw = {0};
			for (size_t i = 0; i < toloop; i++)
			{
				auto & v = Blocks->at(i);
				auto ind = std::find(temparr.begin(), newlistend, v);
				UInt64 towrite = static_cast<UInt64>(ind - temparr.begin());
				tbuf |= static_cast<UInt64>(towrite << BitIndex);
				BitIndex += bitused;
				// bitswritten += bitused;
				// ASSERT(bitswritten >= bw[bw.size() - 1]);
				// bw.push_back(bitswritten);
				// LOGD(std::to_string(bitswritten) + " - bits written");
				// ASSERT(bitswritten == (longindex * 64 + BitIndex));
				if (((BitIndex + bitused) > 64) || (i == (toloop - 1)))  // not enough bits in current long for the next value?
				{
					if (use_padding)
					{
						BitIndex = 0;
						ASSERT(longindex < longarrsize);
						arr[longindex] = static_cast<Int64>(tbuf);
						tbuf = 0;
						longindex++;
					}
					else
					{
						ASSERT(longindex < longarrsize);
						UInt64 upperpart = 0;
						if (BitIndex != 64)
						{
							upperpart = towrite >> (64 - BitIndex);
							UInt64 lowerpart = towrite & ((static_cast<UInt64>(1) << (64 - BitIndex)) - 1);
							tbuf |= lowerpart;
							// bitswritten += 64 - BitIndex;
							BitIndex = bitused - (64 - BitIndex);
							// bitswritten += BitIndex;
							// ASSERT(bitswritten >= bw[bw.size() - 1]);
							// bw.push_back(bitswritten);
							i++;
						}
						else
						{
							BitIndex = 0;
							tbuf = 0;
						}
						arr[longindex] = static_cast<Int64>(tbuf);
						longindex++;
						tbuf = 0;
						tbuf |= upperpart;
						// ASSERT(bitswritten == ((longindex * 64) + BitIndex));
					}
				}
			}

			if (Blocks != nullptr)
			{
				aWriter.AddLongArray("data", arr, static_cast<UInt64>(longindex));
			}
			else
			{
				// aWriter.AddByteArray("BlockStates", ChunkBlockData::SectionBlockCount, ChunkBlockData::DefaultValue);  // BROKEN
			}
			aWriter.EndCompound();
			delete[] arr;
		}

		if (BlockLights != nullptr)
		{
			aWriter.AddByteArray("BlockLight", reinterpret_cast<const char *>(BlockLights->data()), BlockLights->size());
		}
		else
		{
			aWriter.AddByteArray("BlockLight", ChunkLightData::SectionLightCount, ChunkLightData::DefaultBlockLightValue);
		}

		if (SkyLights != nullptr)
		{
			aWriter.AddByteArray("SkyLight", reinterpret_cast<const char *>(SkyLights->data()), SkyLights->size());
		}
		else
		{
			aWriter.AddByteArray("SkyLight", ChunkLightData::SectionLightCount, ChunkLightData::DefaultSkyLightValue);
		}
		aWriter.EndCompound();
	}
	aWriter.EndList();  // "Sections"

	aWriter.AddLong("LastUpdate", aWorld.GetWorldAge().count());

	aWriter.AddLong("InhabitedTime", 0);

	aWriter.AddString("Status", "minecraft:full");

	aWriter.AddByte("isLightOn", 1);

	// TODO: height maps not implemented yet
}
