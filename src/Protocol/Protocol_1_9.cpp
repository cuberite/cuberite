
// Protocol_1_9.cpp

/*
Implements the 1.9 protocol classes:
	- cProtocol_1_9_0
		- release 1.9 protocol (#107)
	- cProtocol_1_9_1
		- release 1.9.1 protocol (#108)
	- cProtocol_1_9_2
		- release 1.9.2 protocol (#109)
	- cProtocol_1_9_4
		- release 1.9.4 protocol (#110)
*/

#include "Globals.h"
#include "Protocol_1_9.h"
#include "../mbedTLS++/Sha1Checksum.h"
#include "Packetizer.h"

#include "../ClientHandle.h"
#include "../Root.h"
#include "../Server.h"
#include "../World.h"
#include "../StringCompression.h"
#include "../CompositeChat.h"
#include "../JsonUtils.h"

#include "../WorldStorage/FastNBT.h"

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

#include "../Items/ItemSpawnEgg.h"

#include "../Mobs/IncludeAllMonsters.h"
#include "../UI/HorseWindow.h"

#include "../BlockEntities/BeaconEntity.h"
#include "../BlockEntities/CommandBlockEntity.h"
#include "../BlockEntities/MobHeadEntity.h"
#include "../BlockEntities/MobSpawnerEntity.h"
#include "../BlockEntities/FlowerPotEntity.h"
#include "../Bindings/PluginManager.h"





/** Value for main hand in Hand parameter for Protocol 1.9. */
static const UInt32 MAIN_HAND = 0;
static const UInt32 OFF_HAND = 1;





#define HANDLE_READ(ByteBuf, Proc, Type, Var) \
	Type Var; \
	do { \
		if (!ByteBuf.Proc(Var))\
		{\
			return;\
		} \
	} while (false)





////////////////////////////////////////////////////////////////////////////////
// cProtocol_1_9_0:

cProtocol_1_9_0::cProtocol_1_9_0(cClientHandle * a_Client, const AString & a_ServerAddress, State a_State) :
	Super(a_Client, a_ServerAddress, a_State),
	m_IsTeleportIdConfirmed(true),
	m_OutstandingTeleportId(0)
{
}





void cProtocol_1_9_0::SendAttachEntity(const cEntity & a_Entity, const cEntity & a_Vehicle)
{
	ASSERT(m_State == 3);  // In game mode?
	cPacketizer Pkt(*this, pktAttachEntity);
	Pkt.WriteVarInt32(a_Vehicle.GetUniqueID());
	Pkt.WriteVarInt32(1);  // 1 passenger
	Pkt.WriteVarInt32(a_Entity.GetUniqueID());
}





void cProtocol_1_9_0::SendDetachEntity(const cEntity & a_Entity, const cEntity & a_PreviousVehicle)
{
	ASSERT(m_State == 3);  // In game mode?
	cPacketizer Pkt(*this, pktAttachEntity);
	Pkt.WriteVarInt32(a_PreviousVehicle.GetUniqueID());
	Pkt.WriteVarInt32(0);  // No passangers
}





void cProtocol_1_9_0::SendEntityEquipment(const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktEntityEquipment);
	Pkt.WriteVarInt32(a_Entity.GetUniqueID());
	// Needs to be adjusted due to the insertion of offhand at slot 1
	if (a_SlotNum > 0)
	{
		a_SlotNum++;
	}
	Pkt.WriteVarInt32(static_cast<UInt32>(a_SlotNum));
	WriteItem(Pkt, a_Item);
}





void cProtocol_1_9_0::SendEntityMetadata(const cEntity & a_Entity)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktEntityMeta);
	Pkt.WriteVarInt32(a_Entity.GetUniqueID());
	WriteEntityMetadata(Pkt, a_Entity);
	Pkt.WriteBEUInt8(0xff);  // The termination byte
}





void cProtocol_1_9_0::SendEntityPosition(const cEntity & a_Entity)
{
	ASSERT(m_State == 3);  // In game mode?

	const auto Delta = (a_Entity.GetPosition() - a_Entity.GetLastSentPosition()) * 32 * 128;

	// Limitations of a short
	static const auto Max = std::numeric_limits<Int16>::max();

	if ((std::abs(Delta.x) <= Max) && (std::abs(Delta.y) <= Max) && (std::abs(Delta.z) <= Max))
	{
		const auto Move = static_cast<Vector3<Int16>>(Delta);

		// Difference within limitations, use a relative move packet
		if (a_Entity.IsOrientationDirty())
		{
			cPacketizer Pkt(*this, pktEntityRelMoveLook);
			Pkt.WriteVarInt32(a_Entity.GetUniqueID());
			Pkt.WriteBEInt16(Move.x);
			Pkt.WriteBEInt16(Move.y);
			Pkt.WriteBEInt16(Move.z);
			Pkt.WriteByteAngle(a_Entity.GetYaw());
			Pkt.WriteByteAngle(a_Entity.GetPitch());
			Pkt.WriteBool(a_Entity.IsOnGround());
		}
		else
		{
			cPacketizer Pkt(*this, pktEntityRelMove);
			Pkt.WriteVarInt32(a_Entity.GetUniqueID());
			Pkt.WriteBEInt16(Move.x);
			Pkt.WriteBEInt16(Move.y);
			Pkt.WriteBEInt16(Move.z);
			Pkt.WriteBool(a_Entity.IsOnGround());
		}

		return;
	}

	// Too big a movement, do a teleport
	cPacketizer Pkt(*this, pktTeleportEntity);
	Pkt.WriteVarInt32(a_Entity.GetUniqueID());
	Pkt.WriteBEDouble(a_Entity.GetPosX());
	Pkt.WriteBEDouble(a_Entity.GetPosY());
	Pkt.WriteBEDouble(a_Entity.GetPosZ());
	Pkt.WriteByteAngle(a_Entity.GetYaw());
	Pkt.WriteByteAngle(a_Entity.GetPitch());
	Pkt.WriteBool(a_Entity.IsOnGround());
}





void cProtocol_1_9_0::SendEntityStatus(const cEntity & a_Entity, char a_Status)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktEntityStatus);
	Pkt.WriteBEUInt32(a_Entity.GetUniqueID());
	Pkt.WriteBEInt8(a_Status);
}





void cProtocol_1_9_0::SendExperienceOrb(const cExpOrb & a_ExpOrb)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktSpawnExperienceOrb);
	Pkt.WriteVarInt32(a_ExpOrb.GetUniqueID());
	Pkt.WriteBEDouble(a_ExpOrb.GetPosX());
	Pkt.WriteBEDouble(a_ExpOrb.GetPosY());
	Pkt.WriteBEDouble(a_ExpOrb.GetPosZ());
	Pkt.WriteBEInt16(static_cast<Int16>(a_ExpOrb.GetReward()));
}





void cProtocol_1_9_0::SendKeepAlive(UInt32 a_PingID)
{
	// Drop the packet if the protocol is not in the Game state yet (caused a client crash):
	if (m_State != 3)
	{
		LOG("Trying to send a KeepAlive packet to a player who's not yet fully logged in (%d). The protocol class prevented the packet.", m_State);
		return;
	}

	cPacketizer Pkt(*this, pktKeepAlive);
	Pkt.WriteVarInt32(a_PingID);
}





void cProtocol_1_9_0::SendLeashEntity(const cEntity & a_Entity, const cEntity & a_EntityLeashedTo)
{
	ASSERT(m_State == 3);  // In game mode?
	cPacketizer Pkt(*this, pktLeashEntity);
	Pkt.WriteBEUInt32(a_Entity.GetUniqueID());
	Pkt.WriteBEUInt32(a_EntityLeashedTo.GetUniqueID());
}





void cProtocol_1_9_0::SendUnleashEntity(const cEntity & a_Entity)
{
	ASSERT(m_State == 3);  // In game mode?
	cPacketizer Pkt(*this, pktLeashEntity);
	Pkt.WriteBEUInt32(a_Entity.GetUniqueID());
	Pkt.WriteBEInt32(-1);  // Unleash a_Entity
}





void cProtocol_1_9_0::SendPaintingSpawn(const cPainting & a_Painting)
{
	ASSERT(m_State == 3);  // In game mode?
	double PosX = a_Painting.GetPosX();
	double PosY = a_Painting.GetPosY();
	double PosZ = a_Painting.GetPosZ();

	cPacketizer Pkt(*this, pktSpawnPainting);
	Pkt.WriteVarInt32(a_Painting.GetUniqueID());
	// TODO: Bad way to write a UUID, and it's not a true UUID, but this is functional for now.
	Pkt.WriteBEUInt64(0);
	Pkt.WriteBEUInt64(a_Painting.GetUniqueID());
	Pkt.WriteString(a_Painting.GetName());
	Pkt.WriteXYZPosition64(static_cast<Int32>(PosX), static_cast<Int32>(PosY), static_cast<Int32>(PosZ));
	Pkt.WriteBEInt8(static_cast<Int8>(a_Painting.GetProtocolFacing()));
}





