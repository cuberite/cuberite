
// CryptoKey.cpp

// Implements the cCryptoKey class representing a RSA public key in PolarSSL

#include "Globals.h"
#include "CryptoKey.h"





cCryptoKey::cCryptoKey(void)
{
	pk_init(&m_Pk);
	m_CtrDrbg.Initialize("rsa_pubkey", 10);
}





cCryptoKey::cCryptoKey(const AString & a_PublicKeyData)
{
	pk_init(&m_Pk);
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
	pk_init(&m_Pk);
	m_CtrDrbg.Initialize("rsa_privkey", 11);
	int res = ParsePrivate(a_PrivateKeyData.data(), a_PrivateKeyData.size(), a_Password);
	if (res != 0)
	{
		LOGWARNING("Failed to parse private key: -0x%x", res);
		ASSERT(!"Cannot parse PubKey");
		return;
	}
}





cCryptoKey::~cCryptoKey()
{
	pk_free(&m_Pk);
}





int cCryptoKey::Decrypt(const Byte * a_EncryptedData, size_t a_EncryptedLength, Byte * a_DecryptedData, size_t a_DecryptedMaxLength)
{
	ASSERT(IsValid());
	
	size_t DecryptedLen = a_DecryptedMaxLength;
	int res = pk_decrypt(&m_Pk,
		a_EncryptedData, a_EncryptedLength,
		a_DecryptedData, &DecryptedLen, a_DecryptedMaxLength,
		ctr_drbg_random, m_CtrDrbg.GetInternal()
	);
	if (res != 0)
	{
		return res;
	}
	return (int)DecryptedLen;
}





int cCryptoKey::Encrypt(const Byte * a_PlainData, size_t a_PlainLength, Byte * a_EncryptedData, size_t a_EncryptedMaxLength)
{
	ASSERT(IsValid());
	
	size_t EncryptedLength = a_EncryptedMaxLength;
	int res = pk_encrypt(&m_Pk,
		a_PlainData, a_PlainLength, a_EncryptedData, &EncryptedLength, a_EncryptedMaxLength,
		ctr_drbg_random, m_CtrDrbg.GetInternal()
	);
	if (res != 0)
	{
		return res;
	}
	return (int)EncryptedLength;
}






int cCryptoKey::ParsePublic(const void * a_Data, size_t a_NumBytes)
{
	ASSERT(!IsValid());  // Cannot parse a second key
	
	return pk_parse_public_key(&m_Pk, (const unsigned char *)a_Data, a_NumBytes);
}






int cCryptoKey::ParsePrivate(const void * a_Data, size_t a_NumBytes, const AString & a_Password)
{
	ASSERT(!IsValid());  // Cannot parse a second key
	
	if (a_Password.empty())
	{
		return pk_parse_key(&m_Pk, (const unsigned char *)a_Data, a_NumBytes, NULL, 0);
	}
	else
	{
		return pk_parse_key(
			&m_Pk,
			(const unsigned char *)a_Data, a_NumBytes,
			(const unsigned char *)a_Password.c_str(), a_Password.size()
		);
	}
}





bool cCryptoKey::IsValid(void) const
{
	return (pk_get_type(&m_Pk) != POLARSSL_PK_NONE);
}




