
// Minecart.h

// Declares the cMinecart class representing a minecart in the world





#pragma once

#include "Entity.h"





class cMinecart :
	public cEntity
{
	typedef cEntity super;
	
public:
	CLASS_PROTODEF(cMinecart);
	
	enum ePayload
	{
		mpNone,     // Empty minecart, ridable by player or mobs
		mpChest,    // Minecart-with-chest, can store a grid of 3*8 items
		mpFurnace,  // Minecart-with-furnace, can be powered
		// TODO: Other 1.5 features: hopper, tnt, dispenser, spawner
	} ;
	
	// cEntity overrides:
	virtual void Initialize(cWorld * a_World) override;
	virtual void SpawnOn(cClientHandle & a_ClientHandle) override;
	virtual void Tick(float a_Dt, MTRand & a_TickRandom) override;
	
	ePayload GetPayload(void) const { return m_Payload; }
	
protected:
	ePayload m_Payload;
	
	cMinecart(ePayload a_Payload, double a_X, double a_Y, double a_Z);
} ;





class cEmptyMinecart :
	public cMinecart
{
	typedef cMinecart super;
	
public:
	CLASS_PROTODEF(cEmptyMinecart);
	
	cEmptyMinecart(double a_X, double a_Y, double a_Z);

	// cEntity overrides:
	virtual void OnRightClicked(cPlayer & a_Player) override;
} ;





class cMinecartWithChest :
	public cMinecart
{
	typedef cMinecart super;
	
public:
	CLASS_PROTODEF(cMinecartWithChest);
	
	cMinecartWithChest(double a_X, double a_Y, double a_Z);
	
	// cEntity overrides:
	virtual void OnRightClicked(cPlayer & a_Player) override;
} ;





class cMinecartWithFurnace :
	public cMinecart
{
	typedef cMinecart super;
	
public:
	CLASS_PROTODEF(cMinecartWithFurnace);
	
	cMinecartWithFurnace(double a_X, double a_Y, double a_Z);
	
	// cEntity overrides:
	virtual void OnRightClicked(cPlayer & a_Player) override;
} ;