void cProtocol_1_9_0::SendMapData(const cMap & a_Map, int a_DataStartX, int a_DataStartY)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktMapData);
	Pkt.WriteVarInt32(a_Map.GetID());
	Pkt.WriteBEUInt8(static_cast<UInt8>(a_Map.GetScale()));

	Pkt.WriteBool(true);
	Pkt.WriteVarInt32(static_cast<UInt32>(a_Map.GetDecorators().size()));
	for (const auto & Decorator : a_Map.GetDecorators())
	{
		Pkt.WriteBEUInt8(static_cast<Byte>((static_cast<Int32>(Decorator.GetType()) << 4) | (Decorator.GetRot() & 0xF)));
		Pkt.WriteBEUInt8(static_cast<UInt8>(Decorator.GetPixelX()));
		Pkt.WriteBEUInt8(static_cast<UInt8>(Decorator.GetPixelZ()));
	}

	Pkt.WriteBEUInt8(128);
	Pkt.WriteBEUInt8(128);
	Pkt.WriteBEUInt8(static_cast<UInt8>(a_DataStartX));
	Pkt.WriteBEUInt8(static_cast<UInt8>(a_DataStartY));
	Pkt.WriteVarInt32(static_cast<UInt32>(a_Map.GetData().size()));
	for (auto itr = a_Map.GetData().cbegin(); itr != a_Map.GetData().cend(); ++itr)
	{
		Pkt.WriteBEUInt8(*itr);
	}
}





void cProtocol_1_9_0::SendPlayerMaxSpeed(void)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktPlayerMaxSpeed);
	cPlayer * Player = m_Client->GetPlayer();
	Pkt.WriteVarInt32(Player->GetUniqueID());
	Pkt.WriteBEInt32(1);  // Count
	Pkt.WriteString("generic.movementSpeed");
	// The default game speed is 0.1, multiply that value by the relative speed:
	Pkt.WriteBEDouble(0.1 * Player->GetNormalMaxSpeed());
	if (Player->IsSprinting())
	{
		Pkt.WriteVarInt32(1);  // Modifier count
		Pkt.WriteBEUInt64(0x662a6b8dda3e4c1c);
		Pkt.WriteBEUInt64(0x881396ea6097278d);  // UUID of the modifier
		Pkt.WriteBEDouble(Player->GetSprintingMaxSpeed() - Player->GetNormalMaxSpeed());
		Pkt.WriteBEUInt8(2);
	}
	else
	{
		Pkt.WriteVarInt32(0);  // Modifier count
	}
}





void cProtocol_1_9_0::SendPlayerMoveLook(void)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktPlayerMoveLook);
	cPlayer * Player = m_Client->GetPlayer();
	Pkt.WriteBEDouble(Player->GetPosX());
	Pkt.WriteBEDouble(Player->GetPosY());
	Pkt.WriteBEDouble(Player->GetPosZ());
	Pkt.WriteBEFloat(static_cast<float>(Player->GetYaw()));
	Pkt.WriteBEFloat(static_cast<float>(Player->GetPitch()));
	Pkt.WriteBEUInt8(0);
	Pkt.WriteVarInt32(++m_OutstandingTeleportId);

	// This teleport ID hasn't been confirmed yet
	m_IsTeleportIdConfirmed = false;
}





void cProtocol_1_9_0::SendPlayerSpawn(const cPlayer & a_Player)
{
	// Called to spawn another player for the client
	cPacketizer Pkt(*this, pktSpawnOtherPlayer);
	Pkt.WriteVarInt32(a_Player.GetUniqueID());
	Pkt.WriteUUID(a_Player.GetUUID());
	Vector3d LastSentPos = a_Player.GetLastSentPosition();
	Pkt.WriteBEDouble(LastSentPos.x);
	Pkt.WriteBEDouble(LastSentPos.y + 0.001);  // The "+ 0.001" is there because otherwise the player falls through the block they were standing on.
	Pkt.WriteBEDouble(LastSentPos.z);
	Pkt.WriteByteAngle(a_Player.GetYaw());
	Pkt.WriteByteAngle(a_Player.GetPitch());
	WriteEntityMetadata(Pkt, a_Player);
	Pkt.WriteBEUInt8(0xff);  // Metadata: end
}





void cProtocol_1_9_0::SendSoundEffect(const AString & a_SoundName, double a_X, double a_Y, double a_Z, float a_Volume, float a_Pitch)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktSoundEffect);
	Pkt.WriteString(a_SoundName);
	Pkt.WriteVarInt32(0);  // Master sound category (may want to be changed to a parameter later)
	Pkt.WriteBEInt32(static_cast<Int32>(a_X * 8.0));
	Pkt.WriteBEInt32(static_cast<Int32>(a_Y * 8.0));
	Pkt.WriteBEInt32(static_cast<Int32>(a_Z * 8.0));
	Pkt.WriteBEFloat(a_Volume);
	Pkt.WriteBEUInt8(static_cast<Byte>(a_Pitch * 63));
}





void cProtocol_1_9_0::SendSpawnMob(const cMonster & a_Mob)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktSpawnMob);
	Pkt.WriteVarInt32(a_Mob.GetUniqueID());
	// TODO: Bad way to write a UUID, and it's not a true UUID, but this is functional for now.
	Pkt.WriteBEUInt64(0);
	Pkt.WriteBEUInt64(a_Mob.GetUniqueID());
	Pkt.WriteBEUInt8(static_cast<Byte>(GetProtocolMobType(a_Mob.GetMobType())));
	Vector3d LastSentPos = a_Mob.GetLastSentPosition();
	Pkt.WriteBEDouble(LastSentPos.x);
	Pkt.WriteBEDouble(LastSentPos.y);
	Pkt.WriteBEDouble(LastSentPos.z);
	Pkt.WriteByteAngle(a_Mob.GetHeadYaw());  // Doesn't seem to be used
	Pkt.WriteByteAngle(a_Mob.GetPitch());
	Pkt.WriteByteAngle(a_Mob.GetYaw());
	Pkt.WriteBEInt16(static_cast<Int16>(a_Mob.GetSpeedX() * 400));
	Pkt.WriteBEInt16(static_cast<Int16>(a_Mob.GetSpeedY() * 400));
	Pkt.WriteBEInt16(static_cast<Int16>(a_Mob.GetSpeedZ() * 400));
	WriteEntityMetadata(Pkt, a_Mob);
	Pkt.WriteBEUInt8(0xff);  // Metadata terminator
}





void cProtocol_1_9_0::SendThunderbolt(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktSpawnGlobalEntity);
	Pkt.WriteVarInt32(0);  // EntityID = 0, always
	Pkt.WriteBEUInt8(1);  // Type = Thunderbolt
	Pkt.WriteBEDouble(a_BlockX);
	Pkt.WriteBEDouble(a_BlockY);
	Pkt.WriteBEDouble(a_BlockZ);
}





void cProtocol_1_9_0::SendUnloadChunk(int a_ChunkX, int a_ChunkZ)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktUnloadChunk);
	Pkt.WriteBEInt32(a_ChunkX);
	Pkt.WriteBEInt32(a_ChunkZ);
}





