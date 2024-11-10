
// RsaPrivateKey.cpp

#include "Globals.h"
#include "RsaPrivateKey.h"
#include "mbedtls/pk.h"





cRsaPrivateKey::cRsaPrivateKey(void)
{
	mbedtls_rsa_init(&m_Rsa);
	m_CtrDrbg.Initialize("RSA", 3);
}





cRsaPrivateKey::cRsaPrivateKey(const cRsaPrivateKey & a_Other)
{
	mbedtls_rsa_init(&m_Rsa);
	mbedtls_rsa_copy(&m_Rsa, &a_Other.m_Rsa);
	m_CtrDrbg.Initialize("RSA", 3);
}





cRsaPrivateKey::~cRsaPrivateKey()
{
	mbedtls_rsa_free(&m_Rsa);
}





bool cRsaPrivateKey::Generate(unsigned a_KeySizeBits)
{
	int res = mbedtls_rsa_gen_key(&m_Rsa, mbedtls_ctr_drbg_random, m_CtrDrbg.GetInternal(), a_KeySizeBits, 65537);
	if (res != 0)
	{
		LOG("RSA key generation failed: -0x%x", -res);
		return false;
	}

	return true;
}





ContiguousByteBuffer cRsaPrivateKey::GetPubKeyDER(void)
{
	class cPubKey
	{
	public:
		cPubKey(mbedtls_rsa_context * a_Rsa) :
			m_IsValid(false)
		{
			mbedtls_pk_init(&m_Key);
			if (mbedtls_pk_setup(&m_Key, mbedtls_pk_info_from_type(MBEDTLS_PK_RSA)) != 0)
			{
				ASSERT(!"Cannot init PrivKey context");
				return;
			}
			if (mbedtls_rsa_copy(mbedtls_pk_rsa(m_Key), a_Rsa) != 0)
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
				mbedtls_pk_free(&m_Key);
			}
		}

		operator mbedtls_pk_context * (void) { return &m_Key; }

	protected:
		bool m_IsValid;
		mbedtls_pk_context m_Key;
	} PkCtx(&m_Rsa);

	unsigned char buf[3000];
	int res = mbedtls_pk_write_pubkey_der(PkCtx, buf, sizeof(buf));
	if (res < 0)
	{
		return {};
	}
	return { reinterpret_cast<const std::byte *>(buf + sizeof(buf) - res), static_cast<size_t>(res) };
}





int cRsaPrivateKey::Decrypt(const ContiguousByteBufferView a_EncryptedData, Byte * a_DecryptedData, size_t a_DecryptedMaxLength)
{
	const auto KeyLength = mbedtls_rsa_get_len(&m_Rsa);
	if (a_EncryptedData.size() < KeyLength)
	{
		LOGD("%s: Invalid a_EncryptedLength: got %zu, exp at least %zu", __FUNCTION__, a_EncryptedData.size(), KeyLength);
		ASSERT(!"Invalid a_DecryptedMaxLength!");
		return -1;
	}
	if (a_DecryptedMaxLength < KeyLength)
	{
		LOGD("%s: Invalid a_DecryptedMaxLength: got %zu, exp at least %zu", __FUNCTION__, a_DecryptedMaxLength, KeyLength);
		ASSERT(!"Invalid a_DecryptedMaxLength!");
		return -1;
	}
	size_t DecryptedLength;
	int res = mbedtls_rsa_pkcs1_decrypt(
		&m_Rsa, mbedtls_ctr_drbg_random, m_CtrDrbg.GetInternal(), &DecryptedLength,
		reinterpret_cast<const unsigned char *>(a_EncryptedData.data()), a_DecryptedData, a_DecryptedMaxLength
	);
	if (res != 0)
	{
		return -1;
	}
	return static_cast<int>(DecryptedLength);
}
