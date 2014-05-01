
// PublicKey.h

// Declares the cPublicKey class representing a RSA public key in PolarSSL





#pragma once

#include "CtrDrbgContext.h"
#include "polarssl/pk.h"





class cPublicKey
{
public:
	/** Constructs the public key out of the DER-encoded pubkey data */
	cPublicKey(const AString & a_PublicKeyDER);
	
	~cPublicKey();
	
	/** Decrypts the data using the stored public key
	Both a_EncryptedData and a_DecryptedData must be at least <KeySizeBytes> bytes large.
	Returns the number of bytes decrypted, or negative number for error. */
	int Decrypt(const Byte * a_EncryptedData, size_t a_EncryptedLength, Byte * a_DecryptedData, size_t a_DecryptedMaxLength);
	
	/** Encrypts the data using the stored public key
	Both a_EncryptedData and a_DecryptedData must be at least <KeySizeBytes> bytes large.
	Returns the number of bytes decrypted, or negative number for error. */
	int Encrypt(const Byte * a_PlainData, size_t a_PlainLength, Byte * a_EncryptedData, size_t a_EncryptedMaxLength);

protected:
	/** The public key PolarSSL representation */
	pk_context m_Pk;
	
	/** The random generator used in encryption and decryption */
	cCtrDrbgContext m_CtrDrbg;
} ;





