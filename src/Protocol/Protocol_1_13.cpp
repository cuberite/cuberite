
// Protocol_1_13.cpp

/*
Implements the 1.13 protocol classes:
- release 1.13 protocol (#393)
*/

#include "Globals.h"
#include "Protocol_1_13.h"
#include "ProtocolRecognizer.h"
#include "ChunkDataSerializer.h"
#include "Packetizer.h"
#include "ProtocolPalettes.h"

#include "../Entities/Boat.h"
#include "../Entities/Minecart.h"
#include "../Entities/Pickup.h"
#include "../Entities/Player.h"
#include "../Entities/ItemFrame.h"
#include "../Entities/ArrowEntity.h"
#include "../Entities/FireworkEntity.h"
#include "../Entities/SplashPotionEntity.h"

#include "../Mobs/IncludeAllMonsters.h"

#include "../BlockTypePalette.h"
#include "../ClientHandle.h"
#include "../Root.h"
#include "../Server.h"
#include "../World.h"
#include "../JsonUtils.h"

#include "../Bindings/PluginManager.h"





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
		BOAT_SPLASH_TIMER,

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





#define HANDLE_READ(ByteBuf, Proc, Type, Var) \
	Type Var; \
	do { \
		if (!ByteBuf.Proc(Var))\
		{\
			return;\
		} \
	} while (false)





#define HANDLE_PACKET_READ(ByteBuf, Proc, Type, Var) \
	Type Var; \
	do { \
		{ \
			if (!ByteBuf.Proc(Var)) \
			{ \
				ByteBuf.CheckValid(); \
				return false; \
			} \
			ByteBuf.CheckValid(); \
		} \
	} while (false)





cProtocol_1_13::cProtocol_1_13(cClientHandle * a_Client, const AString & a_ServerAddress, UInt16 a_ServerPort, UInt32 a_State) :
	Super(a_Client, a_ServerAddress, a_ServerPort, a_State)
{
}





void cProtocol_1_13::Initialize(cClientHandle & a_Client)
{
	// Get the palettes; fail if not available:
	auto paletteVersion = this->GetPaletteVersion();
	m_BlockTypePalette = cRoot::Get()->GetProtocolPalettes().blockTypePalette(paletteVersion);
	if (m_BlockTypePalette == nullptr)
	{
		throw std::runtime_error(Printf("This server doesn't support protocol %s.", paletteVersion));
	}

	// Process the palette into the temporary BLOCKTYPE -> NetBlockID map:
	auto upg = cRoot::Get()->GetUpgradeBlockTypePalette();
	m_BlockTypeMap = m_BlockTypePalette->createTransformMapWithFallback(upg, 0);
}





