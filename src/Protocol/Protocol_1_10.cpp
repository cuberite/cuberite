
// Protocol_1_10.cpp

/*
Implements the 1.10 protocol classes:
	- cProtocol_1_10_0
		- release 1.10 protocol (#210), also used by 1.10.1 and 1.10.2
*/

#include "Globals.h"
#include "Protocol_1_10.h"
#include "Packetizer.h"

#include "json/json.h"

#include "../Root.h"
#include "../Server.h"
#include "../ClientHandle.h"
#include "../JsonUtils.h"

#include "../WorldStorage/FastNBT.h"

#include "../Entities/Boat.h"
#include "../Entities/EnderCrystal.h"
#include "../Entities/ExpOrb.h"
#include "../Entities/Minecart.h"
#include "../Entities/FallingBlock.h"
#include "../Entities/Painting.h"
#include "../Entities/Pickup.h"
#include "../Entities/Player.h"
#include "../Entities/ItemFrame.h"
#include "../Entities/ArrowEntity.h"
#include "../Entities/FireworkEntity.h"
#include "../Entities/SplashPotionEntity.h"

#include "../Mobs/IncludeAllMonsters.h"

#include "../BlockEntities/BeaconEntity.h"
#include "../BlockEntities/CommandBlockEntity.h"
#include "../BlockEntities/MobHeadEntity.h"
#include "../BlockEntities/MobSpawnerEntity.h"
#include "../BlockEntities/FlowerPotEntity.h"

#include "../Bindings/PluginManager.h"





#define HANDLE_READ(ByteBuf, Proc, Type, Var) \
	Type Var; \
	do { \
		if (!ByteBuf.Proc(Var))\
		{\
			return;\
		} \
	} while (false)





// The disabled error is intended, since the Metadata have overlapping indexes
// based on the type of the Entity.
//
// IMPORTANT: The enum is used to automate the sequential counting of the
// Metadata indexes. Adding a new enum value causes the following values to
// increase their index. Therefore the ordering of the enum values is VERY important!
#ifdef __clang__
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wduplicate-enum"
#endif

namespace Metadata
{
	enum Metadata_Index
	{
		// Entity
		ENTITY_FLAGS,
		ENTITY_AIR,
		ENTITY_CUSTOM_NAME,
		ENTITY_CUSTOM_NAME_VISIBLE,
		ENTITY_SILENT,
		ENTITY_NO_GRAVITY,
		_ENTITY_NEXT,  // Used by descendants

		// Potion
		POTION_THROWN = _ENTITY_NEXT,

		// FallingBlock
		FALLING_BLOCK_POSITION = _ENTITY_NEXT,

		// AreaEffectCloud
		AREA_EFFECT_CLOUD_RADIUS = _ENTITY_NEXT,
		AREA_EFFECT_CLOUD_COLOR,
		AREA_EFFECT_CLOUD_SINGLE_POINT_EFFECT,
		AREA_EFFECT_CLOUD_PARTICLE_ID,
		AREA_EFFECT_CLOUD_PARTICLE_PARAMETER1,
		AREA_EFFECT_CLOUD_PARTICLE_PARAMETER2,

		// Arrow
		ARROW_CRITICAL = _ENTITY_NEXT,
		_ARROW_NEXT,

		// TippedArrow
		TIPPED_ARROW_COLOR = _ARROW_NEXT,

		// Boat
		BOAT_LAST_HIT_TIME = _ENTITY_NEXT,
		BOAT_FORWARD_DIRECTION,
		BOAT_DAMAGE_TAKEN,
		BOAT_TYPE,
		BOAT_RIGHT_PADDLE_TURNING,
		BOAT_LEFT_PADDLE_TURNING,

		// EnderCrystal
		ENDER_CRYSTAL_BEAM_TARGET = _ENTITY_NEXT,
		ENDER_CRYSTAL_SHOW_BOTTOM,

		// Fireball
		_FIREBALL_NEXT = _ENTITY_NEXT,

		// WitherSkull
		WITHER_SKULL_INVULNERABLE = _FIREBALL_NEXT,

		// Fireworks
		FIREWORK_INFO = _ENTITY_NEXT,

		// Hanging
		_HANGING_NEXT = _ENTITY_NEXT,

		// ItemFrame
		ITEM_FRAME_ITEM = _HANGING_NEXT,
		ITEM_FRAME_ROTATION,

		// Item
		ITEM_ITEM = _ENTITY_NEXT,

