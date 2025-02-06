
// Protocol_1_12.cpp

/*
Implements the 1.12 protocol classes:
- release 1.12 protocol (#335)
- release 1.12.1 protocol (#338)
- release 1.12.2 protocol (#340)
*/

#include "Globals.h"
#include "Protocol_1_12.h"
#include "Packetizer.h"

#include "../Entities/Boat.h"
#include "../Entities/EnderCrystal.h"
#include "../Entities/Minecart.h"
#include "../Entities/Pickup.h"
#include "../Entities/Player.h"
#include "../Entities/ItemFrame.h"
#include "../Entities/ArrowEntity.h"
#include "../Entities/FireworkEntity.h"
#include "../Entities/SplashPotionEntity.h"

#include "../Mobs/IncludeAllMonsters.h"

#include "../Root.h"
#include "../Server.h"
#include "../ClientHandle.h"
#include "../CraftingRecipes.h"
#include "../Bindings/PluginManager.h"
#include "../JsonUtils.h"





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

namespace Metadata_1_12
{
	enum MetadataIndex
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
		FIREWORK_BOOSTED_ENTITY_ID,  // 1.11.1 only

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

		// Abstract horse
		ABSTRACT_HORSE_STATUS = _ANIMAL_NEXT,
		ABSTRACT_HORSE_OWNER,
		_ABSTRACT_HORSE_NEXT,

		// Horse
		HORSE_VARIANT = _ABSTRACT_HORSE_NEXT,
		HORSE_ARMOR,

		// Chested horse
		CHESTED_HORSE_CHESTED = _ABSTRACT_HORSE_NEXT,
		_CHESTED_HORSE_NEXT,

		// Llama
		LLAMA_STRENGTH = _CHESTED_HORSE_NEXT,
		LLAMA_CARPET_COLOR,
		LLAMA_VARIANT,

		// Pig
		PIG_HAS_SADDLE = _ANIMAL_NEXT,
		PIG_TOTAL_CARROT_ON_A_STICK_BOOST,  // 1.11.1 only

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

		// Abstract Skeleton
		ABSTRACT_SKELETON_ARMS_SWINGING = _MONSTER_NEXT,

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
		ZOMBIE_UNUSED,  // Was type
		ZOMBIE_HANDS_RISED_UP,
		_ZOMBIE_NEXT,

		// Zombie villager
		ZOMBIE_VILLAGER_CONVERTING = _ZOMBIE_NEXT,
		ZOMBIE_VILLAGER_PROFESSION,

		// Enderman
		ENDERMAN_CARRIED_BLOCK = _MONSTER_NEXT,
		ENDERMAN_SCREAMING,

		// Evocation illager
		EVOKER_SPELL = _MONSTER_NEXT,

		// Vex
		VEX_FLAGS = _MONSTER_NEXT,

		// Vindication illager
		VINDICATOR_FLAGS = _MONSTER_NEXT,

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
// cProtocol_1_12:

void cProtocol_1_12::WriteEntityMetadata(cPacketizer & a_Pkt, const cEntity & a_Entity, bool a_WriteCommon) const
{
	using namespace Metadata_1_12;

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
	if (a_Entity.IsElytraFlying())
	{
		Flags |= 0x80;
	}
	a_Pkt.WriteBEUInt8(ENTITY_FLAGS);  // Index
	a_Pkt.WriteBEUInt8(METADATA_TYPE_BYTE);  // Type
	a_Pkt.WriteBEInt8(Flags);
}





UInt32 cProtocol_1_12::GetPacketID(cProtocol::ePacketType a_Packet) const
{
	switch (a_Packet)
	{
		case pktAttachEntity:        return 0x42;
		case pktCameraSetTo:         return 0x38;
		case pktCollectEntity:       return 0x4a;
		case pktDestroyEntity:       return 0x31;
		case pktDisplayObjective:    return 0x3a;
		case pktEntityEffect:        return 0x4e;
		case pktEntityEquipment:     return 0x3e;
		case pktEntityHeadLook:      return 0x35;
		case pktEntityLook:          return 0x28;
		case pktEntityMeta:          return 0x3b;
		case pktEntityProperties:    return 0x4d;
		case pktEntityRelMove:       return 0x26;
		case pktEntityRelMoveLook:   return 0x27;
		case pktEntityVelocity:      return 0x3d;
		case pktExperience:          return 0x3f;
		case pktHeldItemChange:      return 0x39;
		case pktLeashEntity:         return 0x3c;
		case pktRemoveEntityEffect:  return 0x32;
		case pktResourcePack:        return 0x33;
		case pktRespawn:             return 0x34;
		case pktScoreboardObjective: return 0x41;
		case pktSpawnPosition:       return 0x45;
		case pktTeleportEntity:      return 0x4b;
		case pktTimeUpdate:          return 0x46;
		case pktTitle:               return 0x47;
		case pktUnlockRecipe:        return 0x30;
		case pktUpdateBlockEntity:   return 0x09;
		case pktUpdateHealth:        return 0x40;
		case pktUpdateScore:         return 0x44;

		default: return Super::GetPacketID(a_Packet);
	}
}





void cProtocol_1_12::HandleCraftRecipe(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8,  WindowID);
	HANDLE_READ(a_ByteBuffer, ReadVarInt,  UInt32, RecipeID);
	HANDLE_READ(a_ByteBuffer, ReadBool,    bool,   MakeAll);
	auto CuberiteRecipeId = cRoot::Get()->GetRecipeMapper()->GetCuberiteRecipeId(RecipeID, m_Client->GetProtocolVersion());
	if (CuberiteRecipeId.has_value())
	{
		m_Client->HandleCraftRecipe(CuberiteRecipeId.value());
	}
}





