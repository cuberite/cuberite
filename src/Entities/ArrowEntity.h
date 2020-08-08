
// ArrowEntity.h

// Declares the cArrowEntity representing the arrow that has been shot by the player or by a skeleton





#pragma once

#include "ProjectileEntity.h"





// tolua_begin

class cArrowEntity :
	public cProjectileEntity
{
	// tolua_end

	using Super = cProjectileEntity;

	// tolua_begin


public:

	/** Determines when the arrow can be picked up (depending on player gamemode). Corresponds to the MCA file "pickup" field */
	enum ePickupState
	{
		psNoPickup             = 0,
		psInSurvivalOrCreative = 1,
		psInCreative           = 2,
	} ;

	// tolua_end

	CLASS_PROTODEF(cArrowEntity)

	/** Creates a new arrow with psNoPickup state and default damage modifier coeff */
	cArrowEntity(cEntity * a_Creator, Vector3d a_Pos, Vector3d a_Speed);

	/** Creates a new arrow as shot by a player, initializes it from the player object */
	cArrowEntity(cPlayer & a_Player, double a_Force);

	// tolua_begin

	/** Returns whether the arrow can be picked up by players */
	ePickupState GetPickupState(void) const { return m_PickupState; }

	/** Sets a new pickup state */
	void SetPickupState(ePickupState a_PickupState) { m_PickupState = a_PickupState; }

	/** Returns the damage modifier coeff. */
	double GetDamageCoeff(void) const { return m_DamageCoeff; }

	/** Sets the damage modifier coeff */
	void SetDamageCoeff(double a_DamageCoeff) { m_DamageCoeff = a_DamageCoeff; }

	/** Returns true if the specified player can pick the arrow up */
	bool CanPickup(const cPlayer & a_Player) const;

	/** Returns true if the arrow is set as critical */
	bool IsCritical(void) const { return m_IsCritical; }

	/** Sets the IsCritical flag */
	void SetIsCritical(bool a_IsCritical) { m_IsCritical = a_IsCritical; }

	/** Gets the block arrow is in */
	Vector3i GetBlockHit(void) const { return m_HitBlockPos; }

	// tolua_end

	/** Sets the block arrow is in. To be used by the MCA loader only! */
	void SetBlockHit(const Vector3i & a_BlockHit) { m_HitBlockPos = a_BlockHit; }

protected:

	/** Determines when the arrow can be picked up by players */
	ePickupState m_PickupState;

	/** The coefficient applied to the damage that the arrow will deal, based on the bow enchantment. 2.0 for normal arrow */
	double m_DamageCoeff;

	/** If true, the arrow deals more damage */
	bool m_IsCritical;

	/** Timer for pickup collection animation or five minute timeout */
	std::chrono::milliseconds m_Timer;

	/** If true, the arrow is in the process of being collected - don't go to anyone else */
	bool m_bIsCollected;

	/** Stores the block position that arrow is lodged into, sets m_IsInGround to false if it becomes air */
	Vector3i m_HitBlockPos;

	// cProjectileEntity overrides:
	virtual void OnHitSolidBlock(Vector3d a_HitPos, eBlockFace a_HitFace) override;
	virtual void OnHitEntity(cEntity & a_EntityHit, Vector3d a_HitPos) override;
	virtual void CollectedBy(cPlayer & a_Player) override;
	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;

	// cEntity overrides:
	virtual bool DoesPreventBlockPlacement(void) const override;

};  // tolua_export