		// Living
		LIVING_ACTIVE_HAND = _ENTITY_NEXT,
		LIVING_HEALTH,
		LIVING_POTION_EFFECT_COLOR,
		LIVING_POTION_EFFECT_AMBIENT,
		LIVING_NUMBER_OF_ARROWS,
		_LIVING_NEXT,

		// Player
		PLAYER_ADDITIONAL_HEARTHS = _LIVING_NEXT,
		PLAYER_SCORE,
		PLAYER_DISPLAYED_SKIN_PARTS,
		PLAYER_MAIN_HAND,

		// ArmorStand
		ARMOR_STAND_STATUS = _LIVING_NEXT,
		ARMOR_STAND_HEAD_ROTATION,
		ARMOR_STAND_BODY_ROTATION,
		ARMOR_STAND_LEFT_ARM_ROTATION,
		ARMOR_STAND_RIGHT_ARM_ROTATION,
		ARMOR_STAND_LEFT_LEG_ROTATION,
		ARMOR_STAND_RIGHT_LEG_ROTATION,

		// Insentient
		INSENTIENT_STATUS = _LIVING_NEXT,
		_INSENTIENT_NEXT,

		// Ambient
		_AMBIENT_NEXT = _INSENTIENT_NEXT,

		// Bat
		BAT_HANGING = _AMBIENT_NEXT,

		// Creature
		_CREATURE_NEXT = _INSENTIENT_NEXT,

		// Ageable
		AGEABLE_BABY = _CREATURE_NEXT,
		_AGEABLE_NEXT,

		// PolarBear
		POLAR_BEAR_STANDING = _AGEABLE_NEXT,

		// Animal
		_ANIMAL_NEXT = _AGEABLE_NEXT,

		// Horse
		HORSE_STATUS = _ANIMAL_NEXT,
		HORSE_TYPE,
		HORSE_VARIANT,
		HORSE_OWNER,
		HORSE_ARMOR,

		// Pig
		PIG_HAS_SADDLE = _ANIMAL_NEXT,

		// Rabbit
		RABBIT_TYPE = _ANIMAL_NEXT,

		// Sheep
		SHEEP_STATUS = _ANIMAL_NEXT,

		// TameableAnimal
		TAMEABLE_ANIMAL_STATUS = _ANIMAL_NEXT,
		TAMEABLE_ANIMAL_OWNER,
		_TAMEABLE_NEXT,

		// Ocelot
		OCELOT_TYPE = _TAMEABLE_NEXT,

		// Wolf
		WOLF_DAMAGE_TAKEN = _TAMEABLE_NEXT,
		WOLF_BEGGING,
		WOLF_COLLAR_COLOR,

		// Villager
		VILLAGER_PROFESSION = _AGEABLE_NEXT,

		// Golem
		_GOLEM_NEXT = _CREATURE_NEXT,

		// IronGolem
		IRON_GOLEM_PLAYER_CREATED = _GOLEM_NEXT,

		// Shulker
		SHULKER_FACING_DIRECTION = _GOLEM_NEXT,
		SHULKER_ATTACHMENT_FALLING_BLOCK_POSITION,
		SHULKER_SHIELD_HEIGHT,

		// Monster
		_MONSTER_NEXT = _CREATURE_NEXT,

		// Blaze
		BLAZE_ON_FIRE = _MONSTER_NEXT,

		// Creeper
		CREEPER_STATE = _MONSTER_NEXT,
		CREEPER_POWERED,
		CREEPER_IGNITED,

		// Guardian
		GUARDIAN_STATUS = _MONSTER_NEXT,
		GUARDIAN_TARGET,

		// Skeleton
		SKELETON_TYPE = _MONSTER_NEXT,
		SKELETON_ARMS_SWINGING,

		// Spider
		SPIDER_CLIMBING = _MONSTER_NEXT,

		// Witch
		WITCH_AGGRESIVE = _MONSTER_NEXT,

		// Wither
		WITHER_FIRST_HEAD_TARGET = _MONSTER_NEXT,
		WITHER_SECOND_HEAD_TARGET,
		WITHER_THIRD_HEAD_TARGET,
		WITHER_INVULNERABLE_TIMER,

		// Zombie
		ZOMBIE_IS_BABY = _MONSTER_NEXT,
		ZOMBIE_TYPE,
		ZOMBIE_CONVERTING,
		ZOMBIE_HANDS_RISED_UP,