void cProtocol_1_12::HandlePacketCraftingBookData(cByteBuffer & a_ByteBuffer)
{
	// TODO not yet used, not sure if it is needed
	// https://wiki.vg/index.php?title=Protocol&oldid=14204#Crafting_Book_Data
	a_ByteBuffer.SkipRead(a_ByteBuffer.GetReadableSpace());
}





void cProtocol_1_12::HandlePacketAdvancementTab(cByteBuffer & a_ByteBuffer)
{
	a_ByteBuffer.SkipRead(a_ByteBuffer.GetReadableSpace());
	m_Client->GetPlayer()->SendMessageInfo("The new advancements are not implemented.");
}





signed char cProtocol_1_12::GetProtocolEntityStatus(EntityAnimation a_Animation) const
{
	switch (a_Animation)
	{
		case EntityAnimation::PawnBurns: return 37;
		case EntityAnimation::PawnDrowns: return 36;
		default: return Super::GetProtocolEntityStatus(a_Animation);
	}
}





cProtocol::Version cProtocol_1_12::GetProtocolVersion() const
{
	return Version::v1_12;
}





bool cProtocol_1_12::HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType)
{
	switch (m_State)
	{
		case State::Status:
		{
			switch (a_PacketType)
			{
				case 0x00: HandlePacketStatusRequest(a_ByteBuffer); return true;
				case 0x01: HandlePacketStatusPing(a_ByteBuffer); return true;
			}
			break;
		}

		case State::Login:
		{
			switch (a_PacketType)
			{
				case 0x00: HandlePacketLoginStart(a_ByteBuffer); return true;
				case 0x01: HandlePacketLoginEncryptionResponse(a_ByteBuffer); return true;
			}
			break;
		}

		case State::Game:
		{
			switch (a_PacketType)
			{
				case 0x00: HandleConfirmTeleport(a_ByteBuffer); return true;
				case 0x01: break;  // Prepare Crafting Grid, not yet implemented
				case 0x02: HandlePacketTabComplete(a_ByteBuffer); return true;
				case 0x03: HandlePacketChatMessage(a_ByteBuffer); return true;
				case 0x04: HandlePacketClientStatus(a_ByteBuffer); return true;
				case 0x05: HandlePacketClientSettings(a_ByteBuffer); return true;
				case 0x06: break;  // Confirm transaction - not used in Cuberite
				case 0x07: HandlePacketEnchantItem(a_ByteBuffer); return true;
				case 0x08: HandlePacketWindowClick(a_ByteBuffer); return true;
				case 0x09: HandlePacketWindowClose(a_ByteBuffer); return true;
				case 0x0a: HandlePacketPluginMessage(a_ByteBuffer); return true;
				case 0x0b: HandlePacketUseEntity(a_ByteBuffer); return true;
				case 0x0c: HandlePacketKeepAlive(a_ByteBuffer); return true;
				case 0x0d: HandlePacketPlayer(a_ByteBuffer); return true;
				case 0x0e: HandlePacketPlayerPos(a_ByteBuffer); return true;
				case 0x0f: HandlePacketPlayerPosLook(a_ByteBuffer); return true;
				case 0x10: HandlePacketPlayerLook(a_ByteBuffer); return true;
				case 0x11: HandlePacketVehicleMove(a_ByteBuffer); return true;
				case 0x12: HandlePacketBoatSteer(a_ByteBuffer); return true;
				case 0x13: HandlePacketPlayerAbilities(a_ByteBuffer); return true;
				case 0x14: HandlePacketBlockDig(a_ByteBuffer); return true;
				case 0x15: HandlePacketEntityAction(a_ByteBuffer); return true;
				case 0x16: HandlePacketSteerVehicle(a_ByteBuffer); return true;
				case 0x17: HandlePacketCraftingBookData(a_ByteBuffer); return true;
				case 0x18: HandlePacketResourcePackStatus(a_ByteBuffer); return true;
				case 0x19: HandlePacketAdvancementTab(a_ByteBuffer); return true;
				case 0x1a: HandlePacketSlotSelect(a_ByteBuffer); return true;
				case 0x1b: HandlePacketCreativeInventoryAction(a_ByteBuffer); return true;
				case 0x1c: HandlePacketUpdateSign(a_ByteBuffer); return true;
				case 0x1d: HandlePacketAnimation(a_ByteBuffer); return true;
				case 0x1e: HandlePacketSpectate(a_ByteBuffer); return true;
				case 0x1f: HandlePacketBlockPlace(a_ByteBuffer); return true;
				case 0x20: HandlePacketUseItem(a_ByteBuffer); return true;
			}
			break;
		}
	}  // switch (m_State)

	// Unknown packet type, report to the ClientHandle:
	m_Client->PacketUnknown(a_PacketType);
	return false;
}





