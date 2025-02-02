
// Protocol_1_15.cpp

/*
Implements the 1.15 protocol classes:
		- release 1.15 protocol (#573)
		- release 1.15.1 protocol (#574)
		- release 1.15.2 protocol (#577)
*/

#include "Globals.h"
#include "Protocol_1_15.h"
#include "Packetizer.h"
#include "JsonUtils.h"
#include "../Root.h"
#include "../Server.h"
#include "../World.h"
#include "../UI/HorseWindow.h"
#include "../ClientHandle.h"
#include "../WorldStorage/FastNBT.h"
#include "../BlockEntities/BlockEntity.h"

#include "../Entities/ArrowEntity.h"
#include "../Mobs/Bat.h"
#include "../Entities/Boat.h"
#include "../Mobs/Chicken.h"
#include "../Mobs/Cow.h"
#include "../Mobs/Creeper.h"
#include "../Entities/EnderCrystal.h"
#include "../Mobs/Enderman.h"
#include "../Mobs/Ghast.h"
#include "../Mobs/Horse.h"
#include "../Mobs/MagmaCube.h"
#include "../Entities/Minecart.h"
#include "../Mobs/Ocelot.h"
#include "../Entities/Pickup.h"
#include "../Mobs/Pig.h"
#include "../Entities/Player.h"
#include "../Mobs/Rabbit.h"
#include "../Mobs/Sheep.h"
#include "../Mobs/Skeleton.h"
#include "../Mobs/Slime.h"
#include "../Mobs/Villager.h"
#include "../Mobs/Wolf.h"
#include "../Mobs/Wither.h"
#include "../Mobs/Zombie.h"
#include "../Mobs/ZombiePigman.h"
#include "../Entities/Painting.h"
#include "Palettes/Upgrade.h"

#include "Palettes/Palette_1_15.h"

#include "Protocol.h"



////////////////////////////////////////////////////////////////////////////////
// cProtocol_1_15:

void cProtocol_1_15::SendBlockAction(Vector3i a_BlockPos, char a_Byte1, char a_Byte2, BlockState a_BlockType)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktBlockAction);
	Pkt.WriteXZYPosition64(a_BlockPos);
	Pkt.WriteBEInt8(a_Byte1);
	Pkt.WriteBEInt8(a_Byte2);
	Pkt.WriteVarInt32(GetProtocolBlockType(a_BlockType));
}





void cProtocol_1_15::SendBlockChanges(int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktBlockChanges);
	Pkt.WriteBEInt32(a_ChunkX);
	Pkt.WriteBEInt32(a_ChunkZ);
	Pkt.WriteVarInt32(static_cast<UInt32>(a_Changes.size()));

	for (const auto & Change : a_Changes)
	{
		Int16 Coords = static_cast<Int16>(Change.m_RelY | (Change.m_RelZ << 8) | (Change.m_RelX << 12));
		Pkt.WriteBEInt16(Coords);
		Pkt.WriteVarInt32(GetProtocolBlockType(Change.m_Block));
	}
}





void cProtocol_1_15::SendBlockBreakAnim(UInt32 a_EntityID, Vector3i a_BlockPos, char a_Stage)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktBlockBreakAnim);
	Pkt.WriteVarInt32(a_EntityID);
	Pkt.WriteXZYPosition64(a_BlockPos);
	Pkt.WriteBEInt8(a_Stage);
}





void cProtocol_1_15::SendRenderDistanceCenter(cChunkCoords a_chunk)
{
	cPacketizer Pkt(*this, pktRenderDistanceCenter);
	Pkt.WriteVarInt32(static_cast<UInt32>(a_chunk.m_ChunkX));
	Pkt.WriteVarInt32(static_cast<UInt32>(a_chunk.m_ChunkZ));
}





void cProtocol_1_15::SendEntityAnimation(const cEntity & a_Entity, EntityAnimation a_Animation)
{
	if (a_Animation == EntityAnimation::PlayerEntersBed)
	{
		// Use Bed packet removed, through metadata instead:
		SendEntityMetadata(a_Entity);
		return;
	}

	Super::SendEntityAnimation(a_Entity, a_Animation);
}





void cProtocol_1_15::SendEntitySpawn(const cEntity & a_Entity, const UInt8 a_ObjectType, const Int32 a_ObjectData)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktSpawnObject);
	Pkt.WriteVarInt32(a_Entity.GetUniqueID());

	// TODO: Bad way to write a UUID, and it's not a true UUID, but this is functional for now.
	Pkt.WriteBEUInt64(0);
	Pkt.WriteBEUInt64(a_Entity.GetUniqueID());
	Pkt.WriteVarInt32(a_ObjectType);
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





void cProtocol_1_15::SendLogin(const cPlayer & a_Player, const cWorld & a_World)
{
	// Send the Join Game packet:
	{
		cServer * Server = cRoot::Get()->GetServer();
		cPacketizer Pkt(*this, pktJoinGame);
		Pkt.WriteBEUInt32(a_Player.GetUniqueID());
		Pkt.WriteBEUInt8(static_cast<UInt8>(a_Player.GetEffectiveGameMode()) | (Server->IsHardcore() ? 0x08 : 0));
		Pkt.WriteBEInt32(static_cast<Int32>(a_World.GetDimension()));
		Pkt.WriteBEInt64(0);  // Seed
		Pkt.WriteBEUInt8(static_cast<UInt8>(Clamp<size_t>(Server->GetMaxPlayers(), 0, 255)));
		Pkt.WriteString("default");
		Pkt.WriteVarInt32(ToUnsigned(a_World.GetMaxViewDistance()));
		Pkt.WriteBool(false);
		Pkt.WriteBool(false);  // Death screen
	}

	// Send the spawn position:
	{
		cPacketizer Pkt(*this, pktSpawnPosition);
		Pkt.WriteXZYPosition64(a_World.GetSpawnX(), a_World.GetSpawnY(), a_World.GetSpawnZ());
	}
	// Send the server difficulty:
	{
		cPacketizer Pkt(*this, pktDifficulty);
		Pkt.WriteBEInt8(1);
		Pkt.WriteBool(false);  // Difficulty locked?
	}
	// Send world Border  // temporarily here
	{
		cPacketizer Pkt(*this, pktWorldBorder);
		Pkt.WriteVarInt32(3);
		Pkt.WriteBEDouble(0);  // X
		Pkt.WriteBEDouble(0);  // Z
		Pkt.WriteBEDouble(60000000);
		Pkt.WriteBEDouble(60000000);
		Pkt.WriteVarInt32(0);
		Pkt.WriteVarInt32(29999984);
		Pkt.WriteVarInt32(5);
		Pkt.WriteVarInt32(15);
	}
}





void cProtocol_1_15::SendPlayerActionResponse(Vector3i a_blockpos, int a_state_id, cProtocol::PlayerActionResponses a_action, bool a_IsApproved)
{
	cPacketizer pkt(*this, pktDifficulty);
	pkt.WriteXYZPosition64(a_blockpos.x, a_blockpos.y, a_blockpos.z);
	pkt.WriteVarInt32(static_cast<UInt32>(a_state_id));
	pkt.WriteVarInt32(static_cast<UInt32>(a_action));
	pkt.WriteBool(a_IsApproved);
}





void cProtocol_1_15::SendMapData(const cMap & a_Map, int a_DataStartX, int a_DataStartY)
{
}





void cProtocol_1_15::SendPaintingSpawn(const cPainting & a_Painting)
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
	Pkt.WriteVarInt32(a_Painting.GetPaintingId());
	Pkt.WriteXYZPosition64(static_cast<Int32>(PosX), static_cast<Int32>(PosY), static_cast<Int32>(PosZ));
	Pkt.WriteBEInt8(static_cast<Int8>(a_Painting.GetProtocolFacing()));
}





