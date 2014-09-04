
// Protocol18x.cpp

/*
Implements the 1.8.x protocol classes:
	- cProtocol180
		- release 1.8.0 protocol (#47)
(others may be added later in the future for the 1.8 release series)
*/

#include "Globals.h"
#include "Bindings/PluginManager.h"
#include "json/json.h"
#include "ChunkDataSerializer.h"
#include "Protocol18x.h"

#include "../ClientHandle.h"
#include "../CompositeChat.h"
#include "../Root.h"
#include "../Server.h"
#include "../World.h"

#include "../Entities/Player.h"
#include "Entities/Painting.h"





#define HANDLE_READ(ByteBuf, Proc, Type, Var) \
	Type Var; \
	if (!ByteBuf.Proc(Var))\
	{\
		return;\
	}





const int MAX_ENC_LEN = 512;  // Maximum size of the encrypted message; should be 128, but who knows...
class cProtocol176;





////////////////////////////////////////////////////////////////////////////////
// cProtocol180:

cProtocol180::cProtocol180(cClientHandle * a_Client, const AString & a_ServerAddress, UInt16 a_ServerPort, UInt32 a_State) :
	super(a_Client, a_ServerAddress, a_ServerPort, a_State)
{
}





void cProtocol180::SendLoginSuccess(void)
{
	ASSERT(m_State == 2);  // State: login?

	// Disable compression:
	{
		cPacketizer Pkt(*this, 0x03);  // Set compression packet
		Pkt.WriteVarInt(-1);
	}

	{
		cPacketizer Pkt(*this, 0x02);  // Login success packet
		Pkt.WriteString(cMojangAPI::MakeUUIDDashed(m_Client->GetUUID()));
		Pkt.WriteString(m_Client->GetUsername());
	}

	m_State = 3;  // State = Game
}





void cProtocol180::SendParticleEffect(const AString & a_ParticleName, float a_SrcX, float a_SrcY, float a_SrcZ, float a_OffsetX, float a_OffsetY, float a_OffsetZ, float a_ParticleData, int a_ParticleAmmount)
{
	ASSERT(m_State == 3);  // In game mode?
	
	/*cPacketizer Pkt(*this, 0x2A);
	Pkt.WriteString(a_ParticleName);
	Pkt.WriteFloat(a_SrcX);
	Pkt.WriteFloat(a_SrcY);
	Pkt.WriteFloat(a_SrcZ);
	Pkt.WriteFloat(a_OffsetX);
	Pkt.WriteFloat(a_OffsetY);
	Pkt.WriteFloat(a_OffsetZ);
	Pkt.WriteFloat(a_ParticleData);
	Pkt.WriteInt(a_ParticleAmmount);*/
}





void cProtocol180::SendPlayerMoveLook(void)
{
	ASSERT(m_State == 3);  // In game mode?
	
	/*cPacketizer Pkt(*this, 0x08);  // Player Position And Look packet
	cPlayer * Player = m_Client->GetPlayer();
	Pkt.WriteDouble(Player->GetPosX());
	
	// The "+ 0.001" is there because otherwise the player falls through the block they were standing on.
	Pkt.WriteDouble(Player->GetStance() + 0.001);
	
	Pkt.WriteDouble(Player->GetPosZ());
	Pkt.WriteFloat((float)Player->GetYaw());
	Pkt.WriteFloat((float)Player->GetPitch());
	Pkt.WriteByte(0);*/
}





void cProtocol180::SendUseBed(const cEntity & a_Entity, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	ASSERT(m_State == 3);  // In game mode?
	
	cPacketizer Pkt(*this, 0x0a);
	Pkt.WriteVarInt(a_Entity.GetUniqueID());
	Pkt.WritePosition(Vector3i(a_BlockX, a_BlockY, a_BlockZ));
}





void cProtocol180::SendCollectEntity(const cEntity & a_Entity, const cPlayer & a_Player)
{
	ASSERT(m_State == 3);  // In game mode?
	
	cPacketizer Pkt(*this, 0x0d);  // Collect Item packet
	Pkt.WriteVarInt(a_Entity.GetUniqueID());
	Pkt.WriteVarInt(a_Player.GetUniqueID());
}





