
#pragma once

#include "Entity.h"
#include "EntityEffect.h"





// tolua_begin
class cPawn :
	public cEntity
{
	// tolua_end
	typedef cEntity super;
	
public:
	CLASS_PROTODEF(cPawn);

	cPawn(eEntityType a_EntityType, double a_Width, double a_Height);
	
	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;
	virtual void KilledBy(TakeDamageInfo & a_TDI) override;
	
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
	
	/** Removes all currently applied entity effects (used when drinking milk) */
	void ClearEntityEffects(void);
	
	// tolua_end

protected:
	typedef std::map<cEntityEffect::eType, cEntityEffect *> tEffectMap;
	tEffectMap m_EntityEffects;
} ;  // tolua_export