void cProtocol_1_15::SendParticleEffect(const AString & a_ParticleName, Vector3f a_Src, Vector3f a_Offset, float a_ParticleData, int a_ParticleAmount)
{
	ASSERT(m_State == 3);  // In game mode?

	const auto ParticleID = GetProtocolParticleID(a_ParticleName);

	cPacketizer Pkt(*this, pktParticleEffect);
	Pkt.WriteBEInt32(ParticleID);

	Pkt.WriteBool(false);  // Long Distance
	Pkt.WriteBEDouble(a_Src.x);
	Pkt.WriteBEDouble(a_Src.y);
	Pkt.WriteBEDouble(a_Src.z);

	Pkt.WriteBEFloat(a_Offset.x);
	Pkt.WriteBEFloat(a_Offset.y);
	Pkt.WriteBEFloat(a_Offset.z);

	Pkt.WriteBEFloat(a_ParticleData);
	Pkt.WriteBEInt32(a_ParticleAmount);
}





void cProtocol_1_15::SendParticleEffect(const AString & a_ParticleName, Vector3f a_Src, Vector3f a_Offset, float a_ParticleData, int a_ParticleAmount, std::array<int, 2> a_Data)
{
	ASSERT(m_State == 3);  // In game mode?

	const auto ParticleID = GetProtocolParticleID(a_ParticleName);

	cPacketizer Pkt(*this, pktParticleEffect);
	Pkt.WriteBEInt32(ParticleID);

	Pkt.WriteBool(false);  // Long Distance
	Pkt.WriteBEDouble(a_Src.x);
	Pkt.WriteBEDouble(a_Src.y);
	Pkt.WriteBEDouble(a_Src.z);

	Pkt.WriteBEFloat(a_Offset.x);
	Pkt.WriteBEFloat(a_Offset.y);
	Pkt.WriteBEFloat(a_Offset.z);

	Pkt.WriteBEFloat(a_ParticleData);
	Pkt.WriteBEInt32(a_ParticleAmount);

	switch (ParticleID)
	{
		case 3:  // blockdust
		case 23:  // FALLING_DUST
		{
			Pkt.WriteVarInt32(static_cast<UInt32>(a_Data[0]));
			break;
		}
		case 14:  // DUST
		{
			Pkt.WriteBEDouble(1);  // Red
			Pkt.WriteBEDouble(1);  // Green
			Pkt.WriteBEDouble(1);  // Blue
			Pkt.WriteBEDouble(1);  // Scale

			break;
		}
		case 32:  // ITEM
		{
			cItem senditem;
			senditem.Empty();
			WriteItem(Pkt, senditem);
			break;
		}
	}
}





void cProtocol_1_15::HandlePacketBookUpdate(cByteBuffer & a_ByteBuffer)
{
	cItem book;
	if (!ReadItem(a_ByteBuffer, book, 2))
	{
		UNREACHABLE("Failed to parse book data from bookupdate packet -- item");
	}
	bool signedbook = false;
	if (!a_ByteBuffer.ReadBool(signedbook))
	{
		UNREACHABLE("Failed to parse book data from bookupdate packet -- is signed");
	}
	Int32 enumindex = -1;
	if (!a_ByteBuffer.ReadVarInt(enumindex))
	{
		UNREACHABLE("Failed to parse book data from bookupdate packet -- enum index");
	}
	Hand handused = static_cast<Hand>(enumindex);


	LOG("Book is signed: %d - Hand used: %d", signedbook, handused);
	LOG("PacketBookUpdate is not imlpemented fully");
	return;
}





void cProtocol_1_15::SendRespawn(eDimension a_Dimension)
{
	cPacketizer Pkt(*this, pktRespawn);
	cPlayer * Player = m_Client->GetPlayer();
	Pkt.WriteBEInt32(static_cast<Int32>(a_Dimension));
	Pkt.WriteBEUInt64(0);  // Appears to be a SHA256 od the world seed
	Pkt.WriteBEUInt8(static_cast<Byte>(Player->GetEffectiveGameMode()));
	Pkt.WriteString("default");
}





void cProtocol_1_15::SendSoundParticleEffect(const EffectID a_EffectID, Vector3i a_Origin, int a_Data)
{
	ASSERT(m_State == 3);  // In game mode?

	// Note: Particles from block break missing

	cPacketizer Pkt(*this, pktSoundParticleEffect);
	Pkt.WriteBEInt32(static_cast<int>(a_EffectID));
	Pkt.WriteXZYPosition64(a_Origin);
	Pkt.WriteBEInt32(a_Data);
	Pkt.WriteBool(false);
}





void cProtocol_1_15::SendUpdateBlockEntity(cBlockEntity & a_BlockEntity)
{
	ASSERT(m_State == 3);  // In game mode?

	return;
	/*
	Byte Action;
	switch (a_BlockEntity.GetBlockType())
	{
		case E_BLOCK_CHEST:
		case E_BLOCK_ENCHANTMENT_TABLE:
		case E_BLOCK_END_PORTAL:
		case E_BLOCK_TRAPPED_CHEST:
		{
			// The ones with a action of 0 is just a workaround to send the block entities to a client.
			// Todo: 18.09.2020 - remove this when block entities are transmitted in the ChunkData packet - 12xx12
			Action = 0;
			break;
		}

		case E_BLOCK_MOB_SPAWNER:       Action = 1;  break;  // Update mob spawner spinny mob thing
		case E_BLOCK_COMMAND_BLOCK:     Action = 2;  break;  // Update command block text
		case E_BLOCK_BEACON:            Action = 3;  break;  // Update beacon entity
		case E_BLOCK_HEAD:              Action = 4;  break;  // Update Mobhead entity
		// case E_BLOCK_CONDUIT:        Action = 5;  break;  // Update Conduit entity
		case E_BLOCK_STANDING_BANNER:
		case E_BLOCK_WALL_BANNER:       Action = 6;  break;  // Update banner entity
		// case Structure Block:        Action = 7;  break;  // Update Structure tile entity
		case E_BLOCK_END_GATEWAY:       Action = 8;  break;  // Update destination for a end gateway entity
		case E_BLOCK_SIGN_POST:         Action = 9;  break;  // Update sign entity
		// case E_BLOCK_SHULKER_BOX:    Action = 10; break;  // sets shulker box - not used just here if anyone is confused from reading the protocol wiki
		case E_BLOCK_BED:               Action = 11; break;  // Update bed color
		// case E_BLOCK_JIGSAW:         Action = 12; break;
		// case E_BLOCK_CAMPFIRE:       Action = 13; break;

		default: return;  // Block entities change between versions
	}

	cPacketizer Pkt(*this, pktUpdateBlockEntity);
	Pkt.WriteXZYPosition64(a_BlockEntity.GetPosX(), a_BlockEntity.GetPosY(), a_BlockEntity.GetPosZ());
	Pkt.WriteBEUInt8(Action);

	cFastNBTWriter Writer;
	WriteBlockEntity(Writer, a_BlockEntity);
	Writer.Finish();
	Pkt.WriteBuf(Writer.GetResult());
	*/
}





void cProtocol_1_15::SendUpdateSign(Vector3i a_BlockPos, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4)
{
}





