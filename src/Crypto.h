
// Crypto.h

// Declares classes that wrap the cryptographic code library





#pragma once

#include "polarssl/rsa.h"
#include "polarssl/aes.h"
#include "polarssl/entropy.h"
#include "polarssl/ctr_drbg.h"
#include "polarssl/sha1.h"
#include "polarssl/pk.h"





/** Encapsulates an RSA private key used in PKI cryptography */
class cRSAPrivateKey
{
public:
	/** Creates a new empty object, the key is not assigned */
	cRSAPrivateKey(void);
	
	/** Deep-copies the key from a_Other */
	cRSAPrivateKey(const cRSAPrivateKey & a_Other);
	
	~cRSAPrivateKey();
	
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
	rsa_context m_Rsa;
	entropy_context m_Entropy;
	ctr_drbg_context m_Ctr_drbg;
	
	/** Initializes the m_Entropy and m_Ctr_drbg contexts
	Common part of this object's construction, called from all constructors. */
	void InitRnd(void);
} ;





class cPublicKey
{
public:
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
	pk_context m_Pk;
	entropy_context m_Entropy;
	ctr_drbg_context m_Ctr_drbg;
	
	/** Initializes the m_Entropy and m_Ctr_drbg contexts
	Common part of this object's construction, called from all constructors. */
	void InitRnd(void);
} ;





/** Decrypts data using the AES / CFB (128) algorithm */
class cAESCFBDecryptor
{
public:
	Byte test;
	
	cAESCFBDecryptor(void);
	~cAESCFBDecryptor();
	
	/** Initializes the decryptor with the specified Key / IV */
	void Init(const Byte a_Key[16], const Byte a_IV[16]);
	
	/** Decrypts a_Length bytes of the encrypted data; produces a_Length output bytes */
	void ProcessData(Byte * a_DecryptedOut, const Byte * a_EncryptedIn, size_t a_Length);
	
	/** Returns true if the object has been initialized with the Key / IV */
	bool IsValid(void) const { return m_IsValid; }
	
protected:
	aes_context m_Aes;
	
	/** The InitialVector, used by the CFB mode decryption */
	Byte m_IV[16];
	
	/** Current offset in the m_IV, used by the CFB mode decryption */
	size_t m_IVOffset;
	
	/** Indicates whether the object has been initialized with the Key / IV */
	bool m_IsValid;
} ;





/** Encrypts data using the AES / CFB (128) algorithm */
class cAESCFBEncryptor
{
public:
	Byte test;
	
	cAESCFBEncryptor(void);
	~cAESCFBEncryptor();
	
	/** Initializes the decryptor with the specified Key / IV */
	void Init(const Byte a_Key[16], const Byte a_IV[16]);
	
	/** Encrypts a_Length bytes of the plain data; produces a_Length output bytes */
	void ProcessData(Byte * a_EncryptedOut, const Byte * a_PlainIn, size_t a_Length);
	
	/** Returns true if the object has been initialized with the Key / IV */
	bool IsValid(void) const { return m_IsValid; }
	
protected:
	aes_context m_Aes;
	
	/** The InitialVector, used by the CFB mode encryption */
	Byte m_IV[16];
	
	/** Current offset in the m_IV, used by the CFB mode encryption */
	size_t m_IVOffset;
	
	/** Indicates whether the object has been initialized with the Key / IV */
	bool m_IsValid;
} ;





/** Calculates a SHA1 checksum for data stream */
class cSHA1Checksum
{
public:
	typedef Byte Checksum[20];  // The type used for storing the checksum
	
	cSHA1Checksum(void);
	
	/** Adds the specified data to the checksum */
	void Update(const Byte * a_Data, size_t a_Length);
	
	/** Calculates and returns the final checksum */
	void Finalize(Checksum & a_Output);
	
	/** Returns true if the object is accepts more input data, false if Finalize()-d (need to Restart()) */
	bool DoesAcceptInput(void) const { return m_DoesAcceptInput; }
	
	/** Converts a raw 160-bit SHA1 digest into a Java Hex representation
	According to http://wiki.vg/wiki/index.php?title=Protocol_Encryption&oldid=2802
	*/
	static void DigestToJava(const Checksum & a_Digest, AString & a_JavaOut);
	
	/** Clears the current context and start a new checksum calculation */
	void Restart(void);
	
protected:
	/** True if the object is accepts more input data, false if Finalize()-d (need to Restart()) */
	bool m_DoesAcceptInput;
	
	sha1_context m_Sha1;
} ;