		// Enderman
		ENDERMAN_CARRIED_BLOCK = _MONSTER_NEXT,
		ENDERMAN_SCREAMING,

		// EnderDragon
		ENDER_DRAGON_DRAGON_PHASE = _INSENTIENT_NEXT,

		// Flying
		_FLYING_NEXT = _INSENTIENT_NEXT,

		// Ghast
		GHAST_ATTACKING = _FLYING_NEXT,

		// Slime
		SLIME_SIZE = _INSENTIENT_NEXT,

		// Minecart
		MINECART_SHAKING_POWER = _ENTITY_NEXT,
		MINECART_SHAKING_DIRECTION,
		MINECART_SHAKING_MULTIPLIER,
		MINECART_BLOCK_ID_META,
		MINECART_BLOCK_Y,
		MINECART_SHOW_BLOCK,
		_MINECART_NEXT,

		// MinecartCommandBlock
		MINECART_COMMAND_BLOCK_COMMAND = _MINECART_NEXT,
		MINECART_COMMAND_BLOCK_LAST_OUTPUT,

		// MinecartFurnace
		MINECART_FURNACE_POWERED = _MINECART_NEXT,

		// TNTPrimed
		TNT_PRIMED_FUSE_TIME = _ENTITY_NEXT,
	};
}

#ifdef __clang__
	#pragma clang diagnostic pop  // Restore ignored clang errors
#endif





////////////////////////////////////////////////////////////////////////////////
// cProtocol_1_10_0:

void cProtocol_1_10_0::SendSoundEffect(const AString & a_SoundName, double a_X, double a_Y, double a_Z, float a_Volume, float a_Pitch)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktSoundEffect);
	Pkt.WriteString(a_SoundName);
	Pkt.WriteVarInt32(0);  // Master sound category (may want to be changed to a parameter later)
	Pkt.WriteBEInt32(FloorC(a_X * 8.0));
	Pkt.WriteBEInt32(FloorC(a_Y * 8.0));
	Pkt.WriteBEInt32(FloorC(a_Z * 8.0));
	Pkt.WriteBEFloat(a_Volume);
	Pkt.WriteBEFloat(a_Pitch);
}





cProtocol::Version cProtocol_1_10_0::GetProtocolVersion()
{
	return Version::v1_10_0;
}





void cProtocol_1_10_0::HandlePacketResourcePackStatus(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8, Status);
}