UInt32 cProtocol_1_9_0::GetPacketID(cProtocol::ePacketType a_Packet)
{
	switch (a_Packet)
	{
		case pktAttachEntity:          return 0x40;
		case pktBlockAction:           return 0x0a;
		case pktBlockBreakAnim:        return 0x08;
		case pktBlockChange:           return 0x0b;
		case pktBlockChanges:          return 0x10;
		case pktCameraSetTo:           return 0x36;
		case pktChatRaw:               return 0x0f;
		case pktCollectEntity:         return 0x49;
		case pktDestroyEntity:         return 0x30;
		case pktDifficulty:            return 0x0d;
		case pktDisconnectDuringGame:  return 0x1a;
		case pktDisconnectDuringLogin: return 0x0;
		case pktDisplayObjective:      return 0x38;
		case pktEditSign:              return 0x2a;
		case pktEncryptionRequest:     return 0x01;
		case pktEntityAnimation:       return 0x06;
		case pktEntityEffect:          return 0x4c;
		case pktEntityEquipment:       return 0x3c;
		case pktEntityHeadLook:        return 0x34;
		case pktEntityLook:            return 0x27;
		case pktEntityMeta:            return 0x39;
		case pktEntityProperties:      return 0x4b;
		case pktEntityRelMove:         return 0x25;
		case pktEntityRelMoveLook:     return 0x26;
		case pktEntityStatus:          return 0x1b;
		case pktEntityVelocity:        return 0x3b;
		case pktExperience:            return 0x3d;
		case pktExplosion:             return 0x1c;
		case pktGameMode:              return 0x1e;
		case pktHeldItemChange:        return 0x37;
		case pktInventorySlot:         return 0x16;
		case pktJoinGame:              return 0x23;
		case pktKeepAlive:             return 0x1f;
		case pktLeashEntity:           return 0x3a;
		case pktLoginSuccess:          return 0x02;
		case pktMapData:               return 0x24;
		case pktParticleEffect:        return 0x22;
		case pktPingResponse:          return 0x01;
		case pktPlayerAbilities:       return 0x2b;
		case pktPlayerList:            return 0x2d;
		case pktPlayerMaxSpeed:        return 0x4b;
		case pktPlayerMoveLook:        return 0x2e;
		case pktPluginMessage:         return 0x18;
		case pktRemoveEntityEffect:    return 0x31;
		case pktResourcePack:          return 0x32;
		case pktRespawn:               return 0x33;
		case pktScoreboardObjective:   return 0x3f;
		case pktSpawnExperienceOrb:    return 0x01;
		case pktSpawnGlobalEntity:     return 0x02;
		case pktSpawnObject:           return 0x00;
		case pktSpawnOtherPlayer:      return 0x05;
		case pktSpawnPainting:         return 0x04;
		case pktSpawnPosition:         return 0x43;
		case pktSoundEffect:           return 0x19;
		case pktSoundParticleEffect:   return 0x21;
		case pktSpawnMob:              return 0x03;
		case pktStartCompression:      return 0x03;
		case pktStatistics:            return 0x07;
		case pktStatusResponse:        return 0x00;
		case pktTabCompletionResults:  return 0x0e;
		case pktTeleportEntity:        return 0x4a;
		case pktTimeUpdate:            return 0x44;
		case pktTitle:                 return 0x45;
		case pktUnloadChunk:           return 0x1d;
		case pktUpdateBlockEntity:     return 0x09;
		case pktUpdateHealth:          return 0x3e;
		case pktUpdateScore:           return 0x42;
		case pktUpdateSign:            return 0x46;
		case pktUseBed:                return 0x2f;
		case pktWeather:               return 0x1e;
		case pktWindowClose:           return 0x12;
		case pktWindowItems:           return 0x14;
		case pktWindowOpen:            return 0x13;
		case pktWindowProperty:        return 0x15;

		// Unsupported packets
		case pktUnlockRecipe:
		{
			break;
		}
	}
	UNREACHABLE("Unsupported outgoing packet type");
}





cProtocol::Version cProtocol_1_9_0::GetProtocolVersion()
{
	return Version::v1_9_0;
}





bool cProtocol_1_9_0::HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType)
{
	switch (m_State)
	{
		case State::Status:
		{
			switch (a_PacketType)
			{
				case 0x00: HandlePacketStatusRequest(a_ByteBuffer); return true;
				case 0x01: HandlePacketStatusPing   (a_ByteBuffer); return true;
			}
			break;
		}

		case State::Login:
		{
			switch (a_PacketType)
			{
				case 0x00: HandlePacketLoginStart             (a_ByteBuffer); return true;
				case 0x01: HandlePacketLoginEncryptionResponse(a_ByteBuffer); return true;
			}
			break;
		}

		case State::Game:
		{
			switch (a_PacketType)
			{
				case 0x00: HandleConfirmTeleport              (a_ByteBuffer); return true;
				case 0x01: HandlePacketTabComplete            (a_ByteBuffer); return true;
				case 0x02: HandlePacketChatMessage            (a_ByteBuffer); return true;
				case 0x03: HandlePacketClientStatus           (a_ByteBuffer); return true;
				case 0x04: HandlePacketClientSettings         (a_ByteBuffer); return true;
				case 0x05: break;  // Confirm transaction - not used in MCS
				case 0x06: HandlePacketEnchantItem            (a_ByteBuffer); return true;
				case 0x07: HandlePacketWindowClick            (a_ByteBuffer); return true;
				case 0x08: HandlePacketWindowClose            (a_ByteBuffer); return true;
				case 0x09: HandlePacketPluginMessage          (a_ByteBuffer); return true;
				case 0x0a: HandlePacketUseEntity              (a_ByteBuffer); return true;
				case 0x0b: HandlePacketKeepAlive              (a_ByteBuffer); return true;
				case 0x0c: HandlePacketPlayerPos              (a_ByteBuffer); return true;
				case 0x0d: HandlePacketPlayerPosLook          (a_ByteBuffer); return true;
				case 0x0e: HandlePacketPlayerLook             (a_ByteBuffer); return true;
				case 0x0f: HandlePacketPlayer                 (a_ByteBuffer); return true;
				case 0x10: HandlePacketVehicleMove            (a_ByteBuffer); return true;
				case 0x11: HandlePacketBoatSteer              (a_ByteBuffer); return true;
				case 0x12: HandlePacketPlayerAbilities        (a_ByteBuffer); return true;
				case 0x13: HandlePacketBlockDig               (a_ByteBuffer); return true;
				case 0x14: HandlePacketEntityAction           (a_ByteBuffer); return true;
				case 0x15: HandlePacketSteerVehicle           (a_ByteBuffer); return true;
				case 0x16: HandlePacketResourcePackStatus     (a_ByteBuffer); return true;
				case 0x17: HandlePacketSlotSelect             (a_ByteBuffer); return true;
				case 0x18: HandlePacketCreativeInventoryAction(a_ByteBuffer); return true;
				case 0x19: HandlePacketUpdateSign             (a_ByteBuffer); return true;
				case 0x1a: HandlePacketAnimation              (a_ByteBuffer); return true;
				case 0x1b: HandlePacketSpectate               (a_ByteBuffer); return true;
				case 0x1c: HandlePacketBlockPlace             (a_ByteBuffer); return true;
				case 0x1d: HandlePacketUseItem                (a_ByteBuffer); return true;
			}
			break;
		}
	}  // switch (m_State)

	// Unknown packet type, report to the ClientHandle:
	m_Client->PacketUnknown(a_PacketType);
	return false;
}





void cProtocol_1_9_0::HandlePacketAnimation(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarInt, Int32, Hand);

	m_Client->HandleAnimation(0);  // Packet exists solely for arm-swing notification
}





void cProtocol_1_9_0::HandlePacketBlockDig(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8, Status);

	int BlockX, BlockY, BlockZ;
	if (!a_ByteBuffer.ReadXYZPosition64(BlockX, BlockY, BlockZ))
	{
		return;
	}

	HANDLE_READ(a_ByteBuffer, ReadVarInt, Int32, Face);
	m_Client->HandleLeftClick(BlockX, BlockY, BlockZ, FaceIntToBlockFace(Face), Status);
}





void cProtocol_1_9_0::HandlePacketBlockPlace(cByteBuffer & a_ByteBuffer)
{
	int BlockX, BlockY, BlockZ;
	if (!a_ByteBuffer.ReadXYZPosition64(BlockX, BlockY, BlockZ))
	{
		return;
	}

	HANDLE_READ(a_ByteBuffer, ReadVarInt, Int32, Face);
	HANDLE_READ(a_ByteBuffer, ReadVarInt, Int32, Hand);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8, CursorX);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8, CursorY);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8, CursorZ);
	m_Client->HandleRightClick(BlockX, BlockY, BlockZ, FaceIntToBlockFace(Face), CursorX, CursorY, CursorZ, HandIntToEnum(Hand));
}





void cProtocol_1_9_0::HandlePacketBoatSteer(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBool, bool, RightPaddle);
	HANDLE_READ(a_ByteBuffer, ReadBool, bool, LeftPaddle);

	// Get the players vehicle
	cPlayer * Player = m_Client->GetPlayer();
	cEntity * Vehicle = Player->GetAttached();

	if (Vehicle)
	{
		if (Vehicle->GetEntityType() == cEntity::etBoat)
		{
			auto * Boat = static_cast<cBoat *>(Vehicle);
			Boat->UpdatePaddles(RightPaddle, LeftPaddle);
		}
	}
}





void cProtocol_1_9_0::HandlePacketClientSettings(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Locale);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8,       UInt8,   ViewDistance);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8,       UInt8,   ChatFlags);
	HANDLE_READ(a_ByteBuffer, ReadBool,          bool,    ChatColors);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8,       UInt8,   SkinParts);
	HANDLE_READ(a_ByteBuffer, ReadVarInt,        UInt32,   MainHand);

	m_Client->SetLocale(Locale);
	m_Client->SetViewDistance(ViewDistance);
	m_Client->GetPlayer()->SetSkinParts(SkinParts);
	m_Client->GetPlayer()->SetMainHand(static_cast<eMainHand>(MainHand));
	// TODO: Handle chat flags and chat colors
}





void cProtocol_1_9_0::HandleConfirmTeleport(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarInt32, UInt32, TeleportID);

	// Can we stop throwing away incoming player position packets?
	if (TeleportID == m_OutstandingTeleportId)
	{
		m_IsTeleportIdConfirmed = true;
	}
}





