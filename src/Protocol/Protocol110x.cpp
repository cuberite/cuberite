
// Protocol110x.cpp

/*
Implements the 1.10.x protocol classes:
	- cProtocol1100
		- release 1.10.0 protocol (#210)
(others may be added later in the future for the 1.10 release series)
*/

#include "Globals.h"
#include "Protocol110x.h"
#include "Packetizer.h"

#include "../Root.h"
#include "../Server.h"

#include "../Entities/Boat.h"
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

#include "Bindings/PluginManager.h"





// The disabled error is intended, since the Metadata have overlapping indexes
// based on the type of the Entity.
//
// IMPORTANT: The enum is used to automate the sequential counting of the
// Metadata indexes. Adding a new enum value causes the following values to
// increase their index. Therefore the ordering of the enum values is VERY important!
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wduplicate-enum"

namespace Metadata
{
	enum Metadata_Index
	{
		// Entity
		FLAGS,
		AIR,
		CUSTOM_NAME,
		CUSTOM_NAME_VISIBLE,
		SILENT,
		NO_GRAVITY,
		_ENTITY_NEXT,  // Used by descendants

		// Potion
		THROWN = _ENTITY_NEXT,

		// FallingBlock
		POSITION = _ENTITY_NEXT,

		// AreaEffectCloud
		RADIUS = _ENTITY_NEXT,
		AREAEFFECTCLOUD_COLOR,
		SINGLE_POINT_EFFECT,
		PARTICLE_ID,
		PARTICLE_PARAMETER1,
		PARTICLE_PARAMETER2,

		// Arrow
		CRITICAL = _ENTITY_NEXT,
		_ARROW_NEXT,

		// TippedArrow
		ARROW_COLOR = _ARROW_NEXT,

		// Boat
		LAST_HIT_TIME = _ENTITY_NEXT,
		FORWARD_DIRECTION,
		BOAT_DAMAGE_TAKEN,
		BOAT_TYPE,
		RIGHT_PADDLE_TURNING,
		LEFT_PADDLE_TURNING,

		// EnderCrystal
		BEAM_TARGET = _ENTITY_NEXT,
		SHOW_BOTTOM,

		// Fireball
		_FIREBALL_NEXT = _ENTITY_NEXT,

		// WitherSkull
		INVULNERABLE = _FIREBALL_NEXT,

		// Fireworks
		FIREWORK_INFO = _ENTITY_NEXT,

		// Hanging
		_HANGING_NEXT = _ENTITY_NEXT,

		// ItemFrame
		ITEM_FRAME_ITEM = _HANGING_NEXT,
		ROTATION,

		// Item
		ITEM = _ENTITY_NEXT,

		// Living
		ACTIVE_HAND = _ENTITY_NEXT,
		HEALTH,
		POTION_EFFECT_COLOR,
		POTION_EFFECT_AMBIENT,
		NUMBER_OF_ARROWS,
		_LIVING_NEXT,

		// Player
		ADDITIONAL_HEARTHS = _LIVING_NEXT,
		SCORE,
		DISPLAYED_SKIN_PARTS,
		MAIN_HAND,

		// ArmorStand
		ARMOR_STAND_STATUS = _LIVING_NEXT,
		HEAD_ROTATION,
		BODY_ROTATION,
		LEFT_ARM_ROTATION,
		RIGHT_ARM_ROTATION,
		LEFT_LEG_ROTATION,
		RIGHT_LEG_ROTATION,

		// Insentient
		INSENTIENT_STATUS = _LIVING_NEXT,
		_INSENTIENT_NEXT,

		// Ambient
		_AMBIENT_NEXT = _INSENTIENT_NEXT,

		// Bat
		HANGING = _AMBIENT_NEXT,

		// Creature
		_CREATURE_NEXT = _INSENTIENT_NEXT,

		// Ageable
		BABY = _CREATURE_NEXT,
		_AGEABLE_NEXT,

		// PolarBear
		STANDING = _AGEABLE_NEXT,