void cProtocol180::SendEntityVelocity(const cEntity & a_Entity)
{
	ASSERT(m_State == 3);  // In game mode?
	
	/*cPacketizer Pkt(*this, 0x12);  // Entity Velocity packet
	Pkt.WriteVarInt(a_Entity.GetUniqueID());
	// 400 = 8000 / 20 ... Conversion from our speed in m/s to 8000 m/tick
	Pkt.WriteShort((short)(a_Entity.GetSpeedX() * 400));
	Pkt.WriteShort((short)(a_Entity.GetSpeedY() * 400));
	Pkt.WriteShort((short)(a_Entity.GetSpeedZ() * 400));*/
}





void cProtocol180::SendDestroyEntity(const cEntity & a_Entity)
{
	ASSERT(m_State == 3);  // In game mode?
	
	/*cPacketizer Pkt(*this, 0x13);  // Destroy Entities packet
	Pkt.WriteVarInt(1);
	Pkt.WriteVarInt(a_Entity.GetUniqueID());*/
}





void cProtocol180::SendEntityRelMove(const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ)
{
	ASSERT(m_State == 3);  // In game mode?
	
	/*cPacketizer Pkt(*this, 0x15);  // Entity Relative Move packet
	Pkt.WriteVarInt(a_Entity.GetUniqueID());
	Pkt.WriteByte(a_RelX);
	Pkt.WriteByte(a_RelY);
	Pkt.WriteByte(a_RelZ);
	Pkt.WriteBool(true);  // TODO: IsOnGround() on entities*/
}





void cProtocol180::SendEntityRelMoveLook(const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ)
{
	ASSERT(m_State == 3);  // In game mode?
	
	/*cPacketizer Pkt(*this, 0x17);  // Entity Look And Relative Move packet
	Pkt.WriteVarInt(a_Entity.GetUniqueID());
	Pkt.WriteByte(a_RelX);
	Pkt.WriteByte(a_RelY);
	Pkt.WriteByte(a_RelZ);
	Pkt.WriteByteAngle(a_Entity.GetYaw());
	Pkt.WriteByteAngle(a_Entity.GetPitch());
	Pkt.WriteBool(true);  // TODO: IsOnGround() on entities*/
}





void cProtocol180::SendEntityLook(const cEntity & a_Entity)
{
	ASSERT(m_State == 3);  // In game mode?
	
	/*cPacketizer Pkt(*this, 0x16);  // Entity Look packet
	Pkt.WriteVarInt(a_Entity.GetUniqueID());
	Pkt.WriteByteAngle(a_Entity.GetYaw());
	Pkt.WriteByteAngle(a_Entity.GetPitch());
	Pkt.WriteBool(true);  // TODO: IsOnGround() on entities*/
}





void cProtocol180::SendTeleportEntity(const cEntity & a_Entity)
{
	ASSERT(m_State == 3);  // In game mode?
	
	/*cPacketizer Pkt(*this, 0x18);
	Pkt.WriteVarInt(a_Entity.GetUniqueID());
	Pkt.WriteFPInt(a_Entity.GetPosX());
	Pkt.WriteFPInt(a_Entity.GetPosY());
	Pkt.WriteFPInt(a_Entity.GetPosZ());
	Pkt.WriteByteAngle(a_Entity.GetYaw());
	Pkt.WriteByteAngle(a_Entity.GetPitch());
	Pkt.WriteBool(true);  // TODO: IsOnGrond() on entities*/
}





void cProtocol180::SendEntityHeadLook(const cEntity & a_Entity)
{
	ASSERT(m_State == 3);  // In game mode?
	
	/*cPacketizer Pkt(*this, 0x19);  // Entity Head Look packet
	Pkt.WriteVarInt((UInt32)a_Entity.GetUniqueID());
	Pkt.WriteByteAngle(a_Entity.GetHeadYaw());*/
}