void cProtocol_1_9_0::HandlePacketEntityAction(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarInt,  UInt32, PlayerID);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8,  Action);
	HANDLE_READ(a_ByteBuffer, ReadVarInt,  UInt32, JumpBoost);

	switch (Action)
	{
		case 0: m_Client->HandleEntityCrouch(PlayerID, true);     break;  // Crouch
		case 1: m_Client->HandleEntityCrouch(PlayerID, false);    break;  // Uncrouch
		case 2: m_Client->HandleEntityLeaveBed(PlayerID);         break;  // Leave Bed
		case 3: m_Client->HandleEntitySprinting(PlayerID, true);  break;  // Start sprinting
		case 4: m_Client->HandleEntitySprinting(PlayerID, false); break;  // Stop sprinting
		case 7: m_Client->HandleOpenHorseInventory(PlayerID);     break;  // Open horse inventory
	}
}





void cProtocol_1_9_0::HandlePacketPlayerPos(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, PosX);
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, PosY);
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, PosZ);
	HANDLE_READ(a_ByteBuffer, ReadBool,     bool,   IsOnGround);

	if (m_IsTeleportIdConfirmed)
	{
		m_Client->HandlePlayerPos(PosX, PosY, PosZ, PosY + (m_Client->GetPlayer()->IsCrouched() ? 1.54 : 1.62), IsOnGround);
	}
}





void cProtocol_1_9_0::HandlePacketPlayerPosLook(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, PosX);
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, PosY);
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, PosZ);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat,  float,  Yaw);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat,  float,  Pitch);
	HANDLE_READ(a_ByteBuffer, ReadBool,     bool,   IsOnGround);

	if (m_IsTeleportIdConfirmed)
	{
		m_Client->HandlePlayerMoveLook(PosX, PosY, PosZ, PosY + 1.62, Yaw, Pitch, IsOnGround);
	}
}





void cProtocol_1_9_0::HandlePacketSteerVehicle(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, Sideways);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, Forward);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8, Flags);

	if ((Flags & 0x2) != 0)
	{
		m_Client->HandleUnmount();
	}
	else if ((Flags & 0x1) != 0)
	{
		// TODO: Handle vehicle jump (for animals)
	}
	else
	{
		m_Client->HandleSteerVehicle(Forward, Sideways);
	}
}





void cProtocol_1_9_0::HandlePacketTabComplete(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Text);
	HANDLE_READ(a_ByteBuffer, ReadBool,          bool,    AssumeCommand);
	HANDLE_READ(a_ByteBuffer, ReadBool,          bool,    HasPosition);

	if (HasPosition)
	{
		HANDLE_READ(a_ByteBuffer, ReadBEInt64, Int64, Position);
	}

	m_Client->HandleTabCompletion(Text);
}





void cProtocol_1_9_0::HandlePacketUpdateSign(cByteBuffer & a_ByteBuffer)
{
	int BlockX, BlockY, BlockZ;
	if (!a_ByteBuffer.ReadXYZPosition64(BlockX, BlockY, BlockZ))
	{
		return;
	}

	AString Lines[4];
	for (int i = 0; i < 4; i++)
	{
		HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Line);
		Lines[i] = Line;
	}

	m_Client->HandleUpdateSign(BlockX, BlockY, BlockZ, Lines[0], Lines[1], Lines[2], Lines[3]);
}





void cProtocol_1_9_0::HandlePacketUseEntity(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarInt, UInt32, EntityID);
	HANDLE_READ(a_ByteBuffer, ReadVarInt, UInt32, Type);

	switch (Type)
	{
		case 0:
		{
			HANDLE_READ(a_ByteBuffer, ReadVarInt, UInt32, Hand);
			if (Hand == MAIN_HAND)  // TODO: implement handling of off-hand actions; ignore them for now to avoid processing actions twice
			{
				m_Client->HandleUseEntity(EntityID, false);
			}
			break;
		}
		case 1:
		{
			m_Client->HandleUseEntity(EntityID, true);
			break;
		}
		case 2:
		{
			HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, TargetX);
			HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, TargetY);
			HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, TargetZ);
			HANDLE_READ(a_ByteBuffer, ReadVarInt, UInt32, Hand);

			// TODO: Do anything
			break;
		}
		default:
		{
			ASSERT(!"Unhandled use entity type!");
			return;
		}
	}
}





void cProtocol_1_9_0::HandlePacketUseItem(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarInt, Int32, Hand);

	m_Client->HandleUseItem(HandIntToEnum(Hand));
}





void cProtocol_1_9_0::HandlePacketVehicleMove(cByteBuffer & a_ByteBuffer)
{
	// This handles updating the vehicles location server side
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, xPos);
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, yPos);
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, zPos);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat,  float,  yaw);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat,  float,  pitch);

	// Get the players vehicle
	cEntity * Vehicle = m_Client->GetPlayer()->GetAttached();

	if (Vehicle)
	{
		Vehicle->SetPosX(xPos);
		Vehicle->SetPosY(yPos);
		Vehicle->SetPosZ(zPos);
		Vehicle->SetYaw(yaw);
		Vehicle->SetPitch(pitch);
	}
}





void cProtocol_1_9_0::HandlePacketWindowClick(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8,  UInt8,  WindowID);
	HANDLE_READ(a_ByteBuffer, ReadBEInt16,  Int16,  SlotNum);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8,  UInt8,  Button);
	HANDLE_READ(a_ByteBuffer, ReadBEUInt16, UInt16, TransactionID);
	HANDLE_READ(a_ByteBuffer, ReadVarInt32,  UInt32,  Mode);
	cItem Item;
	ReadItem(a_ByteBuffer, Item);

	/** The slot number that the client uses to indicate "outside the window". */
	static const Int16 SLOT_NUM_OUTSIDE = -999;

	// Convert Button, Mode, SlotNum and HeldItem into eClickAction:
	eClickAction Action;
	switch ((Mode << 8) | Button)
	{
		case 0x0000: Action = (SlotNum != SLOT_NUM_OUTSIDE) ? caLeftClick  : caLeftClickOutside;  break;
		case 0x0001: Action = (SlotNum != SLOT_NUM_OUTSIDE) ? caRightClick : caRightClickOutside; break;
		case 0x0100: Action = caShiftLeftClick;  break;
		case 0x0101: Action = caShiftRightClick; break;
		case 0x0200: Action = caNumber1;         break;
		case 0x0201: Action = caNumber2;         break;
		case 0x0202: Action = caNumber3;         break;
		case 0x0203: Action = caNumber4;         break;
		case 0x0204: Action = caNumber5;         break;
		case 0x0205: Action = caNumber6;         break;
		case 0x0206: Action = caNumber7;         break;
		case 0x0207: Action = caNumber8;         break;
		case 0x0208: Action = caNumber9;         break;
		case 0x0302: Action = caMiddleClick;     break;
		case 0x0400: Action = (SlotNum == SLOT_NUM_OUTSIDE) ? caLeftClickOutsideHoldNothing  : caDropKey;     break;
		case 0x0401: Action = (SlotNum == SLOT_NUM_OUTSIDE) ? caRightClickOutsideHoldNothing : caCtrlDropKey; break;
		case 0x0500: Action = (SlotNum == SLOT_NUM_OUTSIDE) ? caLeftPaintBegin               : caUnknown;     break;
		case 0x0501: Action = (SlotNum != SLOT_NUM_OUTSIDE) ? caLeftPaintProgress            : caUnknown;     break;
		case 0x0502: Action = (SlotNum == SLOT_NUM_OUTSIDE) ? caLeftPaintEnd                 : caUnknown;     break;
		case 0x0504: Action = (SlotNum == SLOT_NUM_OUTSIDE) ? caRightPaintBegin              : caUnknown;     break;
		case 0x0505: Action = (SlotNum != SLOT_NUM_OUTSIDE) ? caRightPaintProgress           : caUnknown;     break;
		case 0x0506: Action = (SlotNum == SLOT_NUM_OUTSIDE) ? caRightPaintEnd                : caUnknown;     break;
		case 0x0508: Action = (SlotNum == SLOT_NUM_OUTSIDE) ? caMiddlePaintBegin             : caUnknown;     break;
		case 0x0509: Action = (SlotNum != SLOT_NUM_OUTSIDE) ? caMiddlePaintProgress          : caUnknown;     break;
		case 0x050a: Action = (SlotNum == SLOT_NUM_OUTSIDE) ? caMiddlePaintEnd               : caUnknown;     break;
		case 0x0600: Action = caDblClick; break;
		default:
		{
			LOGWARNING("Unhandled window click mode / button combination: %d (0x%x)", (Mode << 8) | Button, (Mode << 8) | Button);
			Action = caUnknown;
			break;
		}
	}

	m_Client->HandleWindowClick(WindowID, SlotNum, Action, Item);
}





