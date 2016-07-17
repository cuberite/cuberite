
// Boat.h

// Declares the cBoat class representing a boat in the world





#pragma once

#include "Entity.h"





class cBoat :
	public cEntity
{
	typedef cEntity super;

public:
	CLASS_PROTODEF(cBoat)

	// cEntity overrides:
	virtual void SpawnOn(cClientHandle & a_ClientHandle) override;
	virtual void OnRightClicked(cPlayer & a_Player) override;
	virtual bool DoTakeDamage(TakeDamageInfo & TDI) override;
	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	virtual void HandleSpeedFromAttachee(float a_Forward, float a_Sideways) override;

	cBoat(double a_X, double a_Y, double a_Z);

	int GetLastDamage(void) const { return m_LastDamage; }
	int GetForwardDirection(void) const { return m_ForwardDirection; }

	float GetDamageTaken(void) const { return m_DamageTaken; }

	int GetType(void) const { return m_Type; }

	bool IsRightPaddleUsed(void) const { return m_RightPaddleUsed; }
	bool IsLeftPaddleUsed(void) const { return m_LeftPaddleUsed; }

	void SetLastDamage(int TimeSinceLastHit);

	void UpdatePaddles(bool rightPaddleUsed, bool leftPaddleUsed);

private:
	int m_LastDamage;
	int m_ForwardDirection;

	float m_DamageTaken;

	int m_Type;

	bool m_RightPaddleUsed;
	bool m_LeftPaddleUsed;
} ;