void cProtocol_1_15::SendWindowOpen(const cWindow & a_Window)
{
	ASSERT(m_State == 3);  // In game mode?

	if (a_Window.GetWindowType() < 0)
	{
		// Do not send this packet for player inventory windows
		return;
	}

	if (a_Window.GetWindowType() == cWindow::wtAnimalChest)
	{
		cPacketizer Pkt(*this, pktHorseWindowOpen);
		Pkt.WriteBEUInt8(static_cast<UInt8>(a_Window.GetWindowID()));
		Pkt.WriteVarInt32(static_cast<UInt32>(a_Window.GetNumSlots()));

		UInt32 HorseID = static_cast<const cHorseWindow &>(a_Window).GetHorseID();
		Pkt.WriteBEInt32(static_cast<Int32>(HorseID));
	}
	else
	{
		cPacketizer Pkt(*this, pktWindowOpen);
		Pkt.WriteVarInt32(static_cast<UInt8>(a_Window.GetWindowID()));

		switch (a_Window.GetWindowType())
		{
			case cWindow::wtChest:
			{
				// Chests can have multiple size
				Pkt.WriteVarInt32(static_cast<UInt32>(a_Window.GetNumNonInventorySlots() / 9 - 1));
				break;
			}
			case cWindow::wtDropper:
			case cWindow::wtDropSpenser:
			{
				Pkt.WriteVarInt32(6);
				break;
			}
			case cWindow::wtAnvil:
			{
				Pkt.WriteVarInt32(7);
				break;
			}
			case cWindow::wtBeacon:
			{
				Pkt.WriteVarInt32(8);
				break;
			}
			case cWindow::wtBrewery:
			{
				Pkt.WriteVarInt32(10);
				break;
			}
			case cWindow::wtWorkbench:
			{
				Pkt.WriteVarInt32(11);
				break;
			}
			case cWindow::wtEnchantment:
			{
				Pkt.WriteVarInt32(12);
				break;
			}
			case cWindow::wtFurnace:
			{
				Pkt.WriteVarInt32(13);
				break;
			}
			/*
			case cWindow::wtGrindstone:
			{
				Pkt.WriteVarInt32(14);
				break;
			}
			*/
			case cWindow::wtHopper:
			{
				Pkt.WriteVarInt32(15);
				break;
			}
			/*
			case cWindow::wtLectern:
			{
				Pkt.WriteVarInt32(16);
				break;
			}
			case cWindow::wtLoom:
			{
				Pkt.WriteVarInt32(17);
				break;
			}
			*/
			case cWindow::wtNPCTrade:
			{
				Pkt.WriteVarInt32(18);
				break;
			}
			/*
			case cWindow::wtShulker:
			{
				Pkt.WriteVarInt32(19);
				break;
			}
			case cWindow::wtSmoker:
			{
				Pkt.WriteVarInt32(20);
				break;
			}
			case cWindow::wtCartography:
			{
				Pkt.WriteVarInt32(21);
				break;
			}
			case cWindow::wtStonecutter:
			{
				Pkt.WriteVarInt32(22);
				break;
			}
			*/
			default:
			{
				Pkt.WriteBEUInt8(static_cast<UInt8>(a_Window.GetNumNonInventorySlots()));
				break;
			}
		}

		Pkt.WriteString(JsonUtils::SerializeSingleValueJsonObject("text", a_Window.GetWindowTitle()));
	}
}





UInt32 cProtocol_1_15::GetPacketID(ePacketType a_PacketType) const
{
	switch (a_PacketType)
	{
		case cProtocol::pktBossBar:			     return 0x0D;
		case cProtocol::pktBlockAction:			 return 0x0B;
		case cProtocol::pktCommnadTree:          return 0x12;
		case cProtocol::pktCustomPayload:        return 0x19;
		case cProtocol::pktEntityLook:		     return 0x2B;
		case cProtocol::pktEntityRelMoveLook:	 return 0x2A;
		case cProtocol::pktUpdateBlockEntity:	 return 0x0A;
		case cProtocol::pktBlockChange:			 return 0x0C;
		case cProtocol::pktBlockChanges:		 return 0x10;
		case cProtocol::pktWorldBorder:          return 0x3E;
		case cProtocol::pktEntityRelMove:		 return 0x29;
		case cProtocol::pktEntityStatus:		 return 0x1C;
		case cProtocol::pktJoinGame:             return 0x26;
		case cProtocol::pktDifficulty:           return 0x0E;
		case cProtocol::pktChatRaw:				 return 0x0F;
		case cProtocol::pktAttachEntity:         return 0x4B;
		case cProtocol::pktCameraSetTo:          return 0x3E;
		case cProtocol::pktCollectEntity:        return 0x56;
		case cProtocol::pktDestroyEntity:        return 0x38;
		case cProtocol::pktDisconnectDuringGame: return 0x1B;
		case cProtocol::pktEntityEffect:         return 0x5A;
		case cProtocol::pktEntityEquipment:      return 0x47;
		case cProtocol::pktEntityHeadLook:       return 0x3C;
		case cProtocol::pktEntityMeta:           return 0x44;
		case cProtocol::pktEntityProperties:     return 0x59;
		case cProtocol::pktEntityVelocity:       return 0x46;
		case cProtocol::pktExperience:           return 0x48;
		case cProtocol::pktExplosion:            return 0x1D;
		case cProtocol::pktGameMode:             return 0x1F;
		case cProtocol::pktHeldItemChange:       return 0x40;
		case cProtocol::pktHorseWindowOpen:      return 0x20;
		case cProtocol::pktInventorySlot:        return 0x17;
		case cProtocol::pktKeepAlive:            return 0x21;
		case cProtocol::pktLightUpdate:          return 0x25;
		case cProtocol::pktParticleEffect:       return 0x24;
		case cProtocol::pktPlayerAbilities:      return 0x32;
		case cProtocol::pktPlayerList:           return 0x34;
		case cProtocol::pktPlayerMoveLook:       return 0x36;
		case cProtocol::pktPluginMessage:        return 0x19;
		case cProtocol::pktRenderDistanceCenter: return 0x41;
		case cProtocol::pktRemoveEntityEffect:   return 0x39;
		case cProtocol::pktResourcePack:         return 0x3A;
		case cProtocol::pktRespawn:              return 0x3B;
		case cProtocol::pktScoreboardObjective:  return 0x4A;
		case cProtocol::pktSoundEffect:          return 0x1A;
		case cProtocol::pktSoundParticleEffect:  return 0x23;
		case cProtocol::pktSpawnPosition:        return 0x4E;
		case cProtocol::pktTeleportEntity:       return 0x57;
		case cProtocol::pktTimeUpdate:           return 0x4F;
		case cProtocol::pktTitle:                return 0x50;
		case cProtocol::pktUnloadChunk:          return 0x1E;
		case cProtocol::pktUnlockRecipe:         return 0x37;
		case cProtocol::pktUpdateHealth:         return 0x49;
		case cProtocol::pktUpdateScore:          return 0x4C;
		case cProtocol::pktUpdateSign:           return 0x30;
		case cProtocol::pktWeather:              return 0x1F;
		case cProtocol::pktWindowItems:          return 0x15;
		case cProtocol::pktWindowOpen:           return 0x2F;
		case cProtocol::pktWindowClose:			 return 0x14;
		case cProtocol::pktWindowProperty:       return 0x16;
		default: return Super::GetPacketID(a_PacketType);
	}
}





void cProtocol_1_15::SendPlayerSpawn(const cPlayer & a_Player)
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
	// WriteEntityMetadata(Pkt, a_Player);
	// Pkt.WriteBEUInt8(0xff);  // Metadata: end
}





void cProtocol_1_15::SendSoundEffect(
	const AString & a_SoundName, Vector3d a_Origin, float a_Volume,
	float a_Pitch)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktSoundEffect);
	Pkt.WriteString(a_SoundName);
	Pkt.WriteVarInt32(0);  // Master sound category (may want to be changed to a parameter later)
	Pkt.WriteBEInt32(static_cast<Int32>(a_Origin.x * 8.0));
	Pkt.WriteBEInt32(static_cast<Int32>(a_Origin.y * 8.0));
	Pkt.WriteBEInt32(static_cast<Int32>(a_Origin.z * 8.0));
	Pkt.WriteBEFloat(a_Volume);
	Pkt.WriteBEFloat(a_Pitch);
}





void cProtocol_1_15::SendEntityMetadata(const cEntity & a_Entity)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktEntityMeta);
	Pkt.WriteVarInt32(a_Entity.GetUniqueID());
	WriteEntityMetadata(Pkt, a_Entity);
	Pkt.WriteBEUInt8(0xFF);	 // The termination byte
}





void cProtocol_1_15::SendSpawnMob(const cMonster & a_Mob)
{
	ASSERT(m_State == 3);  // In game mode?

	const auto MobType = GetProtocolMobType(a_Mob.GetMobType());

	// If the type is not valid in this protocol bail out:
	if (MobType == 0)
	{
		return;
	}

	cPacketizer Pkt(*this, pktSpawnMob);
	Pkt.WriteVarInt32(a_Mob.GetUniqueID());
	Pkt.WriteBEInt64(0);
	Pkt.WriteBEInt64(a_Mob.GetUniqueID());
	Pkt.WriteVarInt32(static_cast<Byte>(MobType));
	Vector3d LastSentPos = a_Mob.GetLastSentPosition();
	Pkt.WriteBEDouble(LastSentPos.x);
	Pkt.WriteBEDouble(LastSentPos.y);
	Pkt.WriteBEDouble(LastSentPos.z);
	Pkt.WriteByteAngle(a_Mob.GetHeadYaw());	 // Doesn't seem to be used
	Pkt.WriteByteAngle(a_Mob.GetPitch());
	Pkt.WriteByteAngle(a_Mob.GetYaw());
	Pkt.WriteBEInt16(static_cast<Int16>(a_Mob.GetSpeedX() * 400));
	Pkt.WriteBEInt16(static_cast<Int16>(a_Mob.GetSpeedY() * 400));
	Pkt.WriteBEInt16(static_cast<Int16>(a_Mob.GetSpeedZ() * 400));
	// WriteEntityMetadata(Pkt, a_Mob);
	// Pkt.WriteBEUInt8(0xFF);	 // Metadata terminator
}