void cProtocol180::SendEntityMetadata(const cEntity & a_Entity)
{
	ASSERT(m_State == 3);  // In game mode?
	
	/*cPacketizer Pkt(*this, 0x1c);  // Entity Metadata packet
	Pkt.WriteVarInt(a_Entity.GetUniqueID());
	Pkt.WriteEntityMetadata(a_Entity);
	Pkt.WriteByte(0x7f);  // The termination byte*/
}





void cProtocol180::SendEntityEffect(const cEntity & a_Entity, int a_EffectID, int a_Amplifier, short a_Duration)
{
	ASSERT(m_State == 3);  // In game mode?
	
	cPacketizer Pkt(*this, 0x1D);  // Entity Effect packet
	Pkt.WriteVarInt(a_Entity.GetUniqueID());
	Pkt.WriteByte(a_EffectID);
	Pkt.WriteByte(a_Amplifier);
	Pkt.WriteVarInt((UInt32)a_Duration);
	Pkt.WriteBool(false);  // Hide particles
}





void cProtocol180::SendRemoveEntityEffect(const cEntity & a_Entity, int a_EffectID)
{
	ASSERT(m_State == 3);  // In game mode?
	
	cPacketizer Pkt(*this, 0x1e);
	Pkt.WriteVarInt(a_Entity.GetUniqueID());
	Pkt.WriteByte(a_EffectID);
}





void cProtocol180::SendEntityProperties(const cEntity & a_Entity)
{
	ASSERT(m_State == 3);  // In game mode?
	
	cPacketizer Pkt(*this, 0x20);  // Entity Properties packet
	Pkt.WriteVarInt(a_Entity.GetUniqueID());
	Pkt.WriteEntityProperties(a_Entity);
}





void cProtocol180::SendKeepAlive(int a_PingID)
{
	// Drop the packet if the protocol is not in the Game state yet (caused a client crash):
	if (m_State != 3)
	{
		LOGWARNING("Trying to send a KeepAlive packet to a player who's not yet fully logged in (%d). The protocol class prevented the packet.", m_State);
		return;
	}
	
	cPacketizer Pkt(*this, 0x00);  // Keep Alive packet
	Pkt.WriteVarInt(a_PingID);
}





void cProtocol180::SendHealth(void)
{
	ASSERT(m_State == 3);  // In game mode?

	cPacketizer Pkt(*this, 0x06);  // Update Health packet
	cPlayer * Player = m_Client->GetPlayer();
	Pkt.WriteFloat((float)Player->GetHealth());
	Pkt.WriteVarInt((UInt32)Player->GetFoodLevel());
	Pkt.WriteFloat((float)Player->GetFoodSaturationLevel());
}





void cProtocol180::SendExperience(void)
{
	ASSERT(m_State == 3);  // In game mode?
	
	cPacketizer Pkt(*this, 0x1f);  // Experience Packet
	cPlayer * Player = m_Client->GetPlayer();
	Pkt.WriteFloat(Player->GetXpPercentage());
	Pkt.WriteVarInt((UInt32)Player->GetXpLevel());
	Pkt.WriteVarInt((UInt32)Player->GetCurrentXp());
}





void cProtocol180::SendPaintingSpawn(const cPainting & a_Painting)
{
	ASSERT(m_State == 3);  // In game mode?
	
	/*cPacketizer Pkt(*this, 0x10);  // Spawn Painting packet
	Pkt.WriteVarInt(a_Painting.GetUniqueID());
	Pkt.WriteString(a_Painting.GetName().c_str());
	Pkt.WritePosition(Vector3i(a_Painting.GetPosX(), a_Painting.GetPosY(), a_Painting.GetPosZ()));
	Pkt.WriteChar(a_Painting.GetDirection());*/
}





void cProtocol180::SendPluginMessage(const AString & a_Channel, const AString & a_Message)
{
	ASSERT(m_State == 3);  // In game mode?
	
	/*cPacketizer Pkt(*this, 0x3f);
	Pkt.WriteString(a_Channel);
	Pkt.WriteBuf(a_Message.data(), a_Message.size());*/
}





