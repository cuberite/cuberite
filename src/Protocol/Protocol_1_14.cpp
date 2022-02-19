
// Protocol_1_14.cpp

/*
Implements the 1.14 protocol classes:
- release 1.14 protocol (#477)
*/

#include "Globals.h"
#include "Protocol_1_14.h"
#include "Packetizer.h"
#include "../Entities/Player.h"
#include "../Root.h"
#include "../Server.h"
#include "../World.h"
#include "../UI/HorseWindow.h"

#include "Palettes/Upgrade.h"
#include "Palettes/Palette_1_14.h"





////////////////////////////////////////////////////////////////////////////////
// cProtocol_1_14:

void cProtocol_1_14::SendBlockAction(int a_BlockX, int a_BlockY, int a_BlockZ, char a_Byte1, char a_Byte2, BLOCKTYPE a_BlockType)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktBlockAction);
	Pkt.WriteXZYPosition64(a_BlockX, a_BlockY, a_BlockZ);
	Pkt.WriteBEInt8(a_Byte1);
	Pkt.WriteBEInt8(a_Byte2);
	Pkt.WriteVarInt32(a_BlockType);
}





void cProtocol_1_14::SendBlockBreakAnim(UInt32 a_EntityID, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Stage)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, pktBlockBreakAnim);
	Pkt.WriteVarInt32(a_EntityID);
	Pkt.WriteXZYPosition64(a_BlockX, a_BlockY, a_BlockZ);
	Pkt.WriteBEInt8(a_Stage);
}





void cProtocol_1_14::SendBlockChange(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	cPacketizer Pkt(*this, pktBlockChange);
	Pkt.WriteXZYPosition64(a_BlockX, a_BlockY, a_BlockZ);
	Pkt.WriteVarInt32(GetProtocolBlockType(a_BlockType, a_BlockMeta));
}





void cProtocol_1_14::SendEditSign(int a_BlockX, int a_BlockY, int a_BlockZ)
{
}





void cProtocol_1_14::SendEntityAnimation(const cEntity & a_Entity, EntityAnimation a_Animation)
{
	if (a_Animation == EntityAnimation::PlayerEntersBed)
	{
		// Use Bed packet removed, through metadata instead:
		SendEntityMetadata(a_Entity);
		return;
	}

	Super::SendEntityAnimation(a_Entity, a_Animation);
}





void cProtocol_1_14::SendEntitySpawn(const cEntity & a_Entity, const UInt8 a_ObjectType, const Int32 a_ObjectData)
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





void cProtocol_1_14::SendLogin(const cPlayer & a_Player, const cWorld & a_World)
{
	// Send the Join Game packet:
	{
		cServer * Server = cRoot::Get()->GetServer();
		cPacketizer Pkt(*this, pktJoinGame);
		Pkt.WriteBEUInt32(a_Player.GetUniqueID());
		Pkt.WriteBEUInt8(static_cast<UInt8>(a_Player.GetEffectiveGameMode()) | (Server->IsHardcore() ? 0x08 : 0));
		Pkt.WriteBEInt32(static_cast<Int32>(a_World.GetDimension()));
		Pkt.WriteBEUInt8(static_cast<UInt8>(Clamp<size_t>(Server->GetMaxPlayers(), 0, 255)));
		Pkt.WriteString("default");
		Pkt.WriteVarInt32(ToUnsigned(a_World.GetMaxViewDistance()));
		Pkt.WriteBool(false);
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
}





void cProtocol_1_14::SendMapData(const cMap & a_Map, int a_DataStartX, int a_DataStartY)
{
}





void cProtocol_1_14::SendPaintingSpawn(const cPainting & a_Painting)
{
}





void cProtocol_1_14::SendRespawn(eDimension a_Dimension)
{
	cPacketizer Pkt(*this, pktRespawn);
	cPlayer * Player = m_Client->GetPlayer();
	Pkt.WriteBEInt32(static_cast<Int32>(a_Dimension));
	Pkt.WriteBEUInt8(static_cast<Byte>(Player->GetEffectiveGameMode()));
	Pkt.WriteString("default");
}





void cProtocol_1_14::SendSoundParticleEffect(const EffectID a_EffectID, int a_SrcX, int a_SrcY, int a_SrcZ, int a_Data)
{
}





void cProtocol_1_14::SendUpdateBlockEntity(cBlockEntity & a_BlockEntity)
{
	ASSERT(m_State == 3);  // In game mode?

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
}





void cProtocol_1_14::SendUpdateSign(int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4)
{
}





void cProtocol_1_14::SendWindowOpen(const cWindow & a_Window)
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

		Pkt.WriteString(Printf("{\"text\":\"%s\"}", a_Window.GetWindowTitle().c_str()));
	}
}





