
#pragma once

#include "PassiveMonster.h"





class cOcelot :
	public cPassiveMonster
{
	typedef cPassiveMonster super;

public:

	enum eCatType
	{
		ctWildOcelot,
		ctTuxedo,
		ctTabby,
		ctSiamese,
	} ;

	cOcelot(void);

	CLASS_PROTODEF(cOcelot)

	virtual void OnRightClicked(cPlayer & a_Player) override;
	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	virtual void TickFollowPlayer();
	virtual void SpawnOn(cClientHandle & a_ClientHandle) override;
	virtual void GetBreedingItems(cItems & a_Items) override
	{
		a_Items.Add(E_ITEM_RAW_FISH);
	}

	// Get functions
	bool     IsSitting     (void) const override { return m_IsSitting; }
	bool     IsTame        (void) const override { return m_IsTame; }
	bool     IsBegging     (void) const { return m_IsBegging; }
	AString  GetOwnerName  (void) const { return m_OwnerName; }
	AString  GetOwnerUUID  (void) const { return m_OwnerUUID; }
	eCatType GetOcelotType (void) const { return m_CatType; }

	// Set functions
	void SetIsSitting  (bool a_IsSitting)   { m_IsSitting = a_IsSitting; }
	void SetIsTame     (bool a_IsTame)      { m_IsTame = a_IsTame; }
	void SetIsBegging  (bool a_IsBegging)   { m_IsBegging = a_IsBegging; }
	void SetOwner      (const AString & a_NewOwnerName, const AString & a_NewOwnerUUID)
	{
		m_OwnerName = a_NewOwnerName;
		m_OwnerUUID = a_NewOwnerUUID;
	}
	void SetCatType (eCatType a_CatType)    { m_CatType = a_CatType; }

protected:

	bool      m_IsSitting;
	bool      m_IsTame;
	bool      m_IsBegging;
	eCatType  m_CatType;
	/** Only check for a nearby player holding the breeding items every 23 ticks. */
	int       m_CheckPlayerTickCount;
	AString   m_OwnerName;
	AString   m_OwnerUUID;
} ;




