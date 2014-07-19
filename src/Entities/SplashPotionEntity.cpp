#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "SplashPotionEntity.h"
#include "Pawn.h"
#include "../ClientHandle.h"





/// Converts an angle in radians into a byte representation used by the network protocol
#define ANGLE_TO_PROTO(X) (Byte)(X * 255 / 360)

////////////////////////////////////////////////////////////////////////////////
// cSplashPotionEntityCallback:

/** Used to distribute the splashed potion effect among nearby entities */
class cSplashPotionCallback :
	public cEntityCallback
{
public:
	/** Creates the callback.
	@param a_HitPos            The position where the splash potion has splashed
	@param a_EntityEffectType  The effect type of the potion
	@param a_EntityEffect      The effect description */
	cSplashPotionCallback(const Vector3d & a_HitPos, cEntityEffect::eType a_EntityEffectType, const cEntityEffect & a_EntityEffect) :
		m_HitPos(a_HitPos),
		m_EntityEffectType(a_EntityEffectType),
		m_EntityEffect(a_EntityEffect)
	{
	}
	
	/** Called by cWorld::ForEachEntity(), adds the stored entity effect to the entity, if it is close enough. */
	virtual bool Item(cEntity * a_Entity) override
	{
		double SplashDistance = (a_Entity->GetPosition() - m_HitPos).Length();
		if (SplashDistance >= 20)
		{
			// Too far away
			return false;
		}
		if (!a_Entity->IsPawn())
		{
			// Not an entity that can take effects
			return false;
		}

		// y = -0.25x + 1, where x is the distance from the player. Approximation for potion splash.
		// TODO: better equation
		double Reduction = -0.25 * SplashDistance + 1.0;
		if (Reduction < 0)
		{
			Reduction = 0;
		}
		
		((cPawn *) a_Entity)->AddEntityEffect(m_EntityEffectType, m_EntityEffect.GetDuration(), m_EntityEffect.GetIntensity(), Reduction);
		return false;
	}

private:
	const Vector3d & m_HitPos;
	cEntityEffect::eType m_EntityEffectType;
	const cEntityEffect & m_EntityEffect;
};
	




////////////////////////////////////////////////////////////////////////////////
// cSplashPotionEntity:

cSplashPotionEntity::cSplashPotionEntity(
	cEntity * a_Creator,
	double a_X, double a_Y, double a_Z,
	const Vector3d & a_Speed,
	cEntityEffect::eType a_EntityEffectType,
	cEntityEffect a_EntityEffect,
	int a_PotionParticleType
) :
	super(pkSplashPotion, a_Creator, a_X, a_Y, a_Z, 0.25, 0.25),
	m_EntityEffectType(a_EntityEffectType),
	m_EntityEffect(a_EntityEffect),
	m_PotionParticleType(a_PotionParticleType)
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
	
	m_World->BroadcastSoundParticleEffect(2002, (int)a_HitPos.x, (int)a_HitPos.y, (int)a_HitPos.z, m_PotionParticleType);
}





void cSplashPotionEntity::SpawnOn(cClientHandle & a_Client)
{
	a_Client.SendSpawnObject(*this, 73, m_PotionParticleType, ANGLE_TO_PROTO(GetYaw()), ANGLE_TO_PROTO(GetPitch()));
	a_Client.SendEntityMetadata(*this);
}




