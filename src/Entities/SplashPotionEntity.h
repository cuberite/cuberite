
// SplashPotionEntity.h

// Declares the cSplashPotionEntity class representing a splash potion that has been thrown





#pragma once

#include "ProjectileEntity.h"
#include "EntityEffect.h"
#include "../World.h"



class cEntity;



// tolua_begin

class cSplashPotionEntity :
	public cProjectileEntity
{
	typedef cProjectileEntity super;

public:

	// tolua_end

	CLASS_PROTODEF(cSplashPotionEntity)

	cSplashPotionEntity(
		cEntity * a_Creator,
		double a_X, double a_Y, double a_Z,
		const Vector3d & a_Speed,
		const cItem & a_Item
	);

	// tolua_begin
	cEntityEffect::eType GetEntityEffectType(void) const { return m_EntityEffectType; }
	int                  GetPotionColor(void)      const { return m_PotionColor; }
	const cItem &        GetItem(void)             const { return m_Item; }

	void SetEntityEffectType(cEntityEffect::eType a_EntityEffectType) { m_EntityEffectType = a_EntityEffectType; }
	void SetPotionColor(int a_PotionColor) { m_PotionColor = a_PotionColor; }

	// tolua_end

	cEntityEffect        GetEntityEffect(void)     const { return m_EntityEffect; }
	void SetEntityEffect(cEntityEffect a_EntityEffect) { m_EntityEffect = a_EntityEffect; }

protected:

	cEntityEffect::eType m_EntityEffectType;
	cEntityEffect m_EntityEffect;
	int m_PotionColor;
	cItem m_Item;


	// cProjectileEntity overrides:
	virtual void OnHitSolidBlock(Vector3d a_HitPos, eBlockFace a_HitFace) override;
	virtual void OnHitEntity    (cEntity & a_EntityHit, Vector3d a_HitPos) override;
	virtual void Tick           (std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override
	{
		if (m_DestroyTimer > 0)
		{
			m_DestroyTimer--;
			if (m_DestroyTimer == 0)
			{
				Destroy();
				return;
			}
		}
		else
		{
			super::Tick(a_Dt, a_Chunk);
		}
	}

	/** Splashes the potion, fires its particle effects and sounds
	@param a_HitPos     The position where the potion will splash */
	void Splash(Vector3d a_HitPos);

	virtual void SpawnOn(cClientHandle & a_Client) override;

private:
	/** Time in ticks to wait for the hit animation to begin before destroying */
	int m_DestroyTimer;
} ;  // tolua_export




