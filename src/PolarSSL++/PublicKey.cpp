
// PublicKey.cpp

// Implements the cPublicKey class representing a RSA public key in PolarSSL

#include "Globals.h"
#include "PublicKey.h"





cPublicKey::cPublicKey(const AString & a_PublicKeyDER)
{
	pk_init(&m_Pk);
	if (pk_parse_public_key(&m_Pk, (const Byte *)a_PublicKeyDER.data(), a_PublicKeyDER.size()) != 0)
	{
		ASSERT(!"Cannot parse PubKey");
		return;
	}
	m_CtrDrbg.Initialize("rsa_pubkey", 10);
}





cPublicKey::~cPublicKey()
{
	pk_free(&m_Pk);
}





int cPublicKey::Decrypt(const Byte * a_EncryptedData, size_t a_EncryptedLength, Byte * a_DecryptedData, size_t a_DecryptedMaxLength)
{
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





int cPublicKey::Encrypt(const Byte * a_PlainData, size_t a_PlainLength, Byte * a_EncryptedData, size_t a_EncryptedMaxLength)
{
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





