
// ProjectileEntity.h

// Declares the cProjectileEntity class representing the common base class for projectiles, as well as individual projectile types





#pragma once

#include "Entity.h"





// tolua_begin

class cProjectileEntity :
	public cEntity
{
	typedef cEntity super;
	
public:
	/// The kind of the projectile. The numbers correspond to the network type ID used for spawning via the 0x17 packet.
	enum eKind
	{
		pkArrow         = 60,
		pkSnowball      = 61,
		pkEgg           = 62,
		pkGhastFireball = 63,
		pkFireCharge    = 64,
		pkEnderPearl    = 65,
		pkExpBottle     = 75,
		pkSplashPotion  = 73,
		pkWitherSkull   = 66,
		pkFishingFloat  = 90,
	} ;
	
	// tolua_end
	
	CLASS_PROTODEF(cProjectileEntity);

	cProjectileEntity(eKind a_Kind, cEntity * a_Creator, double a_X, double a_Y, double a_Z, double a_Width, double a_Height);
	cProjectileEntity(eKind a_Kind, cEntity * a_Creator, const Vector3d & a_Pos, const Vector3d & a_Speed, double a_Width, double a_Height);
	
	static cProjectileEntity * Create(eKind a_Kind, cEntity * a_Creator, double a_X, double a_Y, double a_Z, const Vector3d * a_Speed = NULL);
	
	/// Called by the physics blocktracer when the entity hits a solid block, the block's coords and the face hit is given
	virtual void OnHitSolidBlock(int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace);
	
	// tolua_begin

	/// Returns the kind of the projectile (fast class identification)
	eKind GetProjectileKind(void) const { return m_ProjectileKind; }
	
	/// Returns the entity who created this projectile; may be NULL
	cEntity * GetCreator(void) { return m_Creator; }
	
	/// Returns the string that is used as the entity type (class name) in MCA files
	AString GetMCAClassName(void) const;
	
	/// Returns true if the projectile has hit the ground and is stuck there
	bool IsInGround(void) const { return m_IsInGround; }
	
protected:
	eKind m_ProjectileKind;
	
	/// The entity who has created this projectile; may be NULL (e. g. for dispensers)
	cEntity * m_Creator;
	
	/// True if the projectile has hit the ground and is stuck there
	bool m_IsInGround;
	
	// cEntity overrides:
	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;
	virtual void HandlePhysics(float a_Dt, cChunk & a_Chunk) override;
} ;





class cArrowEntity :
	public cProjectileEntity
{
	typedef cProjectileEntity super;
	
public:
	/// Determines when the arrow can be picked up (depending on player gamemode). Corresponds to the MCA file "pickup" field
	enum ePickupState
	{
		psNoPickup             = 0,
		psInSurvivalOrCreative = 1,
		psInCreative           = 2,
	} ;

	// tolua_end
	
	CLASS_PROTODEF(cArrowEntity);

	/// Creates a new arrow with psNoPickup state and default damage modifier coeff
	cArrowEntity(cEntity * a_Creator, double a_X, double a_Y, double a_Z, const Vector3d a_Speed);
	
	// tolua_begin
	
	/// Returns whether the arrow can be picked up by players
	ePickupState GetPickupState(void) const { return m_PickupState; }
	
	/// Sets a new pickup state
	void SetPickupState(ePickupState a_PickupState) { m_PickupState = a_PickupState; }
	
	/// Returns the damage modifier coeff.
	double GetDamageCoeff(void) const { return m_DamageCoeff; }
	
	/// Sets the damage modifier coeff
	void SetDamageCoeff(double a_DamageCoeff) { m_DamageCoeff = a_DamageCoeff; }
	
	/// Returns true if the specified player can pick the arrow up
	bool CanPickup(const cPlayer & a_Player) const;
	
	// tolua_end
	
protected:

	/// Determines when the arrow can be picked up by players
	ePickupState m_PickupState;
	
	/// The coefficient applied to the damage that the arrow will deal, based on the bow enchantment. 2.0 for normal arrow
	double m_DamageCoeff;

	// cEntity overrides:
	virtual void SpawnOn(cClientHandle & a_Client) override;
	
	// tolua_begin
} ;

// tolua_end



