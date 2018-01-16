
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

	cExpOrb(double a_X, double a_Y, double a_Z, int a_Reward);
	cExpOrb(const Vector3d & a_Pos, int a_Reward);

	// Override functions
	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;

	virtual bool DoTakeDamage(TakeDamageInfo & a_TDI) override;

	virtual void SpawnOn(cClientHandle & a_Client) override;

	// tolua_begin

	/** Returns the number of ticks that this entity has existed */
	int GetAge(void) const { return std::chrono::duration_cast<cTickTime>(m_Timer).count(); }

	/** Set the number of ticks that this entity has existed */
	void SetAge(int a_Age) { m_Timer = cTickTime(a_Age); }

	/** Get the exp amount */
	int GetReward(void) const { return m_Reward; }

	/** Set the exp amount */
	void SetReward(int a_Reward) { m_Reward = a_Reward; }

	// tolua_end

protected:
	int m_Reward;

	/** The number of ticks that the entity has existed / timer between collect and destroy; in msec */
	std::chrono::milliseconds m_Timer;
} ;  // tolua_export