////////////////////////////////////////////////////////////////////////////////
// cProtocol_1_12_1:

UInt32 cProtocol_1_12_1::GetPacketID(ePacketType a_Packet) const
{
	switch (a_Packet)
	{
		case pktAttachEntity:           return 0x43;
		case pktCameraSetTo:            return 0x39;
		case pktCollectEntity:          return 0x4b;
		case pktDestroyEntity:          return 0x32;
		case pktDisplayObjective:       return 0x3b;
		case pktEntityEffect:           return 0x4f;
		case pktEntityEquipment:        return 0x3f;
		case pktEntityHeadLook:         return 0x36;
		case pktEntityMeta:             return 0x3c;
		case pktEntityProperties:       return 0x4e;
		case pktEntityVelocity:         return 0x3e;
		case pktExperience:             return 0x40;
		case pktHeldItemChange:         return 0x3a;
		case pktLeashEntity:            return 0x3d;
		case pktPlayerList:             return 0x2e;
		case pktPlayerListHeaderFooter: return 0x4a;
		case pktPlayerAbilities:        return 0x2c;
		case pktPlayerMoveLook:         return 0x2f;
		case pktRemoveEntityEffect:     return 0x33;
		case pktResourcePack:           return 0x34;
		case pktRespawn:                return 0x35;
		case pktScoreboardObjective:    return 0x42;
		case pktSpawnPosition:          return 0x46;
		case pktUnlockRecipe:           return 0x31;
		case pktUpdateHealth:           return 0x41;
		case pktUpdateScore:            return 0x45;
		case pktUseBed:                 return 0x30;
		case pktTeleportEntity:         return 0x4c;
		case pktTimeUpdate:             return 0x47;
		case pktTitle:                  return 0x48;

		default: return Super::GetPacketID(a_Packet);
	}
}





cProtocol::Version cProtocol_1_12_1::GetProtocolVersion() const
{
	return Version::v1_12_1;
}





