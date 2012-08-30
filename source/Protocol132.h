
// Protocol132.h

// Interfaces to the cProtocol132 class representing the release 1.3.2 protocol (#39)





#pragma once

#include "Protocol125.h"
#include "CryptoPP/modes.h"
#include "CryptoPP/aes.h"





class cProtocol132 :
	public cProtocol125
{
	typedef cProtocol125 super;
public:

	cProtocol132(cClientHandle * a_Client);

	/// Called when client sends some data:
	virtual void DataReceived(const char * a_Data, int a_Size) override;

	/// Handling of the additional packets:
	virtual int ParsePacket(unsigned char a_PacketType) override;
	
	// Modified packets:
	virtual int ParseHandshake(void) override;
	virtual int ParseLogin    (void) override;
	
	// New packets:
	virtual int ParseClientStatuses       (void);
	virtual int ParseEncryptionKeyResponse(void);
	
	virtual void SendData(const char * a_Data, int a_Size);
	
protected:
	bool m_IsEncrypted;
	CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption m_Decryptor;  // ((byte*)sDecryptedSharedSecret.c_str(),(unsigned int)16, IV, 1);
	CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption m_Encryptor;
	
	/// Decrypts the key and nonce, checks nonce, starts the symmetric encryption
	void HandleEncryptionKeyResponse(const AString & a_EncKey, const AString & a_EncNonce);
	
	/// Starts the symmetric encryption with the specified key
	void StartEncryption(const byte * a_Key);
} ;