UInt8 cProtocol_1_15::GetEntityMetadataID(EntityMetadata a_Metadata) const
{
	const UInt8 Entity = 7;
	const UInt8 Living = Entity + 7;
	const UInt8 Insentient = Living + 1;
	const UInt8 Ageable = Insentient + 1;
	const UInt8 AbstractHorse = Ageable + 2;
	const UInt8 ChestedHorse = AbstractHorse + 1;
	const UInt8 TameableAnimal = Ageable + 2;
	const UInt8 Minecart = Entity + 6;
	const UInt8 RaidParticipent = Insentient + 1;

	switch (a_Metadata)
	{
		case EntityMetadata::EntityFlags:                           return 0;
		case EntityMetadata::EntityAir:                             return 1;
		case EntityMetadata::EntityCustomName:                      return 2;
		case EntityMetadata::EntityCustomNameVisible:               return 3;
		case EntityMetadata::EntitySilent:                          return 4;
		case EntityMetadata::EntityNoGravity:                       return 5;
		case EntityMetadata::EntityPose:                            return 6;
		case EntityMetadata::ThrowableItem:                         return Entity;
		case EntityMetadata::PotionThrown:                          return Entity;
		case EntityMetadata::FallingBlockPosition:                  return Entity;
		case EntityMetadata::AreaEffectCloudRadius:                 return Entity;
		case EntityMetadata::AreaEffectCloudColor:                  return Entity + 1;
		case EntityMetadata::AreaEffectCloudSinglePointEffect:      return Entity + 2;
		case EntityMetadata::AreaEffectCloudParticleId:             return Entity + 3;
		case EntityMetadata::ArrowFlags:                            return Entity;
		case EntityMetadata::PiercingLevel:                         return Entity + 2;
		case EntityMetadata::TippedArrowColor:                      return Entity + 3;
		case EntityMetadata::BoatLastHitTime:                       return Entity;
		case EntityMetadata::BoatForwardDirection:                  return Entity + 1;
		case EntityMetadata::BoatDamageTaken:                       return Entity + 2;
		case EntityMetadata::BoatType:                              return Entity + 3;
		case EntityMetadata::BoatLeftPaddleTurning:                 return Entity + 4;
		case EntityMetadata::BoatRightPaddleTurning:                return Entity + 5;
		case EntityMetadata::BoatSplashTimer:                       return Entity + 6;
		case EntityMetadata::EnderCrystalBeamTarget:                return Entity;
		case EntityMetadata::EnderCrystalShowBottom:                return Entity + 1;
		case EntityMetadata::WitherSkullInvulnerable:               return Entity;
		case EntityMetadata::FireworkInfo:                          return Entity;
		case EntityMetadata::FireworkBoostedEntityId:               return Entity + 1;
		case EntityMetadata::FireworkFromCrossbow:                  return Entity + 2;
		case EntityMetadata::ItemFrameItem:                         return Entity;
		case EntityMetadata::ItemFrameRotation:                     return Entity + 1;
		case EntityMetadata::ItemItem:                              return Entity;
		case EntityMetadata::LivingActiveHand:                      return Entity;
		case EntityMetadata::LivingHealth:                          return Entity + 1;
		case EntityMetadata::LivingPotionEffectColor:               return Entity + 2;
		case EntityMetadata::LivingPotionEffectAmbient:             return Entity + 3;
		case EntityMetadata::LivingNumberOfArrows:                  return Entity + 4;
		case EntityMetadata::PlayerAdditionalHearts:                return Living;
		case EntityMetadata::PlayerScore:                           return Living + 1;
		case EntityMetadata::PlayerDisplayedSkinParts:              return Living + 2;
		case EntityMetadata::PlayerMainHand:                        return Living + 3;
		case EntityMetadata::ArmorStandStatus:                      return Living;
		case EntityMetadata::ArmorStandHeadRotation:                return Living + 1;
		case EntityMetadata::ArmorStandBodyRotation:                return Living + 2;
		case EntityMetadata::ArmorStandLeftArmRotation:             return Living + 3;
		case EntityMetadata::ArmorStandRightArmRotation:            return Living + 4;
		case EntityMetadata::ArmorStandLeftLegRotation:             return Living + 5;
		case EntityMetadata::ArmorStandRightLegRotation:            return Living + 6;
		case EntityMetadata::InsentientFlags:                       return Living;
		case EntityMetadata::BatHanging:                            return Insentient;
		case EntityMetadata::AgeableIsBaby:                         return Insentient;
		case EntityMetadata::AbstractHorseFlags:                    return Ageable;
		case EntityMetadata::AbstractHorseOwner:                    return Ageable + 1;
		case EntityMetadata::HorseVariant:                          return AbstractHorse;
		case EntityMetadata::ChestedHorseChested:                   return AbstractHorse;
		case EntityMetadata::LlamaStrength:                         return ChestedHorse;
		case EntityMetadata::LlamaCarpetColor:                      return ChestedHorse + 1;
		case EntityMetadata::LlamaVariant:                          return ChestedHorse + 2;
		case EntityMetadata::PigHasSaddle:                          return Ageable;
		case EntityMetadata::PigTotalCarrotOnAStickBoost:           return Ageable + 1;
		case EntityMetadata::RabbitType:                            return Ageable;
		case EntityMetadata::PolarBearStanding:                     return Ageable;
		case EntityMetadata::SheepFlags:                            return Ageable;
		case EntityMetadata::TameableAnimalFlags:                   return Ageable;
		case EntityMetadata::TameableAnimalOwner:                   return Ageable + 1;
		case EntityMetadata::OcelotType:                            return TameableAnimal;
		case EntityMetadata::WolfDamageTaken:                       return TameableAnimal;
		case EntityMetadata::WolfBegging:                           return TameableAnimal + 1;
		case EntityMetadata::WolfCollarColour:                      return TameableAnimal + 2;
		case EntityMetadata::VillagerProfession:                    return Ageable;
		case EntityMetadata::IronGolemPlayerCreated:                return Insentient;
		case EntityMetadata::ShulkerFacingDirection:                return Insentient;
		case EntityMetadata::ShulkerAttachmentFallingBlockPosition: return Insentient + 1;
		case EntityMetadata::ShulkerShieldHeight:                   return Insentient + 2;
		case EntityMetadata::BlazeOnFire:                           return Insentient;
		case EntityMetadata::CreeperState:                          return Insentient;
		case EntityMetadata::CreeperPowered:                        return Insentient + 1;
		case EntityMetadata::CreeperIgnited:                        return Insentient + 2;
		case EntityMetadata::GuardianStatus:                        return Insentient;
		case EntityMetadata::GuardianTarget:                        return Insentient + 1;
		case EntityMetadata::IllagerFlags:                          return Insentient;
		case EntityMetadata::SpeIlagerSpell:                        return Insentient + 1;
		case EntityMetadata::VexFlags:                              return Insentient;
		case EntityMetadata::AbstractSkeletonArmsSwinging:          return Insentient;
		case EntityMetadata::SpiderClimbing:                        return Insentient;
		case EntityMetadata::WitchAggresive:                        return Insentient;
		case EntityMetadata::WitherFirstHeadTarget:                 return Insentient;
		case EntityMetadata::WitherSecondHeadTarget:                return Insentient + 1;
		case EntityMetadata::WitherThirdHeadTarget:                 return Insentient + 2;
		case EntityMetadata::WitherInvulnerableTimer:               return Insentient + 3;
		case EntityMetadata::ZombieIsBaby:                          return Insentient;
		case EntityMetadata::ZombieHandsRisedUp:                    return Insentient + 2;
		case EntityMetadata::ZombieVillagerConverting:              return Insentient + 4;
		case EntityMetadata::ZombieVillagerProfession:              return Insentient + 5;
		case EntityMetadata::EndermanCarriedBlock:                  return Insentient;
		case EntityMetadata::EndermanScreaming:                     return Insentient + 1;
		case EntityMetadata::EnderDragonDragonPhase:                return Insentient;
		case EntityMetadata::GhastAttacking:                        return Insentient;
		case EntityMetadata::SlimeSize:                             return Insentient;
		case EntityMetadata::MinecartShakingPower:                  return Entity;
		case EntityMetadata::MinecartShakingDirection:              return Entity + 1;
		case EntityMetadata::MinecartShakingMultiplier:             return Entity + 2;
		case EntityMetadata::MinecartBlockIDMeta:                   return Entity + 3;
		case EntityMetadata::MinecartBlockY:                        return Entity + 4;
		case EntityMetadata::MinecartShowBlock:                     return Entity + 5;
		case EntityMetadata::MinecartCommandBlockCommand:           return Minecart;
		case EntityMetadata::MinecartCommandBlockLastOutput:        return Minecart + 1;
		case EntityMetadata::MinecartFurnacePowered:                return Minecart;
		case EntityMetadata::TNTPrimedFuseTime:                     return Entity;
		case EntityMetadata::TridentLoyaltyLevel:                   return Entity + 3;
		case EntityMetadata::MooshroomType:                         return Ageable;
		case EntityMetadata::WitchDrinking:                         return RaidParticipent;

		case EntityMetadata::AreaEffectCloudParticleParameter1:
		case EntityMetadata::AreaEffectCloudParticleParameter2:
		case EntityMetadata::ZombieUnusedWasType: break;

		default:
			break;
	}
	UNREACHABLE("Retrieved invalid metadata for protocol");
}





