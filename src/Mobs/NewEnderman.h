
#pragma once

#include "NewMonster.h"





class cNewEnderman :
	public cNewMonster
{
	typedef cNewMonster super;
	
public:
	cNewEnderman(void);

	CLASS_PROTODEF(cNewEnderman)

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;

	bool IsScreaming(void) const {return m_bIsScreaming; }
	BLOCKTYPE GetCarriedBlock(void) const {return CarriedBlock; }
	NIBBLETYPE GetCarriedMeta(void) const {return CarriedMeta; }

private:

	bool m_bIsScreaming;
	BLOCKTYPE CarriedBlock;
	NIBBLETYPE CarriedMeta;

} ;




