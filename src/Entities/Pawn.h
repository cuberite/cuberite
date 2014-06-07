
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
	
	void AddEntityEffect(cEntityEffect::eType a_EffectType, cEntityEffect a_Effect);
	void RemoveEntityEffect(cEntityEffect::eType a_EffectType);

protected:
	std::map<cEntityEffect::eType, cEntityEffect> m_EntityEffects;
} ;  // tolua_export




