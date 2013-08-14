#ifndef CRYPTOPP_RC6_H
#define CRYPTOPP_RC6_H

/** \file
*/

#include "seckey.h"
#include "secblock.h"

NAMESPACE_BEGIN(CryptoPP)

//! _
struct RC6_Info : public FixedBlockSize<16>, public VariableKeyLength<16, 0, 255>, public VariableRounds<20>
{
	static const char *StaticAlgorithmName() {return "RC6";}
	typedef word32 RC6_WORD;
};

/// <a href="http://www.weidai.com/scan-mirror/cs.html#RC6">RC6</a>
class RC6 : public RC6_Info, public BlockCipherDocumentation
{
	class CRYPTOPP_NO_VTABLE Base : public BlockCipherImpl<RC6_Info>
	{
	public:
		void UncheckedSetKey(const byte *userKey, unsigned int length, const NameValuePairs &params);

	protected:
		unsigned int r;       // number of rounds
		SecBlock<RC6_WORD> sTable;  // expanded key table
	};

	class CRYPTOPP_NO_VTABLE Enc : public Base
	{
	public:
		void ProcessAndXorBlock(const byte *inBlock, const byte *xorBlock, byte *outBlock) const;
	};

	class CRYPTOPP_NO_VTABLE Dec : public Base
	{
	public:
		void ProcessAndXorBlock(const byte *inBlock, const byte *xorBlock, byte *outBlock) const;
	};

public:
	typedef BlockCipherFinal<ENCRYPTION, Enc> Encryption;
	typedef BlockCipherFinal<DECRYPTION, Dec> Decryption;
};

typedef RC6::Encryption RC6Encryption;
typedef RC6::Decryption RC6Decryption;

NAMESPACE_END

#endif
