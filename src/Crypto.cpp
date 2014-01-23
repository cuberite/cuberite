
// Crypto.cpp

// Implements classes that wrap the cryptographic code library

#include "Globals.h"
#include "Crypto.h"

#include "polarssl/pk.h"





/*
// Self-test the hash formatting for known values:
// sha1(Notch) : 4ed1f46bbe04bc756bcb17c0c7ce3e4632f06a48
// sha1(jeb_)  : -7c9d5b0044c130109a5d7b5fb5c317c02b4e28c1
// sha1(simon) : 88e16a1019277b15d58faf0541e11910eb756f6

class Test
{
public:
	Test(void)
	{
		AString DigestNotch, DigestJeb, DigestSimon;
		Byte Digest[20];
		cSHA1Checksum Checksum;
		Checksum.Update((const Byte *)"Notch", 5);
		Checksum.Finalize(Digest);
		cSHA1Checksum::DigestToJava(Digest, DigestNotch);
		Checksum.Restart();
		Checksum.Update((const Byte *)"jeb_", 4);
		Checksum.Finalize(Digest);
		cSHA1Checksum::DigestToJava(Digest, DigestJeb);
		Checksum.Restart();
		Checksum.Update((const Byte *)"simon", 5);
		Checksum.Finalize(Digest);
		cSHA1Checksum::DigestToJava(Digest, DigestSimon);
		printf("Notch: \"%s\"\n", DigestNotch.c_str());
		printf("jeb_:  \"%s\"\n",  DigestJeb.c_str());
		printf("simon: \"%s\"\n", DigestSimon.c_str());
		assert(DigestNotch == "4ed1f46bbe04bc756bcb17c0c7ce3e4632f06a48");
		assert(DigestJeb   == "-7c9d5b0044c130109a5d7b5fb5c317c02b4e28c1");
		assert(DigestSimon == "88e16a1019277b15d58faf0541e11910eb756f6");
	}
} test;
*/






///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cRSAPrivateKey:

cRSAPrivateKey::cRSAPrivateKey(void)
{
	rsa_init(&m_Rsa, RSA_PKCS_V15, 0);
	InitRnd();
}





cRSAPrivateKey::cRSAPrivateKey(const cRSAPrivateKey & a_Other)
{
	rsa_init(&m_Rsa, RSA_PKCS_V15, 0);
	rsa_copy(&m_Rsa, &a_Other.m_Rsa);
	InitRnd();
}





cRSAPrivateKey::~cRSAPrivateKey()
{
	entropy_free(&m_Entropy);
	rsa_free(&m_Rsa);
}





void cRSAPrivateKey::InitRnd(void)
{
	entropy_init(&m_Entropy);
	const unsigned char pers[] = "rsa_genkey";
	ctr_drbg_init(&m_Ctr_drbg, entropy_func, &m_Entropy, pers, sizeof(pers) - 1);
}





bool cRSAPrivateKey::Generate(unsigned a_KeySizeBits)
{
	if (rsa_gen_key(&m_Rsa, ctr_drbg_random, &m_Ctr_drbg, a_KeySizeBits, 65537) != 0)
	{
		// Key generation failed
		return false;
	}

	return true;
}





AString cRSAPrivateKey::GetPubKeyDER(void)
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
	return AString((const char *)(buf + sizeof(buf) - res), (size_t)res);
}





int cRSAPrivateKey::Decrypt(const Byte * a_EncryptedData, size_t a_EncryptedLength, Byte * a_DecryptedData, size_t a_DecryptedMaxLength)
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
		&m_Rsa, ctr_drbg_random, &m_Ctr_drbg, RSA_PRIVATE, &DecryptedLength,
		a_EncryptedData, a_DecryptedData, a_DecryptedMaxLength
	);
	if (res != 0)
	{
		return -1;
	}
	return (int)DecryptedLength;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cAESCFBDecryptor:

cAESCFBDecryptor::cAESCFBDecryptor(void) :
	m_IsValid(false),
	m_IVOffset(0)
{
}





cAESCFBDecryptor::~cAESCFBDecryptor()
{
	// Clear the leftover in-memory data, so that they can't be accessed by a backdoor
	memset(&m_Aes, 0, sizeof(m_Aes));
}





void cAESCFBDecryptor::Init(const Byte a_Key[16], const Byte a_IV[16])
{
	ASSERT(!IsValid());  // Cannot Init twice
	
	memcpy(m_IV, a_IV, 16);
	aes_setkey_enc(&m_Aes, a_Key, 128);
	m_IsValid = true;
}





void cAESCFBDecryptor::ProcessData(Byte * a_DecryptedOut, const Byte * a_EncryptedIn, size_t a_Length)
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





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cAESCFBEncryptor:

cAESCFBEncryptor::cAESCFBEncryptor(void) :
	m_IsValid(false),
	m_IVOffset(0)
{
}





cAESCFBEncryptor::~cAESCFBEncryptor()
{
	// Clear the leftover in-memory data, so that they can't be accessed by a backdoor
	memset(&m_Aes, 0, sizeof(m_Aes));
}





void cAESCFBEncryptor::Init(const Byte a_Key[16], const Byte a_IV[16])
{
	ASSERT(!IsValid());  // Cannot Init twice
	ASSERT(m_IVOffset == 0);
	
	memcpy(m_IV, a_IV, 16);
	aes_setkey_enc(&m_Aes, a_Key, 128);
	m_IsValid = true;
}





void cAESCFBEncryptor::ProcessData(Byte * a_EncryptedOut, const Byte * a_PlainIn, size_t a_Length)
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





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cSHA1Checksum:

cSHA1Checksum::cSHA1Checksum(void) :
	m_DoesAcceptInput(true)
{
	sha1_starts(&m_Sha1);
}





void cSHA1Checksum::Update(const Byte * a_Data, size_t a_Length)
{
	ASSERT(m_DoesAcceptInput);  // Not Finalize()-d yet, or Restart()-ed
	
	sha1_update(&m_Sha1, a_Data, a_Length);
}





void cSHA1Checksum::Finalize(cSHA1Checksum::Checksum & a_Output)
{
	ASSERT(m_DoesAcceptInput);  // Not Finalize()-d yet, or Restart()-ed
	
	sha1_finish(&m_Sha1, a_Output);
	m_DoesAcceptInput = false;
}





void cSHA1Checksum::DigestToJava(const Checksum & a_Digest, AString & a_Out)
{
	Checksum Digest;
	memcpy(Digest, a_Digest, sizeof(Digest));
	
	bool IsNegative = (Digest[0] >= 0x80);
	if (IsNegative)
	{
		// Two's complement:
		bool carry = true;  // Add one to the whole number
		for (int i = 19; i >= 0; i--)
		{
			Digest[i] = ~Digest[i];
			if (carry)
			{
				carry = (Digest[i] == 0xff);
				Digest[i]++;
			}
		}
	}
	a_Out.clear();
	a_Out.reserve(40);
	for (int i = 0; i < 20; i++)
	{
		AppendPrintf(a_Out, "%02x", Digest[i]);
	}
	while ((a_Out.length() > 0) && (a_Out[0] == '0'))
	{
		a_Out.erase(0, 1);
	}
	if (IsNegative)
	{
		a_Out.insert(0, "-");
	}
}






void cSHA1Checksum::Restart(void)
{
	sha1_starts(&m_Sha1);
	m_DoesAcceptInput = true;
}




