
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

	//virtual void SetMaxFood(short a_MaxFood);
	virtual short GetMaxFood() { return m_MaxFoodLevel / 6; }
	virtual short GetFood() { return m_FoodLevel / 6; }

	//virtual void SetMaxFoodSaturation(float a_MaxFoodSaturation);
	virtual float GetMaxFoodSaturation() { return fmod(m_MaxFoodLevel, 6.f); }
	virtual float GetFoodSaturation() { return fmod(m_FoodLevel, 6.f); }

	virtual void SetMaxFoodLevel(short a_MaxFoodLevel);
	virtual short GetMaxFoodLevel() { return m_MaxFoodLevel; }

protected:

	short m_Health;
	short m_FoodLevel;
	short m_MaxHealth;
	short m_MaxFoodLevel;

	bool m_bBurnable;

	MetaData m_MetaData;

	double m_LastPosX, m_LastPosY, m_LastPosZ;
	float m_TimeLastTeleportPacket;

	float m_FireDamageInterval;
	float m_BurnPeriod;

}; //tolua_export




