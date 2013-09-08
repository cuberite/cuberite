
// Boat.h

// Declares the cBoat class representing a boat in the world





#pragma once

#include "Entity.h"
#include "../Item.h"





class cBoat :
	public cEntity
{
	typedef cEntity super;
	
public:
	CLASS_PROTODEF(cBoat);
		
	// cEntity overrides:
	virtual void SpawnOn(cClientHandle & a_ClientHandle) override;
	virtual void OnRightClicked(cPlayer & a_Player) override;
	virtual void DoTakeDamage(TakeDamageInfo & TDI) override;
	virtual void HandlePhysics(float a_Dt, cChunk & a_Chunk) override;
	
	cBoat(double a_X, double a_Y, double a_Z);
} ;