
#pragma once

#include "Entity.h"





// tolua_begin
class cPainting :
	public cEntity
{
	// tolua_end
	typedef cEntity super;

public:
	CLASS_PROTODEF(cPainting);

	cPainting(const AString & a_Name, int a_Direction, double a_X, double a_Y, double a_Z);
	const AString & GetName(void) const { return m_Name; }  // tolua_export
	int GetDirection(void) const { return m_Direction; }  // tolua_export

private:

	virtual void SpawnOn(cClientHandle & a_Client) override;
	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;
	virtual void GetDrops(cItems & a_Items, cEntity * a_Killer) override;
	virtual void KilledBy(TakeDamageInfo & a_TDI) override
	{
		super::KilledBy(a_TDI);
		Destroy();
	}

	AString m_Name;
	int m_Direction;

};  // tolua_export




