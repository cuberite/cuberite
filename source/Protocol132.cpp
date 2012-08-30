
// Protocol132.cpp

// Implements the cProtocol132 class representing the release 1.3.2 protocol (#39)

#include "Globals.h"
#include "Protocol132.h"
#include "cRoot.h"
#include "cServer.h"
#include "cClientHandle.h"
#include "CryptoPP/osrng.h"





#define HANDLE_PACKET_READ(Proc, Type, Var) \
	Type Var; \
	{ \
		if (!m_ReceivedData.Proc(Var)) \
		{ \
			return PARSE_INCOMPLETE; \
		} \
	}




typedef unsigned char Byte;





using namespace CryptoPP;





const int MAX_ENC_LEN = 512;  // Maximum size of the encrypted message; should be 128, but who knows...





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cProtocol132:

cProtocol132::cProtocol132(cClientHandle * a_Client) :
	super(a_Client),
	m_IsEncrypted(false)
{
}





void cProtocol132::DataReceived(const char * a_Data, int a_Size)
{
	if (m_IsEncrypted)
	{
		byte Decrypted[512];
		while (a_Size > 0)
		{
			int NumBytes = (a_Size > sizeof(Decrypted)) ? sizeof(Decrypted) : a_Size;
			m_Decryptor.ProcessData(Decrypted, (byte *)a_Data, NumBytes);
			super::DataReceived((const char *)Decrypted, NumBytes);
			a_Size -= NumBytes;
			a_Data += NumBytes;
		}
	}
	else
	{
		super::DataReceived(a_Data, a_Size);
	}
}





int cProtocol132::ParsePacket(unsigned char a_PacketType)
{
	switch (a_PacketType)
	{
		default:   return super::ParsePacket(a_PacketType);  // off-load previously known packets into cProtocol125
		case 0xcd: return ParseClientStatuses();
		case 0xfc: return ParseEncryptionKeyResponse();
	}
}





int cProtocol132::ParseHandshake(void)
{
	HANDLE_PACKET_READ(ReadByte,            Byte,    ProtocolVersion);
	HANDLE_PACKET_READ(ReadBEUTF16String16, AString, Username);
	HANDLE_PACKET_READ(ReadBEUTF16String16, AString, ServerHost);
	HANDLE_PACKET_READ(ReadBEInt,           int,     ServerPort);
	m_Username = Username;

	AString key;
	CryptoPP::StringSink sink(key);  // GCC won't allow inline instantiation in the following line, damned temporary refs
	cRoot::Get()->GetServer()->GetPublicKey().Save(sink);
	
	// Send a 0xFD Encryption Key Request http://wiki.vg/Protocol#Encryption_Key_Request_.280xFD.29
	WriteByte((char)0xfd);
	WriteString("MCServer");
	WriteShort((short)key.size());
	SendData(key.data(), key.size());
	WriteShort(4);
	WriteInt((int)this);  // Using 'this' as the cryptographic nonce, so that we don't have to generate one each time :)
	return PARSE_OK;
}





int cProtocol132::ParseLogin(void)
{
	// Login packet not used in 1.3.2
	return PARSE_ERROR;
}





int cProtocol132::ParseClientStatuses(void)
{
	HANDLE_PACKET_READ(ReadByte, byte, Status);
	
	// DEBUG:
	// Kick the client, we don't have all the packets yet and sending wrong packets makes the client freeze
	// m_Client->Kick("I don't speak your language (yet)");
	
	// TODO: 
	// m_Client->HandleLogin(39, m_Username);
	
	return PARSE_OK;
}





int cProtocol132::ParseEncryptionKeyResponse(void)
{
	HANDLE_PACKET_READ(ReadBEShort, short, EncKeyLength);
	AString EncKey;
	if (!m_ReceivedData.ReadString(EncKey, EncKeyLength))
	{
		return PARSE_INCOMPLETE;
	}
	HANDLE_PACKET_READ(ReadBEShort, short, EncNonceLength);
	AString EncNonce;
	if (!m_ReceivedData.ReadString(EncNonce, EncNonceLength))
	{
		return PARSE_INCOMPLETE;
	}
	if ((EncKeyLength > MAX_ENC_LEN) || (EncNonceLength > MAX_ENC_LEN))
	{
		LOGD("Too long encryption");
		m_Client->Kick("Hacked client");
		return PARSE_OK;
	}
	
	HandleEncryptionKeyResponse(EncKey, EncNonce);
	return PARSE_OK;
}





void cProtocol132::SendData(const char * a_Data, int a_Size)
{
	if (m_IsEncrypted)
	{
		byte Encrypted[1024];  // Larger buffer, we may be sending lots of data (chunks)
		while (a_Size > 0)
		{
			int NumBytes = (a_Size > sizeof(Encrypted)) ? sizeof(Encrypted) : a_Size;
			m_Encryptor.ProcessData(Encrypted, (byte *)a_Data, NumBytes);
			super::SendData((const char *)Encrypted, NumBytes);
			a_Size -= NumBytes;
			a_Data += NumBytes;
		}
	}
	else
	{
		super::SendData(a_Data, a_Size);
	}
}





void cProtocol132::HandleEncryptionKeyResponse(const AString & a_EncKey, const AString & a_EncNonce)
{
	// Decrypt EncNonce using privkey
	RSAES<PKCS1v15>::Decryptor rsaDecryptor(cRoot::Get()->GetServer()->GetPrivateKey());
	AutoSeededRandomPool rng;
	byte DecryptedNonce[MAX_ENC_LEN];
	DecodingResult res = rsaDecryptor.Decrypt(rng, (const byte *)a_EncNonce.data(), a_EncNonce.size(), DecryptedNonce);
	if (!res.isValidCoding || (res.messageLength != 4))
	{
		LOGD("Bad nonce length");
		m_Client->Kick("Hacked client");
		return;
	}
	if (ntohl(*((int *)DecryptedNonce)) != (unsigned)this)
	{
		LOGD("Bad nonce value");
		m_Client->Kick("Hacked client");
		return;
	}
	
	// Decrypt the symmetric encryption key using privkey:
	byte DecryptedKey[MAX_ENC_LEN];
	res = rsaDecryptor.Decrypt(rng, (const byte *)a_EncKey.data(), a_EncKey.size(), DecryptedKey);
	if (!res.isValidCoding || (res.messageLength != 16))
	{
		LOGD("Bad key length");
		m_Client->Kick("Hacked client");
		return;
	}
	
	// Send encryption key response:
	WriteByte((char)0xfc);
	WriteShort(0);
	WriteShort(0);
	
	StartEncryption(DecryptedKey);
	return;
}





void cProtocol132::StartEncryption(const byte * a_Key)
{
	m_Encryptor.SetKey(a_Key, 16, MakeParameters(Name::IV(), ConstByteArrayParameter(a_Key, 16))(Name::FeedbackSize(), 1));
	m_Decryptor.SetKey(a_Key, 16, MakeParameters(Name::IV(), ConstByteArrayParameter(a_Key, 16))(Name::FeedbackSize(), 1));
	m_IsEncrypted = true;
}




