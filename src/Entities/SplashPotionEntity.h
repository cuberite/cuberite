//
//  SplashPotionEntity.h
//

#pragma once

#include "ProjectileEntity.h"
#include "EntityEffect.h"
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
	
	cSplashPotionEntity(
		cEntity * a_Creator,
		double a_X, double a_Y, double a_Z,
		const Vector3d & a_Speed,
		cEntityEffect::eType a_EntityEffectType,
		cEntityEffect a_EntityEffect,
		int a_PotionParticleType
	);
	
	cEntityEffect::eType GetEntityEffectType  (void) const { return m_EntityEffectType; }
	cEntityEffect        GetEntityEffect      (void) const { return m_EntityEffect; }
	int                  GetPotionParticleType(void) const { return m_PotionParticleType; }
	
	void SetEntityEffectType(cEntityEffect::eType a_EntityEffectType) { m_EntityEffectType = a_EntityEffectType; }
	void SetEntityEffect(cEntityEffect a_EntityEffect) { m_EntityEffect = a_EntityEffect; }
	void SetPotionParticleType(int a_PotionParticleType) { m_PotionParticleType = a_PotionParticleType; }
	
protected:
	
	cEntityEffect::eType m_EntityEffectType;
	cEntityEffect m_EntityEffect;
	int m_PotionParticleType;
	

	// cProjectileEntity overrides:
	virtual void OnHitSolidBlock(const Vector3d & a_HitPos, eBlockFace a_HitFace) override;
	virtual void OnHitEntity    (cEntity & a_EntityHit, const Vector3d & a_HitPos) override;
	
	/** Splashes the potion, fires its particle effects and sounds
	@param a_HitPos     The position where the potion will splash */
	void Splash(const Vector3d & a_HitPos);
} ; // tolua_export
