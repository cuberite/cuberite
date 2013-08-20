
// ProjectileEntity.h

// Declares the cProjectileEntity class representing the common base class for projectiles





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
		pkGhastFireball = 63,  // TODO: Unverified TypeID, check this in ProtoProxy
		pkFireCharge    = 64,  // TODO: Unverified TypeID, check this in ProtoProxy
		pkEnderPearl    = 65,
		pkExpBottle     = 75,
		pkSplashPotion  = 73,
		pkWitherSkull   = 66,
		pkFishingFloat  = 90,
	} ;
	
	cProjectileEntity(eKind a_Kind, cEntity * a_Creator, double a_X, double a_Y, double a_Z, double a_Width, double a_Height);
	cProjectileEntity(eKind a_Kind, cEntity * a_Creator, const Vector3d & a_Pos, const Vector3d & a_Speed, double a_Width, double a_Height);
	
	/// Called by the physics blocktracer when the entity hits a solid block, the coords and the face hit is given
	virtual void OnHitSolidBlock(double a_BlockX, double a_BlockY, double a_BlockZ, char a_BlockFace) {};
	
protected:
	eKind m_Kind;
	
	/// The entity who has created this projectile; may be NULL (e. g. for dispensers)
	cEntity * m_Creator;
} ;





class cArrowEntity :
	public cProjectileEntity
{
public:
	cArrowEntity(cEntity * a_Creator, double a_X, double a_Y, double a_Z, double a_Width, double a_Height);
	cArrowEntity(cEntity * a_Creator, const Vector3d & a_Pos, const Vector3d & a_Speed, double a_Width, double a_Height);
} ;

// tolua_end



