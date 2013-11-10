
#pragma once

#include "PassiveAggressiveMonster.h"
#include "../Entities/Entity.h"





class cWolf :
	public cPassiveAggressiveMonster
{
	typedef cPassiveAggressiveMonster super;
	
public:
	cWolf(void);

	CLASS_PROTODEF(cWolf);

	virtual void DoTakeDamage(TakeDamageInfo & a_TDI) override;
	virtual void OnRightClicked(cPlayer & a_Player) override;
	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;

	// Get functions
	bool IsSitting(void) const     { return m_bIsSitting; }
	bool IsTame(void) const        { return m_bIsTame; }
	bool IsBegging(void) const     { return m_bIsBegging; }
	bool IsAngry(void) const       { return m_bIsAngry; }
	AString GetOwner(void) const   { return m_bOwner; }
	int GetCollarColor(void) const { return m_bCollar; }

	// Set functions
	void SetIsSitting(bool a_IsSitting)      { m_bIsSitting = a_IsSitting; }
	void SetIsTame(bool a_IsTame)            { m_bIsTame = a_IsTame; }
	void SetIsBegging(bool a_IsBegging)      { m_bIsBegging = a_IsBegging; }
	void SetIsAngry(bool a_IsAngry)          { m_bIsAngry = a_IsAngry; }
	void SetOwner(AString a_NewOwner)        { m_bOwner = a_NewOwner; }

private:

	bool    m_bIsSitting;
	bool    m_bIsTame;
	bool    m_bIsBegging;
	bool    m_bIsAngry;
	AString m_bOwner;
	int     m_bCollar;
} ;




