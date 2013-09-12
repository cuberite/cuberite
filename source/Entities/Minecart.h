
// Minecart.h

// Declares the cMinecart class representing a minecart in the world





#pragma once

#include "Entity.h"
#include "../Item.h"





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
	void HandleRailPhysics(float a_Dt, cChunk & a_Chunk);
	virtual void DoTakeDamage(TakeDamageInfo & TDI) override;

	
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