/* std::pair<short, short> cProtocol_1_15::GetItemFromProtocolID(UInt32 a_ProtocolID) const
{
	return PaletteUpgrade::ToItem(Palette_1_15::ToItem(a_ProtocolID));
}





UInt32 cProtocol_1_15::GetProtocolBlockType(BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) const
{
	return Palette_1_15::From(PaletteUpgrade::FromBlock(a_BlockType, a_Meta));
} */





signed char cProtocol_1_15::GetProtocolEntityStatus(EntityAnimation a_Animation) const
{
	switch (a_Animation)
	{
		case EntityAnimation::FoxChews: return 45;
		case EntityAnimation::OcelotTrusts: return 40;
		case EntityAnimation::OcelotDistrusts: return 41;
		case EntityAnimation::PawnBerryBushPricks: return 44;
		case EntityAnimation::PawnChestEquipmentBreaks: return 50;
		case EntityAnimation::PawnFeetEquipmentBreaks: return 52;
		case EntityAnimation::PawnHeadEquipmentBreaks: return 49;
		case EntityAnimation::PawnLegsEquipmentBreaks: return 51;
		case EntityAnimation::PawnMainHandEquipmentBreaks: return 47;
		case EntityAnimation::PawnOffHandEquipmentBreaks: return 48;
		case EntityAnimation::PawnTeleports: return 46;
		case EntityAnimation::PlayerBadOmenActivates: return 43;
		case EntityAnimation::RavagerAttacks: return 4;
		case EntityAnimation::RavagerBecomesStunned: return 39;
		case EntityAnimation::VillagerSweats: return 42;
		default: return Super::GetProtocolEntityStatus(a_Animation);
	}
}





UInt8 cProtocol_1_15::GetProtocolEntityType(const cEntity & a_Entity) const
{
	using Type = cEntity::eEntityType;

	switch (a_Entity.GetEntityType())
	{
		case Type::etEnderCrystal: return 18;
		case Type::etPickup: return 35;
		case Type::etFallingBlock: return 26;
		case Type::etMinecart: return 42;
		case Type::etBoat: return 6;
		case Type::etTNT: return 59;
		case Type::etProjectile:
		{
			using PType = cProjectileEntity::eKind;
			const auto & Projectile = static_cast<const cProjectileEntity &>(a_Entity);

			switch (Projectile.GetProjectileKind())
			{
				case PType::pkArrow: return 2;
				case PType::pkSnowball: return 71;
				case PType::pkEgg: return 79;
				case PType::pkGhastFireball: return 37;
				case PType::pkFireCharge: return 69;
				case PType::pkEnderPearl: return 80;
				case PType::pkExpBottle: return 81;
				case PType::pkSplashPotion: return 82;
				case PType::pkFirework: return 28;
				case PType::pkWitherSkull: return 93;
			}
			break;
		}
		case Type::etFloater: return 102;
		case Type::etItemFrame: return 36;
		case Type::etLeashKnot: return 38;

		// Non-objects must not be sent
		case Type::etEntity:
		case Type::etPlayer:
		case Type::etMonster:
		case Type::etExpOrb:
		case Type::etPainting: break;
	}
	UNREACHABLE("Unhandled entity kind");
}





/* UInt32 cProtocol_1_15::GetProtocolItemType(short a_ItemID, short a_ItemDamage) const
{
	return Palette_1_15::From(PaletteUpgrade::FromItem(a_ItemID, a_ItemDamage));
} */





UInt32 cProtocol_1_15::GetProtocolMobType(eMonsterType a_MobType) const
{
	switch (a_MobType)
	{
		// Map invalid type to Giant for easy debugging (if this ever spawns, something has gone very wrong)
		case mtInvalidType:           return 29;
		case mtBat:                   return 3;
		case mtBee:					  return 4;
		case mtBlaze:                 return 5;
		case mtCat:                   return 7;
		case mtCaveSpider:            return 8;
		case mtChicken:               return 9;
		case mtCod:                   return 10;
		case mtCow:                   return 11;
		case mtCreeper:               return 12;
		case mtDonkey:                return 13;
		case mtDolphin:               return 14;
		case mtDrowned:               return 16;
		case mtElderGuardian:         return 17;
		case mtEnderDragon:           return 19;
		case mtEnderman:              return 20;
		case mtEndermite:             return 21;
		case mtEvoker:                return 23;
		case mtFox:                   return 28;
		case mtGhast:                 return 29;
		case mtGiant:                 return 30;
		case mtGuardian:              return 31;
		case mtHorse:                 return 32;
		case mtHusk:                  return 33;
		case mtIllusioner:            return 34;
		case mtLlama:                 return 39;
		case mtMagmaCube:             return 41;
		case mtMule:                  return 49;
		case mtMooshroom:             return 50;
		case mtOcelot:                return 51;
		case mtPanda:                 return 53;
		case mtParrot:                return 54;
		case mtPig:                   return 55;
		case mtPufferfish:            return 56;
		case mtZombiePigman:		  return 57;
		case mtPolarBear:             return 58;
		case mtRabbit:                return 60;
		case mtSalmon:                return 61;
		case mtSheep:                 return 62;
		case mtShulker:               return 63;
		case mtSilverfish:            return 65;
		case mtSkeleton:              return 66;
		case mtSkeletonHorse:         return 67;
		case mtSlime:                 return 68;
		case mtSnowGolem:             return 70;
		case mtSpider:                return 73;
		case mtSquid:                 return 74;
		case mtStray:                 return 75;
		case mtTraderLlama:           return 76;
		case mtTropicalFish:          return 77;
		case mtTurtle:                return 78;
		case mtVex:                   return 84;
		case mtVillager:              return 85;
		case mtIronGolem:			  return 86;
		case mtVindicator:            return 87;
		case mtPillager:              return 88;
		case mtWanderingTrader:       return 89;
		case mtWitch:                 return 90;
		case mtWither:                return 91;
		case mtWitherSkeleton:        return 92;
		case mtWolf:                  return 94;
		case mtZombie:                return 95;
		case mtZombieHorse:           return 96;
		case mtZombieVillager:        return 97;
		case mtPhantom:               return 98;
		case mtRavager:               return 99;

		default:                      return 0;
	}
}