void cProtocol_1_10_0::WriteEntityMetadata(cPacketizer & a_Pkt, const cEntity & a_Entity)
{
	using namespace Metadata;

	// Common metadata:
	Int8 Flags = 0;
	if (a_Entity.IsOnFire())
	{
		Flags |= 0x01;
	}
	if (a_Entity.IsCrouched())
	{
		Flags |= 0x02;
	}
	if (a_Entity.IsSprinting())
	{
		Flags |= 0x08;
	}
	if (a_Entity.IsRclking())
	{
		Flags |= 0x10;
	}
	if (a_Entity.IsInvisible())
	{
		Flags |= 0x20;
	}
	a_Pkt.WriteBEUInt8(ENTITY_FLAGS);  // Index
	a_Pkt.WriteBEUInt8(METADATA_TYPE_BYTE);  // Type
	a_Pkt.WriteBEInt8(Flags);

	switch (a_Entity.GetEntityType())
	{
		case cEntity::etPlayer:
		{
			auto & Player = static_cast<const cPlayer &>(a_Entity);

			// TODO Set player custom name to their name.
			// Then it's possible to move the custom name of mobs to the entities
			// and to remove the "special" player custom name.
			a_Pkt.WriteBEUInt8(ENTITY_CUSTOM_NAME);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_STRING);
			a_Pkt.WriteString(Player.GetName());

			a_Pkt.WriteBEUInt8(LIVING_HEALTH);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_FLOAT);
			a_Pkt.WriteBEFloat(static_cast<float>(Player.GetHealth()));

			a_Pkt.WriteBEUInt8(PLAYER_DISPLAYED_SKIN_PARTS);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BYTE);
			a_Pkt.WriteBEUInt8(static_cast<UInt8>(Player.GetSkinParts()));

			a_Pkt.WriteBEUInt8(PLAYER_MAIN_HAND);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BYTE);
			a_Pkt.WriteBEUInt8(static_cast<UInt8>(Player.GetMainHand()));
			break;
		}
		case cEntity::etPickup:
		{
			a_Pkt.WriteBEUInt8(ITEM_ITEM);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_ITEM);
			WriteItem(a_Pkt, static_cast<const cPickup &>(a_Entity).GetItem());
			break;
		}
		case cEntity::etMinecart:
		{
			a_Pkt.WriteBEUInt8(MINECART_SHAKING_POWER);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);

			// The following expression makes Minecarts shake more with less health or higher damage taken
			auto & Minecart = static_cast<const cMinecart &>(a_Entity);
			auto maxHealth = a_Entity.GetMaxHealth();
			auto curHealth = a_Entity.GetHealth();
			a_Pkt.WriteVarInt32(static_cast<UInt32>((maxHealth - curHealth) * Minecart.LastDamage() * 4));

			a_Pkt.WriteBEUInt8(MINECART_SHAKING_DIRECTION);  // (doesn't seem to effect anything)
			a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(1);

			a_Pkt.WriteBEUInt8(MINECART_SHAKING_MULTIPLIER);  // or damage taken
			a_Pkt.WriteBEUInt8(METADATA_TYPE_FLOAT);
			a_Pkt.WriteBEFloat(static_cast<float>(Minecart.LastDamage() + 10));

			if (Minecart.GetPayload() == cMinecart::mpNone)
			{
				auto & RideableMinecart = static_cast<const cRideableMinecart &>(Minecart);
				const cItem & MinecartContent = RideableMinecart.GetContent();
				if (!MinecartContent.IsEmpty())
				{
					a_Pkt.WriteBEUInt8(MINECART_BLOCK_ID_META);
					a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
					int Content = MinecartContent.m_ItemType;
					Content |= MinecartContent.m_ItemDamage << 8;
					a_Pkt.WriteVarInt32(static_cast<UInt32>(Content));

					a_Pkt.WriteBEUInt8(MINECART_BLOCK_Y);
					a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
					a_Pkt.WriteVarInt32(static_cast<UInt32>(RideableMinecart.GetBlockHeight()));

					a_Pkt.WriteBEUInt8(MINECART_SHOW_BLOCK);
					a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
					a_Pkt.WriteBool(true);
				}
			}
			else if (Minecart.GetPayload() == cMinecart::mpFurnace)
			{
				a_Pkt.WriteBEUInt8(MINECART_FURNACE_POWERED);
				a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
				a_Pkt.WriteBool(static_cast<const cMinecartWithFurnace &>(Minecart).IsFueled());
			}
			break;
		}  // case etMinecart

		case cEntity::etProjectile:
		{
			auto & Projectile = static_cast<const cProjectileEntity &>(a_Entity);
			switch (Projectile.GetProjectileKind())
			{
				case cProjectileEntity::pkArrow:
				{
					a_Pkt.WriteBEUInt8(ARROW_CRITICAL);
					a_Pkt.WriteBEUInt8(METADATA_TYPE_BYTE);
					a_Pkt.WriteBEInt8(static_cast<const cArrowEntity &>(Projectile).IsCritical() ? 1 : 0);
					break;
				}
				case cProjectileEntity::pkFirework:
				{
					a_Pkt.WriteBEUInt8(FIREWORK_INFO);  // Firework item used for this firework
					a_Pkt.WriteBEUInt8(METADATA_TYPE_ITEM);
					WriteItem(a_Pkt, static_cast<const cFireworkEntity &>(Projectile).GetItem());
					break;
				}
				case cProjectileEntity::pkSplashPotion:
				{
					a_Pkt.WriteBEUInt8(POTION_THROWN);  // Potion item which was thrown
					a_Pkt.WriteBEUInt8(METADATA_TYPE_ITEM);
					WriteItem(a_Pkt, static_cast<const cSplashPotionEntity &>(Projectile).GetItem());
				}
				default:
				{
					break;
				}
			}
			break;
		}  // case etProjectile

		case cEntity::etMonster:
		{
			WriteMobMetadata(a_Pkt, static_cast<const cMonster &>(a_Entity));
			break;
		}

		case cEntity::etBoat:
		{
			auto & Boat = static_cast<const cBoat &>(a_Entity);

			a_Pkt.WriteBEInt8(BOAT_LAST_HIT_TIME);
			a_Pkt.WriteBEInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Boat.GetLastDamage()));

			a_Pkt.WriteBEInt8(BOAT_FORWARD_DIRECTION);
			a_Pkt.WriteBEInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Boat.GetForwardDirection()));

			a_Pkt.WriteBEInt8(BOAT_DAMAGE_TAKEN);
			a_Pkt.WriteBEInt8(METADATA_TYPE_FLOAT);
			a_Pkt.WriteBEFloat(Boat.GetDamageTaken());

			a_Pkt.WriteBEInt8(BOAT_TYPE);
			a_Pkt.WriteBEInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Boat.GetMaterial()));

			a_Pkt.WriteBEInt8(BOAT_RIGHT_PADDLE_TURNING);
			a_Pkt.WriteBEInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Boat.IsRightPaddleUsed());

			a_Pkt.WriteBEInt8(BOAT_LEFT_PADDLE_TURNING);
			a_Pkt.WriteBEInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Boat.IsLeftPaddleUsed());

			break;
		}  // case etBoat

		case cEntity::etItemFrame:
		{
			auto & Frame = static_cast<const cItemFrame &>(a_Entity);
			a_Pkt.WriteBEUInt8(ITEM_FRAME_ITEM);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_ITEM);
			WriteItem(a_Pkt, Frame.GetItem());
			a_Pkt.WriteBEUInt8(ITEM_FRAME_ROTATION);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(Frame.GetItemRotation());
			break;
		}  // case etItemFrame

		case cEntity::etEnderCrystal:
		{
			const auto & EnderCrystal = static_cast<const cEnderCrystal &>(a_Entity);
			a_Pkt.WriteBEUInt8(7);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_OPTIONAL_POSITION);
			a_Pkt.WriteBool(EnderCrystal.DisplaysBeam());
			if (EnderCrystal.DisplaysBeam())
			{
				a_Pkt.WriteXYZPosition64(EnderCrystal.GetBeamTarget());
			}
			a_Pkt.WriteBEUInt8(8);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(EnderCrystal.ShowsBottom());
			break;
		}  // case etEnderCrystal

		default:
		{
			break;
		}
	}
}





