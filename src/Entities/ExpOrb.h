
#pragma once

#include "Entity.h"





// tolua_begin
class cExpOrb :
	public cEntity
{
	typedef cEntity super;
	
public:
	// tolua_end

	CLASS_PROTODEF(cExpOrb);

	cExpOrb(double a_X, double a_Y, double a_Z, int a_Reward);
	cExpOrb(const Vector3d & a_Pos, int a_Reward);

	// Override functions
	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;
	virtual void SpawnOn(cClientHandle & a_Client) override;

	/** Returns the number of ticks that this entity has existed */
	int GetAge(void) const { return (int)(m_Timer / 50); }     // tolua_export

	/** Set the number of ticks that this entity has existed */
	void SetAge(int a_Age) { m_Timer = (float)(a_Age * 50); }  // tolua_export

	/** Get the exp amount */
	int GetReward(void) const { return m_Reward; }  // tolua_export

	/** Set the exp amount */
	void SetReward(int a_Reward) { m_Reward = a_Reward; }  // tolua_export

protected:
	int m_Reward;
	
	/** The number of ticks that the entity has existed / timer between collect and destroy; in msec */
	float m_Timer;
} ;  // tolua_export