int cProtocol_1_15::GetProtocolParticleID(const AString & a_ParticleName) const
{
	static const std::unordered_map<AString, int> ParticleMap
	{
		// Initialize the ParticleMap:
		{ "ambiantentity",           0 },
		{ "angryvillager",           1 },
		{ "barrier",                 2 },
		{ "blockdust",               3 },
		{ "bubble",                  4 },
		{ "cloud",                   5 },
		{ "crit",                    6 },
		{ "damageindicator",         7 },
		{ "dragonbreath",            8 },
		{ "driplava",                9 },
		{ "fallinglava",            10 },
		{ "landinglava",            11 },
		{ "dripwater",              12 },
		{ "fallingwater",           13 },
		{ "dust",                   14 },
		{ "effect",                 15 },
		{ "elderguardian",          16 },
		{ "enchantedhit",           17 },
		{ "enchant",                18 },
		{ "endrod",                 19 },
		{ "entityeffect",           20 },
		{ "explosionemitter",       21 },
		{ "explode",                22 },
		{ "fallingdust",            23 },
		{ "firework",               24 },
		{ "fishing",                25 },
		{ "flame",                  26 },
		{ "flash",                  27 },
		{ "happyvillager",          28 },
		{ "composter",              29 },
		{ "heart",                  30 },
		{ "instanteffect",          31 },
		{ "item",                   32 },
		{ "slime",                  33 },
		{ "snowball",               34 },
		{ "largesmoke",             35 },
		{ "lava",                   36 },
		{ "mycelium",               37 },
		{ "note",                   38 },
		{ "poof",                   39 },
		{ "portal",                 40 },
		{ "rain",                   41 },
		{ "smoke",                  42 },
		{ "sneeze",                 43 },
		{ "spit",                   44 },
		{ "squidink",               45 },
		{ "sweepattack",            46 },
		{ "totem",                  47 },
		{ "underwater",             48 },
		{ "splash",                 49 },
		{ "witch",                  50 },
		{ "bubblepop",              51 },
		{ "currentdown",            52 },
		{ "bubblecolumnup",         53 },
		{ "nautilus",               54 },
		{ "dolphin",                55 },
		{ "campfirecosysmoke",      56 },
		{ "campfiresignalsmoke",    57 },
		{ "dripping_honey",         58 },
		{ "falling_honey",          59 },
		{ "landing_honey",          60 },
		{ "falling_nectar",         61 },
	};


	const auto ParticleName = StrToLower(a_ParticleName);
	const auto FindResult = ParticleMap.find(ParticleName);
	if (FindResult == ParticleMap.end())
	{
		LOGWARNING("Unknown particle: %s", a_ParticleName.c_str());
		ASSERT(!"Unknown particle");
		return 0;
	}

	return FindResult->second;
}





UInt32 cProtocol_1_15::GetProtocolStatisticType(const CustomStatistic a_Statistic) const
{
	return Palette_1_15::From(a_Statistic);
}





cProtocol::Version cProtocol_1_15::GetProtocolVersion() const
{
	return Version::v1_15;
}





bool cProtocol_1_15::HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType)
{
	if (m_State != State::Game)
	{
		return Super::HandlePacket(a_ByteBuffer, a_PacketType);
	}

	// Game
	switch (a_PacketType)
	{
		case 0x01: /* query nbt packet */ return false;
		case 0x02: /* update difficulty */ return false;
		case 0x03: HandlePacketChatMessage(a_ByteBuffer); return true;
		case 0x04: HandlePacketClientStatus(a_ByteBuffer); return true;
		case 0x05: HandlePacketClientSettings(a_ByteBuffer); return true;
		case 0x06: HandlePacketTabComplete(a_ByteBuffer); return true;
		case 0x07: break;  // Confirm transaction - not used in Cuberite
		case 0x08: HandlePacketEnchantItem(a_ByteBuffer); return true;
		case 0x09: HandlePacketWindowClick(a_ByteBuffer); return true;
		case 0x0A: HandlePacketWindowClose(a_ByteBuffer); return true;
		case 0x0B: HandlePacketPluginMessage(a_ByteBuffer); return true;
		case 0x0C: HandlePacketBookUpdate(a_ByteBuffer); return true;  // not fully implemented
		case 0x0E: HandlePacketUseEntity(a_ByteBuffer); return true;
		case 0x0F: HandlePacketKeepAlive(a_ByteBuffer); return true;
		case 0x10: /* Update difficulty lock */ return false;  // only used in single player
		case 0x11: HandlePacketPlayerPos(a_ByteBuffer); return true;
		case 0x12: HandlePacketPlayerPosLook(a_ByteBuffer); return true;
		case 0x13: HandlePacketPlayerLook(a_ByteBuffer); return true;
		case 0x14: HandlePacketPlayer(a_ByteBuffer); return true;
		case 0x15: HandlePacketVehicleMove(a_ByteBuffer); return true;
		case 0x16: HandlePacketBoatSteer(a_ByteBuffer); return true;
		case 0x17: /* pick from inventory */ return false;
		case 0x18: HandleCraftRecipe(a_ByteBuffer); return true;
		case 0x19: HandlePacketPlayerAbilities(a_ByteBuffer); return true;
		case 0x1A: HandlePacketBlockDig(a_ByteBuffer); return true;
		case 0x1B: /* client command packet */ return false;
		case 0x1C: HandlePacketSteerVehicle(a_ByteBuffer); return true;  // player input packet
		case 0x1D: HandlePacketCraftingBookData(a_ByteBuffer); return true;
		case 0x1E: HandlePacketNameItem(a_ByteBuffer); return true;
		case 0x1F: HandlePacketResourcePackStatus(a_ByteBuffer); return true;
		case 0x20: HandlePacketAdvancementTab(a_ByteBuffer); return true;
		case 0x21: /* select villager trade */ return false;
		case 0x22: HandlePacketSetBeaconEffect(a_ByteBuffer); return true;
		case 0x23: HandlePacketSlotSelect(a_ByteBuffer); return true;
		case 0x24: HandlePacketCommandBlockUpdate(a_ByteBuffer); return true;
		case 0x25: /* update minecart command block */ return false;
		case 0x26: HandlePacketCreativeInventoryAction(a_ByteBuffer); return true;
		case 0x27: /* Update jigsaw block */ return false;
		case 0x28: /* Update structure block */ return false;
		case 0x29: HandlePacketUpdateSign(a_ByteBuffer); return true;
		case 0x2B: /* Spectator teleport */ return false;
		case 0x2A: /* Update hand swing */ return false;
		case 0x2C: HandlePacketBlockPlace(a_ByteBuffer); return true;
		case 0x2D: HandlePacketUseItem(a_ByteBuffer); return true;
		default: break;
	}
	return Super::HandlePacket(a_ByteBuffer, a_PacketType);
}





void cProtocol_1_15::HandlePacketBlockDig(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8, Status);

	int BlockX, BlockY, BlockZ;
	if (!a_ByteBuffer.ReadXZYPosition64(BlockX, BlockY, BlockZ))
	{
		return;
	}

	HANDLE_READ(a_ByteBuffer, ReadVarInt, Int32, Face);
	m_Client->HandleLeftClick({BlockX, BlockY, BlockZ}, FaceIntToBlockFace(Face), Status);
}





void cProtocol_1_15::HandlePacketBlockPlace(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarInt, Int32, Hand);

	int BlockX, BlockY, BlockZ;
	if (!a_ByteBuffer.ReadXZYPosition64(BlockX, BlockY, BlockZ))
	{
		return;
	}

	HANDLE_READ(a_ByteBuffer, ReadVarInt, Int32, Face);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, CursorX);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, CursorY);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, CursorZ);
	HANDLE_READ(a_ByteBuffer, ReadBool, bool, InsideBlock);

	m_Client->HandleRightClick({BlockX, BlockY, BlockZ}, FaceIntToBlockFace(Face), {FloorC(CursorX * 16), FloorC(CursorY * 16), FloorC(CursorZ * 16)}, Hand == 0);
}





