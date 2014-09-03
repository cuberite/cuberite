
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
#include "Protocol18x.h"

#include "../ClientHandle.h"
#include "../CompositeChat.h"
#include "../Root.h"
#include "../Server.h"
#include "../World.h"

#include "../Entities/Player.h"

class cProtocol176;





const int MAX_ENC_LEN = 512;  // Maximum size of the encrypted message; should be 128, but who knows...





////////////////////////////////////////////////////////////////////////////////
// cProtocol180:

cProtocol180::cProtocol180(cClientHandle * a_Client, const AString & a_ServerAddress, UInt16 a_ServerPort, UInt32 a_State) :
	super(a_Client, a_ServerAddress, a_ServerPort, a_State)
{
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

	cPacketizer Pkt(*this, 0x22);  // Multi Block Change packet
	Pkt.WriteInt(a_ChunkX);
	Pkt.WriteInt(a_ChunkZ);
	Pkt.WriteVarInt((UInt32)a_Changes.size());
	for (sSetBlockVector::const_iterator itr = a_Changes.begin(), end = a_Changes.end(); itr != end; ++itr)
	{
		short Coords = itr->y | (itr->z << 8) | (itr->x << 12);
		Pkt.WriteShort(Coords);

		UInt32 Block = ((UInt32)itr->BlockType << 4) | ((UInt32)itr->BlockMeta & 15);
		Pkt.WriteVarInt(Block);
	}  // for itr - a_Changes[]
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
	
	cPacketizer Pkt(*this, 0x04);  // Entity Equipment packet
	Pkt.WriteVarInt((UInt32)a_Entity.GetUniqueID());
	Pkt.WriteShort(a_SlotNum);
	Pkt.WriteItem(a_Item);
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




