
// Minecart.h

// Declares the cMinecart class representing a minecart in the world





#pragma once

#include "Entity.h"





inline bool IsBlockRail(BLOCKTYPE a_BlockType)
	{
		return (
			(a_BlockType == E_BLOCK_RAIL) ||
			(a_BlockType == E_BLOCK_ACTIVATOR_RAIL) ||
			(a_BlockType == E_BLOCK_DETECTOR_RAIL) ||
			(a_BlockType == E_BLOCK_POWERED_RAIL)
			) ;
	}





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
		mpTNT,      // Minecart-with-TNT, can be blown up with activator rail
		mpHopper,   // Minecart-with-hopper, can be hopper
		// TODO: Spawner minecarts, (and possibly any block in a minecart with NBT editing)
	} ;
	
	// cEntity overrides:
	virtual void SpawnOn(cClientHandle & a_ClientHandle) override;
	virtual void HandlePhysics(float a_Dt, cChunk & a_Chunk) override;
	virtual void DoTakeDamage(TakeDamageInfo & TDI) override;
	virtual void Destroyed() override;
	
	int LastDamage(void) const { return m_LastDamage; }
	ePayload GetPayload(void) const { return m_Payload; }
	
protected:
	ePayload m_Payload;
	int m_LastDamage;
	Vector3i m_DetectorRailPosition;
	bool m_bIsOnDetectorRail;
	
	cMinecart(ePayload a_Payload, double a_X, double a_Y, double a_Z);

	/** Handles physics on normal rails
		For each tick, slow down on flat rails, speed up or slow down on ascending/descending rails (depending on direction), and turn on curved rails
	*/
	void HandleRailPhysics(NIBBLETYPE a_RailMeta, float a_Dt);

	/** Handles powered rail physics
		Each tick, speed up or slow down cart, depending on metadata of rail (powered or not)
	*/
	void HandlePoweredRailPhysics(NIBBLETYPE a_RailMeta);

	/** Handles detector rail activation
		Activates detector rails when a minecart is on them. Calls HandleRailPhysics() for physics simulations
	*/
	void HandleDetectorRailPhysics(NIBBLETYPE a_RailMeta, float a_Dt);

	/** Snaps a minecart to a rail's axis, resetting its speed */
	void SnapToRail(NIBBLETYPE a_RailMeta);
	/** Tests is a solid block is in front of a cart, and stops the cart (and returns true) if so; returns false if no obstruction*/
	bool TestBlockCollision(NIBBLETYPE a_RailMeta);

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
	
	/// Number of item slots in the chest
	static const int NumSlots = 9 * 3;
	
	cMinecartWithChest(double a_X, double a_Y, double a_Z);
	
	const cItem & GetSlot(int a_Idx) const { return m_Items[a_Idx]; }
	cItem &       GetSlot(int a_Idx)       { return m_Items[a_Idx]; }
	
	void SetSlot(int a_Idx, const cItem & a_Item);

protected:

	/// The chest contents:
	cItem m_Items[NumSlots];
	
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
	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;

	// Set functions.
	void SetIsFueled(bool a_IsFueled, int a_FueledTimeLeft = -1) {m_IsFueled = a_IsFueled; m_FueledTimeLeft = a_FueledTimeLeft;}

	// Get functions.
	int  GetFueledTimeLeft(void) const {return m_FueledTimeLeft; }
	bool IsFueled (void)         const {return m_IsFueled;}

private:

	int m_FueledTimeLeft;
	bool m_IsFueled;

} ;





class cMinecartWithTNT :
	public cMinecart
{
	typedef cMinecart super;
	
public:
	CLASS_PROTODEF(cMinecartWithTNT);
	
	cMinecartWithTNT(double a_X, double a_Y, double a_Z);
} ;





class cMinecartWithHopper :
	public cMinecart
{
	typedef cMinecart super;
	
public:
	CLASS_PROTODEF(cMinecartWithHopper);
	
	cMinecartWithHopper(double a_X, double a_Y, double a_Z);
} ;