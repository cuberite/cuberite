
// RsaPrivateKey.cpp

#include "Globals.h"
#include "RsaPrivateKey.h"
#include "CtrDrbgContext.h"
#include "polarssl/pk.h"






cRsaPrivateKey::cRsaPrivateKey(void)
{
	rsa_init(&m_Rsa, RSA_PKCS_V15, 0);
	m_CtrDrbg.Initialize("RSA", 3);
}





cRsaPrivateKey::cRsaPrivateKey(const cRsaPrivateKey & a_Other)
{
	rsa_init(&m_Rsa, RSA_PKCS_V15, 0);
	rsa_copy(&m_Rsa, &a_Other.m_Rsa);
	m_CtrDrbg.Initialize("RSA", 3);
}





cRsaPrivateKey::~cRsaPrivateKey()
{
	rsa_free(&m_Rsa);
}





bool cRsaPrivateKey::Generate(unsigned a_KeySizeBits)
{
	int res = rsa_gen_key(&m_Rsa, ctr_drbg_random, m_CtrDrbg.GetInternal(), a_KeySizeBits, 65537);
	if (res != 0)
	{
		LOG("RSA key generation failed: -0x%x", -res);
		return false;
	}

	return true;
}





AString cRsaPrivateKey::GetPubKeyDER(void)
{
	class cPubKey
	{
	public:
		cPubKey(rsa_context * a_Rsa) :
			m_IsValid(false)
		{
			pk_init(&m_Key);
			if (pk_init_ctx(&m_Key, pk_info_from_type(POLARSSL_PK_RSA)) != 0)
			{
				ASSERT(!"Cannot init PrivKey context");
				return;
			}
			if (rsa_copy(pk_rsa(m_Key), a_Rsa) != 0)
			{
				ASSERT(!"Cannot copy PrivKey to PK context");
				return;
			}
			m_IsValid = true;
		}
		
		~cPubKey()
		{
			if (m_IsValid)
			{
				pk_free(&m_Key);
			}
		}
		
		operator pk_context * (void) { return &m_Key; }
		
	protected:
		bool m_IsValid;
		pk_context m_Key;
	} PkCtx(&m_Rsa);
	
	unsigned char buf[3000];
	int res = pk_write_pubkey_der(PkCtx, buf, sizeof(buf));
	if (res < 0)
	{
		return AString();
	}
	return AString(reinterpret_cast<const char *>(buf + sizeof(buf) - res), static_cast<size_t>(res));
}





int cRsaPrivateKey::Decrypt(const Byte * a_EncryptedData, size_t a_EncryptedLength, Byte * a_DecryptedData, size_t a_DecryptedMaxLength)
{
	if (a_EncryptedLength < m_Rsa.len)
	{
		LOGD("%s: Invalid a_EncryptedLength: got %u, exp at least %u",
			__FUNCTION__, (unsigned)a_EncryptedLength, (unsigned)(m_Rsa.len)
		);
		ASSERT(!"Invalid a_DecryptedMaxLength!");
		return -1;
	}
	if (a_DecryptedMaxLength < m_Rsa.len)
	{
		LOGD("%s: Invalid a_DecryptedMaxLength: got %u, exp at least %u",
			__FUNCTION__, (unsigned)a_EncryptedLength, (unsigned)(m_Rsa.len)
		);
		ASSERT(!"Invalid a_DecryptedMaxLength!");
		return -1;
	}
	size_t DecryptedLength;
	int res = rsa_pkcs1_decrypt(
		&m_Rsa, ctr_drbg_random, m_CtrDrbg.GetInternal(), RSA_PRIVATE, &DecryptedLength,
		a_EncryptedData, a_DecryptedData, a_DecryptedMaxLength
	);
	if (res != 0)
	{
		return -1;
	}
	return static_cast<int>(DecryptedLength);
}





int cRsaPrivateKey::Encrypt(const Byte * a_PlainData, size_t a_PlainLength, Byte * a_EncryptedData, size_t a_EncryptedMaxLength)
{
	if (a_EncryptedMaxLength < m_Rsa.len)
	{
		LOGD("%s: Invalid a_EncryptedMaxLength: got %u, exp at least %u",
			__FUNCTION__, (unsigned)a_EncryptedMaxLength, (unsigned)(m_Rsa.len)
		);
		ASSERT(!"Invalid a_DecryptedMaxLength!");
		return -1;
	}
	if (a_PlainLength < m_Rsa.len)
	{
		LOGD("%s: Invalid a_PlainLength: got %u, exp at least %u",
			__FUNCTION__, (unsigned)a_PlainLength, (unsigned)(m_Rsa.len)
		);
		ASSERT(!"Invalid a_PlainLength!");
		return -1;
	}
	int res = rsa_pkcs1_encrypt(
		&m_Rsa, ctr_drbg_random, m_CtrDrbg.GetInternal(), RSA_PRIVATE,
		a_PlainLength, a_PlainData, a_EncryptedData
	);
	if (res != 0)
	{
		return -1;
	}
	return static_cast<int>(m_Rsa.len);
}





