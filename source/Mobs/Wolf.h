
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
	bool IsSitting(void) const     { return m_IsSitting; }
	bool IsTame(void) const        { return m_IsTame; }
	bool IsBegging(void) const     { return m_IsBegging; }
	bool IsAngry(void) const       { return m_IsAngry; }
	AString GetOwner(void) const   { return m_Owner; }
	int GetCollarColor(void) const { return m_Collar; }

	// Set functions
	void SetIsSitting(bool a_IsSitting)      { m_IsSitting = a_IsSitting; }
	void SetIsTame(bool a_IsTame)            { m_IsTame = a_IsTame; }
	void SetIsBegging(bool a_IsBegging)      { m_IsBegging = a_IsBegging; }
	void SetIsAngry(bool a_IsAngry)          { m_IsAngry = a_IsAngry; }
	void SetOwner(AString a_NewOwner)        { m_Owner = a_NewOwner; }

private:

	bool    m_IsSitting;
	bool    m_IsTame;
	bool    m_IsBegging;
	bool    m_IsAngry;
	AString m_Owner;
	int     m_Collar;
} ;




