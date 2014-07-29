
// AesCfb128Decryptor.h

// Declares the cAesCfb128Decryptor class decrypting data using AES CFB-128





#pragma once

#include "polarssl/aes.h"





/** Decrypts data using the AES / CFB 128 algorithm */
class cAesCfb128Decryptor
{
public:
	
	cAesCfb128Decryptor(void);
	~cAesCfb128Decryptor();
	
	/** Initializes the decryptor with the specified Key / IV */
	void Init(const Byte a_Key[16], const Byte a_IV[16]);
	
	/** Decrypts a_Length bytes of the encrypted data; produces a_Length output bytes */
	void ProcessData(Byte * a_DecryptedOut, const Byte * a_EncryptedIn, size_t a_Length);
	
	/** Returns true if the object has been initialized with the Key / IV */
	bool IsValid(void) const { return m_IsValid; }
	
protected:
	aes_context m_Aes;
	
	/** The InitialVector, used by the CFB mode decryption */
	Byte m_IV[16];
	
	/** Current offset in the m_IV, used by the CFB mode decryption */
	size_t m_IVOffset;
	
	/** Indicates whether the object has been initialized with the Key / IV */
	bool m_IsValid;
} ;





