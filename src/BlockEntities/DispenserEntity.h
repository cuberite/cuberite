
#pragma once

#include "DropSpenserEntity.h"





// tolua_begin
class cDispenserEntity :
	public cDropSpenserEntity
{
	// tolua_end

	using Super = cDropSpenserEntity;

public:  // tolua_export

	/** Constructor used for normal operation */
	cDispenserEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World);

	// tolua_begin

	/** Spawns a projectile of the given kind in front of the dispenser with the specified speed.
	a_Item is the item from the internal storage from which the projectile originated.
	Returns the UniqueID of the spawned projectile, or cEntity::INVALID_ID on failure. */
	UInt32 SpawnProjectileFromDispenser(
		Vector3i a_BlockPos,
		cProjectileEntity::eKind a_Kind,
		const Vector3d & a_Speed,
		const cItem * a_Item = nullptr
	);

	/** OBSOLETE, use the Vector3i-based overload instead.
	Spawns a projectile of the given kind in front of the dispenser with the specified speed.
	a_Item is the item from the internal storage from which the projectile originated.
	Returns the UniqueID of the spawned projectile, or cEntity::INVALID_ID on failure. */
	UInt32 SpawnProjectileFromDispenser(
		int a_BlockX, int a_BlockY, int a_BlockZ,
		cProjectileEntity::eKind a_Kind,
		const Vector3d & a_Speed,
		const cItem * a_Item = nullptr
	)
	{
		return SpawnProjectileFromDispenser({a_BlockX, a_BlockY, a_BlockZ}, a_Kind, a_Speed, a_Item);
	}

	/** Returns a unit vector in the cardinal direction of where the dispenser with the specified meta would be facing. */
	static Vector3d GetShootVector(NIBBLETYPE a_BlockMeta);

	// tolua_end

private:

	// cDropSpenser overrides:
	virtual void DropSpenseFromSlot(cChunk & a_Chunk, int a_SlotNum) override;

	/** If such a bucket can fit, adds it to m_Contents and returns true */
	bool ScoopUpLiquid(int a_SlotNum, short a_ResultingBucketItemType);

	/** If the a_BlockInFront can be washed away by liquid and the empty bucket can fit,
	does the m_Contents processing and returns true. Returns false otherwise. */
	bool EmptyLiquidBucket(BLOCKTYPE a_BlockInFront, int a_SlotNum);
} ;  // tolua_export
