#pragma once
#include "cEntity.h"
#include "math.h"

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

	virtual void TeleportTo( cEntity* a_Entity );														//tolua_export
	virtual void TeleportTo( const double & a_PosX, const double & a_PosY, const double & a_PosZ );		//tolua_export

	virtual void Tick(float a_Dt);

	void Heal( int a_Health );													//tolua_export
	virtual void TakeDamage( int a_Damage, cEntity* a_Instigator );				//tolua_export
	virtual void KilledBy( cEntity* a_Killer );									//tolua_export
	int GetHealth() { return m_Health; }										//tolua_export

	enum MetaData {NORMAL, BURNING, CROUCHED, RIDING, SPRINTING, EATING, BLOCKING};

	virtual inline void SetMetaData(MetaData a_MetaData);
	virtual inline MetaData GetMetaData() { return m_MetaData; }

	virtual inline void InStateBurning(float a_Dt);

	virtual void CheckMetaDataBurn();

	virtual inline void SetMaxHealth(short a_MaxHealth);
	virtual inline short GetMaxHealth() { return m_MaxHealth; }

	//virtual inline void SetMaxFood(short a_MaxFood);
	virtual inline short GetMaxFood() { return m_MaxFoodLevel/6; }
	virtual inline short GetFood() { return m_FoodLevel/6; }

	//virtual inline void SetMaxFoodSaturation(float a_MaxFoodSaturation);
	virtual inline float GetMaxFoodSaturation() { return fmod(m_MaxFoodLevel, 6.f); }
	virtual inline float GetFoodSaturation() { return fmod(m_FoodLevel, 6.f); }

	virtual inline void SetMaxFoodLevel(short a_MaxFoodLevel);
	virtual inline short GetMaxFoodLevel() { return m_MaxFoodLevel; }

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
