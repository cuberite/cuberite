
// EntropyContext.h

// Declares the cEntropyContext class representing a wrapper over entropy contexts in PolarSSL





#pragma once

#include "polarssl/entropy.h"





class cEntropyContext
{
	friend class cCtrDrbgContext;
public:
	cEntropyContext(void);
	~cEntropyContext();
	
protected:
	entropy_context m_Entropy;
} ;