UInt32 cProtocol_1_14::GetPacketID(ePacketType a_PacketType) const
{
	switch (a_PacketType)
	{
		case cProtocol::pktAttachEntity:         return 0x4A;
		case cProtocol::pktCameraSetTo:          return 0x3E;
		case cProtocol::pktCollectEntity:        return 0x55;
		case cProtocol::pktDestroyEntity:        return 0x37;
		case cProtocol::pktDisconnectDuringGame: return 0x1A;
		case cProtocol::pktEntityEffect:         return 0x59;
		case cProtocol::pktEntityEquipment:      return 0x46;
		case cProtocol::pktEntityHeadLook:       return 0x3B;
		case cProtocol::pktEntityMeta:           return 0x43;
		case cProtocol::pktEntityProperties:     return 0x58;
		case cProtocol::pktEntityStatus:         return 0x1B;
		case cProtocol::pktEntityVelocity:       return 0x45;
		case cProtocol::pktExperience:           return 0x47;
		case cProtocol::pktExplosion:            return 0x1C;
		case cProtocol::pktGameMode:             return 0x1E;
		case cProtocol::pktHeldItemChange:       return 0x3F;
		case cProtocol::pktHorseWindowOpen:      return 0x1F;
		case cProtocol::pktInventorySlot:        return 0x16;
		case cProtocol::pktKeepAlive:            return 0x20;
		case cProtocol::pktParticleEffect:       return 0x23;
		case cProtocol::pktPlayerAbilities:      return 0x31;
		case cProtocol::pktPlayerList:           return 0x33;
		case cProtocol::pktPlayerMoveLook:       return 0x35;
		case cProtocol::pktPluginMessage:        return 0x18;
		case cProtocol::pktRemoveEntityEffect:   return 0x38;
		case cProtocol::pktResourcePack:         return 0x39;
		case cProtocol::pktRespawn:              return 0x3A;
		case cProtocol::pktScoreboardObjective:  return 0x49;
		case cProtocol::pktSoundEffect:          return 0x19;
		case cProtocol::pktSoundParticleEffect:  return 0x22;
		case cProtocol::pktSpawnPosition:        return 0x4D;
		case cProtocol::pktTeleportEntity:       return 0x56;
		case cProtocol::pktTimeUpdate:           return 0x4E;
		case cProtocol::pktTitle:                return 0x4F;
		case cProtocol::pktUnloadChunk:          return 0x1D;
		case cProtocol::pktUnlockRecipe:         return 0x36;
		case cProtocol::pktUpdateHealth:         return 0x48;
		case cProtocol::pktUpdateScore:          return 0x4C;
		case cProtocol::pktUpdateSign:           return 0x2F;
		case cProtocol::pktWeather:              return 0x1E;
		case cProtocol::pktWindowItems:          return 0x14;
		case cProtocol::pktWindowOpen:           return 0x2E;
		case cProtocol::pktWindowProperty:       return 0x15;
		default: return Super::GetPacketID(a_PacketType);
	}
}





std::pair<short, short> cProtocol_1_14::GetItemFromProtocolID(UInt32 a_ProtocolID) const
{
	return PaletteUpgrade::ToItem(Palette_1_14::ToItem(a_ProtocolID));
}





UInt32 cProtocol_1_14::GetProtocolBlockType(BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta) const
{
	return Palette_1_14::From(PaletteUpgrade::FromBlock(a_BlockType, a_Meta));
}





signed char cProtocol_1_14::GetProtocolEntityStatus(EntityAnimation a_Animation) const
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





