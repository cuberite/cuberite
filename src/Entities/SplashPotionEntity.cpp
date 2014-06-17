#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "SplashPotionEntity.h"
#include "Pawn.h"





cSplashPotionEntity::cSplashPotionEntity(cEntity * a_Creator, double a_X, double a_Y, double a_Z, const Vector3d & a_Speed, cEntityEffect::eType a_EntityEffectType, cEntityEffect a_EntityEffect, int a_PotionName) :
	super(pkSplashPotion, a_Creator, a_X, a_Y, a_Z, 0.25, 0.25),
	m_EntityEffectType(a_EntityEffectType),
	m_EntityEffect(a_EntityEffect),
	m_PotionName(a_PotionName)
{
	SetSpeed(a_Speed);
}





void cSplashPotionEntity::OnHitSolidBlock(const Vector3d & a_HitPos, eBlockFace a_HitFace)
{
	Splash(a_HitPos);
	Destroy();
}





void cSplashPotionEntity::OnHitEntity(cEntity & a_EntityHit, const Vector3d & a_HitPos)
{
	a_EntityHit.TakeDamage(dtRangedAttack, this, 0, 1);
	Splash(a_HitPos);
	Destroy(true);
}





void cSplashPotionEntity::Splash(const Vector3d & a_HitPos)
{
	cSplashPotionCallback Callback(a_HitPos, m_EntityEffectType, m_EntityEffect);
	m_World->ForEachEntity(Callback);
	
	m_World->BroadcastSoundParticleEffect(2002, a_HitPos.x, a_HitPos.y, a_HitPos.z, m_PotionName);
}





cSplashPotionEntity::cSplashPotionCallback::cSplashPotionCallback(const Vector3d & a_HitPos, cEntityEffect::eType &a_EntityEffectType, cEntityEffect &a_EntityEffect):
	m_HitPos(a_HitPos),
	m_EntityEffectType(a_EntityEffectType),
	m_EntityEffect(a_EntityEffect)
{
	
}





bool cSplashPotionEntity::cSplashPotionCallback::Item(cEntity * a_Entity)
{
	double SplashDistance = (a_Entity->GetPosition() - m_HitPos).Length();
	if (SplashDistance < 20 && a_Entity->IsPawn())
	{
		// y = -0.25x + 1, where x is the distance from the player. Approximation for potion splash.
		// TODO: better equation
		double Reduction = -0.25 * SplashDistance + 1.0;
		if (Reduction < 0)
		{
			Reduction = 0;
		}
		
		m_EntityEffect.SetDistanceModifier(Reduction);
		((cPawn *) a_Entity)->AddEntityEffect(m_EntityEffectType, m_EntityEffect.GetDuration(), m_EntityEffect.GetIntensity(), m_EntityEffect.GetCreator(), Reduction);
	}
	return false;
}