UInt32 cProtocol_1_13::GetPacketID(ePacketType a_PacketType)
{
	switch (a_PacketType)
	{
		case pktAttachEntity:         return 0x40;
		case pktBlockChanges:         return 0x0f;
		case pktCameraSetTo:          return 0x3c;
		case pktChatRaw:              return 0x0e;
		case pktCollectEntity:        return 0x4f;
		case pktDestroyEntity:        return 0x35;
		case pktDisconnectDuringGame: return 0x1b;
		case pktEditSign:             return 0x2c;
		case pktEntityEffect:         return 0x53;
		case pktEntityEquipment:      return 0x42;
		case pktEntityHeadLook:       return 0x39;
		case pktEntityLook:           return 0x2a;
		case pktEntityMeta:           return 0x3f;
		case pktEntityProperties:     return 0x52;
		case pktEntityRelMove:        return 0x28;
		case pktEntityRelMoveLook:    return 0x29;
		case pktEntityStatus:         return 0x1c;
		case pktEntityVelocity:       return 0x41;
		case pktExperience:           return 0x43;
		case pktExplosion:            return 0x1e;
		case pktGameMode:             return 0x20;
		case pktHeldItemChange:       return 0x3d;
		case pktInventorySlot:        return 0x17;
		case pktJoinGame:             return 0x25;
		case pktKeepAlive:            return 0x21;
		case pktMapData:              return 0x26;
		case pktParticleEffect:       return 0x24;
		case pktPlayerAbilities:      return 0x2e;
		case pktPlayerList:           return 0x30;
		case pktPlayerMaxSpeed:       return 0x52;
		case pktPlayerMoveLook:       return 0x32;
		case pktPluginMessage:        return 0x19;
		case pktRemoveEntityEffect:   return 0x36;
		case pktRespawn:              return 0x38;
		case pktScoreboardObjective:  return 0x45;
		case pktSoundEffect:          return 0x1a;
		case pktSoundParticleEffect:  return 0x23;
		case pktSpawnPosition:        return 0x49;
		case pktTabCompletionResults: return 0x10;
		case pktTeleportEntity:       return 0x50;
		case pktTimeUpdate:           return 0x4a;
		case pktTitle:                return 0x4b;
		case pktUnloadChunk:          return 0x1f;
		case pktUnlockRecipe:         return 0x32;
		case pktUpdateHealth:         return 0x44;
		case pktUpdateScore:          return 0x48;
		case pktUpdateSign:           return GetPacketID(pktUpdateBlockEntity);
		case pktUseBed:               return 0x33;
		case pktWindowClose:          return 0x13;
		case pktWindowItems:          return 0x15;
		case pktWindowOpen:           return 0x14;
		case pktWindowProperty:       return 0x16;
		default: return Super::GetPacketID(a_PacketType);
	}
}





AString cProtocol_1_13::GetPaletteVersion() const
{
	return "1.13";
}





bool cProtocol_1_13::HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType)
{
	if (m_State != 3)
	{
		return Super::HandlePacket(a_ByteBuffer, a_PacketType);
	}

	// Game
	switch (a_PacketType)
	{
		case 0x00: HandleConfirmTeleport(a_ByteBuffer); return true;
		case 0x05: HandlePacketTabComplete(a_ByteBuffer); return true;
		case 0x02: HandlePacketChatMessage(a_ByteBuffer); return true;
		case 0x03: HandlePacketClientStatus(a_ByteBuffer); return true;
		case 0x04: HandlePacketClientSettings(a_ByteBuffer); return true;
		case 0x06: break;  // Confirm transaction - not used in Cuberite
		case 0x07: HandlePacketEnchantItem(a_ByteBuffer); return true;
		case 0x08: HandlePacketWindowClick(a_ByteBuffer); return true;
		case 0x09: HandlePacketWindowClose(a_ByteBuffer); return true;
		case 0x0a: HandlePacketPluginMessage(a_ByteBuffer); return true;
		case 0x0d: HandlePacketUseEntity(a_ByteBuffer); return true;
		case 0x0e: HandlePacketKeepAlive(a_ByteBuffer); return true;
		case 0x0f: HandlePacketPlayer(a_ByteBuffer); return true;
		case 0x10: HandlePacketPlayerPos(a_ByteBuffer); return true;
		case 0x11: HandlePacketPlayerPosLook(a_ByteBuffer); return true;
		case 0x12: HandlePacketPlayerLook(a_ByteBuffer); return true;
		case 0x13: HandlePacketVehicleMove(a_ByteBuffer); return true;
		case 0x14: HandlePacketBoatSteer(a_ByteBuffer); return true;
		case 0x15: break;  // Pick item - not yet implemented
		case 0x16: break;  // Craft Recipe Request - not yet implemented
		case 0x17: HandlePacketPlayerAbilities(a_ByteBuffer); return true;
		case 0x18: HandlePacketBlockDig(a_ByteBuffer); return true;
		case 0x19: HandlePacketEntityAction(a_ByteBuffer); return true;
		case 0x1a: HandlePacketSteerVehicle(a_ByteBuffer); return true;
		case 0x1b: HandlePacketCraftingBookData(a_ByteBuffer); return true;
		case 0x1d: break;  // Resource pack status - not yet implemented
		case 0x1e: HandlePacketAdvancementTab(a_ByteBuffer); return true;
		case 0x21: HandlePacketSlotSelect(a_ByteBuffer); return true;
		case 0x24: HandlePacketCreativeInventoryAction(a_ByteBuffer); return true;
		case 0x26: HandlePacketUpdateSign(a_ByteBuffer); return true;
		case 0x27: HandlePacketAnimation(a_ByteBuffer); return true;
		case 0x28: HandlePacketSpectate(a_ByteBuffer); return true;
		case 0x29: HandlePacketBlockPlace(a_ByteBuffer); return true;
		case 0x2a: HandlePacketUseItem(a_ByteBuffer); return true;
	}

	return Super::HandlePacket(a_ByteBuffer, a_PacketType);
}