bool cProtocol_1_12_1::HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType)
{
	switch (m_State)
	{
		case State::Status:
		{
			switch (a_PacketType)
			{
				case 0x00: HandlePacketStatusRequest(a_ByteBuffer); return true;
				case 0x01: HandlePacketStatusPing(a_ByteBuffer); return true;
			}
			break;
		}

		case State::Login:
		{
			switch (a_PacketType)
			{
				case 0x00: HandlePacketLoginStart(a_ByteBuffer); return true;
				case 0x01: HandlePacketLoginEncryptionResponse(a_ByteBuffer); return true;
			}
			break;
		}

		case State::Game:
		{
			switch (a_PacketType)
			{
				case 0x00: HandleConfirmTeleport(a_ByteBuffer); return true;
				case 0x01: HandlePacketTabComplete(a_ByteBuffer); return true;
				case 0x02: HandlePacketChatMessage(a_ByteBuffer); return true;
				case 0x03: HandlePacketClientStatus(a_ByteBuffer); return true;
				case 0x04: HandlePacketClientSettings(a_ByteBuffer); return true;
				case 0x05: break;  // Confirm transaction - not used in Cuberite
				case 0x06: HandlePacketEnchantItem(a_ByteBuffer); return true;
				case 0x07: HandlePacketWindowClick(a_ByteBuffer); return true;
				case 0x08: HandlePacketWindowClose(a_ByteBuffer); return true;
				case 0x09: HandlePacketPluginMessage(a_ByteBuffer); return true;
				case 0x0a: HandlePacketUseEntity(a_ByteBuffer); return true;
				case 0x0b: HandlePacketKeepAlive(a_ByteBuffer); return true;
				case 0x0c: HandlePacketPlayer(a_ByteBuffer); return true;
				case 0x0d: HandlePacketPlayerPos(a_ByteBuffer); return true;
				case 0x0e: HandlePacketPlayerPosLook(a_ByteBuffer); return true;
				case 0x0f: HandlePacketPlayerLook(a_ByteBuffer); return true;
				case 0x10: HandlePacketVehicleMove(a_ByteBuffer); return true;
				case 0x11: HandlePacketBoatSteer(a_ByteBuffer); return true;
				case 0x12: HandleCraftRecipe(a_ByteBuffer); return true;
				case 0x13: HandlePacketPlayerAbilities(a_ByteBuffer); return true;
				case 0x14: HandlePacketBlockDig(a_ByteBuffer); return true;
				case 0x15: HandlePacketEntityAction(a_ByteBuffer); return true;
				case 0x16: HandlePacketSteerVehicle(a_ByteBuffer); return true;
				case 0x17: HandlePacketCraftingBookData(a_ByteBuffer); return true;
				case 0x18: HandlePacketResourcePackStatus(a_ByteBuffer); return true;
				case 0x19: HandlePacketAdvancementTab(a_ByteBuffer); return true;
				case 0x1a: HandlePacketSlotSelect(a_ByteBuffer); return true;
				case 0x1b: HandlePacketCreativeInventoryAction(a_ByteBuffer); return true;
				case 0x1c: HandlePacketUpdateSign(a_ByteBuffer); return true;
				case 0x1d: HandlePacketAnimation(a_ByteBuffer); return true;
				case 0x1e: HandlePacketSpectate(a_ByteBuffer); return true;
				case 0x1f: HandlePacketBlockPlace(a_ByteBuffer); return true;
				case 0x20: HandlePacketUseItem(a_ByteBuffer); return true;
			}
			break;
		}
	}  // switch (m_State)

	// Unknown packet type, report to the ClientHandle:
	m_Client->PacketUnknown(a_PacketType);
	return false;
}




////////////////////////////////////////////////////////////////////////////////
// cProtocol_1_12_2::

cProtocol::Version cProtocol_1_12_2::GetProtocolVersion() const
{
	return Version::v1_12_2;
}





void cProtocol_1_12_2::HandlePacketKeepAlive(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEInt64, Int64, KeepAliveID);
	if (
		(KeepAliveID <= std::numeric_limits<UInt32>::max()) &&
		(KeepAliveID >= 0)
	)
	{
		// The server will only send a UInt32 so any value out of that range shouldn't keep the client alive.
		m_Client->HandleKeepAlive(static_cast<UInt32>(KeepAliveID));
	}
}





void cProtocol_1_12_2::SendKeepAlive(UInt32 a_PingID)
{
	// Drop the packet if the protocol is not in the Game state yet (caused a client crash):
	if (m_State != 3)
	{
		LOGWARNING("Trying to send a KeepAlive packet to a player who's not yet fully logged in (%d). The protocol class prevented the packet.", m_State);
		return;
	}

	cPacketizer Pkt(*this, pktKeepAlive);
	Pkt.WriteBEInt64(a_PingID);
}





void cProtocol_1_12_2::SendUnlockRecipe(UInt32 a_RecipeID)
{
	ASSERT(m_State == 3);  // In game mode?

	auto ProtocolRecipeId = cRoot::Get()->GetRecipeMapper()->GetProtocolRecipeId(a_RecipeID, m_Client->GetProtocolVersion());
	if (ProtocolRecipeId.has_value())
	{
		cPacketizer Pkt(*this, pktUnlockRecipe);
		Pkt.WriteVarInt32(1);
		Pkt.WriteBool(true);
		Pkt.WriteBool(false);
		Pkt.WriteVarInt32(1);
		Pkt.WriteVarInt32(ProtocolRecipeId.value());
	}
}





void cProtocol_1_12_2::SendInitRecipes(UInt32 a_RecipeID)
{
	ASSERT(m_State == 3);  // In game mode?

	auto ProtocolRecipeId = cRoot::Get()->GetRecipeMapper()->GetProtocolRecipeId(a_RecipeID, m_Client->GetProtocolVersion());
	if (!ProtocolRecipeId.has_value())
	{
		return;
	}

	cPacketizer Pkt(*this, pktUnlockRecipe);
	Pkt.WriteVarInt32(0);
	Pkt.WriteBool(true);
	Pkt.WriteBool(false);
	if (a_RecipeID == 0)
	{
		Pkt.WriteVarInt32(0);
		Pkt.WriteVarInt32(0);
	}
	else
	{
		Pkt.WriteVarInt32(1);
		Pkt.WriteVarInt32(ProtocolRecipeId.value());
		Pkt.WriteVarInt32(1);
		Pkt.WriteVarInt32(ProtocolRecipeId.value());
	}
}