void cProtocol_1_10_0::WriteBlockEntity(cPacketizer & a_Pkt, const cBlockEntity & a_BlockEntity)
{
	cFastNBTWriter Writer;

	switch (a_BlockEntity.GetBlockType())
	{
		case E_BLOCK_BEACON:
		{
			auto & BeaconEntity = static_cast<const cBeaconEntity &>(a_BlockEntity);
			Writer.AddInt("x", BeaconEntity.GetPosX());
			Writer.AddInt("y", BeaconEntity.GetPosY());
			Writer.AddInt("z", BeaconEntity.GetPosZ());
			Writer.AddInt("Primary", BeaconEntity.GetPrimaryEffect());
			Writer.AddInt("Secondary", BeaconEntity.GetSecondaryEffect());
			Writer.AddInt("Levels", BeaconEntity.GetBeaconLevel());
			Writer.AddString("id", "Beacon");  // "Tile Entity ID" - MC wiki; vanilla server always seems to send this though
			break;
		}

		case E_BLOCK_COMMAND_BLOCK:
		{
			auto & CommandBlockEntity = static_cast<const cCommandBlockEntity &>(a_BlockEntity);
			Writer.AddByte("TrackOutput", 1);  // Neither I nor the MC wiki has any idea about this
			Writer.AddInt("SuccessCount", CommandBlockEntity.GetResult());
			Writer.AddInt("x", CommandBlockEntity.GetPosX());
			Writer.AddInt("y", CommandBlockEntity.GetPosY());
			Writer.AddInt("z", CommandBlockEntity.GetPosZ());
			Writer.AddString("Command", CommandBlockEntity.GetCommand());
			// You can set custom names for windows in Vanilla
			// For a command block, this would be the 'name' prepended to anything it outputs into global chat
			// MCS doesn't have this, so just leave it @ '@'. (geddit?)
			Writer.AddString("CustomName", "@");
			Writer.AddString("id", "Control");  // "Tile Entity ID" - MC wiki; vanilla server always seems to send this though
			if (!CommandBlockEntity.GetLastOutput().empty())
			{
				Writer.AddString("LastOutput", Printf("{\"text\":\"%s\"}", CommandBlockEntity.GetLastOutput().c_str()));
			}
			break;
		}

		case E_BLOCK_HEAD:
		{
			auto & MobHeadEntity = static_cast<const cMobHeadEntity &>(a_BlockEntity);
			Writer.AddInt("x", MobHeadEntity.GetPosX());
			Writer.AddInt("y", MobHeadEntity.GetPosY());
			Writer.AddInt("z", MobHeadEntity.GetPosZ());
			Writer.AddByte("SkullType", MobHeadEntity.GetType() & 0xFF);
			Writer.AddByte("Rot", MobHeadEntity.GetRotation() & 0xFF);
			Writer.AddString("id", "Skull");  // "Tile Entity ID" - MC wiki; vanilla server always seems to send this though

			// The new Block Entity format for a Mob Head. See: https://minecraft.gamepedia.com/Head#Block_entity
			Writer.BeginCompound("Owner");
			Writer.AddString("Id", MobHeadEntity.GetOwnerUUID().ToShortString());
			Writer.AddString("Name", MobHeadEntity.GetOwnerName());
			Writer.BeginCompound("Properties");
			Writer.BeginList("textures", TAG_Compound);
			Writer.BeginCompound("");
			Writer.AddString("Signature", MobHeadEntity.GetOwnerTextureSignature());
			Writer.AddString("Value", MobHeadEntity.GetOwnerTexture());
			Writer.EndCompound();
			Writer.EndList();
			Writer.EndCompound();
			Writer.EndCompound();
			break;
		}

		case E_BLOCK_FLOWER_POT:
		{
			auto & FlowerPotEntity = static_cast<const cFlowerPotEntity &>(a_BlockEntity);
			Writer.AddInt("x", FlowerPotEntity.GetPosX());
			Writer.AddInt("y", FlowerPotEntity.GetPosY());
			Writer.AddInt("z", FlowerPotEntity.GetPosZ());
			Writer.AddInt("Item", static_cast<Int32>(FlowerPotEntity.GetItem().m_ItemType));
			Writer.AddInt("Data", static_cast<Int32>(FlowerPotEntity.GetItem().m_ItemDamage));
			Writer.AddString("id", "FlowerPot");  // "Tile Entity ID" - MC wiki; vanilla server always seems to send this though
			break;
		}

		case E_BLOCK_MOB_SPAWNER:
		{
			auto & MobSpawnerEntity = static_cast<const cMobSpawnerEntity &>(a_BlockEntity);
			Writer.AddInt("x", MobSpawnerEntity.GetPosX());
			Writer.AddInt("y", MobSpawnerEntity.GetPosY());
			Writer.AddInt("z", MobSpawnerEntity.GetPosZ());
			Writer.BeginCompound("SpawnData");
				Writer.AddString("id", cMonster::MobTypeToVanillaName(MobSpawnerEntity.GetEntity()));
			Writer.EndCompound();
			Writer.AddShort("Delay", MobSpawnerEntity.GetSpawnDelay());
			Writer.AddString("id", "MobSpawner");
			break;
		}

		default:
		{
			break;
		}
	}

	Writer.Finish();
	a_Pkt.WriteBuf(Writer.GetResult().data(), Writer.GetResult().size());
}





