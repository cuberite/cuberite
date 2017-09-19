
// AesCfb128Encryptor.cpp

// Implements the cAesCfb128Encryptor class encrypting data using AES CFB-128

#include "Globals.h"
#include "AesCfb128Encryptor.h"





cAesCfb128Encryptor::cAesCfb128Encryptor(void):
	m_IsValid(false)
{
	mbedtls_aes_init(&m_Aes);
}





cAesCfb128Encryptor::~cAesCfb128Encryptor()
{
	// Clear the leftover in-memory data, so that they can't be accessed by a backdoor
	mbedtls_aes_free(&m_Aes);
}





void cAesCfb128Encryptor::Init(const Byte a_Key[16], const Byte a_IV[16])
{
	ASSERT(!IsValid());  // Cannot Init twice

	memcpy(m_IV, a_IV, 16);
	mbedtls_aes_setkey_enc(&m_Aes, a_Key, 128);
	m_IsValid = true;
}





void cAesCfb128Encryptor::ProcessData(Byte * a_EncryptedOut, const Byte * a_PlainIn, size_t a_Length)
{
	ASSERT(IsValid());  // Must Init() first
	mbedtls_aes_crypt_cfb8(&m_Aes, MBEDTLS_AES_ENCRYPT, a_Length, m_IV, a_PlainIn, a_EncryptedOut);
}





