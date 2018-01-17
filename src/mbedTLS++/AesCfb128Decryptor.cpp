
// AesCfb128Decryptor.cpp

// Implements the cAesCfb128Decryptor class decrypting data using AES CFB-128

#include "Globals.h"
#include "AesCfb128Decryptor.h"





cAesCfb128Decryptor::cAesCfb128Decryptor(void):
	m_IsValid(false)
{
	mbedtls_aes_init(&m_Aes);
}





cAesCfb128Decryptor::~cAesCfb128Decryptor()
{
	// Clear the leftover in-memory data, so that they can't be accessed by a backdoor
	mbedtls_aes_free(&m_Aes);
}





void cAesCfb128Decryptor::Init(const Byte a_Key[16], const Byte a_IV[16])
{
	ASSERT(!IsValid());  // Cannot Init twice

	memcpy(m_IV, a_IV, 16);
	mbedtls_aes_setkey_enc(&m_Aes, a_Key, 128);
	m_IsValid = true;
}





void cAesCfb128Decryptor::ProcessData(Byte * a_DecryptedOut, const Byte * a_EncryptedIn, size_t a_Length)
{
	ASSERT(IsValid());  // Must Init() first
	mbedtls_aes_crypt_cfb8(&m_Aes, MBEDTLS_AES_DECRYPT, a_Length, m_IV, a_EncryptedIn, a_DecryptedOut);
}