void cProtocol180::SendUnloadChunk(int a_ChunkX, int a_ChunkZ)
{
	ASSERT(m_State == 3);  // In game mode?
	
	cPacketizer Pkt(*this, 0x21);  // Chunk Data packet
	Pkt.WriteInt(a_ChunkX);
	Pkt.WriteInt(a_ChunkZ);
	Pkt.WriteBool(true);
	Pkt.WriteShort(0);  // Primary bitmap
	Pkt.WriteVarInt(0);  // Data size
}





void cProtocol180::SendChunkData(int a_ChunkX, int a_ChunkZ, cChunkDataSerializer & a_Serializer)
{
	ASSERT(m_State == 3);  // In game mode?

	// Serialize first, before creating the Packetizer (the packetizer locks a CS)
	// This contains the flags and bitmasks, too
	const AString & ChunkData = a_Serializer.Serialize(cChunkDataSerializer::RELEASE_1_8_0);

	cPacketizer Pkt(*this, 0x21);  // Chunk Data packet
	Pkt.WriteInt(a_ChunkX);
	Pkt.WriteInt(a_ChunkZ);
	Pkt.WriteBuf(ChunkData.data(), ChunkData.size());
}





void cProtocol180::SendBlockChange(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	ASSERT(m_State == 3);  // In game mode?
	
	cPacketizer Pkt(*this, 0x23);  // Block Change packet
	Pkt.WritePosition(Vector3i(a_BlockX, a_BlockY, a_BlockZ));

	UInt32 Block = ((UInt32)a_BlockType << 4) | ((UInt32)a_BlockMeta & 15);
	Pkt.WriteVarInt(Block);
}





void cProtocol180::SendBlockChanges(int a_ChunkX, int a_ChunkZ, const sSetBlockVector & a_Changes)
{
	ASSERT(m_State == 3);  // In game mode?

	/*cPacketizer Pkt(*this, 0x22);  // Multi Block Change packet
	Pkt.WriteInt(a_ChunkX);
	Pkt.WriteInt(a_ChunkZ);
	Pkt.WriteVarInt((UInt32)a_Changes.size());
	for (sSetBlockVector::const_iterator itr = a_Changes.begin(), end = a_Changes.end(); itr != end; ++itr)
	{
		short Coords = itr->y | (itr->z << 8) | (itr->x << 12);
		Pkt.WriteShort(Coords);

		UInt32 Block = ((UInt32)itr->BlockType << 4) | ((UInt32)itr->BlockMeta & 15);
		Pkt.WriteVarInt(Block);
	}  // for itr - a_Changes[]*/
}





void cProtocol180::SendChat(const AString & a_Message)
{
	ASSERT(m_State == 3);  // In game mode?
	
	cPacketizer Pkt(*this, 0x02);  // Chat Message packet
	Pkt.WriteString(Printf("{\"text\":\"%s\"}", EscapeString(a_Message).c_str()));
	Pkt.WriteChar(0);
}





