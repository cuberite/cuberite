
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
	virtual void TickFollowPlayer();

	// Get functions
	bool    IsSitting     (void) const { return m_IsSitting; }
	bool    IsTame        (void) const { return m_IsTame; }
	bool    IsBegging     (void) const { return m_IsBegging; }
	bool    IsAngry       (void) const { return m_IsAngry; }
	AString GetOwner      (void) const { return m_OwnerName; }
	int     GetCollarColor(void) const { return m_CollarColor; }

	// Set functions
	void SetIsSitting  (bool a_IsSitting)   { m_IsSitting = a_IsSitting; }
	void SetIsTame     (bool a_IsTame)      { m_IsTame = a_IsTame; }
	void SetIsBegging  (bool a_IsBegging)   { m_IsBegging = a_IsBegging; }
	void SetIsAngry    (bool a_IsAngry)     { m_IsAngry = a_IsAngry; }
	void SetOwner      (AString a_NewOwner) { m_OwnerName = a_NewOwner; }
	void SetCollarColor(int a_CollarColor)  { m_CollarColor = a_CollarColor; }

protected:

	bool      m_IsSitting;
	bool      m_IsTame;
	bool      m_IsBegging;
	bool      m_IsAngry;
	AString   m_OwnerName;
	int       m_CollarColor;
} ;