void cProtocol_1_15::HandlePacketUpdateSign(cByteBuffer & a_ByteBuffer)
{
}





void cProtocol_1_15::WriteEntityMetadata(cPacketizer & a_Pkt, const EntityMetadata a_Metadata, const EntityMetadataType a_FieldType) const
{
	a_Pkt.WriteBEUInt8(GetEntityMetadataID(a_Metadata));	      // Index
	a_Pkt.WriteVarInt32(static_cast<UInt32>(a_FieldType));        // Type
}





void cProtocol_1_15::WriteEntityMetadata(cPacketizer & a_Pkt, const cEntity & a_Entity) const
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
	/*
	if (a_Entity.IsGlowing())
	{
		Flags |= 0x40;
	}
	*/
	if (a_Entity.IsElytraFlying())
	{
		Flags |= 0x80;
	}

	WriteEntityMetadata(a_Pkt, EntityMetadata::EntityFlags, EntityMetadataType::Byte);
	a_Pkt.WriteBEInt8(Flags);

	switch (a_Entity.GetEntityType())
	{
		case cEntity::etPlayer:
		{
			auto & Player = static_cast<const cPlayer &>(a_Entity);

			// TODO Set player custom name to their name.
			// Then it's possible to move the custom name of mobs to the entities
			// and to remove the "special" player custom name.
			WriteEntityMetadata(a_Pkt, EntityMetadata::EntityCustomName, EntityMetadataType::OptChat);
			a_Pkt.WriteBool(true);
			a_Pkt.WriteString("\"{" + Player.GetName() + "\"}");	 // needs to be json formatted

			WriteEntityMetadata(a_Pkt, EntityMetadata::LivingHealth, EntityMetadataType::Float);
			a_Pkt.WriteBEFloat(static_cast<float>(Player.GetHealth()));

			WriteEntityMetadata(a_Pkt, EntityMetadata::PlayerDisplayedSkinParts, EntityMetadataType::Byte);
			a_Pkt.WriteBEUInt8(static_cast<UInt8>(Player.GetSkinParts()));

			WriteEntityMetadata(a_Pkt, EntityMetadata::PlayerMainHand, EntityMetadataType::Byte);
			a_Pkt.WriteBEUInt8(Player.IsLeftHanded() ? 0 : 1);
			break;
		}

		default:
		{
			Super::WriteEntityMetadata(a_Pkt, a_Entity);
		}
	}
}