		// Animal
		_ANIMAL_NEXT = _AGEABLE_NEXT,

		// Horse
		HORSE_STATUS = _ANIMAL_NEXT,
		HORSE_TYPE,
		HORSE_VARIANT,
		HORSE_OWNER,
		ARMOR,

		// Pig
		HAS_SADDLE = _ANIMAL_NEXT,

		// Rabbit
		RABBIT_TYPE = _ANIMAL_NEXT,

		// Sheep
		SHEEP_STATUS = _ANIMAL_NEXT,

		// TameableAnimal
		TAMEABLEANIMAL_STATUS = _ANIMAL_NEXT,
		TAMEABLE_OWNER,
		_TAMEABLE_NEXT,

		// Ocelot
		OCELOT_TYPE = _TAMEABLE_NEXT,

		// Wolf
		WOLF_DAMAGE_TAKEN = _TAMEABLE_NEXT,
		BEGGING,
		COLLAR_COLOR,

		// Villager
		PROFESSION = _AGEABLE_NEXT,

		// Golem
		_GOLEM_NEXT = _CREATURE_NEXT,

		// IronGolem
		PLAYER_CREATED = _GOLEM_NEXT,

		// Shulker
		FACING_DIRECTION = _GOLEM_NEXT,
		ATTACHMENT_POSITION,
		SHIELD_HEIGHT,

		// Monster
		_MONSTER_NEXT = _CREATURE_NEXT,

		// Blaze
		ON_FIRE = _MONSTER_NEXT,

		// Creeper
		CREEPER_STATE = _MONSTER_NEXT,
		CREEPER_POWERED,
		IGNITED,

		// Guardian
		GUARDIAN_STATUS = _MONSTER_NEXT,
		TARGET,

		// Skeleton
		SKELETON_TYPE = _MONSTER_NEXT,
		ARMS_SWINGING,

		// Spider
		CLIMBING = _MONSTER_NEXT,

		// Witch
		AGGRESIVE = _MONSTER_NEXT,

		// Wither
		FIRST_HEAD_TARGET = _MONSTER_NEXT,
		SECOND_HEAD_TARGET,
		THIRD_HEAD_TARGET,
		INVULNERABLE_TIMER,

		// Zombie
		ZOMBIE_IS_BABY = _MONSTER_NEXT,
		ZOMBIE_TYPE,
		CONVERTING,
		HANDS_RISED_UP,

		// Enderman
		CARRIED_BLOCK = _MONSTER_NEXT,
		SCREAMING,

		// EnderDragon
		DRAGON_PHASE = _INSENTIENT_NEXT,

		// Flying
		_FLYING_NEXT = _INSENTIENT_NEXT,

		// Ghast
		ATTACKING = _FLYING_NEXT,

		// Slime
		SLIME_SIZE = _INSENTIENT_NEXT,

		// Minecart
		SHAKING_POWER = _ENTITY_NEXT,
		SHAKING_DIRECTION,
		SHAKING_MULTIPLIER,
		BLOCK_ID_META,
		BLOCK_Y,
		SHOW_BLOCK,
		_MINECART_NEXT,

		// MinecartCommandBlock
		COMMAND = _MINECART_NEXT,
		LAST_OUTPUT,

		// MinecartFurnace
		MINECARTFURNACE_POWERED = _MINECART_NEXT,

		// TNTPrimed
		FUSE_TIME = _ENTITY_NEXT,
	};
}

#pragma clang diagnostic pop  // Restore ignored clang errors





cProtocol1100::cProtocol1100(cClientHandle * a_Client, const AString &a_ServerAddress, UInt16 a_ServerPort, UInt32 a_State) :
	super(a_Client, a_ServerAddress, a_ServerPort, a_State)
{
}





