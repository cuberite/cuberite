
// FireworkEntity.h

// Declares the cFireworkEntity class representing the flying firework rocket





#pragma once

#include "ProjectileEntity.h"





// tolua_begin

class cFireworkEntity :
	public cProjectileEntity
{
	// tolua_end

	using Super = cProjectileEntity;

public:  // tolua_export

	CLASS_PROTODEF(cFireworkEntity)

	cFireworkEntity(cEntity * a_Creator, Vector3d a_Pos, const cItem & a_Item);

	// tolua_begin

	/** Returns the item used to create the rocket (has all the firework effects on it) */
	const cItem & GetItem(void) const { return m_FireworkItem; }

	/** Sets the item that is used to create the rocket (has all the firework effects on it) */
	void SetItem(const cItem & a_Item) { m_FireworkItem = a_Item; }

	/** Returns the number of ticks left until the firework explosion. */
	int GetTicksToExplosion(void) const { return m_TicksToExplosion; }

	/** Sets the number of ticks left until the firework explosion. */
	void SetTicksToExplosion(int a_TicksToExplosion) { m_TicksToExplosion = a_TicksToExplosion; }

	// tolua_end

protected:

	// cProjectileEntity overrides:
	virtual void HandlePhysics(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;

private:

	int m_TicksToExplosion;
	cItem m_FireworkItem;

};  // tolua_export