void cProtocol_1_13::HandlePacketStatusRequest(cByteBuffer & a_ByteBuffer)
{
	cServer * Server = cRoot::Get()->GetServer();
	AString ServerDescription = Server->GetDescription();
	auto NumPlayers = static_cast<signed>(Server->GetNumPlayers());
	auto MaxPlayers = static_cast<signed>(Server->GetMaxPlayers());
	AString Favicon = Server->GetFaviconData();
	cRoot::Get()->GetPluginManager()->CallHookServerPing(*m_Client, ServerDescription, NumPlayers, MaxPlayers, Favicon);

	// Version:
	Json::Value Version;
	Version["name"] = "Cuberite 1.13";
	Version["protocol"] = cMultiVersionProtocol::PROTO_VERSION_1_13;

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
	m_Client->ForgeAugmentServerListPing(ResponseValue);
	if (!Favicon.empty())
	{
		ResponseValue["favicon"] = Printf("data:image/png;base64,%s", Favicon.c_str());
	}

	// Serialize the response into a packet:
	cPacketizer Pkt(*this, pktStatusResponse);
	Pkt.WriteString(JsonUtils::WriteFastString(ResponseValue));
}





void cProtocol_1_13::HandlePacketPluginMessage(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Channel);

	// If the plugin channel is recognized vanilla, handle it directly:
	if (Channel.substr(0, 15) == "minecraft:brand")
	{
		HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Brand);
		m_Client->SetClientBrand(Brand);

		// Send back our brand, including the length:
		SendPluginMessage("minecraft:brand", "\x08""Cuberite");
		return;
	}

	// Read the plugin message and relay to clienthandle:
	AString Data;
	VERIFY(a_ByteBuffer.ReadString(Data, a_ByteBuffer.GetReadableSpace() - 1));  // Always succeeds
	m_Client->HandlePluginMessage(Channel, Data);
}





void cProtocol_1_13::SendAttachEntity(const cEntity & a_Entity, const cEntity & a_Vehicle)
{
	/* TODO
	ASSERT(m_State == 3);  // In game mode?
	cPacketizer Pkt(*this, pktAttachEntity);
	Pkt.WriteVarInt32(a_Vehicle.GetUniqueID());
	Pkt.WriteVarInt32(1);  // 1 passenger
	Pkt.WriteVarInt32(a_Entity.GetUniqueID());
	*/
}





void cProtocol_1_13::SendBlockChange(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktBlockChange);
	Pkt.WritePosition64(a_BlockX, a_BlockY, a_BlockZ);
	Pkt.WriteVarInt32(static_cast<UInt32>(a_BlockType));  // TODO: Palette
}





void cProtocol_1_13::SendBlockChanges(int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktBlockChanges);
	Pkt.WriteBEInt32(a_ChunkX);
	Pkt.WriteBEInt32(a_ChunkZ);
	Pkt.WriteVarInt32(static_cast<UInt32>(a_Changes.size()));
	for (sSetBlockVector::const_iterator itr = a_Changes.begin(), end = a_Changes.end(); itr != end; ++itr)
	{
		Int16 Coords = static_cast<Int16>(itr->m_RelY | (itr->m_RelZ << 8) | (itr->m_RelX << 12));
		Pkt.WriteBEInt16(Coords);
		Pkt.WriteVarInt32(static_cast<UInt32>(itr->m_BlockType));  // TODO: Palette
	}  // for itr - a_Changes[]
}





