
// AesCfb128Decryptor.cpp

// Implements the cAesCfb128Decryptor class decrypting data using AES CFB-128

#include "Globals.h"
#include "AesCfb128Decryptor.h"





cAesCfb128Decryptor::cAesCfb128Decryptor(void) :
	m_IsValid(false)
{
#if PLATFORM_CRYPTOGRAPHY && defined(_WIN32)
	if (!CryptAcquireContext(&m_Aes, nullptr, nullptr, PROV_RSA_AES, CRYPT_VERIFYCONTEXT))
	{
		throw std::system_error(GetLastError(), std::system_category());
	}
#else
	mbedtls_aes_init(&m_Aes);
#endif
}





cAesCfb128Decryptor::~cAesCfb128Decryptor()
{
	// Clear the leftover in-memory data, so that they can't be accessed by a backdoor:
#if PLATFORM_CRYPTOGRAPHY && defined(_WIN32)
	CryptReleaseContext(m_Aes, 0);
#else
	mbedtls_aes_free(&m_Aes);
#endif
}





void cAesCfb128Decryptor::Init(const Byte a_Key[16], const Byte a_IV[16])
{
	ASSERT(!IsValid());  // Cannot Init twice

#if PLATFORM_CRYPTOGRAPHY && defined(_WIN32)
	struct Key
	{
		PUBLICKEYSTRUC Header;
		DWORD Length;
		Byte Key[16];
	} Key;

	const DWORD Mode = CRYPT_MODE_CFB;
	Key.Header = { PLAINTEXTKEYBLOB, CUR_BLOB_VERSION, 0, CALG_AES_128 };
	Key.Length = 16;
	std::copy_n(a_Key, 16, Key.Key);

	CryptImportKey(m_Aes, reinterpret_cast<const BYTE *>(&Key), sizeof(Key), 0, 0, &m_Key);
	CryptSetKeyParam(m_Key, KP_MODE, reinterpret_cast<const BYTE *>(&Mode), 0);
	CryptSetKeyParam(m_Key, KP_IV, a_IV, 0);
#else
	std::copy_n(a_IV, 16, m_IV);
	mbedtls_aes_setkey_enc(&m_Aes, a_Key, 128);
#endif

	m_IsValid = true;
}





void cAesCfb128Decryptor::ProcessData(std::byte * const a_EncryptedIn, const size_t a_Length)
{
	ASSERT(IsValid());  // Must Init() first

#if PLATFORM_CRYPTOGRAPHY && defined(_WIN32)
	ASSERT(a_Length <= std::numeric_limits<DWORD>::max());

	DWORD Length = static_cast<DWORD>(a_Length);
	CryptDecrypt(m_Key, 0, FALSE, 0, reinterpret_cast<BYTE *>(a_EncryptedIn), &Length);
#else
	mbedtls_aes_crypt_cfb8(&m_Aes, MBEDTLS_AES_DECRYPT, a_Length, m_IV, reinterpret_cast<unsigned char *>(a_EncryptedIn), reinterpret_cast<unsigned char *>(a_EncryptedIn));
#endif
}
