
#pragma once

#include "Entity.h"
#include "EntityEffect.h"

// fwd cMonster
class cMonster;





// tolua_begin
class cPawn :
	public cEntity
{
	// tolua_end
	typedef cEntity super;

public:
	CLASS_PROTODEF(cPawn)

	cPawn(eEntityType a_EntityType, double a_Width, double a_Height);
	~cPawn();
	virtual void Destroyed() override;

	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	virtual void KilledBy(TakeDamageInfo & a_TDI) override;

	virtual bool IsFireproof(void) const override;
	virtual void HandleAir(void) override;
	virtual void HandleFalling(void);

	/** Tells all pawns which are targeting us to stop targeting us. */
	void StopEveryoneFromTargetingMe();



	// tolua_begin

	/** Applies an entity effect
	Checks with plugins if they allow the addition.
	@param a_EffectType          The entity effect to apply
	@param a_EffectDurationTicks The duration of the effect
	@param a_EffectIntensity     The level of the effect (0 = Potion I, 1 = Potion II, etc)
	@param a_DistanceModifier    The scalar multiplied to the potion duration, only applies to splash potions)
	*/
	void AddEntityEffect(cEntityEffect::eType a_EffectType, int a_EffectDurationTicks, short a_EffectIntensity, double a_DistanceModifier = 1);

	/** Removes a currently applied entity effect
	@param a_EffectType The entity effect to remove
	*/
	void RemoveEntityEffect(cEntityEffect::eType a_EffectType);

	/** Returns true, if the entity effect is currently applied
	@param a_EffectType The entity effect to check
	*/
	bool HasEntityEffect(cEntityEffect::eType a_EffectType) const;

	/** Removes all currently applied entity effects (used when drinking milk) */
	void ClearEntityEffects(void);

	// tolua_end

	/** remove the monster from the list of monsters targeting this pawn. */
	void NoLongerTargetingMe(cMonster * a_Monster);

	/** Add the monster to the list of monsters targeting this pawn. (Does not check if already in list!) */
	void TargetingMe(cMonster * a_Monster);

	void SetPosition(double a_PosX, double a_PosY, double a_PosZ);

	virtual void SetPosition(const Vector3d & a_Position) override;

protected:
	typedef std::map<cEntityEffect::eType, cEntityEffect *> tEffectMap;
	tEffectMap m_EntityEffects;

	double m_LastGroundHeight;
	bool m_bTouchGround;

private:

	/** A list of all monsters that are targeting this pawn. */
	std::vector<cMonster*> m_TargetingMe;
} ;  // tolua_export




