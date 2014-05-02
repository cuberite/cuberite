
// AesCfb128Decryptor.cpp

// Implements the cAesCfb128Decryptor class decrypting data using AES CFB-128

#include "Globals.h"
#include "AesCfb128Decryptor.h"





cAesCfb128Decryptor::cAesCfb128Decryptor(void) :
	m_IVOffset(0),
	m_IsValid(false)
{
}





cAesCfb128Decryptor::~cAesCfb128Decryptor()
{
	// Clear the leftover in-memory data, so that they can't be accessed by a backdoor
	memset(&m_Aes, 0, sizeof(m_Aes));
}





void cAesCfb128Decryptor::Init(const Byte a_Key[16], const Byte a_IV[16])
{
	ASSERT(!IsValid());  // Cannot Init twice
	
	memcpy(m_IV, a_IV, 16);
	aes_setkey_enc(&m_Aes, a_Key, 128);
	m_IsValid = true;
}





void cAesCfb128Decryptor::ProcessData(Byte * a_DecryptedOut, const Byte * a_EncryptedIn, size_t a_Length)
{
	ASSERT(IsValid());  // Must Init() first
	
	// PolarSSL doesn't support AES-CFB8, need to implement it manually:
	for (size_t i = 0; i < a_Length; i++)
	{
		Byte Buffer[sizeof(m_IV)];
		aes_crypt_ecb(&m_Aes, AES_ENCRYPT, m_IV, Buffer);
		for (size_t idx = 0; idx < sizeof(m_IV) - 1; idx++)
		{
			m_IV[idx] = m_IV[idx + 1];
		}
		m_IV[sizeof(m_IV) - 1] = a_EncryptedIn[i];
		a_DecryptedOut[i] = a_EncryptedIn[i] ^ Buffer[0];
	}
}