void cProtocol_1_9_0::ParseItemMetadata(cItem & a_Item, const AString & a_Metadata)
{
	// Parse into NBT:
	cParsedNBT NBT(a_Metadata.data(), a_Metadata.size());
	if (!NBT.IsValid())
	{
		AString HexDump;
		CreateHexDump(HexDump, a_Metadata.data(), std::max<size_t>(a_Metadata.size(), 1024), 16);
		LOGWARNING("Cannot parse NBT item metadata: %s at (%zu / %zu bytes)\n%s",
			NBT.GetErrorCode().message().c_str(), NBT.GetErrorPos(), a_Metadata.size(), HexDump.c_str()
		);
		return;
	}

	// Load enchantments and custom display names from the NBT data:
	for (int tag = NBT.GetFirstChild(NBT.GetRoot()); tag >= 0; tag = NBT.GetNextSibling(tag))
	{
		AString TagName = NBT.GetName(tag);
		switch (NBT.GetType(tag))
		{
			case TAG_List:
			{
				if ((TagName == "ench") || (TagName == "StoredEnchantments"))  // Enchantments tags
				{
					EnchantmentSerializer::ParseFromNBT(a_Item.m_Enchantments, NBT, tag);
				}
				break;
			}
			case TAG_Compound:
			{
				if (TagName == "display")  // Custom name and lore tag
				{
					for (int displaytag = NBT.GetFirstChild(tag); displaytag >= 0; displaytag = NBT.GetNextSibling(displaytag))
					{
						if ((NBT.GetType(displaytag) == TAG_String) && (NBT.GetName(displaytag) == "Name"))  // Custon name tag
						{
							a_Item.m_CustomName = NBT.GetString(displaytag);
						}
						else if ((NBT.GetType(displaytag) == TAG_List) && (NBT.GetName(displaytag) == "Lore"))  // Lore tag
						{
							a_Item.m_LoreTable.clear();
							for (int loretag = NBT.GetFirstChild(displaytag); loretag >= 0; loretag = NBT.GetNextSibling(loretag))  // Loop through array of strings
							{
								a_Item.m_LoreTable.push_back(NBT.GetString(loretag));
							}
						}
						else if ((NBT.GetType(displaytag) == TAG_Int) && (NBT.GetName(displaytag) == "color"))
						{
							a_Item.m_ItemColor.m_Color = static_cast<unsigned int>(NBT.GetInt(displaytag));
						}
					}
				}
				else if ((TagName == "Fireworks") || (TagName == "Explosion"))
				{
					cFireworkItem::ParseFromNBT(a_Item.m_FireworkItem, NBT, tag, static_cast<ENUM_ITEM_TYPE>(a_Item.m_ItemType));
				}
				else if (TagName == "EntityTag")
				{
					for (int entitytag = NBT.GetFirstChild(tag); entitytag >= 0; entitytag = NBT.GetNextSibling(entitytag))
					{
						if ((NBT.GetType(entitytag) == TAG_String) && (NBT.GetName(entitytag) == "id"))
						{
							AString NBTName = NBT.GetString(entitytag);
							ReplaceString(NBTName, "minecraft:", "");
							eMonsterType MonsterType = cMonster::StringToMobType(NBTName);
							a_Item.m_ItemDamage = static_cast<short>(GetProtocolMobType(MonsterType));

						}
					}
				}
				break;
			}
			case TAG_Int:
			{
				if (TagName == "RepairCost")
				{
					a_Item.m_RepairCost = NBT.GetInt(tag);
				}
				break;
			}
			case TAG_String:
			{
				if (TagName == "Potion")
				{
					AString PotionEffect = NBT.GetString(tag);
					if (PotionEffect.find("minecraft:") == AString::npos)
					{
						LOGD("Unknown or missing domain on potion effect name %s!", PotionEffect.c_str());
						continue;
					}

					if (PotionEffect.find("empty") != AString::npos)
					{
						a_Item.m_ItemDamage = 0;
					}
					else if (PotionEffect.find("mundane") != AString::npos)
					{
						a_Item.m_ItemDamage = 64;
					}
					else if (PotionEffect.find("thick") != AString::npos)
					{
						a_Item.m_ItemDamage = 32;
					}
					else if (PotionEffect.find("awkward") != AString::npos)
					{
						a_Item.m_ItemDamage = 16;
					}
					else if (PotionEffect.find("regeneration") != AString::npos)
					{
						a_Item.m_ItemDamage = 1;
					}
					else if (PotionEffect.find("swiftness") != AString::npos)
					{
						a_Item.m_ItemDamage = 2;
					}
					else if (PotionEffect.find("fire_resistance") != AString::npos)
					{
						a_Item.m_ItemDamage = 3;
					}
					else if (PotionEffect.find("poison") != AString::npos)
					{
						a_Item.m_ItemDamage = 4;
					}
					else if (PotionEffect.find("healing") != AString::npos)
					{
						a_Item.m_ItemDamage = 5;
					}
					else if (PotionEffect.find("night_vision") != AString::npos)
					{
						a_Item.m_ItemDamage = 6;
					}
					else if (PotionEffect.find("weakness") != AString::npos)
					{
						a_Item.m_ItemDamage = 8;
					}
					else if (PotionEffect.find("strength") != AString::npos)
					{
						a_Item.m_ItemDamage = 9;
					}
					else if (PotionEffect.find("slowness") != AString::npos)
					{
						a_Item.m_ItemDamage = 10;
					}
					else if (PotionEffect.find("leaping") != AString::npos)
					{
						a_Item.m_ItemDamage = 11;
					}
					else if (PotionEffect.find("harming") != AString::npos)
					{
						a_Item.m_ItemDamage = 12;
					}
					else if (PotionEffect.find("water_breathing") != AString::npos)
					{
						a_Item.m_ItemDamage = 13;
					}
					else if (PotionEffect.find("invisibility") != AString::npos)
					{
						a_Item.m_ItemDamage = 14;
					}
					else if (PotionEffect.find("water") != AString::npos)
					{
						a_Item.m_ItemDamage = 0;
						// Water bottles shouldn't have other bits set on them; exit early.
						continue;
					}
					else
					{
						// Note: luck potions are not handled and will reach this location
						LOGD("Unknown potion type for effect name %s!", PotionEffect.c_str());
						continue;
					}

					if (PotionEffect.find("strong") != AString::npos)
					{
						a_Item.m_ItemDamage |= 0x20;
					}
					if (PotionEffect.find("long") != AString::npos)
					{
						a_Item.m_ItemDamage |= 0x40;
					}

					// Ugly special case with the changed splash potion ID in 1.9
					if ((a_Item.m_ItemType == 438) || (a_Item.m_ItemType == 441))
					{
						// Splash or lingering potions - change the ID to the normal one and mark as splash potions
						a_Item.m_ItemType = E_ITEM_POTION;
						a_Item.m_ItemDamage |= 0x4000;  // Is splash potion
					}
					else
					{
						a_Item.m_ItemDamage |= 0x2000;  // Is drinkable
					}
				}
				break;
			}
			default: LOGD("Unimplemented NBT data when parsing!"); break;
		}
	}
}





eHand cProtocol_1_9_0::HandIntToEnum(Int32 a_Hand)
{
	// Convert hand parameter into eHand enum
	switch (a_Hand)
	{
		case MAIN_HAND: return eHand::hMain;
		case OFF_HAND: return eHand::hOff;
		default:
		{
			ASSERT(!"Unknown hand value");
			return eHand::hMain;
		}
	}
}





void cProtocol_1_9_0::SendEntitySpawn(const cEntity & a_Entity, const UInt8 a_ObjectType, const Int32 a_ObjectData)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktSpawnObject);
	Pkt.WriteVarInt32(a_Entity.GetUniqueID());

	// TODO: Bad way to write a UUID, and it's not a true UUID, but this is functional for now.
	Pkt.WriteBEUInt64(0);
	Pkt.WriteBEUInt64(a_Entity.GetUniqueID());

	Pkt.WriteBEUInt8(a_ObjectType);
	Pkt.WriteBEDouble(a_Entity.GetPosX());
	Pkt.WriteBEDouble(a_Entity.GetPosY());
	Pkt.WriteBEDouble(a_Entity.GetPosZ());
	Pkt.WriteByteAngle(a_Entity.GetPitch());
	Pkt.WriteByteAngle(a_Entity.GetYaw());
	Pkt.WriteBEInt32(a_ObjectData);
	Pkt.WriteBEInt16(static_cast<Int16>(a_Entity.GetSpeedX() * 400));
	Pkt.WriteBEInt16(static_cast<Int16>(a_Entity.GetSpeedY() * 400));
	Pkt.WriteBEInt16(static_cast<Int16>(a_Entity.GetSpeedZ() * 400));
}