void cProtocol1100::SendSoundEffect(const AString & a_SoundName, double a_X, double a_Y, double a_Z, float a_Volume, float a_Pitch)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x19);  // Named sound effect packet
	Pkt.WriteString(a_SoundName);
	Pkt.WriteVarInt32(0);  // Master sound category (may want to be changed to a parameter later)
	Pkt.WriteBEInt32(FloorC(a_X * 8.0));
	Pkt.WriteBEInt32(FloorC(a_Y * 8.0));
	Pkt.WriteBEInt32(FloorC(a_Z * 8.0));
	Pkt.WriteBEFloat(a_Volume);
	Pkt.WriteBEFloat(a_Pitch * 63);
}





void cProtocol1100::HandlePacketStatusRequest(cByteBuffer & a_ByteBuffer)
{
	cServer * Server = cRoot::Get()->GetServer();
	AString ServerDescription = Server->GetDescription();
	int NumPlayers = Server->GetNumPlayers();
	int MaxPlayers = Server->GetMaxPlayers();
	AString Favicon = Server->GetFaviconData();
	cRoot::Get()->GetPluginManager()->CallHookServerPing(*m_Client, ServerDescription, NumPlayers, MaxPlayers, Favicon);

	// Version:
	Json::Value Version;
	Version["name"] = "Cuberite 1.10";
	Version["protocol"] = 210;

	// Players:
	Json::Value Players;
	Players["online"] = NumPlayers;
	Players["max"] = MaxPlayers;
	// TODO: Add "sample"

	// Description:
	Json::Value Description;
	Description["text"] = ServerDescription.c_str();

	// Create the response:
	Json::Value ResponseValue;
	ResponseValue["version"] = Version;
	ResponseValue["players"] = Players;
	ResponseValue["description"] = Description;
	if (!Favicon.empty())
	{
		ResponseValue["favicon"] = Printf("data:image/png;base64,%s", Favicon.c_str());
	}

	Json::StyledWriter Writer;
	AString Response = Writer.write(ResponseValue);

	cPacketizer Pkt(*this, 0x00);  // Response packet
	Pkt.WriteString(Response);
}