void cProtocol_1_13::SendChunkData(int a_ChunkX, int a_ChunkZ, cChunkDataSerializer & a_Serializer)
{
	ASSERT(m_State == 3);  // In game mode?

	const AString & ChunkData = a_Serializer.Serialize(cChunkDataSerializer::RELEASE_1_13, a_ChunkX, a_ChunkZ, m_BlockTypeMap);
	cCSLock Lock(m_CSPacket);
	SendData(ChunkData.data(), ChunkData.size());
}





void cProtocol_1_13::SendDetachEntity(const cEntity & a_Entity, const cEntity & a_PreviousVehicle)
{
	/* TODO
	ASSERT(m_State == 3);  // In game mode?
	cPacketizer Pkt(*this, pktAttachEntity);
	Pkt.WriteVarInt32(static_cast<UInt32>(a_PreviousVehicle.GetUniqueID()));
	Pkt.WriteVarInt32(0);  // No passangers
	*/
}





void cProtocol_1_13::SendMapData(const cMap & a_Map, int a_DataStartX, int a_DataStartY)
{
	// TODO
}





void cProtocol_1_13::SendPaintingSpawn(const cPainting & a_Painting)
{
	// TODO
}





void cProtocol_1_13::SendPluginMessage(const AString & a_Channel, const AString & a_Message)
{
	// TODO
}





void cProtocol_1_13::SendScoreboardObjective(const AString & a_Name, const AString & a_DisplayName, Byte a_Mode)
{
	// TODO
}





void cProtocol_1_13::SendSpawnMob(const cMonster & a_Mob)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktSpawnMob);
	Pkt.WriteVarInt32(a_Mob.GetUniqueID());
	// TODO: Bad way to write a UUID, and it's not a true UUID, but this is functional for now.
	Pkt.WriteBEUInt64(0);
	Pkt.WriteBEUInt64(a_Mob.GetUniqueID());
	Pkt.WriteVarInt32(static_cast<UInt32>(a_Mob.GetMobType()));
	Vector3d LastSentPos = a_Mob.GetLastSentPos();
	Pkt.WriteBEDouble(LastSentPos.x);
	Pkt.WriteBEDouble(LastSentPos.y);
	Pkt.WriteBEDouble(LastSentPos.z);
	Pkt.WriteByteAngle(a_Mob.GetPitch());
	Pkt.WriteByteAngle(a_Mob.GetHeadYaw());
	Pkt.WriteByteAngle(a_Mob.GetYaw());
	Pkt.WriteBEInt16(static_cast<Int16>(a_Mob.GetSpeedX() * 400));
	Pkt.WriteBEInt16(static_cast<Int16>(a_Mob.GetSpeedY() * 400));
	Pkt.WriteBEInt16(static_cast<Int16>(a_Mob.GetSpeedZ() * 400));
	WriteEntityMetadata(Pkt, a_Mob);
	Pkt.WriteBEUInt8(0xff);  // Metadata terminator
}





void cProtocol_1_13::SendStatistics(const cStatManager & a_Manager)
{
	// TODO
}





void cProtocol_1_13::SendTabCompletionResults(const AStringVector & a_Results)
{
	// TODO
}





void cProtocol_1_13::SendUpdateBlockEntity(cBlockEntity & a_BlockEntity)
{
	// TODO
}





UInt32 cProtocol_1_13::GetProtocolMobType(eMonsterType a_MobType)
{
	switch (a_MobType)
	{
		// Map invalid type to Giant for easy debugging (if this ever spawns, something has gone very wrong)
		case mtInvalidType:           return 27;
		case mtBat:                   return 3;
		case mtBlaze:                 return 4;
		case mtCaveSpider:            return 6;
		case mtChicken:               return 7;
		case mtCow:                   return 9;
		case mtCreeper:               return 10;
		case mtEnderDragon:           return 17;
		case mtEnderman:              return 18;
		case mtGhast:                 return 26;
		case mtGiant:                 return 27;
		case mtGuardian:              return 28;
		case mtHorse:                 return 29;
		case mtIronGolem:             return 80;
		case mtMagmaCube:             return 38;
		case mtMooshroom:             return 47;
		case mtOcelot:                return 48;
		case mtPig:                   return 51;
		case mtRabbit:                return 56;
		case mtSheep:                 return 58;
		case mtSilverfish:            return 61;
		case mtSkeleton:              return 62;
		case mtSlime:                 return 64;
		case mtSnowGolem:             return 66;
		case mtSpider:                return 69;
		case mtSquid:                 return 70;
		case mtVillager:              return 79;
		case mtWitch:                 return 82;
		case mtWither:                return 83;
		case mtWitherSkeleton:        return 84;
		case mtWolf:                  return 86;
		case mtZombie:                return 87;
		case mtZombiePigman:          return 53;
		case mtZombieVillager:        return 89;
	}
	UNREACHABLE("Unsupported mob type");
}





