
#pragma once

#include "PassiveAggressiveMonster.h"





class cEnderman :
	public cPassiveAggressiveMonster
{
	typedef cPassiveAggressiveMonster super;
	
public:
	cEnderman(void);

	CLASS_PROTODEF(cEnderman);

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;

	bool IsScreaming(void) const {return m_bIsScreaming; }
	BLOCKTYPE GetCarriedBlock(void) const {return CarriedBlock; }
	NIBBLETYPE GetCarriedMeta(void) const {return CarriedMeta; }

private:

	bool m_bIsScreaming;
	BLOCKTYPE CarriedBlock;
	NIBBLETYPE CarriedMeta;

} ;




