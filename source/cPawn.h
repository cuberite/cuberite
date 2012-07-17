
#pragma once

#include "cEntity.h"





struct TakeDamageInfo		//tolua_export
{							//tolua_export
	int Damage;				//tolua_export
	cEntity* Instigator;	//tolua_export
};							//tolua_export





class cPawn :	public cEntity													//tolua_export
{																				//tolua_export
public:
	CLASS_PROTOTYPE();

	cPawn();
	virtual ~cPawn();

	virtual void TeleportToEntity( cEntity* a_Entity );													//tolua_export
	virtual void TeleportTo( const double & a_PosX, const double & a_PosY, const double & a_PosZ );		//tolua_export

	virtual void Tick(float a_Dt) override;

	void Heal( int a_Health );													//tolua_export
	virtual void TakeDamage( int a_Damage, cEntity* a_Instigator );				//tolua_export
	virtual void KilledBy( cEntity* a_Killer );									//tolua_export
	int GetHealth() { return m_Health; }										//tolua_export

	enum MetaData {NORMAL, BURNING, CROUCHED, RIDING, SPRINTING, EATING, BLOCKING};

	virtual void SetMetaData(MetaData a_MetaData);
	virtual MetaData GetMetaData() { return m_MetaData; }

	virtual void InStateBurning(float a_Dt);

	virtual void CheckMetaDataBurn();

	virtual void SetMaxHealth(short a_MaxHealth);
	virtual short GetMaxHealth() { return m_MaxHealth; }

protected:

	short m_Health;
	short m_MaxHealth;
	

	bool m_bBurnable;

	MetaData m_MetaData;

	double m_LastPosX, m_LastPosY, m_LastPosZ;
	float m_TimeLastTeleportPacket;

	float m_FireDamageInterval;
	float m_BurnPeriod;

}; //tolua_export




