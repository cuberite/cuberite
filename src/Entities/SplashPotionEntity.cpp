#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "SplashPotionEntity.h"
#include "Pawn.h"
#include "../EffectID.h"





////////////////////////////////////////////////////////////////////////////////
// cSplashPotionEntity:

cSplashPotionEntity::cSplashPotionEntity(
	cEntity * a_Creator,
	Vector3d a_Pos,
	Vector3d a_Speed,
	const cItem & a_Item
):
	Super(pkSplashPotion, a_Creator, a_Pos, 0.25, 0.25),
	m_Item(a_Item),
	m_DestroyTimer(-1)
{
	SetSpeed(a_Speed);
	m_EntityEffectType = cEntityEffect::GetPotionEffectType(a_Item.m_ItemDamage);
	m_EntityEffect = cEntityEffect(
		cEntityEffect::GetPotionEffectDuration(a_Item.m_ItemDamage),
		cEntityEffect::GetPotionEffectIntensity(a_Item.m_ItemDamage)
	);
	m_PotionColor = cEntityEffect::GetPotionColor(a_Item.m_ItemDamage);
}





void cSplashPotionEntity::OnHitSolidBlock(Vector3d a_HitPos, eBlockFace a_HitFace)
{
	Splash(a_HitPos);
	m_DestroyTimer = 2;
}





void cSplashPotionEntity::OnHitEntity(cEntity & a_EntityHit, Vector3d a_HitPos)
{
	a_EntityHit.TakeDamage(dtRangedAttack, this, 0, 1);
	Splash(a_HitPos);
	m_DestroyTimer = 5;
}





void cSplashPotionEntity::Splash(Vector3d a_HitPos)
{
	double XZCalculation = 8.25/2;
	double YCalculation = 4.25/2;
	cBoundingBox SplashDistanceBox = cBoundingBox(a_HitPos.x - XZCalculation, a_HitPos.x + XZCalculation, a_HitPos.y - YCalculation, a_HitPos.y + YCalculation, a_HitPos.z - XZCalculation, a_HitPos.z + XZCalculation);

	m_World->ForEachEntityInBox(SplashDistanceBox, [=](cEntity & a_Entity)
		{
			if (!a_Entity.IsPawn())
			{
				// Not an entity that can take effects
				return false;
			}

			// y = -0.25x + 1, where x is the distance from the player. Approximation for potion splash.
			double SplashDistance = (a_Entity.GetPosition() - a_HitPos).Length();
			double Reduction = -0.25 * SplashDistance + 1.0;
			Reduction = std::max(Reduction, 0.0);

			static_cast<cPawn &>(a_Entity).AddEntityEffect(m_EntityEffectType, m_EntityEffect.GetDuration(), m_EntityEffect.GetIntensity(), Reduction);
			return false;
		}
	);

	m_World->BroadcastSoundParticleEffect(
		EffectID::PARTICLE_SPLASH_POTION,
		a_HitPos.Floor(),
		m_PotionColor
	);
}
