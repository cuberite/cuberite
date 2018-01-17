
// EntropyContext.h

// Declares the cEntropyContext class representing a wrapper over entropy contexts in mbedTLS





#pragma once

#include "mbedtls/entropy.h"





class cEntropyContext
{
	friend class cCtrDrbgContext;
public:
	cEntropyContext(void);
	~cEntropyContext();

protected:
	mbedtls_entropy_context m_Entropy;
} ;