void cProtocol180::SendChat(const cCompositeChat & a_Message)
{
	ASSERT(m_State == 3);  // In game mode?

	// Compose the complete Json string to send:
	Json::Value msg;
	cWorld * World = m_Client->GetPlayer()->GetWorld();
	msg["text"] = cClientHandle::FormatMessageType((World == NULL) ? false : World->ShouldUseChatPrefixes(), a_Message.GetMessageType(), a_Message.GetAdditionalMessageTypeData());  // The client crashes without this field being present
	const cCompositeChat::cParts & Parts = a_Message.GetParts();
	for (cCompositeChat::cParts::const_iterator itr = Parts.begin(), end = Parts.end(); itr != end; ++itr)
	{
		Json::Value Part;
		switch ((*itr)->m_PartType)
		{
			case cCompositeChat::ptText:
			{
				Part["text"] = (*itr)->m_Text;
				AddChatPartStyle(Part, (*itr)->m_Style);
				break;
			}
			
			case cCompositeChat::ptClientTranslated:
			{
				const cCompositeChat::cClientTranslatedPart & p = (const cCompositeChat::cClientTranslatedPart &)**itr;
				Part["translate"] = p.m_Text;
				Json::Value With;
				for (AStringVector::const_iterator itrW = p.m_Parameters.begin(), endW = p.m_Parameters.end(); itrW != endW; ++itr)
				{
					With.append(*itrW);
				}
				if (!p.m_Parameters.empty())
				{
					Part["with"] = With;
				}
				AddChatPartStyle(Part, p.m_Style);
				break;
			}
			
			case cCompositeChat::ptUrl:
			{
				const cCompositeChat::cUrlPart & p = (const cCompositeChat::cUrlPart &)**itr;
				Part["text"] = p.m_Text;
				Json::Value Url;
				Url["action"] = "open_url";
				Url["value"] = p.m_Url;
				Part["clickEvent"] = Url;
				AddChatPartStyle(Part, p.m_Style);
				break;
			}
			
			case cCompositeChat::ptSuggestCommand:
			case cCompositeChat::ptRunCommand:
			{
				const cCompositeChat::cCommandPart & p = (const cCompositeChat::cCommandPart &)**itr;
				Part["text"] = p.m_Text;
				Json::Value Cmd;
				Cmd["action"] = (p.m_PartType == cCompositeChat::ptRunCommand) ? "run_command" : "suggest_command";
				Cmd["value"] = p.m_Command;
				Part["clickEvent"] = Cmd;
				AddChatPartStyle(Part, p.m_Style);
				break;
			}

			case cCompositeChat::ptShowAchievement:
			{
				const cCompositeChat::cShowAchievementPart & p = (const cCompositeChat::cShowAchievementPart &)**itr;
				Part["translate"] = "chat.type.achievement";

				Json::Value Ach;
				Ach["action"] = "show_achievement";
				Ach["value"] = p.m_Text;
				
				Json::Value AchColourAndName;
				AchColourAndName["color"] = "green";
				AchColourAndName["translate"] = p.m_Text;
				AchColourAndName["hoverEvent"] = Ach;

				Json::Value Extra;
				Extra.append(AchColourAndName);

				Json::Value Name;
				Name["text"] = p.m_PlayerName;

				Json::Value With;
				With.append(Name);
				With.append(Extra);

				Part["with"] = With;
				AddChatPartStyle(Part, p.m_Style);
				break;
			}
		}
		msg["extra"].append(Part);
	}  // for itr - Parts[]
	
	// Send the message to the client:
	cPacketizer Pkt(*this, 0x02);
	Pkt.WriteString(msg.toStyledString());
	Pkt.WriteChar(0);
}





void cProtocol180::SendEntityEquipment(const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item)
{
	ASSERT(m_State == 3);  // In game mode?
	
	/*cPacketizer Pkt(*this, 0x04);  // Entity Equipment packet
	Pkt.WriteVarInt((UInt32)a_Entity.GetUniqueID());
	Pkt.WriteShort(a_SlotNum);
	Pkt.WriteItem(a_Item);*/
}





void cProtocol180::SendLogin(const cPlayer & a_Player, const cWorld & a_World)
{
	// Send the Join Game packet:
	{
		cServer * Server = cRoot::Get()->GetServer();
		cPacketizer Pkt(*this, 0x01);  // Join Game packet
		Pkt.WriteInt(a_Player.GetUniqueID());
		Pkt.WriteByte((Byte)a_Player.GetEffectiveGameMode() | (Server->IsHardcore() ? 0x08 : 0));  // Hardcore flag bit 4
		Pkt.WriteChar((char)a_World.GetDimension());
		Pkt.WriteByte(2);  // TODO: Difficulty (set to Normal)
		Pkt.WriteByte(std::min(Server->GetMaxPlayers(), 60));
		Pkt.WriteString("default");  // Level type - wtf?
		Pkt.WriteBool(false);  // Reduced Debug Info - wtf?
	}
	m_LastSentDimension = a_World.GetDimension();
	
	// Send the spawn position:
	{
		cPacketizer Pkt(*this, 0x05);  // Spawn Position packet
		Vector3i Position(a_World.GetSpawnX(), a_World.GetSpawnY(), a_World.GetSpawnZ());
		Pkt.WritePosition(Position);
	}
	
	// Send player abilities:
	SendPlayerAbilities();
}