void cProtocol_1_9_0::WriteItem(cPacketizer & a_Pkt, const cItem & a_Item)
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

	if ((ItemType == E_ITEM_POTION) && ((a_Item.m_ItemDamage & 0x4000) != 0))
	{
		// Ugly special case for splash potion ids which changed in 1.9; this can be removed when the new 1.9 ids are implemented
		a_Pkt.WriteBEInt16(438);  // minecraft:splash_potion
	}
	else
	{
		// Normal item
		a_Pkt.WriteBEInt16(ItemType);
	}
	a_Pkt.WriteBEInt8(a_Item.m_ItemCount);
	if ((ItemType == E_ITEM_POTION) || (ItemType == E_ITEM_SPAWN_EGG))
	{
		// These items lost their metadata; if it is sent they don't render correctly.
		a_Pkt.WriteBEInt16(0);
	}
	else
	{
		a_Pkt.WriteBEInt16(a_Item.m_ItemDamage);
	}

	if (a_Item.m_Enchantments.IsEmpty() && a_Item.IsBothNameAndLoreEmpty() && (ItemType != E_ITEM_FIREWORK_ROCKET) && (ItemType != E_ITEM_FIREWORK_STAR) && !a_Item.m_ItemColor.IsValid() && (ItemType != E_ITEM_POTION) && (ItemType != E_ITEM_SPAWN_EGG))
	{
		a_Pkt.WriteBEInt8(0);
		return;
	}


	// Send the enchantments and custom names:
	cFastNBTWriter Writer;
	if (a_Item.m_RepairCost != 0)
	{
		Writer.AddInt("RepairCost", a_Item.m_RepairCost);
	}
	if (!a_Item.m_Enchantments.IsEmpty())
	{
		const char * TagName = (a_Item.m_ItemType == E_ITEM_BOOK) ? "StoredEnchantments" : "ench";
		EnchantmentSerializer::WriteToNBTCompound(a_Item.m_Enchantments, Writer, TagName);
	}
	if (!a_Item.IsBothNameAndLoreEmpty() || a_Item.m_ItemColor.IsValid())
	{
		Writer.BeginCompound("display");
		if (a_Item.m_ItemColor.IsValid())
		{
			Writer.AddInt("color", static_cast<Int32>(a_Item.m_ItemColor.m_Color));
		}

		if (!a_Item.IsCustomNameEmpty())
		{
			Writer.AddString("Name", a_Item.m_CustomName);
		}
		if (!a_Item.IsLoreEmpty())
		{
			Writer.BeginList("Lore", TAG_String);

			for (const auto & Line : a_Item.m_LoreTable)
			{
				Writer.AddString("", Line);
			}

			Writer.EndList();
		}
		Writer.EndCompound();
	}
	if ((a_Item.m_ItemType == E_ITEM_FIREWORK_ROCKET) || (a_Item.m_ItemType == E_ITEM_FIREWORK_STAR))
	{
		cFireworkItem::WriteToNBTCompound(a_Item.m_FireworkItem, Writer, static_cast<ENUM_ITEM_TYPE>(a_Item.m_ItemType));
	}
	if (a_Item.m_ItemType == E_ITEM_POTION)
	{
		// 1.9 potions use a different format.  In the future (when only 1.9+ is supported) this should be its own class
		AString PotionID = "empty";  // Fallback of "Uncraftable potion" for unhandled cases

		cEntityEffect::eType Type = cEntityEffect::GetPotionEffectType(a_Item.m_ItemDamage);
		if (Type != cEntityEffect::effNoEffect)
		{
			switch (Type)
			{
				case cEntityEffect::effRegeneration: PotionID = "regeneration"; break;
				case cEntityEffect::effSpeed: PotionID = "swiftness"; break;
				case cEntityEffect::effFireResistance: PotionID = "fire_resistance"; break;
				case cEntityEffect::effPoison: PotionID = "poison"; break;
				case cEntityEffect::effInstantHealth: PotionID = "healing"; break;
				case cEntityEffect::effNightVision: PotionID = "night_vision"; break;
				case cEntityEffect::effWeakness: PotionID = "weakness"; break;
				case cEntityEffect::effStrength: PotionID = "strength"; break;
				case cEntityEffect::effSlowness: PotionID = "slowness"; break;
				case cEntityEffect::effJumpBoost: PotionID = "leaping"; break;
				case cEntityEffect::effInstantDamage: PotionID = "harming"; break;
				case cEntityEffect::effWaterBreathing: PotionID = "water_breathing"; break;
				case cEntityEffect::effInvisibility: PotionID = "invisibility"; break;
				default: ASSERT(!"Unknown potion effect"); break;
			}
			if (cEntityEffect::GetPotionEffectIntensity(a_Item.m_ItemDamage) == 1)
			{
				PotionID = "strong_" + PotionID;
			}
			else if (a_Item.m_ItemDamage & 0x40)
			{
				// Extended potion bit
				PotionID = "long_" + PotionID;
			}
		}
		else
		{
			// Empty potions: Water bottles and other base ones
			if (a_Item.m_ItemDamage == 0)
			{
				// No other bits set; thus it's a water bottle
				PotionID = "water";
			}
			else
			{
				switch (a_Item.m_ItemDamage & 0x3f)
				{
					case 0x00: PotionID = "mundane"; break;
					case 0x10: PotionID = "awkward"; break;
					case 0x20: PotionID = "thick"; break;
				}
				// Default cases will use "empty" from before.
			}
		}

		PotionID = "minecraft:" + PotionID;

		Writer.AddString("Potion", PotionID);
	}
	if (a_Item.m_ItemType == E_ITEM_SPAWN_EGG)
	{
		// Convert entity ID to the name.
		eMonsterType MonsterType = cItemSpawnEggHandler::ItemDamageToMonsterType(a_Item.m_ItemDamage);
		if (MonsterType != eMonsterType::mtInvalidType)
		{
			Writer.BeginCompound("EntityTag");
			Writer.AddString("id", "minecraft:" + cMonster::MobTypeToVanillaNBT(MonsterType));
			Writer.EndCompound();
		}
	}

	Writer.Finish();

	AString Result = Writer.GetResult();
	if (Result.size() == 0)
	{
		a_Pkt.WriteBEInt8(0);
		return;
	}
	a_Pkt.WriteBuf(Result.data(), Result.size());
}





