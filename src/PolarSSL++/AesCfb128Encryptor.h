
// AesCfb128Encryptor.h

// Declares the cAesCfb128Encryptor class encrypting data using AES CFB-128





#pragma once

#include "polarssl/aes.h"





/** Encrypts data using the AES / CFB (128) algorithm */
class cAesCfb128Encryptor
{
public:
	cAesCfb128Encryptor(void);
	~cAesCfb128Encryptor();
	
	/** Initializes the decryptor with the specified Key / IV */
	void Init(const Byte a_Key[16], const Byte a_IV[16]);
	
	/** Encrypts a_Length bytes of the plain data; produces a_Length output bytes */
	void ProcessData(Byte * a_EncryptedOut, const Byte * a_PlainIn, size_t a_Length);
	
	/** Returns true if the object has been initialized with the Key / IV */
	bool IsValid(void) const { return m_IsValid; }
	
protected:
	aes_context m_Aes;
	
	/** The InitialVector, used by the CFB mode encryption */
	Byte m_IV[16];
	
	/** Current offset in the m_IV, used by the CFB mode encryption */
	size_t m_IVOffset;
	
	/** Indicates whether the object has been initialized with the Key / IV */
	bool m_IsValid;
} ;