void cProtocol180::HandlePacketClientStatus(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadChar, char, ActionID);
	switch (ActionID)
	{
		case 0:
		{
			// Respawn
			m_Client->HandleRespawn();
			break;
		}
		case 1:
		{
			// Request stats
			const cStatManager & Manager = m_Client->GetPlayer()->GetStatManager();
			SendStatistics(Manager);

			break;
		}
		case 2:
		{
			// Open Inventory achievement
			m_Client->GetPlayer()->AwardAchievement(achOpenInv);
			break;
		}
	}
}





void cProtocol180::HandlePacketPluginMessage(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Channel);
	AString Data;
	if (!a_ByteBuffer.ReadString(Data, a_ByteBuffer.GetReadableSpace() - 1))
	{
		return;
	}
	m_Client->HandlePluginMessage(Channel, Data);
}





void cProtocol180::HandlePacketStatusRequest(cByteBuffer & a_ByteBuffer)
{
	cServer * Server = cRoot::Get()->GetServer();
	AString ServerDescription = Server->GetDescription();
	int NumPlayers = Server->GetNumPlayers();
	int MaxPlayers = Server->GetMaxPlayers();
	AString Favicon = Server->GetFaviconData();
	cRoot::Get()->GetPluginManager()->CallHookServerPing(*m_Client, ServerDescription, NumPlayers, MaxPlayers, Favicon);

	// Version:
	Json::Value Version;
	Version["name"] = "1.8";
	Version["protocol"] = 47;

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





void cProtocol180::HandlePacketLoginStart(cByteBuffer & a_ByteBuffer)
{
	AString Username;
	if (!a_ByteBuffer.ReadVarUTF8String(Username))
	{
		m_Client->Kick("Bad username");
		return;
	}
	
	if (!m_Client->HandleHandshake(Username))
	{
		// The client is not welcome here, they have been sent a Kick packet already
		return;
	}
	
	cServer * Server = cRoot::Get()->GetServer();
	// If auth is required, then send the encryption request:
	if (Server->ShouldAuthenticate())
	{
		cPacketizer Pkt(*this, 0x01);
		Pkt.WriteString(Server->GetServerID());
		const AString & PubKeyDer = Server->GetPublicKeyDER();
		Pkt.WriteVarInt((short)PubKeyDer.size());
		Pkt.WriteBuf(PubKeyDer.data(), PubKeyDer.size());
		Pkt.WriteVarInt(4);
		Pkt.WriteInt((int)(intptr_t)this);  // Using 'this' as the cryptographic nonce, so that we don't have to generate one each time :)
		m_Client->SetUsername(Username);
		return;
	}
	
	m_Client->HandleLogin(4, Username);
}





void cProtocol180::HandlePacketLoginEncryptionResponse(cByteBuffer & a_ByteBuffer)
{
	UInt32 EncKeyLength, EncNonceLength;
	a_ByteBuffer.ReadVarInt(EncKeyLength);
	AString EncKey;
	if (!a_ByteBuffer.ReadString(EncKey, EncKeyLength))
	{
		return;
	}
	a_ByteBuffer.ReadVarInt(EncNonceLength);
	AString EncNonce;
	if (!a_ByteBuffer.ReadString(EncNonce, EncNonceLength))
	{
		return;
	}
	if ((EncKeyLength > MAX_ENC_LEN) || (EncNonceLength > MAX_ENC_LEN))
	{
		LOGD("Too long encryption");
		m_Client->Kick("Hacked client");
		return;
	}

	// Decrypt EncNonce using privkey
	cRsaPrivateKey & rsaDecryptor = cRoot::Get()->GetServer()->GetPrivateKey();
	Int32 DecryptedNonce[MAX_ENC_LEN / sizeof(Int32)];
	int res = rsaDecryptor.Decrypt((const Byte *)EncNonce.data(), EncNonce.size(), (Byte *)DecryptedNonce, sizeof(DecryptedNonce));
	if (res != 4)
	{
		LOGD("Bad nonce length: got %d, exp %d", res, 4);
		m_Client->Kick("Hacked client");
		return;
	}
	if (ntohl(DecryptedNonce[0]) != (unsigned)(uintptr_t)this)
	{
		LOGD("Bad nonce value");
		m_Client->Kick("Hacked client");
		return;
	}
	
	// Decrypt the symmetric encryption key using privkey:
	Byte DecryptedKey[MAX_ENC_LEN];
	res = rsaDecryptor.Decrypt((const Byte *)EncKey.data(), EncKey.size(), DecryptedKey, sizeof(DecryptedKey));
	if (res != 16)
	{
		LOGD("Bad key length");
		m_Client->Kick("Hacked client");
		return;
	}
	
	StartEncryption(DecryptedKey);
	m_Client->HandleLogin(4, m_Client->GetUsername());
}





void cProtocol180::HandlePacketSteerVehicle(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, Forward);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, Sideways);
	HANDLE_READ(a_ByteBuffer, ReadChar,    char,  Flags);

	if ((Flags & 0x2) != 0)
	{
		m_Client->HandleUnmount();
	}
	else if ((Flags & 0x1) != 0)
	{
		m_Client->HandleSteerVehicle(Forward, Sideways);
	}
}