void cProtocol_1_10_0::WriteMobMetadata(cPacketizer & a_Pkt, const cMonster & a_Mob)
{
	using namespace Metadata;

	// Living Enitiy Metadata
	if (a_Mob.HasCustomName())
	{
		// TODO: As of 1.9 _all_ entities can have custom names; should this be moved up?
		a_Pkt.WriteBEUInt8(ENTITY_CUSTOM_NAME);
		a_Pkt.WriteBEUInt8(METADATA_TYPE_STRING);
		a_Pkt.WriteString(a_Mob.GetCustomName());

		a_Pkt.WriteBEUInt8(ENTITY_CUSTOM_NAME_VISIBLE);  // Custom name always visible
		a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
		a_Pkt.WriteBool(a_Mob.IsCustomNameAlwaysVisible());
	}

	a_Pkt.WriteBEUInt8(LIVING_HEALTH);
	a_Pkt.WriteBEUInt8(METADATA_TYPE_FLOAT);
	a_Pkt.WriteBEFloat(static_cast<float>(a_Mob.GetHealth()));

	switch (a_Mob.GetMobType())
	{
		case mtBat:
		{
			auto & Bat = static_cast<const cBat &>(a_Mob);
			a_Pkt.WriteBEUInt8(BAT_HANGING);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BYTE);
			a_Pkt.WriteBEInt8(Bat.IsHanging() ? 1 : 0);
			break;
		}  // case mtBat

		case mtChicken:
		{
			auto & Chicken = static_cast<const cChicken &>(a_Mob);

			a_Pkt.WriteBEUInt8(AGEABLE_BABY);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Chicken.IsBaby());
			break;
		}  // case mtChicken

		case mtCow:
		{
			auto & Cow = static_cast<const cCow &>(a_Mob);

			a_Pkt.WriteBEUInt8(AGEABLE_BABY);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Cow.IsBaby());
			break;
		}  // case mtCow

		case mtCreeper:
		{
			auto & Creeper = static_cast<const cCreeper &>(a_Mob);
			a_Pkt.WriteBEUInt8(CREEPER_STATE);  // (idle or "blowing")
			a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(Creeper.IsBlowing() ? 1 : static_cast<UInt32>(-1));

			a_Pkt.WriteBEUInt8(CREEPER_POWERED);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Creeper.IsCharged());

			a_Pkt.WriteBEUInt8(CREEPER_IGNITED);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Creeper.IsBurnedWithFlintAndSteel());
			break;
		}  // case mtCreeper

		case mtEnderman:
		{
			auto & Enderman = static_cast<const cEnderman &>(a_Mob);
			a_Pkt.WriteBEUInt8(ENDERMAN_CARRIED_BLOCK);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BLOCKID);
			UInt32 Carried = 0;
			Carried |= static_cast<UInt32>(Enderman.GetCarriedBlock() << 4);
			Carried |= Enderman.GetCarriedMeta();
			a_Pkt.WriteVarInt32(Carried);

			a_Pkt.WriteBEUInt8(ENDERMAN_SCREAMING);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Enderman.IsScreaming());
			break;
		}  // case mtEnderman

		case mtGhast:
		{
			auto & Ghast = static_cast<const cGhast &>(a_Mob);
			a_Pkt.WriteBEUInt8(GHAST_ATTACKING);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Ghast.IsCharging());
			break;
		}  // case mtGhast

		case mtHorse:
		{
			auto & Horse = static_cast<const cHorse &>(a_Mob);
			Int8 Flags = 0;
			if (Horse.IsTame())
			{
				Flags |= 0x02;
			}
			if (Horse.IsSaddled())
			{
				Flags |= 0x04;
			}
			if (Horse.IsChested())
			{
				Flags |= 0x08;
			}
			if (Horse.IsEating())
			{
				Flags |= 0x20;
			}
			if (Horse.IsRearing())
			{
				Flags |= 0x40;
			}
			if (Horse.IsMthOpen())
			{
				Flags |= 0x80;
			}
			a_Pkt.WriteBEUInt8(HORSE_STATUS);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BYTE);
			a_Pkt.WriteBEInt8(Flags);

			a_Pkt.WriteBEUInt8(HORSE_TYPE);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Horse.GetHorseType()));

			a_Pkt.WriteBEUInt8(HORSE_VARIANT);  // Color / style
			a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
			int Appearance = 0;
			Appearance = Horse.GetHorseColor();
			Appearance |= Horse.GetHorseStyle() << 8;
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Appearance));

			a_Pkt.WriteBEUInt8(HORSE_ARMOR);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Horse.GetHorseArmour()));

			a_Pkt.WriteBEUInt8(AGEABLE_BABY);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Horse.IsBaby());
			break;
		}  // case mtHorse

		case mtMagmaCube:
		{
			auto & MagmaCube = static_cast<const cMagmaCube &>(a_Mob);
			a_Pkt.WriteBEUInt8(SLIME_SIZE);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(MagmaCube.GetSize()));
			break;
		}  // case mtMagmaCube

		case mtOcelot:
		{
			auto & Ocelot = static_cast<const cOcelot &>(a_Mob);

			a_Pkt.WriteBEUInt8(AGEABLE_BABY);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Ocelot.IsBaby());
			break;
		}  // case mtOcelot

		case mtPig:
		{
			auto & Pig = static_cast<const cPig &>(a_Mob);

			a_Pkt.WriteBEUInt8(AGEABLE_BABY);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Pig.IsBaby());

			a_Pkt.WriteBEUInt8(PIG_HAS_SADDLE);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Pig.IsSaddled());

			break;
		}  // case mtPig

		case mtRabbit:
		{
			auto & Rabbit = static_cast<const cRabbit &>(a_Mob);
			a_Pkt.WriteBEUInt8(AGEABLE_BABY);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Rabbit.IsBaby());

			a_Pkt.WriteBEUInt8(RABBIT_TYPE);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Rabbit.GetRabbitType()));
			break;
		}  // case mtRabbit

		case mtSheep:
		{
			auto & Sheep = static_cast<const cSheep &>(a_Mob);

			a_Pkt.WriteBEUInt8(AGEABLE_BABY);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Sheep.IsBaby());

			a_Pkt.WriteBEUInt8(SHEEP_STATUS);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BYTE);
			Int8 SheepMetadata = 0;
			SheepMetadata = static_cast<Int8>(Sheep.GetFurColor());
			if (Sheep.IsSheared())
			{
				SheepMetadata |= 0x10;
			}
			a_Pkt.WriteBEInt8(SheepMetadata);
			break;
		}  // case mtSheep

		case mtSlime:
		{
			auto & Slime = static_cast<const cSlime &>(a_Mob);
			a_Pkt.WriteBEUInt8(SLIME_SIZE);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Slime.GetSize()));
			break;
		}  // case mtSlime

		case mtVillager:
		{
			auto & Villager = static_cast<const cVillager &>(a_Mob);
			a_Pkt.WriteBEUInt8(AGEABLE_BABY);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Villager.IsBaby());

			a_Pkt.WriteBEUInt8(VILLAGER_PROFESSION);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Villager.GetVilType()));
			break;
		}  // case mtVillager

		case mtWitch:
		{
			auto & Witch = static_cast<const cWitch &>(a_Mob);
			a_Pkt.WriteBEUInt8(WITCH_AGGRESIVE);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Witch.IsAngry());
			break;
		}  // case mtWitch

		case mtWither:
		{
			auto & Wither = static_cast<const cWither &>(a_Mob);
			a_Pkt.WriteBEUInt8(WITHER_INVULNERABLE_TIMER);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(Wither.GetWitherInvulnerableTicks());

			// TODO: Use boss bar packet for health
			break;
		}  // case mtWither

		case mtWitherSkeleton:
		{
			a_Pkt.WriteBEUInt8(SKELETON_TYPE);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(1);  // Is wither skeleton
			break;
		}  // case mtWitherSkeleton

		case mtWolf:
		{
			auto & Wolf = static_cast<const cWolf &>(a_Mob);
			a_Pkt.WriteBEUInt8(AGEABLE_BABY);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Wolf.IsBaby());

			Int8 WolfStatus = 0;
			if (Wolf.IsSitting())
			{
				WolfStatus |= 0x1;
			}
			if (Wolf.IsAngry())
			{
				WolfStatus |= 0x2;
			}
			if (Wolf.IsTame())
			{
				WolfStatus |= 0x4;
			}
			a_Pkt.WriteBEUInt8(TAMEABLE_ANIMAL_STATUS);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BYTE);
			a_Pkt.WriteBEInt8(WolfStatus);

			a_Pkt.WriteBEUInt8(WOLF_DAMAGE_TAKEN);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_FLOAT);
			a_Pkt.WriteBEFloat(static_cast<float>(a_Mob.GetHealth()));  // TODO Not use the current health

			a_Pkt.WriteBEUInt8(WOLF_BEGGING);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Wolf.IsBegging());

			a_Pkt.WriteBEUInt8(WOLF_COLLAR_COLOR);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Wolf.GetCollarColor()));
			break;
		}  // case mtWolf

		case mtZombie:
		{
			auto & Zombie = static_cast<const cZombie &>(a_Mob);
			a_Pkt.WriteBEUInt8(ZOMBIE_IS_BABY);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Zombie.IsBaby());

			a_Pkt.WriteBEUInt8(ZOMBIE_TYPE);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(0);

			a_Pkt.WriteBEUInt8(ZOMBIE_CONVERTING);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(false);
			break;
		}  // case mtZombie

		case mtZombiePigman:
		{
			auto & ZombiePigman = static_cast<const cZombiePigman &>(a_Mob);
			a_Pkt.WriteBEUInt8(AGEABLE_BABY);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(ZombiePigman.IsBaby());
			break;
		}  // case mtZombiePigman

		case mtZombieVillager:
		{
			auto & ZombieVillager = reinterpret_cast<const cZombieVillager &>(a_Mob);
			a_Pkt.WriteBEUInt8(ZOMBIE_IS_BABY);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(ZombieVillager.IsBaby());

			a_Pkt.WriteBEUInt8(ZOMBIE_TYPE);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(ZombieVillager.GetProfession());

			a_Pkt.WriteBEUInt8(ZOMBIE_CONVERTING);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(ZombieVillager.ConversionTime() != -1);
			break;
		}  // case mtZombieVillager

		default: break;
	}  // switch (a_Mob.GetType())
}