UInt8 cProtocol_1_14::GetProtocolEntityType(const cEntity & a_Entity) const
{
	using Type = cEntity::eEntityType;

	switch (a_Entity.GetEntityType())
	{
		case Type::etEnderCrystal: return 17;
		case Type::etPickup: return 34;
		case Type::etFallingBlock: return 25;
		case Type::etMinecart: return 41;
		case Type::etBoat: return 5;
		case Type::etTNT: return 58;
		case Type::etProjectile:
		{
			using PType = cProjectileEntity::eKind;
			const auto & Projectile = static_cast<const cProjectileEntity &>(a_Entity);

			switch (Projectile.GetProjectileKind())
			{
				case PType::pkArrow: return 2;
				case PType::pkSnowball: return 70;
				case PType::pkEgg: return 78;
				case PType::pkGhastFireball: return 36;
				case PType::pkFireCharge: return 68;
				case PType::pkEnderPearl: return 79;
				case PType::pkExpBottle: return 80;
				case PType::pkSplashPotion: return 81;
				case PType::pkFirework: return 26;
				case PType::pkWitherSkull: return 92;
			}
		}
		case Type::etFloater: return 101;
		case Type::etItemFrame: return 35;
		case Type::etLeashKnot: return 37;

		// Non-objects must not be sent
		case Type::etEntity:
		case Type::etPlayer:
		case Type::etMonster:
		case Type::etExpOrb:
		case Type::etPainting: break;
	}
	UNREACHABLE("Unhandled entity kind");
}





UInt32 cProtocol_1_14::GetProtocolItemType(short a_ItemID, short a_ItemDamage) const
{
	return Palette_1_14::From(PaletteUpgrade::FromItem(a_ItemID, a_ItemDamage));
}





UInt32 cProtocol_1_14::GetProtocolMobType(eMonsterType a_MobType) const
{
	switch (a_MobType)
	{
		// Map invalid type to Giant for easy debugging (if this ever spawns, something has gone very wrong)
		case mtInvalidType:           return 29;
		case mtBat:                   return 3;
		case mtBlaze:                 return 4;
		case mtCat:                   return 6;
		case mtCaveSpider:            return 7;
		case mtChicken:               return 8;
		case mtCod:                   return 9;
		case mtCow:                   return 10;
		case mtCreeper:               return 11;
		case mtDonkey:                return 12;
		case mtDolphin:               return 13;
		case mtDrowned:               return 15;
		case mtElderGuardian:         return 16;
		case mtEnderDragon:           return 18;
		case mtEnderman:              return 19;
		case mtEndermite:             return 20;
		case mtEvoker:                return 22;
		case mtFox:                   return 27;
		case mtGhast:                 return 28;
		case mtGiant:                 return 29;
		case mtGuardian:              return 30;
		case mtHorse:                 return 31;
		case mtHusk:                  return 32;
		case mtIllusioner:            return 33;
		case mtIronGolem:             return 85;
		case mtLlama:                 return 38;
		case mtMagmaCube:             return 40;
		case mtMule:                  return 48;
		case mtMooshroom:             return 49;
		case mtOcelot:                return 50;
		case mtPanda:                 return 52;
		case mtParrot:                return 53;
		case mtPig:                   return 54;
		case mtPufferfish:            return 55;
		case mtPolarBear:             return 57;
		case mtRabbit:                return 59;
		case mtSalmon:                return 60;
		case mtSheep:                 return 61;
		case mtShulker:               return 62;
		case mtSilverfish:            return 64;
		case mtSkeleton:              return 65;
		case mtSkeletonHorse:         return 66;
		case mtSlime:                 return 67;
		case mtSnowGolem:             return 69;
		case mtSpider:                return 72;
		case mtSquid:                 return 73;
		case mtStray:                 return 74;
		case mtTraderLlama:           return 75;
		case mtTropicalFish:          return 76;
		case mtTurtle:                return 77;
		case mtVex:                   return 83;
		case mtVillager:              return 84;
		case mtVindicator:            return 86;
		case mtPillager:              return 87;
		case mtWanderingTrader:       return 88;
		case mtWitch:                 return 89;
		case mtWither:                return 90;
		case mtWitherSkeleton:        return 91;
		case mtWolf:                  return 93;
		case mtZombie:                return 94;
		case mtZombieHorse:           return 95;
		case mtZombiePigman:          return 56;
		case mtZombieVillager:        return 96;
		case mtPhantom:               return 97;
		case mtRavager:               return 98;

		default:                      return 0;
	}
}