void cProtocol_1_15::WriteMobMetadata(cPacketizer & a_Pkt, const cMonster & a_Mob) const
{
	// Living Enitiy Metadata
	if (a_Mob.HasCustomName())
	{
		// TODO: As of 1.9 _all_ entities can have custom names; should this be moved up?
		WriteEntityMetadata(a_Pkt, EntityMetadata::EntityCustomName, EntityMetadataType::OptChat);
		a_Pkt.WriteBool(true);
		a_Pkt.WriteString(a_Mob.GetCustomName());

		WriteEntityMetadata(a_Pkt, EntityMetadata::EntityCustomNameVisible, EntityMetadataType::Boolean);
		a_Pkt.WriteBool(a_Mob.IsCustomNameAlwaysVisible());
	}

	WriteEntityMetadata(a_Pkt, EntityMetadata::LivingHealth, EntityMetadataType::Float);
	a_Pkt.WriteBEFloat(static_cast<float>(a_Mob.GetHealth()));

	// TODO: pose

	switch (a_Mob.GetMobType())
	{
		case mtBat:
		{
			auto & Bat = static_cast<const cBat &>(a_Mob);

			WriteEntityMetadata(a_Pkt, EntityMetadata::BatHanging, EntityMetadataType::Byte);
			a_Pkt.WriteBEInt8(Bat.IsHanging() ? 1 : 0);
			break;
		}  // case mtBat

		case mtChicken:
		{
			auto & Chicken = static_cast<const cChicken &>(a_Mob);

			WriteEntityMetadata(a_Pkt, EntityMetadata::AgeableIsBaby, EntityMetadataType::Boolean);
			a_Pkt.WriteBool(Chicken.IsBaby());
			break;
		}  // case mtChicken

		case mtCow:
		{
			auto & Cow = static_cast<const cCow &>(a_Mob);

			WriteEntityMetadata(a_Pkt, EntityMetadata::AgeableIsBaby, EntityMetadataType::Boolean);
			a_Pkt.WriteBool(Cow.IsBaby());
			break;
		}  // case mtCow

		case mtCreeper:
		{
			auto & Creeper = static_cast<const cCreeper &>(a_Mob);

			WriteEntityMetadata(a_Pkt, EntityMetadata::CreeperState, EntityMetadataType::VarInt);
			a_Pkt.WriteVarInt32(Creeper.IsBlowing() ? 1 : static_cast<UInt32>(-1));  // (idle or "blowing")

			WriteEntityMetadata(a_Pkt, EntityMetadata::CreeperPowered, EntityMetadataType::Boolean);
			a_Pkt.WriteBool(Creeper.IsCharged());

			WriteEntityMetadata(a_Pkt, EntityMetadata::CreeperIgnited, EntityMetadataType::Boolean);
			a_Pkt.WriteBool(Creeper.IsBurnedWithFlintAndSteel());
			break;
		}  // case mtCreeper

		case mtEnderman:
		{
			auto & Enderman = static_cast<const cEnderman &>(a_Mob);
			WriteEntityMetadata(a_Pkt, EntityMetadata::EndermanCarriedBlock, EntityMetadataType::OptBlockID);
			auto Carried = Enderman.GetCarriedBlock();
			a_Pkt.WriteBool(Carried != BlockType::Air);
			if (Carried != BlockType::Air)
			{
				a_Pkt.WriteVarInt32(GetProtocolBlockType(Carried));
			}

			WriteEntityMetadata(a_Pkt, EntityMetadata::EndermanScreaming, EntityMetadataType::Boolean);
			a_Pkt.WriteBool(Enderman.IsScreaming());
			break;
		}  // case mtEnderman

		case mtGhast:
		{
			auto & Ghast = static_cast<const cGhast &>(a_Mob);

			WriteEntityMetadata(a_Pkt, EntityMetadata::GhastAttacking, EntityMetadataType::Boolean);
			a_Pkt.WriteBool(Ghast.IsCharging());
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
			if (Horse.IsInLoveCooldown())
			{
				Flags |= 0x08;
			}
			if (Horse.IsEating())
			{
				Flags |= 0x10;
			}
			if (Horse.IsRearing())
			{
				Flags |= 0x20;
			}
			if (Horse.IsMthOpen())
			{
				Flags |= 0x40;
			}
			WriteEntityMetadata(a_Pkt, EntityMetadata::AbstractHorseFlags, EntityMetadataType::Byte);
			a_Pkt.WriteBEInt8(Flags);

			// Regular horses
			int Appearance = 0;
			Appearance = Horse.GetHorseColor();
			Appearance |= Horse.GetHorseStyle() << 8;
			WriteEntityMetadata(a_Pkt, EntityMetadata::HorseVariant, EntityMetadataType::VarInt);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Appearance));  // Color / style

			WriteEntityMetadata(a_Pkt, EntityMetadata::AgeableIsBaby, EntityMetadataType::Boolean);
			a_Pkt.WriteBool(Horse.IsBaby());
			break;
		}  // case mtHorse

		case mtMagmaCube:
		{
			auto & MagmaCube = static_cast<const cMagmaCube &>(a_Mob);

			WriteEntityMetadata(a_Pkt, EntityMetadata::SlimeSize, EntityMetadataType::VarInt);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(MagmaCube.GetSize()));
			break;
		}  // case mtMagmaCube

		case mtOcelot:
		{
			auto & Ocelot = static_cast<const cOcelot &>(a_Mob);

			WriteEntityMetadata(a_Pkt, EntityMetadata::AgeableIsBaby, EntityMetadataType::Boolean);
			a_Pkt.WriteBool(Ocelot.IsBaby());

			// TODO: Ocelot trusting

			break;
		}  // case mtOcelot

		case mtPig:
		{
			auto & Pig = static_cast<const cPig &>(a_Mob);

			WriteEntityMetadata(a_Pkt, EntityMetadata::AgeableIsBaby, EntityMetadataType::Boolean);
			a_Pkt.WriteBool(Pig.IsBaby());

			WriteEntityMetadata(a_Pkt, EntityMetadata::PigHasSaddle, EntityMetadataType::Boolean);
			a_Pkt.WriteBool(Pig.IsSaddled());

			// PIG_TOTAL_CARROT_ON_A_STICK_BOOST in 1.11.1 only
			break;
		}  // case mtPig

		case mtRabbit:
		{
			auto & Rabbit = static_cast<const cRabbit &>(a_Mob);

			WriteEntityMetadata(a_Pkt, EntityMetadata::AgeableIsBaby, EntityMetadataType::Boolean);
			a_Pkt.WriteBool(Rabbit.IsBaby());

			WriteEntityMetadata(a_Pkt, EntityMetadata::RabbitType, EntityMetadataType::VarInt);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Rabbit.GetRabbitType()));
			break;
		}  // case mtRabbit

		case mtSheep:
		{
			auto & Sheep = static_cast<const cSheep &>(a_Mob);

			WriteEntityMetadata(a_Pkt, EntityMetadata::AgeableIsBaby, EntityMetadataType::Boolean);
			a_Pkt.WriteBool(Sheep.IsBaby());

			Int8 SheepMetadata = 0;
			SheepMetadata = static_cast<Int8>(Sheep.GetFurColor());
			if (Sheep.IsSheared())
			{
				SheepMetadata |= 0x10;
			}
			WriteEntityMetadata(a_Pkt, EntityMetadata::SheepFlags, EntityMetadataType::Byte);
			a_Pkt.WriteBEInt8(SheepMetadata);
			break;
		}  // case mtSheep

		case mtSkeleton:
		{
			auto & Skeleton = static_cast<const cSkeleton &>(a_Mob);
			WriteEntityMetadata(a_Pkt, EntityMetadata::LivingActiveHand, EntityMetadataType::Byte);
			a_Pkt.WriteBEUInt8(Skeleton.IsChargingBow() ? 0x01 : 0x00);

			// TODO: Skeleton animation
			break;
		}  // case mtSkeleton

		case mtSlime:
		{
			auto & Slime = static_cast<const cSlime &>(a_Mob);

			WriteEntityMetadata(a_Pkt, EntityMetadata::SlimeSize, EntityMetadataType::VarInt);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Slime.GetSize()));
			break;
		}  // case mtSlime

		case mtVillager:
		{
			auto & Villager = static_cast<const cVillager &>(a_Mob);

			WriteEntityMetadata(a_Pkt, EntityMetadata::AgeableIsBaby, EntityMetadataType::Boolean);
			a_Pkt.WriteBool(Villager.IsBaby());

			WriteEntityMetadata(a_Pkt, EntityMetadata::VillagerProfession, EntityMetadataType::VillagerData);
			a_Pkt.WriteVarInt32(2);  // Villager from plains
			switch (Villager.GetVilType())
			{
				case cVillager::vtFarmer:
				{
					a_Pkt.WriteVarInt32(5);
					break;
				}
				case cVillager::vtLibrarian:
				{
					a_Pkt.WriteVarInt32(9);
					break;
				}
				case cVillager::vtPriest:
				{
					a_Pkt.WriteVarInt32(4);
					break;
				}
				case cVillager::vtBlacksmith:
				{
					a_Pkt.WriteVarInt32(13);
					break;
				}
				case cVillager::vtButcher:
				{
					a_Pkt.WriteVarInt32(2);
					break;
				}
				case cVillager::vtGeneric:
				{
					a_Pkt.WriteVarInt32(0);
					break;
				}
			}
			a_Pkt.WriteVarInt32(1);  // Level 1 villager
			break;
		}  // case mtVillager

		case mtWitch:
		{
			// auto & Witch = static_cast<const cWitch &>(a_Mob);

			// TODO: Witch drinking potion
			break;
		}  // case mtWitch

		case mtWither:
		{
			auto & Wither = static_cast<const cWither &>(a_Mob);

			WriteEntityMetadata(a_Pkt, EntityMetadata::WitherInvulnerableTimer, EntityMetadataType::VarInt);
			a_Pkt.WriteVarInt32(Wither.GetWitherInvulnerableTicks());

			// TODO: Use boss bar packet for health
			break;
		}  // case mtWither

		case mtWolf:
		{
			auto & Wolf = static_cast<const cWolf &>(a_Mob);

			WriteEntityMetadata(a_Pkt, EntityMetadata::AgeableIsBaby, EntityMetadataType::Boolean);
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
			WriteEntityMetadata(a_Pkt, EntityMetadata::TameableAnimalFlags, EntityMetadataType::Byte);
			a_Pkt.WriteBEInt8(WolfStatus);

			WriteEntityMetadata(a_Pkt, EntityMetadata::WolfDamageTaken, EntityMetadataType::Float);
			a_Pkt.WriteBEFloat(static_cast<float>(a_Mob.GetHealth()));  // TODO Not use the current health

			WriteEntityMetadata(a_Pkt, EntityMetadata::WolfBegging, EntityMetadataType::Boolean);
			a_Pkt.WriteBool(Wolf.IsBegging());

			WriteEntityMetadata(a_Pkt, EntityMetadata::WolfCollarColour, EntityMetadataType::VarInt);
			a_Pkt.WriteVarInt32(static_cast<UInt32>(Wolf.GetCollarColor()));
			break;
		}  // case mtWolf

		case mtZombie:
		{
			// XXX Zombies were also split into new sublcasses; this doesn't handle that.

			auto & Zombie = static_cast<const cZombie &>(a_Mob);

			WriteEntityMetadata(a_Pkt, EntityMetadata::ZombieIsBaby, EntityMetadataType::Boolean);
			a_Pkt.WriteBool(Zombie.IsBaby());
			break;
		}  // case mtZombie

		case mtZombiePigman:
		{
			auto & ZombiePigman = static_cast<const cZombiePigman &>(a_Mob);

			WriteEntityMetadata(a_Pkt, EntityMetadata::AgeableIsBaby, EntityMetadataType::Boolean);
			a_Pkt.WriteBool(ZombiePigman.IsBaby());
			break;
		}  // case mtZombiePigman

		case mtBlaze:
		case mtEnderDragon:
		case mtIronGolem:
		case mtSnowGolem:
		case mtSpider:
		case mtZombieVillager:

		case mtElderGuardian:
		case mtGuardian:
		{
			// TODO: Mobs with extra fields that aren't implemented
			break;
		}

		case mtCat:

		case mtCod:

		case mtDolphin:

		case mtDonkey:

		case mtDrowned:

		case mtEvoker:

		case mtIllusioner:

		case mtLlama:

		case mtMule:

		case mtParrot:

		case mtPhantom:

		case mtPolarBear:

		case mtPufferfish:

		case mtSalmon:

		case mtShulker:

		case mtStray:

		case mtSkeletonHorse:
		case mtZombieHorse:

		case mtTropicalFish:

		case mtTurtle:

		case mtVex:

		case mtVindicator:

		case mtHusk:
		{
			// Todo: Mobs not added yet. Grouped ones have the same metadata
			ASSERT(!"cProtocol_1_15::WriteMobMetadata: received unimplemented type");
			break;
		}

		case mtMooshroom:
		case mtCaveSpider:
		{
			// Not mentioned on http://wiki.vg/Entities
			break;
		}

		case mtEndermite:
		case mtGiant:
		case mtSilverfish:
		case mtSquid:
		case mtWitherSkeleton:
		{
			// Mobs with no extra fields
			break;
		}

		default: UNREACHABLE("cProtocol_1_15::WriteMobMetadata: received mob of invalid type");
	}  // switch (a_Mob.GetType())
}





////////////////////////////////////////////////////////////////////////////////
// cProtocol_1_15_1:

cProtocol::Version cProtocol_1_15_1::GetProtocolVersion() const
{
	return Version::v1_15_1;
}





////////////////////////////////////////////////////////////////////////////////
// cProtocol_1_15_2:

cProtocol::Version cProtocol_1_15_2::GetProtocolVersion() const
{
	return Version::v1_15_2;
}
