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
	Super(etPotion, a_Creator, a_Pos, a_Speed, 0.25f, 0.25f),
	m_Item(a_Item)
{
	m_EntityEffectType = cEntityEffect::GetPotionEffectType(a_Item.m_ItemDamage);
	m_EntityEffect = cEntityEffect(
		cEntityEffect::GetPotionEffectDuration(a_Item.m_ItemDamage),
		cEntityEffect::GetPotionEffectIntensity(a_Item.m_ItemDamage)
	);
	m_PotionColor = cEntityEffect::GetPotionColor(a_Item.m_ItemDamage);
}





void cSplashPotionEntity::Splash(Vector3d a_HitPos)
{
	// Look for entities in 8.25 (width) x 4.25 (height) cuboid _centred_ on hit position:
	m_World->ForEachEntityInBox({ a_HitPos.addedY(-4.25 / 2), 8.25 / 2, 4.25 }, [this, a_HitPos](cEntity & a_Entity)
	{
		if (!a_Entity.IsPawn())
		{
			// Not an entity that can take effects
			return false;
		}

		double SplashDistance = (a_Entity.GetPosition() - a_HitPos).Length();
		double Reduction = -0.25 * SplashDistance + 1.0;  // y = -0.25x + 1, where x is the distance from the player. Approximation for potion splash.
		Reduction = std::max(Reduction, 0.0);

		static_cast<cPawn &>(a_Entity).AddEntityEffect(m_EntityEffectType, m_EntityEffect.GetDuration(), m_EntityEffect.GetIntensity(), Reduction);
		return false;
	});

	m_World->BroadcastSoundParticleEffect(
		EffectID::PARTICLE_SPLASH_POTION,
		a_HitPos.Floor(),
		m_PotionColor
	);
}





void cSplashPotionEntity::OnHitEntity(cEntity & a_EntityHit, Vector3d a_HitPos)
{
	Super::OnHitEntity(a_EntityHit, a_HitPos);

	a_EntityHit.TakeDamage(dtRangedAttack, this, 0, 1);
	Splash(a_HitPos);
	Destroy();
}





void cSplashPotionEntity::OnHitSolidBlock(Vector3d a_HitPos, eBlockFace a_HitFace)
{
	Super::OnHitSolidBlock(a_HitPos, a_HitFace);

	Splash(a_HitPos);
	Destroy();
}
