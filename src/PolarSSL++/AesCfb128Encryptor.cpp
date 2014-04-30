
// AesCfb128Encryptor.cpp

// Implements the cAesCfb128Encryptor class encrypting data using AES CFB-128

#include "Globals.h"
#include "AesCfb128Encryptor.h"





cAesCfb128Encryptor::cAesCfb128Encryptor(void) :
	m_IVOffset(0),
	m_IsValid(false)
{
}





cAesCfb128Encryptor::~cAesCfb128Encryptor()
{
	// Clear the leftover in-memory data, so that they can't be accessed by a backdoor
	memset(&m_Aes, 0, sizeof(m_Aes));
}





void cAesCfb128Encryptor::Init(const Byte a_Key[16], const Byte a_IV[16])
{
	ASSERT(!IsValid());  // Cannot Init twice
	ASSERT(m_IVOffset == 0);
	
	memcpy(m_IV, a_IV, 16);
	aes_setkey_enc(&m_Aes, a_Key, 128);
	m_IsValid = true;
}





void cAesCfb128Encryptor::ProcessData(Byte * a_EncryptedOut, const Byte * a_PlainIn, size_t a_Length)
{
	ASSERT(IsValid());  // Must Init() first
	
	// PolarSSL doesn't do AES-CFB8, so we need to implement it ourselves:
	for (size_t i = 0; i < a_Length; i++)
	{
		Byte Buffer[sizeof(m_IV)];
		aes_crypt_ecb(&m_Aes, AES_ENCRYPT, m_IV, Buffer);
		for (size_t idx = 0; idx < sizeof(m_IV) - 1; idx++)
		{
			m_IV[idx] = m_IV[idx + 1];
		}
		a_EncryptedOut[i] = a_PlainIn[i] ^ Buffer[0];
		m_IV[sizeof(m_IV) - 1] = a_EncryptedOut[i];
	}
}