bool cProtocol_1_13::ReadItem(cByteBuffer & a_ByteBuffer, cItem & a_Item, size_t a_KeepRemainingBytes)
{
	HANDLE_PACKET_READ(a_ByteBuffer, ReadBEInt16, Int16, ItemType);
	if (ItemType == -1)
	{
		// The item is empty, no more data follows
		a_Item.Empty();
		return true;
	}
	a_Item.m_ItemType = ItemType;

	HANDLE_PACKET_READ(a_ByteBuffer, ReadBEInt8, Int8, ItemCount);
	a_Item.m_ItemCount = ItemCount;
	a_Item.m_ItemDamage = 0;  // o no, no more damage in 1.13
	if (ItemCount <= 0)
	{
		a_Item.Empty();
	}

	AString Metadata;
	if (!a_ByteBuffer.ReadString(Metadata, a_ByteBuffer.GetReadableSpace() - a_KeepRemainingBytes - 1) || (Metadata.size() == 0) || (Metadata[0] == 0))
	{
		// No metadata
		return true;
	}

	ParseItemMetadata(a_Item, Metadata);
	return true;
}





void cProtocol_1_13::WriteItem(cPacketizer & a_Pkt, const cItem & a_Item)
{
	short ItemType = a_Item.m_ItemType;
	ASSERT(ItemType >= -1);  // Check validity of packets in debug runtime
	if (ItemType <= 0)
	{
		// Fix, to make sure no invalid values are sent.
		ItemType = -1;
	}

	if (a_Item.IsEmpty())
	{
		a_Pkt.WriteBEInt16(-1);
		return;
	}

	// Normal item
	// TODO: use new item ids
	a_Pkt.WriteBEInt16(ItemType);
	a_Pkt.WriteBEInt8(a_Item.m_ItemCount);

	// TODO: NBT
	a_Pkt.WriteBEInt8(0);
}





void cProtocol_1_13::WriteEntityMetadata(cPacketizer & a_Pkt, const cEntity & a_Entity)
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
			/* TODO
			a_Pkt.WriteBEUInt8(ITEM_ITEM);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_ITEM);
			WriteItem(a_Pkt, static_cast<const cPickup &>(a_Entity).GetItem());
			*/
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
					// TODO
					break;
				}
				case cProjectileEntity::pkSplashPotion:
				{
					// TODO
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

			// TODO: some issue with these, investigate
			/*
			a_Pkt.WriteBEInt8(BOAT_RIGHT_PADDLE_TURNING);
			a_Pkt.WriteBEInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Boat.IsRightPaddleUsed());

			a_Pkt.WriteBEInt8(BOAT_LEFT_PADDLE_TURNING);
			a_Pkt.WriteBEInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(static_cast<bool>(Boat.IsLeftPaddleUsed()));

			// TODO: splash timer?
			a_Pkt.WriteBEInt8(BOAT_SPLASH_TIMER);
			a_Pkt.WriteBEInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(0);
			*/

			break;
		}  // case etBoat

		case cEntity::etItemFrame:
		{
			// TODO
			break;
		}  // case etItemFrame

		default:
		{
			break;
		}
	}
}





