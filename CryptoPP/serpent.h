#ifndef CRYPTOPP_SERPENT_H
#define CRYPTOPP_SERPENT_H

/** \file
*/

#include "seckey.h"
#include "secblock.h"

NAMESPACE_BEGIN(CryptoPP)

//! _
struct Serpent_Info : public FixedBlockSize<16>, public VariableKeyLength<16, 0, 32>, public FixedRounds<32>
{
	static const char *StaticAlgorithmName() {return "Serpent";}
};

/// <a href="http://www.weidai.com/scan-mirror/cs.html#Serpent">Serpent</a>
class Serpent : public Serpent_Info, public BlockCipherDocumentation
{
	class CRYPTOPP_NO_VTABLE Base : public BlockCipherImpl<Serpent_Info>
	{
	public:
		void UncheckedSetKey(const byte *userKey, unsigned int length, const NameValuePairs &params);

	protected:
		FixedSizeSecBlock<word32, 33*4> m_key;
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

typedef Serpent::Encryption SerpentEncryption;
typedef Serpent::Decryption SerpentDecryption;

NAMESPACE_END

#endif
