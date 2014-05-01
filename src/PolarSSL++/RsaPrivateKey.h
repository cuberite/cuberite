
// RsaPrivateKey.h

// Declares the cRsaPrivateKey class representing a private key for RSA operations.





#pragma once

#include "CtrDrbgContext.h"
#include "polarssl/rsa.h"





/** Encapsulates an RSA private key used in PKI cryptography */
class cRsaPrivateKey
{
public:
	/** Creates a new empty object, the key is not assigned */
	cRsaPrivateKey(void);
	
	/** Deep-copies the key from a_Other */
	cRsaPrivateKey(const cRsaPrivateKey & a_Other);
	
	~cRsaPrivateKey();
	
	/** Generates a new key within this object, with the specified size in bits.
	Returns true on success, false on failure. */
	bool Generate(unsigned a_KeySizeBits = 1024);
	
	/** Returns the public key part encoded in ASN1 DER encoding */
	AString GetPubKeyDER(void);
	
	/** Decrypts the data using RSAES-PKCS#1 algorithm.
	Both a_EncryptedData and a_DecryptedData must be at least <KeySizeBytes> bytes large.
	Returns the number of bytes decrypted, or negative number for error. */
	int Decrypt(const Byte * a_EncryptedData, size_t a_EncryptedLength, Byte * a_DecryptedData, size_t a_DecryptedMaxLength);
	
	/** Encrypts the data using RSAES-PKCS#1 algorithm.
	Both a_EncryptedData and a_DecryptedData must be at least <KeySizeBytes> bytes large.
	Returns the number of bytes decrypted, or negative number for error. */
	int Encrypt(const Byte * a_PlainData, size_t a_PlainLength, Byte * a_EncryptedData, size_t a_EncryptedMaxLength);
	
protected:
	/** The PolarSSL key context */
	rsa_context m_Rsa;

	/** The random generator used for generating the key and encryption / decryption */
	cCtrDrbgContext m_CtrDrbg;
} ;