void cProtocol180::HandlePacketEntityAction(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarInt, UInt32,  PlayerID);
	HANDLE_READ(a_ByteBuffer, ReadChar,   char,    Action);
	HANDLE_READ(a_ByteBuffer, ReadVarInt, UInt32,  JumpBoost);

	switch (Action)
	{
		case 0: m_Client->HandleEntityCrouch(PlayerID, true);     break;  // Crouch
		case 1: m_Client->HandleEntityCrouch(PlayerID, false);    break;  // Uncrouch
		case 2: m_Client->HandleEntityLeaveBed(PlayerID);         break;  // Leave Bed
		case 3: m_Client->HandleEntitySprinting(PlayerID, true);  break;  // Start sprinting
		case 4: m_Client->HandleEntitySprinting(PlayerID, false); break;  // Stop sprinting
	}
}





void cProtocol180::HandlePacketUseEntity(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarInt, UInt32, EntityID);
	HANDLE_READ(a_ByteBuffer, ReadVarInt, UInt32, Type);

	switch (Type)
	{
		case 0:
		{
			m_Client->HandleUseEntity((int)EntityID, false);
			break;
		}
		case 1:
		{
			m_Client->HandleUseEntity((int)EntityID, true);
			break;
		}
		case 2:
		{
			HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, TargetX);
			HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, TargetY);
			HANDLE_READ(a_ByteBuffer, ReadBEFloat, float, TargetZ);

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





void cProtocol180::HandlePacketKeepAlive(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarInt, UInt32, KeepAliveID);
	m_Client->HandleKeepAlive((int)KeepAliveID);
}





void cProtocol180::HandlePacketPlayerPos(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, PosX);
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, PosY);
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, PosZ);
	HANDLE_READ(a_ByteBuffer, ReadBool,     bool,   IsOnGround);
	m_Client->HandlePlayerPos(PosX, PosY, PosZ, PosY + 1.62, IsOnGround);
}





void cProtocol180::HandlePacketPlayerPosLook(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, PosX);
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, PosY);
	HANDLE_READ(a_ByteBuffer, ReadBEDouble, double, PosZ);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat,  float,  Yaw);
	HANDLE_READ(a_ByteBuffer, ReadBEFloat,  float,  Pitch);
	HANDLE_READ(a_ByteBuffer, ReadBool,     bool,   IsOnGround);
	m_Client->HandlePlayerMoveLook(PosX, PosY, PosZ, PosY + 1.62, Yaw, Pitch, IsOnGround);
}





void cProtocol180::HandlePacketClientSettings(cByteBuffer & a_ByteBuffer)
{
	HANDLE_READ(a_ByteBuffer, ReadVarUTF8String, AString, Locale);
	HANDLE_READ(a_ByteBuffer, ReadByte,          Byte,    ViewDistance);
	HANDLE_READ(a_ByteBuffer, ReadByte,          Byte,    ChatFlags);
	HANDLE_READ(a_ByteBuffer, ReadByte,          Byte,    ChatColors);
	HANDLE_READ(a_ByteBuffer, ReadChar,          char,    SkinFlags);
	
	m_Client->SetLocale(Locale);
	// TODO: Handle other values
}