void cProtocol_1_9_0::WriteBlockEntity(cPacketizer & a_Pkt, const cBlockEntity & a_BlockEntity)
{
	cFastNBTWriter Writer;

	switch (a_BlockEntity.GetBlockType())
	{
		case E_BLOCK_BEACON:
		{
			auto & BeaconEntity = static_cast<const cBeaconEntity &>(a_BlockEntity);
			Writer.AddInt("x",         BeaconEntity.GetPosX());
			Writer.AddInt("y",         BeaconEntity.GetPosY());
			Writer.AddInt("z",         BeaconEntity.GetPosZ());
			Writer.AddInt("Primary",   BeaconEntity.GetPrimaryEffect());
			Writer.AddInt("Secondary", BeaconEntity.GetSecondaryEffect());
			Writer.AddInt("Levels",    BeaconEntity.GetBeaconLevel());
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





void cProtocol_1_9_0::WriteEntityMetadata(cPacketizer & a_Pkt, const cEntity & a_Entity)
{
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
	a_Pkt.WriteBEUInt8(0);  // Index 0
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
			a_Pkt.WriteBEUInt8(2);  // Index 2: Custom name
			a_Pkt.WriteBEUInt8(METADATA_TYPE_STRING);
			a_Pkt.WriteString(Player.GetName());

			a_Pkt.WriteBEUInt8(6);  // Index 6: Health
			a_Pkt.WriteBEUInt8(METADATA_TYPE_FLOAT);
			a_Pkt.WriteBEFloat(static_cast<float>(Player.GetHealth()));

			a_Pkt.WriteBEUInt8(12);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BYTE);
			a_Pkt.WriteBEUInt8(static_cast<UInt8>(Player.GetSkinParts()));

			a_Pkt.WriteBEUInt8(13);
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BYTE);
			a_Pkt.WriteBEUInt8(static_cast<UInt8>(Player.GetMainHand()));
			break;
		}
		case cEntity::etPickup:
		{
			a_Pkt.WriteBEUInt8(5);  // Index 5: Item
			a_Pkt.WriteBEUInt8(METADATA_TYPE_ITEM);
			WriteItem(a_Pkt, static_cast<const cPickup &>(a_Entity).GetItem());
			break;
		}
		case cEntity::etMinecart:
		{
			a_Pkt.WriteBEUInt8(5);  // Index 5: Shaking power
			a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);

			// The following expression makes Minecarts shake more with less health or higher damage taken
			auto & Minecart = static_cast<const cMinecart &>(a_Entity);
			auto maxHealth = a_Entity.GetMaxHealth();
			auto curHealth = a_Entity.GetHealth();
			a_Pkt.WriteVarInt32(static_cast<UInt32>((maxHealth - curHealth) * Minecart.LastDamage() * 4));

			a_Pkt.WriteBEUInt8(6);  // Index 6: Shaking direction (doesn't seem to effect anything)
			a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(1);

			a_Pkt.WriteBEUInt8(7);  // Index 7: Shake multiplier / damage taken
			a_Pkt.WriteBEUInt8(METADATA_TYPE_FLOAT);
			a_Pkt.WriteBEFloat(static_cast<float>(Minecart.LastDamage() + 10));

			if (Minecart.GetPayload() == cMinecart::mpNone)
			{
				auto & RideableMinecart = static_cast<const cRideableMinecart &>(Minecart);
				const cItem & MinecartContent = RideableMinecart.GetContent();
				if (!MinecartContent.IsEmpty())
				{
					a_Pkt.WriteBEUInt8(8);  // Index 8: Block ID and damage
					a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
					int Content = MinecartContent.m_ItemType;
					Content |= MinecartContent.m_ItemDamage << 8;
					a_Pkt.WriteVarInt32(static_cast<UInt32>(Content));

					a_Pkt.WriteBEUInt8(9);  // Index 9: Block ID and damage
					a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
					a_Pkt.WriteVarInt32(static_cast<UInt32>(RideableMinecart.GetBlockHeight()));

					a_Pkt.WriteBEUInt8(10);  // Index 10: Show block
					a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
					a_Pkt.WriteBool(true);
				}
			}
			else if (Minecart.GetPayload() == cMinecart::mpFurnace)
			{
				a_Pkt.WriteBEUInt8(11);  // Index 11: Is powered
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
					a_Pkt.WriteBEUInt8(5);  // Index 5: Is critical
					a_Pkt.WriteBEUInt8(METADATA_TYPE_BYTE);
					a_Pkt.WriteBEInt8(static_cast<const cArrowEntity &>(Projectile).IsCritical() ? 1 : 0);
					break;
				}
				case cProjectileEntity::pkFirework:
				{
					a_Pkt.WriteBEUInt8(5);  // Index 5: Firework item used for this firework
					a_Pkt.WriteBEUInt8(METADATA_TYPE_ITEM);
					WriteItem(a_Pkt, static_cast<const cFireworkEntity &>(Projectile).GetItem());
					break;
				}
				case cProjectileEntity::pkSplashPotion:
				{
					a_Pkt.WriteBEUInt8(5);  // Index 5: Potion item which was thrown
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

			a_Pkt.WriteBEInt8(5);  // Index 6: Time since last hit
			a_Pkt.WriteBEInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Boat.GetLastDamage()));

			a_Pkt.WriteBEInt8(6);  // Index 7: Forward direction
			a_Pkt.WriteBEInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Boat.GetForwardDirection()));

			a_Pkt.WriteBEInt8(7);  // Index 8: Damage taken
			a_Pkt.WriteBEInt8(METADATA_TYPE_FLOAT);
			a_Pkt.WriteBEFloat(Boat.GetDamageTaken());

			a_Pkt.WriteBEInt8(8);  // Index 9: Type
			a_Pkt.WriteBEInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Boat.GetMaterial()));

			a_Pkt.WriteBEInt8(9);  // Index 10: Right paddle turning
			a_Pkt.WriteBEInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Boat.IsRightPaddleUsed());

			a_Pkt.WriteBEInt8(10);  // Index 11: Left paddle turning
			a_Pkt.WriteBEInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Boat.IsLeftPaddleUsed());

			break;
		}  // case etBoat

		case cEntity::etItemFrame:
		{
			auto & Frame = static_cast<const cItemFrame &>(a_Entity);
			a_Pkt.WriteBEUInt8(5);  // Index 5: Item
			a_Pkt.WriteBEUInt8(METADATA_TYPE_ITEM);
			WriteItem(a_Pkt, Frame.GetItem());
			a_Pkt.WriteBEUInt8(6);  // Index 6: Rotation
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





void cProtocol_1_9_0::WriteMobMetadata(cPacketizer & a_Pkt, const cMonster & a_Mob)
{
	// Living Enitiy Metadata
	if (a_Mob.HasCustomName())
	{
		// TODO: As of 1.9 _all_ entities can have custom names; should this be moved up?
		a_Pkt.WriteBEUInt8(2);  // Index 2: Custom name
		a_Pkt.WriteBEUInt8(METADATA_TYPE_STRING);
		a_Pkt.WriteString(a_Mob.GetCustomName());

		a_Pkt.WriteBEUInt8(3);  // Index 3: Custom name always visible
		a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
		a_Pkt.WriteBool(a_Mob.IsCustomNameAlwaysVisible());
	}

	a_Pkt.WriteBEUInt8(6);  // Index 6: Health
	a_Pkt.WriteBEUInt8(METADATA_TYPE_FLOAT);
	a_Pkt.WriteBEFloat(static_cast<float>(a_Mob.GetHealth()));

	switch (a_Mob.GetMobType())
	{
		case mtBat:
		{
			auto & Bat = static_cast<const cBat &>(a_Mob);
			a_Pkt.WriteBEUInt8(11);  // Index 11: Bat flags - currently only hanging
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BYTE);
			a_Pkt.WriteBEInt8(Bat.IsHanging() ? 1 : 0);
			break;
		}  // case mtBat

		case mtChicken:
		{
			auto & Chicken = static_cast<const cChicken &>(a_Mob);

			a_Pkt.WriteBEUInt8(11);  // Index 11: Is baby
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Chicken.IsBaby());
			break;
		}  // case mtChicken

		case mtCow:
		{
			auto & Cow = static_cast<const cCow &>(a_Mob);

			a_Pkt.WriteBEUInt8(11);  // Index 11: Is baby
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Cow.IsBaby());
			break;
		}  // case mtCow

		case mtCreeper:
		{
			auto & Creeper = static_cast<const cCreeper &>(a_Mob);
			a_Pkt.WriteBEUInt8(11);  // Index 11: State (idle or "blowing")
			a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(Creeper.IsBlowing() ? 1 : 0xffffffff);

			a_Pkt.WriteBEUInt8(12);  // Index 12: Is charged
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Creeper.IsCharged());

			a_Pkt.WriteBEUInt8(13);  // Index 13: Is ignited
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Creeper.IsBurnedWithFlintAndSteel());
			break;
		}  // case mtCreeper

		case mtEnderman:
		{
			auto & Enderman = static_cast<const cEnderman &>(a_Mob);
			a_Pkt.WriteBEUInt8(11);  // Index 11: Carried block
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BLOCKID);
			UInt32 Carried = 0;
			Carried |= static_cast<UInt32>(Enderman.GetCarriedBlock() << 4);
			Carried |= Enderman.GetCarriedMeta();
			a_Pkt.WriteVarInt32(Carried);

			a_Pkt.WriteBEUInt8(12);  // Index 12: Is screaming
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Enderman.IsScreaming());
			break;
		}  // case mtEnderman

		case mtGhast:
		{
			auto & Ghast = static_cast<const cGhast &>(a_Mob);
			a_Pkt.WriteBEUInt8(11);  // Is attacking
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
			a_Pkt.WriteBEUInt8(12);  // Index 12: flags
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BYTE);
			a_Pkt.WriteBEInt8(Flags);

			a_Pkt.WriteBEUInt8(13);  // Index 13: Variant / type
			a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Horse.GetHorseType()));

			a_Pkt.WriteBEUInt8(14);  // Index 14: Color / style
			a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
			int Appearance = 0;
			Appearance = Horse.GetHorseColor();
			Appearance |= Horse.GetHorseStyle() << 8;
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Appearance));

			a_Pkt.WriteBEUInt8(16);  // Index 16: Armor
			a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Horse.GetHorseArmour()));

			a_Pkt.WriteBEUInt8(11);  // Index 11: Is baby
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Horse.IsBaby());
			break;
		}  // case mtHorse

		case mtMagmaCube:
		{
			auto & MagmaCube = static_cast<const cMagmaCube &>(a_Mob);
			a_Pkt.WriteBEUInt8(11);  // Index 11: Size
			a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(MagmaCube.GetSize()));
			break;
		}  // case mtMagmaCube

		case mtOcelot:
		{
			auto & Ocelot = static_cast<const cOcelot &>(a_Mob);

			a_Pkt.WriteBEUInt8(11);  // Index 11: Is baby
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Ocelot.IsBaby());
			break;
		}  // case mtOcelot

		case mtPig:
		{
			auto & Pig = static_cast<const cPig &>(a_Mob);

			a_Pkt.WriteBEUInt8(11);  // Index 11: Is baby
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Pig.IsBaby());

			a_Pkt.WriteBEUInt8(12);  // Index 12: Is saddled
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Pig.IsSaddled());

			break;
		}  // case mtPig

		case mtRabbit:
		{
			auto & Rabbit = static_cast<const cRabbit &>(a_Mob);
			a_Pkt.WriteBEUInt8(11);  // Index 11: Is baby
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Rabbit.IsBaby());

			a_Pkt.WriteBEUInt8(12);  // Index 12: Type
			a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Rabbit.GetRabbitType()));
			break;
		}  // case mtRabbit

		case mtSheep:
		{
			auto & Sheep = static_cast<const cSheep &>(a_Mob);

			a_Pkt.WriteBEUInt8(11);  // Index 11: Is baby
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Sheep.IsBaby());

			a_Pkt.WriteBEUInt8(12);  // Index 12: sheared, color
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
			a_Pkt.WriteBEUInt8(11);  // Index 11: Size
			a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Slime.GetSize()));
			break;
		}  // case mtSlime

		case mtVillager:
		{
			auto & Villager = static_cast<const cVillager &>(a_Mob);
			a_Pkt.WriteBEUInt8(11);  // Index 11: Is baby
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Villager.IsBaby());

			a_Pkt.WriteBEUInt8(12);  // Index 12: Type
			a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Villager.GetVilType()));
			break;
		}  // case mtVillager

		case mtWitch:
		{
			auto & Witch = static_cast<const cWitch &>(a_Mob);
			a_Pkt.WriteBEUInt8(11);  // Index 11: Is angry
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Witch.IsAngry());
			break;
		}  // case mtWitch

		case mtWither:
		{
			auto & Wither = static_cast<const cWither &>(a_Mob);
			a_Pkt.WriteBEUInt8(14);  // Index 14: Invulnerable ticks
			a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(Wither.GetWitherInvulnerableTicks());

			// TODO: Use boss bar packet for health
			break;
		}  // case mtWither

		case mtWitherSkeleton:
		{
			a_Pkt.WriteBEUInt8(11);  // Index 11: Type
			a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(1);  // Is wither skeleton
			break;
		}  // case mtWitherSkeleton

		case mtWolf:
		{
			auto & Wolf = static_cast<const cWolf &>(a_Mob);
			a_Pkt.WriteBEUInt8(11);  // Index 11: Is baby
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
			a_Pkt.WriteBEUInt8(12);  // Index 12: status
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BYTE);
			a_Pkt.WriteBEInt8(WolfStatus);

			a_Pkt.WriteBEUInt8(14);  // Index 14: Health
			a_Pkt.WriteBEUInt8(METADATA_TYPE_FLOAT);
			a_Pkt.WriteBEFloat(static_cast<float>(a_Mob.GetHealth()));

			a_Pkt.WriteBEUInt8(15);  // Index 15: Is begging
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Wolf.IsBegging());

			a_Pkt.WriteBEUInt8(16);  // Index 16: Collar color
			a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Wolf.GetCollarColor()));
			break;
		}  // case mtWolf

		case mtZombie:
		{
			auto & Zombie = static_cast<const cZombie &>(a_Mob);
			a_Pkt.WriteBEUInt8(11);  // Index 11: Is baby
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(Zombie.IsBaby());

			a_Pkt.WriteBEUInt8(12);  // Index 12: Is a villager
			a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(0);

			a_Pkt.WriteBEUInt8(13);  // Index 13: Is converting
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(false);
			break;
		}  // case mtZombie

		case mtZombiePigman:
		{
			auto & ZombiePigman = static_cast<const cZombiePigman &>(a_Mob);
			a_Pkt.WriteBEUInt8(11);  // Index 11: Is baby
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(ZombiePigman.IsBaby());
			break;
		}  // case mtZombiePigman

		case mtZombieVillager:
		{
			auto & ZombieVillager = reinterpret_cast<const cZombieVillager &>(a_Mob);
			a_Pkt.WriteBEUInt8(11);  // Index 11: Is baby
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(ZombieVillager.IsBaby());

			a_Pkt.WriteBEUInt8(12);  // Index 12: Is a villager
			a_Pkt.WriteBEUInt8(METADATA_TYPE_VARINT);
			a_Pkt.WriteVarInt32(ZombieVillager.GetProfession());

			a_Pkt.WriteBEUInt8(13);  // Index 13: Is converting
			a_Pkt.WriteBEUInt8(METADATA_TYPE_BOOL);
			a_Pkt.WriteBool(ZombieVillager.ConversionTime() != -1);
			break;
		}  // case mtZombieVillager

		default: break;
	}  // switch (a_Mob.GetType())
}





