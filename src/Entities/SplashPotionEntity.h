//
//  SplashPotionEntity.h
//

#pragma once

#include "ProjectileEntity.h"
#include "EntityEffects.h"
#include "../World.h"
#include "Entity.h"




// tolua_begin

class cSplashPotionEntity :
	public cProjectileEntity
{
	typedef cProjectileEntity super;
	
public:
	
	// tolua_end
	
	CLASS_PROTODEF(cSplashPotionEntity);
	
	cSplashPotionEntity(cEntity * a_Creator, double a_X, double a_Y, double a_Z, const Vector3d & a_Speed, cEntityEffect::eType a_EntityEffectType, cEntityEffect a_EntityEffect, int a_PotionName);
	
protected:
	
	// cProjectileEntity overrides:
	virtual void OnHitSolidBlock(const Vector3d & a_HitPos, eBlockFace a_HitFace) override;
	virtual void OnHitEntity    (cEntity & a_EntityHit, const Vector3d & a_HitPos) override;
	
	/** Splashes the potion, fires its particle effects and sounds
	 *  @param a_HitPos     The position where the potion will splash
	 */
	void Splash(const Vector3d & a_HitPos);
	
	cEntityEffect::eType m_EntityEffectType;
	cEntityEffect m_EntityEffect;
	int m_PotionName;
	
	class cSplashPotionCallback :
		public cEntityCallback
	{
	public:
		cSplashPotionCallback(const Vector3d & a_HitPos, cEntityEffect::eType &a_EntityEffectType, cEntityEffect &a_EntityEffect);
		
		virtual bool Item(cEntity *a_Entity) override;
		
	private:
		const Vector3d &m_HitPos;
		cEntityEffect::eType &m_EntityEffectType;
		cEntityEffect &m_EntityEffect;
	};
	
} ; // tolua_export
