
#pragma once

#include "PassiveAggressiveMonster.h"





// tolua_begin
class cEnderman :
	public cPassiveAggressiveMonster
{
	typedef cPassiveAggressiveMonster super;
	
public:
	// tolua_end
	cEnderman(void);

	CLASS_PROTODEF(cEnderman);

	virtual void GetDrops(cItems & a_Drops, cEntity * a_Killer = NULL) override;

	// tolua_begin
	bool IsScreaming(void) const {return m_bIsScreaming; }
	BLOCKTYPE GetCarriedBlock(void) const {return CarriedBlock; }
	NIBBLETYPE GetCarriedMeta(void) const {return CarriedMeta; }
	// tolua_end

private:

	bool m_bIsScreaming;
	BLOCKTYPE CarriedBlock;
	NIBBLETYPE CarriedMeta;
} ; // tolua_export