void cProtocol1100::WriteEntityMetadata(cPacketizer & a_Pkt, const cEntity & a_Entity)
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
	a_Pkt.WriteBEUInt8(FLAGS);  // Index
	a_Pkt.WriteBEUInt8(METADATA_TYPE_BYTE);  // Type
	a_Pkt.WriteBEInt8(Flags);

	switch (a_Entity.GetEntityType())
	{
		case cEntity::etPlayer: break;  // TODO?
		case cEntity::etPickup:
		{
			a_Pkt.WriteBEUInt8(ITEM);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_ITEM);
			WriteItem(a_Pkt, reinterpret_cast<const cPickup &>(a_Entity).GetItem());
			break;
		}
		case cEntity::etMinecart:
		{
			a_Pkt.WriteBEUInt8(SHAKING_POWER);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);

			// The following expression makes Minecarts shake more with less health or higher damage taken
			auto & Minecart = reinterpret_cast<const cMinecart &>(a_Entity);
			auto maxHealth = a_Entity.GetMaxHealth();
			auto curHealth = a_Entity.GetHealth();
			a_Pkt.WriteVarInt32(static_cast<UInt32>((maxHealth - curHealth) * Minecart.LastDamage() * 4));

			a_Pkt.WriteBEUInt8(SHAKING_DIRECTION);  // (doesn't seem to effect anything)
			a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(1);

			a_Pkt.WriteBEUInt8(SHAKING_MULTIPLIER);  // or damage taken
			a_Pkt.WriteBEUInt8(METADATA_TYPE_FLOAT);
			a_Pkt.WriteBEFloat(static_cast<float>(Minecart.LastDamage() + 10));

			if (Minecart.GetPayload() == cMinecart::mpNone)
			{
				auto & RideableMinecart = reinterpret_cast<const cRideableMinecart &>(Minecart);
				const cItem & MinecartContent = RideableMinecart.GetContent();
				if (!MinecartContent.IsEmpty())
				{
					a_Pkt.WriteBEUInt8(BLOCK_ID_META);
					a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
					int Content = MinecartContent.m_ItemType;
					Content |= MinecartContent.m_ItemDamage << 8;
					a_Pkt.WriteVarInt32(static_cast<UInt32>(Content));

					a_Pkt.WriteBEUInt8(BLOCK_Y);
					a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
					a_Pkt.WriteVarInt32(static_cast<UInt32>(RideableMinecart.GetBlockHeight()));

					a_Pkt.WriteBEUInt8(SHOW_BLOCK);
					a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
					a_Pkt.WriteBool(true);
				}
			}
			else if (Minecart.GetPayload() == cMinecart::mpFurnace)
			{
				a_Pkt.WriteBEUInt8(MINECARTFURNACE_POWERED);
				a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
				a_Pkt.WriteBool(reinterpret_cast<const cMinecartWithFurnace &>(Minecart).IsFueled());
			}
			break;
		}  // case etMinecart

		case cEntity::etProjectile:
		{
			auto & Projectile = reinterpret_cast<const cProjectileEntity &>(a_Entity);
			switch (Projectile.GetProjectileKind())
			{
				case cProjectileEntity::pkArrow:
				{
					a_Pkt.WriteBEUInt8(CRITICAL);
					a_Pkt.WriteBEUInt8(METADATA_TYPE_BYTE);
					a_Pkt.WriteBEInt8(reinterpret_cast<const cArrowEntity &>(Projectile).IsCritical() ? 1 : 0);
					break;
				}
				case cProjectileEntity::pkFirework:
				{
					a_Pkt.WriteBEUInt8(FIREWORK_INFO);  // Firework item used for this firework
					a_Pkt.WriteBEUInt8(METADATA_TYPE_ITEM);
					WriteItem(a_Pkt, reinterpret_cast<const cFireworkEntity &>(Projectile).GetItem());
					break;
				}
				case cProjectileEntity::pkSplashPotion:
				{
					a_Pkt.WriteBEUInt8(THROWN);  // Potion item which was thrown
					a_Pkt.WriteBEUInt8(METADATA_TYPE_ITEM);
					WriteItem(a_Pkt, reinterpret_cast<const cSplashPotionEntity &>(Projectile).GetItem());
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
			WriteMobMetadata(a_Pkt, reinterpret_cast<const cMonster &>(a_Entity));
			break;
		}

		case cEntity::etBoat:
		{
			auto & Boat = reinterpret_cast<const cBoat &>(a_Entity);

			a_Pkt.WriteBEInt8(LAST_HIT_TIME);
			a_Pkt.WriteBEInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteBEInt32(Boat.GetLastDamage());

			a_Pkt.WriteBEInt8(FORWARD_DIRECTION);
			a_Pkt.WriteBEInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteBEInt32(Boat.GetForwardDirection());

			a_Pkt.WriteBEInt8(BOAT_DAMAGE_TAKEN);
			a_Pkt.WriteBEInt8(METADATA_TYPE_FLOAT);
			a_Pkt.WriteBEFloat(Boat.GetDamageTaken());

			a_Pkt.WriteBEInt8(BOAT_TYPE);
			a_Pkt.WriteBEInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteBEInt32(Boat.GetType());

			a_Pkt.WriteBEInt8(RIGHT_PADDLE_TURNING);
			a_Pkt.WriteBEInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Boat.IsRightPaddleUsed());

			a_Pkt.WriteBEInt8(LEFT_PADDLE_TURNING);
			a_Pkt.WriteBEInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Boat.IsLeftPaddleUsed());

			break;
		}  // case etBoat

		case cEntity::etItemFrame:
		{
			auto & Frame = reinterpret_cast<const cItemFrame &>(a_Entity);
			a_Pkt.WriteBEUInt8(ITEM_FRAME_ITEM);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_ITEM);
			WriteItem(a_Pkt, Frame.GetItem());
			a_Pkt.WriteBEUInt8(ROTATION);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(Frame.GetItemRotation());
			break;
		}  // case etItemFrame

		default:
		{
			break;
		}
	}
}





