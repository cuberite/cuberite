//
//  FireworkEntity.h
//

#pragma once

#include "ProjectileEntity.h"





// tolua_begin

class cFireworkEntity :
	public cProjectileEntity
{
	typedef cProjectileEntity super;
	
public:
	
	// tolua_end
	
	CLASS_PROTODEF(cFireworkEntity);
	
	cFireworkEntity(cEntity * a_Creator, double a_X, double a_Y, double a_Z, const cItem & a_Item);
	const cItem & GetItem(void) const { return m_FireworkItem; }
	
protected:
	
	// cProjectileEntity overrides:
	virtual void HandlePhysics(float a_Dt, cChunk & a_Chunk) override;
	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;
	
private:
	
	int m_ExplodeTimer;
	cItem m_FireworkItem;
	
};  // tolua_export
