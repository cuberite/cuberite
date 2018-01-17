
// CtrDrbgContext.h

// Declares the cCtrDrbgContext class representing a wrapper over CTR-DRBG implementation in mbedTLS





#pragma once

#include "mbedtls/ctr_drbg.h"





// fwd: EntropyContext.h
class cEntropyContext;





class cCtrDrbgContext
{
	friend class cSslConfig;
	friend class cRsaPrivateKey;
	friend class cCryptoKey;

public:
	/** Constructs the context with a new entropy context. */
	cCtrDrbgContext(void);

	/** Constructs the context with the specified entropy context. */
	cCtrDrbgContext(const std::shared_ptr<cEntropyContext> & a_EntropyContext);

	/** Initializes the context.
	a_Custom is optional additional data to use for entropy, nullptr is accepted.
	Returns 0 if successful, mbedTLS error code on failure. */
	int Initialize(const void * a_Custom, size_t a_CustomSize);

	/** Returns true if the object is valid (has been initialized properly) */
	bool IsValid(void) const { return m_IsValid; }

protected:
	/** The entropy source used for generating the random */
	std::shared_ptr<cEntropyContext> m_EntropyContext;

	/** The random generator context */
	mbedtls_ctr_drbg_context m_CtrDrbg;

	/** Set to true if the object is valid (has been initialized properly) */
	bool m_IsValid;


	/** Returns the internal context ptr. Only use in mbedTLS API calls. */
	mbedtls_ctr_drbg_context * GetInternal(void) { return &m_CtrDrbg; }
} ;




