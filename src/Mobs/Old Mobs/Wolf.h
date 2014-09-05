
#pragma once

#include "PassiveAggressiveMonster.h"
#include "../Entities/Entity.h"





class cWolf :
	public cPassiveAggressiveMonster
{
	typedef cPassiveAggressiveMonster super;
	
public:
	cWolf(void);

	CLASS_PROTODEF(cWolf)

	virtual bool DoTakeDamage(TakeDamageInfo & a_TDI) override;
	virtual void OnRightClicked(cPlayer & a_Player) override;
	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;
	virtual void TickFollowPlayer();
	virtual void Attack(float a_Dt) override;

	// Get functions
	bool    IsSitting     (void) const { return m_IsSitting; }
	bool    IsTame        (void) const { return m_IsTame; }
	bool    IsBegging     (void) const { return m_IsBegging; }
	bool    IsAngry       (void) const { return m_IsAngry; }
	AString GetOwnerName  (void) const { return m_OwnerName; }
	AString GetOwnerUUID  (void) const { return m_OwnerUUID; }
	int     GetCollarColor(void) const { return m_CollarColor; }

	// Set functions
	void SetIsSitting  (bool a_IsSitting)   { m_IsSitting = a_IsSitting; }
	void SetIsTame     (bool a_IsTame)      { m_IsTame = a_IsTame; }
	void SetIsBegging  (bool a_IsBegging)   { m_IsBegging = a_IsBegging; }
	void SetIsAngry    (bool a_IsAngry)     { m_IsAngry = a_IsAngry; }
	void SetCollarColor(int a_CollarColor)  { m_CollarColor = a_CollarColor; }
	void SetOwner      (const AString & a_NewOwnerName, const AString & a_NewOwnerUUID)
	{
		m_OwnerName = a_NewOwnerName;
		m_OwnerUUID = a_NewOwnerUUID;
	}

protected:

	bool    m_IsSitting;
	bool    m_IsTame;
	bool    m_IsBegging;
	bool    m_IsAngry;
	AString m_OwnerName;
	AString m_OwnerUUID;
	int     m_CollarColor;
} ;




