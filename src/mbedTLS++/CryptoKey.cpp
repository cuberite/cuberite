
// CryptoKey.cpp

// Implements the cCryptoKey class representing a RSA public key in mbedTLS

#include "Globals.h"
#include "CryptoKey.h"





cCryptoKey::cCryptoKey(void)
{
	mbedtls_pk_init(&m_Pk);
	m_CtrDrbg.Initialize("rsa_pubkey", 10);
}





cCryptoKey::cCryptoKey(const AString & a_PublicKeyData)
{
	mbedtls_pk_init(&m_Pk);
	m_CtrDrbg.Initialize("rsa_pubkey", 10);
	int res = ParsePublic(a_PublicKeyData.data(), a_PublicKeyData.size());
	if (res != 0)
	{
		LOGWARNING("Failed to parse public key: -0x%x", res);
		ASSERT(!"Cannot parse PubKey");
		return;
	}
}





cCryptoKey::cCryptoKey(const AString & a_PrivateKeyData, const AString & a_Password)
{
	mbedtls_pk_init(&m_Pk);
	m_CtrDrbg.Initialize("rsa_privkey", 11);
	int res = ParsePrivate(a_PrivateKeyData.data(), a_PrivateKeyData.size(), a_Password);
	if (res != 0)
	{
		LOGWARNING("Failed to parse private key: -0x%x", res);
		ASSERT(!"Cannot parse PrivKey");
		return;
	}
}





cCryptoKey::~cCryptoKey()
{
	mbedtls_pk_free(&m_Pk);
}





int cCryptoKey::Decrypt(const Byte * a_EncryptedData, size_t a_EncryptedLength, Byte * a_DecryptedData, size_t a_DecryptedMaxLength)
{
	ASSERT(IsValid());

	size_t DecryptedLen = a_DecryptedMaxLength;
	int res = mbedtls_pk_decrypt(&m_Pk,
		a_EncryptedData, a_EncryptedLength,
		a_DecryptedData, &DecryptedLen, a_DecryptedMaxLength,
		mbedtls_ctr_drbg_random, m_CtrDrbg.GetInternal()
	);
	if (res != 0)
	{
		return res;
	}
	return static_cast<int>(DecryptedLen);
}





int cCryptoKey::Encrypt(const Byte * a_PlainData, size_t a_PlainLength, Byte * a_EncryptedData, size_t a_EncryptedMaxLength)
{
	ASSERT(IsValid());

	size_t EncryptedLength = a_EncryptedMaxLength;
	int res = mbedtls_pk_encrypt(&m_Pk,
		a_PlainData, a_PlainLength, a_EncryptedData, &EncryptedLength, a_EncryptedMaxLength,
		mbedtls_ctr_drbg_random, m_CtrDrbg.GetInternal()
	);
	if (res != 0)
	{
		return res;
	}
	return static_cast<int>(EncryptedLength);
}






int cCryptoKey::ParsePublic(const void * a_Data, size_t a_NumBytes)
{
	ASSERT(!IsValid());  // Cannot parse a second key

	return mbedtls_pk_parse_public_key(&m_Pk, reinterpret_cast<const unsigned char *>(a_Data), a_NumBytes);
}






int cCryptoKey::ParsePrivate(const void * a_Data, size_t a_NumBytes, const AString & a_Password)
{
	ASSERT(!IsValid());  // Cannot parse a second key
	// mbedTLS requires that PEM-encoded data is passed including the terminating NUL byte,
	// and DER-encoded data is decoded properly even with an extra trailing NUL byte, so we simply add one to everything:
	AString keyData(reinterpret_cast<const char *>(a_Data), a_NumBytes);

	if (a_Password.empty())
	{
		return mbedtls_pk_parse_key(&m_Pk, reinterpret_cast<const unsigned char *>(keyData.data()), a_NumBytes + 1, nullptr, 0);
	}
	else
	{
		return mbedtls_pk_parse_key(
			&m_Pk,
			reinterpret_cast<const unsigned char *>(keyData.data()), a_NumBytes + 1,
			reinterpret_cast<const unsigned char *>(a_Password.c_str()), a_Password.size()
		);
	}
}





bool cCryptoKey::IsValid(void) const
{
	return (mbedtls_pk_get_type(&m_Pk) != MBEDTLS_PK_NONE);
}