void cProtocol_1_9_0::WriteEntityProperties(cPacketizer & a_Pkt, const cEntity & a_Entity)
{
	if (!a_Entity.IsMob())
	{
		// No properties for anything else than mobs
		a_Pkt.WriteBEInt32(0);
		return;
	}

	// const cMonster & Mob = (const cMonster &)a_Entity;

	// TODO: Send properties and modifiers based on the mob type

	a_Pkt.WriteBEInt32(0);  // NumProperties
}





////////////////////////////////////////////////////////////////////////////////
// cProtocol_1_9_1:

void cProtocol_1_9_1::SendLogin(const cPlayer & a_Player, const cWorld & a_World)
{
	// Send the Join Game packet:
	{
		cServer * Server = cRoot::Get()->GetServer();
		cPacketizer Pkt(*this, pktJoinGame);
		Pkt.WriteBEUInt32(a_Player.GetUniqueID());
		Pkt.WriteBEUInt8(static_cast<UInt8>(a_Player.GetEffectiveGameMode()) | (Server->IsHardcore() ? 0x08 : 0));  // Hardcore flag bit 4
		Pkt.WriteBEInt32(static_cast<Int32>(a_World.GetDimension()));  // This is the change from 1.9.0 (Int8 to Int32)
		Pkt.WriteBEUInt8(2);  // TODO: Difficulty (set to Normal)
		Pkt.WriteBEUInt8(static_cast<UInt8>(Clamp<size_t>(Server->GetMaxPlayers(), 0, 255)));
		Pkt.WriteString("default");  // Level type - wtf?
		Pkt.WriteBool(false);  // Reduced Debug Info - wtf?
	}

	// Send the spawn position:
	{
		cPacketizer Pkt(*this, pktSpawnPosition);
		Pkt.WriteXYZPosition64(FloorC(a_World.GetSpawnX()), FloorC(a_World.GetSpawnY()), FloorC(a_World.GetSpawnZ()));
	}

	// Send the server difficulty:
	{
		cPacketizer Pkt(*this, pktDifficulty);
		Pkt.WriteBEInt8(1);
	}

	// Send player abilities:
	SendPlayerAbilities();
}





cProtocol::Version cProtocol_1_9_1::GetProtocolVersion()
{
	return Version::v1_9_1;
}





////////////////////////////////////////////////////////////////////////////////
// cProtocol_1_9_2:

cProtocol::Version cProtocol_1_9_2::GetProtocolVersion()
{
	return Version::v1_9_2;
}





////////////////////////////////////////////////////////////////////////////////
// cProtocol_1_9_4:

void cProtocol_1_9_4::SendUpdateSign(int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4)
{
	ASSERT(m_State == 3);  // In game mode?

	// 1.9.4 removed the update sign packet and now uses Update Block Entity
	cPacketizer Pkt(*this, pktUpdateBlockEntity);
	Pkt.WriteXYZPosition64(a_BlockX, a_BlockY, a_BlockZ);
	Pkt.WriteBEUInt8(9);  // Action 9 - update sign

	cFastNBTWriter Writer;
	Writer.AddInt("x",        a_BlockX);
	Writer.AddInt("y",        a_BlockY);
	Writer.AddInt("z",        a_BlockZ);
	Writer.AddString("id", "Sign");

	Json::Value Line1;
	Line1["text"] = a_Line1;
	Writer.AddString("Text1", JsonUtils::WriteFastString(Line1));
	Json::Value Line2;
	Line2["text"] = a_Line2;
	Writer.AddString("Text2", JsonUtils::WriteFastString(Line2));
	Json::Value Line3;
	Line3["text"] = a_Line3;
	Writer.AddString("Text3", JsonUtils::WriteFastString(Line3));
	Json::Value Line4;
	Line4["text"] = a_Line4;
	Writer.AddString("Text4", JsonUtils::WriteFastString(Line4));

	Writer.Finish();
	Pkt.WriteBuf(Writer.GetResult().data(), Writer.GetResult().size());
}





cProtocol::Version cProtocol_1_9_4::GetProtocolVersion()
{
	return Version::v1_9_4;
}





UInt32 cProtocol_1_9_4::GetPacketID(cProtocol::ePacketType a_Packet)
{
	switch (a_Packet)
	{
		case pktCollectEntity:    return 0x48;
		case pktEntityEffect:     return 0x4b;
		case pktEntityProperties: return 0x4a;
		case pktPlayerMaxSpeed:   return 0x4a;
		case pktTeleportEntity:   return 0x49;

		default: return Super::GetPacketID(a_Packet);
	}
}