UInt32 cProtocol_1_14::GetProtocolStatisticType(const CustomStatistic a_Statistic) const
{
	return Palette_1_14::From(a_Statistic);
}





cProtocol::Version cProtocol_1_14::GetProtocolVersion() const
{
	return Version::v1_14;
}





bool cProtocol_1_14::HandlePacket(cByteBuffer & a_ByteBuffer, UInt32 a_PacketType)
{
	if (m_State != State::Game)
	{
		return Super::HandlePacket(a_ByteBuffer, a_PacketType);
	}

	// Game
	switch (a_PacketType)
	{
		case 0x03: HandlePacketChatMessage(a_ByteBuffer); return true;
		case 0x04: HandlePacketClientStatus(a_ByteBuffer); return true;
		case 0x05: HandlePacketClientSettings(a_ByteBuffer); return true;
		case 0x06: HandlePacketTabComplete(a_ByteBuffer); return true;
		case 0x07: break;  // Confirm transaction - not used in Cuberite
		case 0x08: HandlePacketEnchantItem(a_ByteBuffer); return true;
		case 0x09: HandlePacketWindowClick(a_ByteBuffer); return true;
		case 0x0A: HandlePacketWindowClose(a_ByteBuffer); return true;
		case 0x0B: HandlePacketPluginMessage(a_ByteBuffer); return true;
		case 0x0E: HandlePacketUseEntity(a_ByteBuffer); return true;
		case 0x0F: HandlePacketKeepAlive(a_ByteBuffer); return true;
		case 0x11: HandlePacketPlayerPos(a_ByteBuffer); return true;
		case 0x12: HandlePacketPlayerPosLook(a_ByteBuffer); return true;
		case 0x13: HandlePacketPlayerLook(a_ByteBuffer); return true;
		case 0x14: HandlePacketPlayer(a_ByteBuffer); return true;
		case 0x15: HandlePacketVehicleMove(a_ByteBuffer); return true;
		case 0x16: HandlePacketBoatSteer(a_ByteBuffer); return true;
		case 0x18: HandleCraftRecipe(a_ByteBuffer); return true;
		case 0x19: HandlePacketPlayerAbilities(a_ByteBuffer); return true;
		case 0x1A: HandlePacketBlockDig(a_ByteBuffer); return true;
		case 0x1C: HandlePacketSteerVehicle(a_ByteBuffer); return true;
		case 0x1D: HandlePacketCraftingBookData(a_ByteBuffer); return true;
		case 0x1E: HandlePacketNameItem(a_ByteBuffer); return true;
		case 0x1F: HandlePacketResourcePackStatus(a_ByteBuffer); return true;
		case 0x20: HandlePacketAdvancementTab(a_ByteBuffer); return true;
		case 0x22: HandlePacketSetBeaconEffect(a_ByteBuffer); return true;
		case 0x23: HandlePacketSlotSelect(a_ByteBuffer); return true;
		case 0x26: HandlePacketCreativeInventoryAction(a_ByteBuffer); return true;
		case 0x2C: HandlePacketBlockPlace(a_ByteBuffer); return true;
		case 0x2D: HandlePacketUseItem(a_ByteBuffer); return true;

		default: break;
	}

	return Super::HandlePacket(a_ByteBuffer, a_PacketType);
}





void cProtocol_1_14::HandlePacketBlockDig(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEUInt8, UInt8, Status);

	int BlockX, BlockY, BlockZ;
	if (!a_ByteBuffer.ReadXZYPosition64(BlockX, BlockY, BlockZ))
	{
		return;
	}

	HANDLE_READ(a_ByteBuffer, ReadVarInt, Int32, Face);
	m_Client->HandleLeftClick(BlockX, BlockY, BlockZ, FaceIntToBlockFace(Face), Status);
}





void cProtocol_1_14::HandlePacketBlockPlace(cByteBuffer & a_ByteBuffer)
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

	m_Client->HandleRightClick(BlockX, BlockY, BlockZ, FaceIntToBlockFace(Face), FloorC(CursorX * 16), FloorC(CursorY * 16), FloorC(CursorZ * 16), Hand == 0);
}





void cProtocol_1_14::HandlePacketUpdateSign(cByteBuffer & a_ByteBuffer)
{
}
