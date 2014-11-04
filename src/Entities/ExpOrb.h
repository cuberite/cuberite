
#pragma once

#include "Entity.h"





// tolua_begin
class cExpOrb :
	public cEntity
{
	typedef cEntity super;
	
public:
	// tolua_end

	CLASS_PROTODEF(cExpOrb)

	cExpOrb(CreateEntityInfo a_Info, int a_Reward);

	// Override functions
	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;
	virtual void SpawnOn(cClientHandle & a_Client) override;

	/** Get the exp amount */
	int GetReward(void) const { return m_Reward; }  // tolua_export

	/** Set the exp amount */
	void SetReward(int a_Reward) { m_Reward = a_Reward; }  // tolua_export

protected:
	int m_Reward;
} ;  // tolua_export
