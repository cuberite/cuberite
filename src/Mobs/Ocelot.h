
#pragma once

#include "PassiveMonster.h"
#include "../UUID.h"





class cOcelot:
	public cPassiveMonster
{
	using Super = cPassiveMonster;

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
	virtual void GetBreedingItems(cItems & a_Items) override
	{
		a_Items.Add(E_ITEM_RAW_FISH);
	}
	virtual bool DoTakeDamage(TakeDamageInfo & a_TDI) override;

	// Get functions
	bool     IsSitting     (void) const override { return m_IsSitting; }
	bool     IsTame        (void) const override { return m_IsTame; }
	bool     IsBegging     (void) const { return m_IsBegging; }
	AString  GetOwnerName  (void) const { return m_OwnerName; }
	cUUID    GetOwnerUUID  (void) const { return m_OwnerUUID; }
	eCatType GetOcelotType (void) const { return m_CatType; }

	// Set functions
	void SetIsSitting  (bool a_IsSitting)   { m_IsSitting = a_IsSitting; }
	void SetIsTame     (bool a_IsTame)      { m_IsTame = a_IsTame; }
	void SetIsBegging  (bool a_IsBegging)   { m_IsBegging = a_IsBegging; }
	void SetOwner      (const AString & a_NewOwnerName, const cUUID & a_NewOwnerUUID)
	{
		m_OwnerName = a_NewOwnerName;
		m_OwnerUUID = a_NewOwnerUUID;
	}
	void SetCatType (eCatType a_CatType)    { m_CatType = a_CatType; }

	/** Returns true if there's a cat sitting above the given position */
	static bool IsCatSittingOnBlock(cWorld * a_World, Vector3d a_BlockPosition);

protected:

	bool      m_IsSitting;
	bool      m_IsTame;
	bool      m_IsBegging;
	eCatType  m_CatType;
	/** Only check for a nearby player holding the breeding items every 23 ticks. */
	int       m_CheckPlayerTickCount;
	AString   m_OwnerName;
	cUUID     m_OwnerUUID;
} ;




