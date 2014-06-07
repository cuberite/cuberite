
#pragma once

#include "Entity.h"
#include "EntityEffects.h"





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
	
	/** Applies an entity effect
	 *  @param a_EffectType The entity effect to apply
	 *  @param a_Effect     The parameters of the effect
	 */
	void AddEntityEffect(cEntityEffect::eType a_EffectType, cEntityEffect a_Effect);
	
	/** Removes a currently applied entity effect
	 *  @param a_EffectType The entity effect to remove
	 */
	void RemoveEntityEffect(cEntityEffect::eType a_EffectType);
	
	/** Removes all currently applied entity effects (used when drinking milk) */
	void ClearEntityEffects();

protected:
	typedef std::map<cEntityEffect::eType, cEntityEffect> tEffectMap;
	tEffectMap m_EntityEffects;
	
	/** Applies entity effect effects
	 *  @param a_EffectType The selected entity effect
	 *  @param a_Effect     The parameters of the selected entity effect
	 */
	virtual void HandleEntityEffects(cEntityEffect::eType a_EffectType, cEntityEffect a_Effect);
} ;  // tolua_export