void cProtocol_1_13::WriteMobMetadata(cPacketizer & a_Pkt, const cMonster & a_Mob)
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
			/* "ata cannot be cast to java.lang.Boolean"? Investigate
			auto & Chicken = static_cast<const cChicken &>(a_Mob);

			a_Pkt.WriteBEUInt8(AGEABLE_BABY);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Chicken.IsBaby());
			*/
			break;
		}  // case mtChicken

		case mtCow:
		{
			/* "ata cannot be cast to java.lang.Boolean"? Investigate
			auto & Cow = static_cast<const cCow &>(a_Mob);

			a_Pkt.WriteBEUInt8(AGEABLE_BABY);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Cow.IsBaby());
			*/
			break;
		}  // case mtCow

		case mtCreeper:
		{
			/* "ata cannot be cast to java.lang.Boolean"? Investigate
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
			*/
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

			/* "ata cannot be cast to java.lang.Boolean"? Investigate
			a_Pkt.WriteBEUInt8(ENDERMAN_SCREAMING);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Enderman.IsScreaming());
			*/
			break;
		}  // case mtEnderman

		case mtGhast:
		{
			/* "ata cannot be cast to java.lang.Boolean"? Investigate
			auto & Ghast = static_cast<const cGhast &>(a_Mob);

			a_Pkt.WriteBEUInt8(GHAST_ATTACKING);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Ghast.IsCharging());
			*/
			break;
		}  // case mtGhast

		case mtHorse:
		{
			// XXX This behaves incorrectly with different varients; horses have different entity IDs now

			// Abstract horse
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
			a_Pkt.WriteBEUInt8(ABSTRACT_HORSE_STATUS);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BYTE);
			a_Pkt.WriteBEInt8(Flags);

			// This doesn't exist any more; it'll cause horses to all be the normal type
			// a_Pkt.WriteBEUInt8(HORSE_TYPE);
			// a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
			// a_Pkt.WriteVarInt32(static_cast<UInt32>(Horse.GetHorseType()));

			// Regular horses
			a_Pkt.WriteBEUInt8(HORSE_VARIANT);  // Color / style
			a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
			int Appearance = 0;
			Appearance = Horse.GetHorseColor();
			Appearance |= Horse.GetHorseStyle() << 8;
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Appearance));

			a_Pkt.WriteBEUInt8(HORSE_ARMOR);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Horse.GetHorseArmour()));

			/* "ata cannot be cast to java.lang.Boolean"? Investigate
			a_Pkt.WriteBEUInt8(AGEABLE_BABY);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Horse.IsBaby());
			*/
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

			/* "ata cannot be cast to java.lang.Boolean"? Investigate
			a_Pkt.WriteBEUInt8(AGEABLE_BABY);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Ocelot.IsBaby());
			*/

			Int8 OcelotStatus = 0;
			if (Ocelot.IsSitting())
			{
				OcelotStatus |= 0x1;
			}
			if (Ocelot.IsTame())
			{
				OcelotStatus |= 0x4;
			}
			a_Pkt.WriteBEUInt8(TAMEABLE_ANIMAL_STATUS);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BYTE);
			a_Pkt.WriteBEInt8(OcelotStatus);

			a_Pkt.WriteBEUInt8(OCELOT_TYPE);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Ocelot.GetOcelotType()));

			break;
		}  // case mtOcelot

		case mtPig:
		{
			/* "ata cannot be cast to java.lang.Boolean"? Investigate
			auto & Pig = static_cast<const cPig &>(a_Mob);

			a_Pkt.WriteBEUInt8(AGEABLE_BABY);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Pig.IsBaby());

			a_Pkt.WriteBEUInt8(PIG_HAS_SADDLE);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Pig.IsSaddled());
			*/

			// PIG_TOTAL_CARROT_ON_A_STICK_BOOST in 1.11.1 only
			break;
		}  // case mtPig

		case mtRabbit:
		{
			auto & Rabbit = static_cast<const cRabbit &>(a_Mob);
			/* "ata cannot be cast to java.lang.Boolean"? Investigate
			a_Pkt.WriteBEUInt8(AGEABLE_BABY);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Rabbit.IsBaby());
			*/

			a_Pkt.WriteBEUInt8(RABBIT_TYPE);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Rabbit.GetRabbitType()));
			break;
		}  // case mtRabbit

		case mtSheep:
		{
			auto & Sheep = static_cast<const cSheep &>(a_Mob);

			/* "ata cannot be cast to java.lang.Boolean"? Investigate
			a_Pkt.WriteBEUInt8(AGEABLE_BABY);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Sheep.IsBaby());
			*/

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

		case mtSkeleton:
		{
			// XXX Skeletons are separate entities; all skeletons are currently treated as regular ones

			// auto & Skeleton = static_cast<const cSkeleton &>(a_Mob);
			// a_Pkt.WriteBEUInt8(SKELETON_TYPE);
			// a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
			// a_Pkt.WriteVarInt32(Skeleton.IsWither() ? 1 : 0);
			break;
		}  // case mtSkeleton

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
			/* "ata cannot be cast to java.lang.Boolean"? Investigate
			a_Pkt.WriteBEUInt8(AGEABLE_BABY);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Villager.IsBaby());
			*/

			a_Pkt.WriteBEUInt8(VILLAGER_PROFESSION);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Villager.GetVilType()));
			break;
		}  // case mtVillager

		case mtWitch:
		{
			/* "ata cannot be cast to java.lang.Boolean"? Investigate
			auto & Witch = static_cast<const cWitch &>(a_Mob);

			a_Pkt.WriteBEUInt8(WITCH_AGGRESIVE);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Witch.IsAngry());
			*/
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

		case mtWolf:
		{
			auto & Wolf = static_cast<const cWolf &>(a_Mob);
			
			/* "ata cannot be cast to java.lang.Boolean"? Investigate
			a_Pkt.WriteBEUInt8(AGEABLE_BABY);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Wolf.IsBaby());
			*/

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

			/* "ata cannot be cast to java.lang.Boolean"? Investigate
			a_Pkt.WriteBEUInt8(WOLF_BEGGING);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Wolf.IsBegging());
			*/

			a_Pkt.WriteBEUInt8(WOLF_COLLAR_COLOR);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Wolf.GetCollarColor()));
			break;
		}  // case mtWolf

		case mtZombie:
		{
			// XXX Zombies were also split into new sublcasses; this doesn't handle that.
			
			/* "ata cannot be cast to java.lang.Boolean"? Investigate
			auto & Zombie = static_cast<const cZombie &>(a_Mob);

			a_Pkt.WriteBEUInt8(ZOMBIE_IS_BABY);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Zombie.IsBaby());
			*/

			// These don't exist
			// a_Pkt.WriteBEUInt8(ZOMBIE_TYPE);
			// a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
			// a_Pkt.WriteVarInt32(Zombie.IsVillagerZombie() ? 1 : 0);

			// a_Pkt.WriteBEUInt8(ZOMBIE_CONVERTING);
			// a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			// a_Pkt.WriteBool(Zombie.IsConverting());
			break;
		}  // case mtZombie

		case mtZombiePigman:
		{
			/* "ata cannot be cast to java.lang.Boolean"? Investigate
			auto & ZombiePigman = static_cast<const cZombiePigman &>(a_Mob);

			a_Pkt.WriteBEUInt8(AGEABLE_BABY);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(ZombiePigman.IsBaby());
			*/
			break;
		}  // case mtZombiePigman

		case mtBlaze:
		case mtEnderDragon:
		case mtGuardian:
		case mtIronGolem:
		case mtSnowGolem:
		case mtSpider:
		{
			// TODO: Mobs with extra fields that aren't implemented
			break;
		}

		case mtMooshroom:
		case mtCaveSpider:
		{
			// Not mentioned on http://wiki.vg/Entities
			break;
		}

		case mtGiant:
		case mtSilverfish:
		case mtSquid:
		{
			// Mobs with no extra fields
			break;
		}

		case mtInvalidType:
		{
			ASSERT(!"cProtocol_1_12::WriteMobMetadata: Recieved mob of invalid type");
			break;
		}
	}  // switch (a_Mob.GetType())
}
