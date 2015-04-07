
// NBTChunkSerializer.cpp


#include "Globals.h"
#include "NBTChunkSerializer.h"
#include "EnchantmentSerializer.h"
#include "../BlockID.h"
#include "../ItemGrid.h"
#include "../StringCompression.h"
#include "FastNBT.h"

#include "../BlockEntities/BeaconEntity.h"
#include "../BlockEntities/ChestEntity.h"
#include "../BlockEntities/CommandBlockEntity.h"
#include "../BlockEntities/DispenserEntity.h"
#include "../BlockEntities/DropperEntity.h"
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
#include "../Entities/Painting.h"

#include "../Mobs/IncludeAllMonsters.h"





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

	// Check if "Entity" and "TileEntities" lists exists. MCEdit requires this.
	if (!m_HasHadEntity)
	{
		m_Writer.BeginList("Entities", TAG_Compound);
		m_Writer.EndList();
	}
	if (!m_HasHadBlockEntity)
	{
		m_Writer.BeginList("TileEntities", TAG_Compound);
		m_Writer.EndList();
	}
}





void cNBTChunkSerializer::AddItem(const cItem & a_Item, int a_Slot, const AString & a_CompoundName)
{
	m_Writer.BeginCompound(a_CompoundName);
	m_Writer.AddShort("id",         (short)(a_Item.m_ItemType));
	m_Writer.AddShort("Damage",     a_Item.m_ItemDamage);
	m_Writer.AddByte ("Count",      a_Item.m_ItemCount);
	if (a_Slot >= 0)
	{
		m_Writer.AddByte ("Slot", (unsigned char)a_Slot);
	}
	
	// Write the tag compound (for enchantment, firework, custom name and repair cost):
	if (
		(!a_Item.m_Enchantments.IsEmpty()) ||
		((a_Item.m_ItemType == E_ITEM_FIREWORK_ROCKET) || (a_Item.m_ItemType == E_ITEM_FIREWORK_STAR)) ||
		(a_Item.m_RepairCost > 0) ||
		(a_Item.m_CustomName != "") ||
		(a_Item.m_Lore != "")
	)
	{
		m_Writer.BeginCompound("tag");
			if (a_Item.m_RepairCost > 0)
			{
				m_Writer.AddInt("RepairCost", a_Item.m_RepairCost);
			}

			if ((a_Item.m_CustomName != "") || (a_Item.m_Lore != ""))
			{
				m_Writer.BeginCompound("display");
				if (a_Item.m_CustomName != "")
				{
					m_Writer.AddString("Name", a_Item.m_CustomName);
				}
				if (a_Item.m_Lore != "")
				{
					m_Writer.AddString("Lore", a_Item.m_Lore);
				}
				m_Writer.EndCompound();
			}

			if ((a_Item.m_ItemType == E_ITEM_FIREWORK_ROCKET) || (a_Item.m_ItemType == E_ITEM_FIREWORK_STAR))
			{
				cFireworkItem::WriteToNBTCompound(a_Item.m_FireworkItem, m_Writer, (ENUM_ITEM_ID)a_Item.m_ItemType);
			}

			if (!a_Item.m_Enchantments.IsEmpty())
			{
				const char * TagName = (a_Item.m_ItemType == E_ITEM_BOOK) ? "StoredEnchantments" : "ench";
				EnchantmentSerializer::WriteToNBTCompound(a_Item.m_Enchantments, m_Writer, TagName);
			}
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





void cNBTChunkSerializer::AddBeaconEntity(cBeaconEntity * a_Entity)
{
	m_Writer.BeginCompound("");
		AddBasicTileEntity(a_Entity, "Beacon");
		m_Writer.AddInt("Levels", a_Entity->GetBeaconLevel());
		m_Writer.AddInt("Primary", (int)a_Entity->GetPrimaryEffect());
		m_Writer.AddInt("Secondary", (int)a_Entity->GetSecondaryEffect());
		m_Writer.BeginList("Items", TAG_Compound);
			AddItemGrid(a_Entity->GetContents());
		m_Writer.EndList();
	m_Writer.EndCompound();
}





void cNBTChunkSerializer::AddChestEntity(cChestEntity * a_Entity, BLOCKTYPE a_ChestType)
{
	m_Writer.BeginCompound("");
		AddBasicTileEntity(a_Entity, (a_ChestType == E_BLOCK_CHEST) ? "Chest" : "TrappedChest");
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





void cNBTChunkSerializer::AddMobSpawnerEntity(cMobSpawnerEntity * a_MobSpawner)
{
	m_Writer.BeginCompound("");
		AddBasicTileEntity(a_MobSpawner, "MobSpawner");
		m_Writer.AddShort("Entity", static_cast<short>(a_MobSpawner->GetEntity()));
		m_Writer.AddString("EntityId", cMonster::MobTypeToVanillaName(a_MobSpawner->GetEntity()));
		m_Writer.AddShort("Delay", a_MobSpawner->GetSpawnDelay());
	m_Writer.EndCompound();
}





void cNBTChunkSerializer::AddNoteEntity(cNoteEntity * a_Note)
{
	m_Writer.BeginCompound("");
		AddBasicTileEntity(a_Note, "Music");
		m_Writer.AddByte("note", a_Note->GetPitch());
	m_Writer.EndCompound();
}





void cNBTChunkSerializer::AddCommandBlockEntity(cCommandBlockEntity * a_CmdBlock)
{
	m_Writer.BeginCompound("");
		AddBasicTileEntity(a_CmdBlock, "Control");
		m_Writer.AddString("Command",      a_CmdBlock->GetCommand());
		m_Writer.AddInt   ("SuccessCount", a_CmdBlock->GetResult());
		m_Writer.AddString("LastOutput",   a_CmdBlock->GetLastOutput());
		m_Writer.AddByte  ("TrackOutput",  1);  // TODO 2014-01-18 xdot: Figure out what TrackOutput is and save it.
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





void cNBTChunkSerializer::AddMobHeadEntity(cMobHeadEntity * a_MobHead)
{
	m_Writer.BeginCompound("");
		AddBasicTileEntity(a_MobHead, "Skull");
		m_Writer.AddByte  ("SkullType", a_MobHead->GetType() & 0xFF);
		m_Writer.AddByte  ("Rot",       a_MobHead->GetRotation() & 0xFF);
		m_Writer.AddString("ExtraType", a_MobHead->GetOwner());
	m_Writer.EndCompound();
}





void cNBTChunkSerializer::AddFlowerPotEntity(cFlowerPotEntity * a_FlowerPot)
{
	m_Writer.BeginCompound("");
		AddBasicTileEntity(a_FlowerPot, "FlowerPot");
		m_Writer.AddInt   ("Item", (Int32) a_FlowerPot->GetItem().m_ItemType);
		m_Writer.AddInt   ("Data", (Int32) a_FlowerPot->GetItem().m_ItemDamage);
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
		m_Writer.AddDouble("", a_Entity->GetYaw());
		m_Writer.AddDouble("", a_Entity->GetPitch());
	m_Writer.EndList();
	m_Writer.AddShort("Health", a_Entity->GetHealth());
}





void cNBTChunkSerializer::AddBoatEntity(cBoat * a_Boat)
{
	m_Writer.BeginCompound("");
		AddBasicEntity(a_Boat, "Boat");
	m_Writer.EndCompound();
}





void cNBTChunkSerializer::AddEnderCrystalEntity(cEnderCrystal * a_EnderCrystal)
{
	m_Writer.BeginCompound("");
		AddBasicEntity(a_EnderCrystal, "EnderCrystal");
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
	m_Writer.BeginCompound("");
	
		switch (a_Minecart->GetPayload())
		{
			case cMinecart::mpChest:
			{
				AddBasicEntity(a_Minecart, "MinecartChest");
				// Add chest contents into the Items tag:
				AddMinecartChestContents((cMinecartWithChest *)a_Minecart);
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
	
	m_Writer.EndCompound();
}





void cNBTChunkSerializer::AddMonsterEntity(cMonster * a_Monster)
{
	const char * EntityClass = nullptr;
	switch (a_Monster->GetMobType())
	{
		case mtBat:           EntityClass = "Bat";            break;
		case mtBlaze:         EntityClass = "Blaze";          break;
		case mtCaveSpider:    EntityClass = "CaveSpider";     break;
		case mtChicken:       EntityClass = "Chicken";        break;
		case mtCow:           EntityClass = "Cow";            break;
		case mtCreeper:       EntityClass = "Creeper";        break;
		case mtEnderDragon:   EntityClass = "EnderDragon";    break;
		case mtEnderman:      EntityClass = "Enderman";       break;
		case mtGhast:         EntityClass = "Ghast";          break;
		case mtGiant:         EntityClass = "Giant";          break;
		case mtGuardian:      EntityClass = "Guardian";       break;
		case mtHorse:         EntityClass = "Horse";          break;
		case mtIronGolem:     EntityClass = "VillagerGolem";  break;
		case mtMagmaCube:     EntityClass = "LavaSlime";      break;
		case mtMooshroom:     EntityClass = "MushroomCow";    break;
		case mtOcelot:        EntityClass = "Ozelot";         break;
		case mtPig:           EntityClass = "Pig";            break;
		case mtRabbit:        EntityClass = "Rabbit";         break;
		case mtSheep:         EntityClass = "Sheep";          break;
		case mtSilverfish:    EntityClass = "Silverfish";     break;
		case mtSkeleton:      EntityClass = "Skeleton";       break;
		case mtSlime:         EntityClass = "Slime";          break;
		case mtSnowGolem:     EntityClass = "SnowMan";        break;
		case mtSpider:        EntityClass = "Spider";         break;
		case mtSquid:         EntityClass = "Squid";          break;
		case mtVillager:      EntityClass = "Villager";       break;
		case mtWitch:         EntityClass = "Witch";          break;
		case mtWither:        EntityClass = "WitherBoss";     break;
		case mtWolf:          EntityClass = "Wolf";           break;
		case mtZombie:        EntityClass = "Zombie";         break;
		case mtZombiePigman:  EntityClass = "PigZombie";      break;
		default:
		{
			ASSERT(!"Unhandled monster type");
			return;
		}
	}  // switch (payload)

	m_Writer.BeginCompound("");
		AddBasicEntity(a_Monster, EntityClass);
		m_Writer.BeginList("DropChances", TAG_Float);
			m_Writer.AddFloat("", a_Monster->GetDropChanceWeapon());
			m_Writer.AddFloat("", a_Monster->GetDropChanceHelmet());
			m_Writer.AddFloat("", a_Monster->GetDropChanceChestplate());
			m_Writer.AddFloat("", a_Monster->GetDropChanceLeggings());
			m_Writer.AddFloat("", a_Monster->GetDropChanceBoots());
		m_Writer.EndList();
		m_Writer.AddByte("CanPickUpLoot", (char)a_Monster->CanPickUpLoot());
		m_Writer.AddString("CustomName", a_Monster->GetCustomName());
		m_Writer.AddByte("CustomNameVisible", (char)a_Monster->IsCustomNameAlwaysVisible());
		switch (a_Monster->GetMobType())
		{
			case mtBat:
			{
				m_Writer.AddByte("BatFlags", ((const cBat *)a_Monster)->IsHanging());
				break;
			}
			case mtCreeper:
			{
				m_Writer.AddByte("powered", ((const cCreeper *)a_Monster)->IsCharged());
				m_Writer.AddByte("ignited", ((const cCreeper *)a_Monster)->IsBlowing());
				break;
			}
			case mtEnderman:
			{
				m_Writer.AddShort("carried",     (Int16)((const cEnderman *)a_Monster)->GetCarriedBlock());
				m_Writer.AddShort("carriedData", (Int16)((const cEnderman *)a_Monster)->GetCarriedMeta());
				break;
			}
			case mtHorse:
			{
				const cHorse & Horse = *((const cHorse *)a_Monster);
				m_Writer.AddByte("ChestedHorse",   Horse.IsChested());
				m_Writer.AddByte("EatingHaystack", Horse.IsEating());
				m_Writer.AddByte("Tame",           Horse.IsTame());
				m_Writer.AddInt ("Type",           Horse.GetHorseType());
				m_Writer.AddInt ("Color",          Horse.GetHorseColor());
				m_Writer.AddInt ("Style",          Horse.GetHorseStyle());
				m_Writer.AddInt ("ArmorType",      Horse.GetHorseArmour());
				m_Writer.AddByte("Saddle",         Horse.IsSaddled());
				break;
			}
			case mtMagmaCube:
			{
				m_Writer.AddInt("Size", ((const cMagmaCube *)a_Monster)->GetSize());
				break;
			}
			case mtSheep:
			{
				m_Writer.AddByte("Sheared", ((const cSheep *)a_Monster)->IsSheared());
				m_Writer.AddByte("Color",   ((const cSheep *)a_Monster)->GetFurColor());
				break;
			}
			case mtSlime:
			{
				m_Writer.AddInt("Size", ((const cSlime *)a_Monster)->GetSize());
				break;
			}
			case mtSkeleton:
			{
				m_Writer.AddByte("SkeletonType", (((const cSkeleton *)a_Monster)->IsWither() ? 1 : 0));
				break;
			}
			case mtVillager:
			{
				m_Writer.AddInt("Profession", ((const cVillager *)a_Monster)->GetVilType());
				break;
			}
			case mtWither:
			{
				m_Writer.AddInt("Invul", ((const cWither *)a_Monster)->GetWitherInvulnerableTicks());
				break;
			}
			case mtWolf:
			{
				const cWolf & Wolf = *((cWolf *)a_Monster);
				if (!Wolf.GetOwnerName().empty())
				{
					m_Writer.AddString("Owner", Wolf.GetOwnerName());
				}
				if (!Wolf.GetOwnerUUID().empty())
				{
					m_Writer.AddString("OwnerUUID", Wolf.GetOwnerUUID());
				}
				m_Writer.AddByte("Sitting",     Wolf.IsSitting() ? 1 : 0);
				m_Writer.AddByte("Angry",       Wolf.IsAngry() ? 1 : 0);
				m_Writer.AddByte("CollarColor", (unsigned char)Wolf.GetCollarColor());
				break;
			}
			case mtZombie:
			{
				m_Writer.AddByte("IsVillager",   (((const cZombie *)a_Monster)->IsVillagerZombie() ? 1 : 0));
				m_Writer.AddByte("IsBaby",       (((const cZombie *)a_Monster)->IsBaby() ? 1 : 0));
				m_Writer.AddByte("IsConverting", (((const cZombie *)a_Monster)->IsConverting() ? 1 : 0));
				break;
			}
			case mtInvalidType:
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
			case mtOcelot:
			case mtPig:
			case mtRabbit:
			case mtSilverfish:
			case mtSnowGolem:
			case mtSpider:
			case mtSquid:
			case mtWitch:
			case mtZombiePigman:
			{
				// Other mobs have no special tags.
				break;
			}
		}
	m_Writer.EndCompound();
}





void cNBTChunkSerializer::AddPickupEntity(cPickup * a_Pickup)
{
	m_Writer.BeginCompound("");
		AddBasicEntity(a_Pickup, "Item");
		AddItem(a_Pickup->GetItem(), -1, "Item");
		m_Writer.AddShort("Age",    (Int16)a_Pickup->GetAge());
	m_Writer.EndCompound();
}





void cNBTChunkSerializer::AddProjectileEntity(cProjectileEntity * a_Projectile)
{
	m_Writer.BeginCompound("");
		AddBasicEntity(a_Projectile, a_Projectile->GetMCAClassName());
		m_Writer.AddByte("inGround", a_Projectile->IsInGround() ? 1 : 0);
		
		switch (a_Projectile->GetProjectileKind())
		{
			case cProjectileEntity::pkArrow:
			{
				cArrowEntity * Arrow = (cArrowEntity *)a_Projectile;

				m_Writer.AddShort("xTile", (Int16)Arrow->GetBlockHit().x);
				m_Writer.AddShort("yTile", (Int16)Arrow->GetBlockHit().y);
				m_Writer.AddShort("zTile", (Int16)Arrow->GetBlockHit().z);
				m_Writer.AddByte("pickup",   Arrow->GetPickupState());
				m_Writer.AddDouble("damage", Arrow->GetDamageCoeff());
				break;
			}
			case cProjectileEntity::pkSplashPotion:
			{
				cSplashPotionEntity * Potion = (cSplashPotionEntity *)a_Projectile;
				
				m_Writer.AddInt("EffectType",               (Int16)Potion->GetEntityEffectType());
				m_Writer.AddInt("EffectDuration",           (Int16)Potion->GetEntityEffect().GetDuration());
				m_Writer.AddShort("EffectIntensity",         Potion->GetEntityEffect().GetIntensity());
				m_Writer.AddDouble("EffectDistanceModifier", Potion->GetEntityEffect().GetDistanceModifier());
				m_Writer.AddInt("PotionName",                Potion->GetPotionColor());
				break;
			}
			case cProjectileEntity::pkGhastFireball:
			{
				m_Writer.AddInt("ExplosionPower", 1);
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
			m_Writer.AddString("ownerName", a_Projectile->GetCreatorName());
		}
	m_Writer.EndCompound();
}





void cNBTChunkSerializer::AddHangingEntity(cHangingEntity * a_Hanging)
{
	m_Writer.AddInt("TileX", FloorC(a_Hanging->GetPosX()));
	m_Writer.AddInt("TileY", FloorC(a_Hanging->GetPosY()));
	m_Writer.AddInt("TileZ", FloorC(a_Hanging->GetPosZ()));
	m_Writer.AddByte("Facing", a_Hanging->GetProtocolFacing());
}





void cNBTChunkSerializer::AddTNTEntity(cTNTEntity * a_TNT)
{
	m_Writer.BeginCompound("");
		AddBasicEntity(a_TNT, "PrimedTnt");
		m_Writer.AddByte("Fuse", (unsigned char)a_TNT->GetFuseTicks());
	m_Writer.EndCompound();
}





void cNBTChunkSerializer::AddExpOrbEntity(cExpOrb * a_ExpOrb)
{
	m_Writer.BeginCompound("");
		AddBasicEntity(a_ExpOrb, "XPOrb");
		m_Writer.AddShort("Age", (Int16)a_ExpOrb->GetAge());
		m_Writer.AddShort("Value", (Int16)a_ExpOrb->GetReward());
	m_Writer.EndCompound();
}





void cNBTChunkSerializer::AddItemFrameEntity(cItemFrame * a_ItemFrame)
{
	m_Writer.BeginCompound("");
		AddBasicEntity(a_ItemFrame, "ItemFrame");
		AddHangingEntity(a_ItemFrame);
		AddItem(a_ItemFrame->GetItem(), -1, "Item");
		m_Writer.AddByte("ItemRotation", (unsigned char)a_ItemFrame->GetItemRotation());
		m_Writer.AddFloat("ItemDropChance", 1.0F);
	m_Writer.EndCompound();
}





void cNBTChunkSerializer::AddPaintingEntity(cPainting * a_Painting)
{
	m_Writer.BeginCompound("");
		AddBasicEntity(a_Painting, "Painting");
		AddHangingEntity(a_Painting);
		m_Writer.AddString("Motive", a_Painting->GetName());
	m_Writer.EndCompound();
}





void cNBTChunkSerializer::AddMinecartChestContents(cMinecartWithChest * a_Minecart)
{
	m_Writer.BeginList("Items", TAG_Compound);
		for (int i = 0; i < cMinecartWithChest::ContentsHeight * cMinecartWithChest::ContentsWidth; i++)
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





void cNBTChunkSerializer::LightIsValid(bool a_IsLightValid)
{
	m_IsLightValid = a_IsLightValid;
}





void cNBTChunkSerializer::HeightMap(const cChunkDef::HeightMap * a_HeightMap)
{
	for (int RelX = 0; RelX < cChunkDef::Width; RelX++)
	{
		for (int RelZ = 0; RelZ < cChunkDef::Width; RelZ++)
		{
			int Height = cChunkDef::GetHeight(*a_HeightMap, RelX, RelZ);
			m_VanillaHeightMap[(RelZ << 4) | RelX] = Height;
		}
	}
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
		case cEntity::etEnderCrystal: AddEnderCrystalEntity((cEnderCrystal *)    a_Entity); break;
		case cEntity::etFallingBlock: AddFallingBlockEntity((cFallingBlock *)    a_Entity); break;
		case cEntity::etMinecart:     AddMinecartEntity    ((cMinecart *)        a_Entity); break;
		case cEntity::etMonster:      AddMonsterEntity     ((cMonster *)         a_Entity); break;
		case cEntity::etPickup:       AddPickupEntity      ((cPickup *)          a_Entity); break;
		case cEntity::etProjectile:   AddProjectileEntity  ((cProjectileEntity *)a_Entity); break;
		case cEntity::etTNT:          AddTNTEntity         ((cTNTEntity *)       a_Entity); break;
		case cEntity::etExpOrb:       AddExpOrbEntity      ((cExpOrb *)          a_Entity); break;
		case cEntity::etItemFrame:    AddItemFrameEntity   ((cItemFrame *)       a_Entity); break;
		case cEntity::etPainting:     AddPaintingEntity    (reinterpret_cast<cPainting *>(a_Entity)); break;
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
		case E_BLOCK_BEACON:        AddBeaconEntity      ((cBeaconEntity *)      a_Entity); break;
		case E_BLOCK_CHEST:         AddChestEntity       ((cChestEntity *)       a_Entity, a_Entity->GetBlockType()); break;
		case E_BLOCK_COMMAND_BLOCK: AddCommandBlockEntity((cCommandBlockEntity *)a_Entity); break;
		case E_BLOCK_DISPENSER:     AddDispenserEntity   ((cDispenserEntity *)   a_Entity); break;
		case E_BLOCK_DROPPER:       AddDropperEntity     ((cDropperEntity *)     a_Entity); break;
		case E_BLOCK_ENDER_CHEST:   /* No data to be saved */                               break;
		case E_BLOCK_FLOWER_POT:    AddFlowerPotEntity   ((cFlowerPotEntity *)   a_Entity); break;
		case E_BLOCK_FURNACE:       AddFurnaceEntity     ((cFurnaceEntity *)     a_Entity); break;
		case E_BLOCK_HEAD:          AddMobHeadEntity     ((cMobHeadEntity *)     a_Entity); break;
		case E_BLOCK_HOPPER:        AddHopperEntity      ((cHopperEntity *)      a_Entity); break;
		case E_BLOCK_JUKEBOX:       AddJukeboxEntity     ((cJukeboxEntity *)     a_Entity); break;
		case E_BLOCK_LIT_FURNACE:   AddFurnaceEntity     ((cFurnaceEntity *)     a_Entity); break;
		case E_BLOCK_MOB_SPAWNER:   AddMobSpawnerEntity  ((cMobSpawnerEntity *)  a_Entity); break;
		case E_BLOCK_NOTE_BLOCK:    AddNoteEntity        ((cNoteEntity *)        a_Entity); break;
		case E_BLOCK_SIGN_POST:     AddSignEntity        ((cSignEntity *)        a_Entity); break;
		case E_BLOCK_TRAPPED_CHEST: AddChestEntity       ((cChestEntity *)       a_Entity, a_Entity->GetBlockType()); break;
		case E_BLOCK_WALLSIGN:      AddSignEntity        ((cSignEntity *)        a_Entity); break;

		default:
		{
			ASSERT(!"Unhandled block entity saved into Anvil");
		}
	}
	m_HasHadBlockEntity = true;
}




