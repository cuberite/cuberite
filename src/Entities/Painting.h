
#pragma once

#include "Entity.h"





// tolua_begin
class cPainting :
	public cEntity
{
	typedef cEntity super;

public:

	// tolua_end

	CLASS_PROTODEF(cPainting)

	cPainting(const AString & a_Name, int a_Direction, double a_X, double a_Y, double a_Z);

	// tolua_begin

	const AString & GetName(void) const { return m_Name; }
	int GetDirection(void) const { return m_Direction; }

	// tolua_end

private:

	virtual void SpawnOn(cClientHandle & a_Client) override;
	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	virtual void GetDrops(cItems & a_Items, cEntity * a_Killer) override;
	virtual void KilledBy(TakeDamageInfo & a_TDI) override
	{
		super::KilledBy(a_TDI);
		Destroy();
	}

	AString m_Name;
	int m_Direction;

};  // tolua_export