void cProtocol1100::WriteMobMetadata(cPacketizer & a_Pkt, const cMonster & a_Mob)
{
	using namespace Metadata;

	// Living Enitiy Metadata
	if (a_Mob.HasCustomName())
	{
		// TODO: As of 1.9 _all_ entities can have custom names; should this be moved up?
		a_Pkt.WriteBEUInt8(CUSTOM_NAME);
		a_Pkt.WriteBEUInt8(METADATA_TYPE_STRING);
		a_Pkt.WriteString(a_Mob.GetCustomName());

		a_Pkt.WriteBEUInt8(CUSTOM_NAME_VISIBLE);  // Custom name always visible
		a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
		a_Pkt.WriteBool(a_Mob.IsCustomNameAlwaysVisible());
	}

	a_Pkt.WriteBEUInt8(HEALTH);
	a_Pkt.WriteBEUInt8(METADATA_TYPE_FLOAT);
	a_Pkt.WriteBEFloat(static_cast<float>(a_Mob.GetHealth()));

	switch (a_Mob.GetMobType())
	{
		case mtBat:
		{
			auto & Bat = reinterpret_cast<const cBat &>(a_Mob);
			a_Pkt.WriteBEUInt8(HANGING);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BYTE);
			a_Pkt.WriteBEInt8(Bat.IsHanging() ? 1 : 0);
			break;
		}  // case mtBat

		case mtCreeper:
		{
			auto & Creeper = reinterpret_cast<const cCreeper &>(a_Mob);
			a_Pkt.WriteBEUInt8(CREEPER_STATE);  // (idle or "blowing")
			a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(Creeper.IsBlowing() ? 1 : static_cast<UInt32>(-1));

			a_Pkt.WriteBEUInt8(CREEPER_POWERED);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Creeper.IsCharged());

			a_Pkt.WriteBEUInt8(IGNITED);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Creeper.IsBurnedWithFlintAndSteel());
			break;
		}  // case mtCreeper

		case mtEnderman:
		{
			auto & Enderman = reinterpret_cast<const cEnderman &>(a_Mob);
			a_Pkt.WriteBEUInt8(CARRIED_BLOCK);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BLOCKID);
			UInt32 Carried = 0;
			Carried |= static_cast<UInt32>(Enderman.GetCarriedBlock() << 4);
			Carried |= Enderman.GetCarriedMeta();
			a_Pkt.WriteVarInt32(Carried);

			a_Pkt.WriteBEUInt8(SCREAMING);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Enderman.IsScreaming());
			break;
		}  // case mtEnderman

		case mtGhast:
		{
			auto & Ghast = reinterpret_cast<const cGhast &>(a_Mob);
			a_Pkt.WriteBEUInt8(ATTACKING);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Ghast.IsCharging());
			break;
		}  // case mtGhast

		case mtHorse:
		{
			auto & Horse = reinterpret_cast<const cHorse &>(a_Mob);
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

			a_Pkt.WriteBEUInt8(ARMOR);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Horse.GetHorseArmour()));

			a_Pkt.WriteBEUInt8(BABY);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Horse.IsBaby());
			break;
		}  // case mtHorse

		case mtMagmaCube:
		{
			auto & MagmaCube = reinterpret_cast<const cMagmaCube &>(a_Mob);
			a_Pkt.WriteBEUInt8(SLIME_SIZE);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(MagmaCube.GetSize()));
			break;
		}  // case mtMagmaCube

		case mtOcelot:
		{
			auto & Ocelot = reinterpret_cast<const cOcelot &>(a_Mob);

			a_Pkt.WriteBEUInt8(BABY);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Ocelot.IsBaby());
			break;
		}  // case mtOcelot

		case mtCow:
		{
			auto & Cow = reinterpret_cast<const cCow &>(a_Mob);

			a_Pkt.WriteBEUInt8(BABY);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Cow.IsBaby());
			break;
		}  // case mtCow

		case mtChicken:
		{
			auto & Chicken = reinterpret_cast<const cChicken &>(a_Mob);

			a_Pkt.WriteBEUInt8(BABY);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Chicken.IsBaby());
			break;
		}  // case mtChicken

		case mtPig:
		{
			auto & Pig = reinterpret_cast<const cPig &>(a_Mob);

			a_Pkt.WriteBEUInt8(BABY);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Pig.IsBaby());

			a_Pkt.WriteBEUInt8(HAS_SADDLE);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Pig.IsSaddled());

			break;
		}  // case mtPig

		case mtSheep:
		{
			auto & Sheep = reinterpret_cast<const cSheep &>(a_Mob);

			a_Pkt.WriteBEUInt8(BABY);
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

		case mtRabbit:
		{
			auto & Rabbit = reinterpret_cast<const cRabbit &>(a_Mob);
			a_Pkt.WriteBEUInt8(BABY);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Rabbit.IsBaby());

			a_Pkt.WriteBEUInt8(RABBIT_TYPE);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Rabbit.GetRabbitType()));
			break;
		}  // case mtRabbit

		case mtSkeleton:
		{
			auto & Skeleton = reinterpret_cast<const cSkeleton &>(a_Mob);
			a_Pkt.WriteBEUInt8(SKELETON_TYPE);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(Skeleton.IsWither() ? 1 : 0);
			break;
		}  // case mtSkeleton

		case mtSlime:
		{
			auto & Slime = reinterpret_cast<const cSlime &>(a_Mob);
			a_Pkt.WriteBEUInt8(SLIME_SIZE);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Slime.GetSize()));
			break;
		}  // case mtSlime

		case mtVillager:
		{
			auto & Villager = reinterpret_cast<const cVillager &>(a_Mob);
			a_Pkt.WriteBEUInt8(BABY);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Villager.IsBaby());

			a_Pkt.WriteBEUInt8(PROFESSION);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Villager.GetVilType()));
			break;
		}  // case mtVillager

		case mtWitch:
		{
			auto & Witch = reinterpret_cast<const cWitch &>(a_Mob);
			a_Pkt.WriteBEUInt8(AGGRESIVE);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Witch.IsAngry());
			break;
		}  // case mtWitch

		case mtWither:
		{
			auto & Wither = reinterpret_cast<const cWither &>(a_Mob);
			a_Pkt.WriteBEUInt8(INVULNERABLE_TIMER);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(Wither.GetWitherInvulnerableTicks());

			// TODO: Use boss bar packet for health
			break;
		}  // case mtWither

		case mtWolf:
		{
			auto & Wolf = reinterpret_cast<const cWolf &>(a_Mob);
			a_Pkt.WriteBEUInt8(BABY);
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
			a_Pkt.WriteBEUInt8(TAMEABLEANIMAL_STATUS);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BYTE);
			a_Pkt.WriteBEInt8(WolfStatus);

			a_Pkt.WriteBEUInt8(WOLF_DAMAGE_TAKEN);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_FLOAT);
			a_Pkt.WriteBEFloat(static_cast<float>(a_Mob.GetHealth()));  // TODO Not use the current health

			a_Pkt.WriteBEUInt8(BEGGING);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Wolf.IsBegging());

			a_Pkt.WriteBEUInt8(COLLAR_COLOR);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Wolf.GetCollarColor()));
			break;
		}  // case mtWolf

		case mtZombie:
		{
			auto & Zombie = reinterpret_cast<const cZombie &>(a_Mob);
			a_Pkt.WriteBEUInt8(BABY);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Zombie.IsBaby());

			a_Pkt.WriteBEUInt8(ZOMBIE_TYPE);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(Zombie.IsVillagerZombie() ? 1 : 0);  // TODO: This actually encodes the zombie villager profession, but that isn't implemented yet.

			a_Pkt.WriteBEUInt8(CONVERTING);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Zombie.IsConverting());
			break;
		}  // case mtZombie

		case mtZombiePigman:
		{
			auto & ZombiePigman = reinterpret_cast<const cZombiePigman &>(a_Mob);
			a_Pkt.WriteBEUInt8(BABY);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(ZombiePigman.IsBaby());
			break;
		}  // case mtZombiePigman
	}  // switch (a_Mob.GetType())
}
