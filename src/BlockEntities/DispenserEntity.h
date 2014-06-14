
#pragma once

#include "DropSpenserEntity.h"





// tolua_begin
class cDispenserEntity :
	public cDropSpenserEntity
{
	typedef cDropSpenserEntity super;

public:

	// tolua_end

	/** Constructor used for normal operation */
	cDispenserEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World);

	static const char * GetClassStatic(void) { return "cDispenserEntity"; }

	// tolua_begin
	
	/** Spawns a projectile of the given kind in front of the dispenser with the specified speed. */
	void SpawnProjectileFromDispenser(int a_BlockX, int a_BlockY, int a_BlockZ, cProjectileEntity::eKind a_Kind, const Vector3d & a_Speed);

	/** Returns a unit vector in the cardinal direction of where the dispenser is facing. */
	Vector3d GetShootVector(NIBBLETYPE a_Meta);
	
	// tolua_end

private:
	// cDropSpenser overrides:
	virtual void DropSpenseFromSlot(cChunk & a_Chunk, int a_SlotNum) override;

	/** If such a bucket can fit, adds it to m_Contents and returns true */
	bool ScoopUpLiquid(int a_SlotNum, short a_BucketItemType);

	/** If the a_BlockInFront can be washed away by liquid and the empty bucket can fit,
	does the m_Contents processing and returns true. Returns false otherwise. */
	bool EmptyLiquidBucket(BLOCKTYPE a_BlockInFront, int a_SlotNum);
} ;  // tolua_export



