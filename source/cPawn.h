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

	virtual void TeleportTo( cEntity* a_Entity );														//tolua_export
	virtual void TeleportTo( const double & a_PosX, const double & a_PosY, const double & a_PosZ );		//tolua_export

	void Heal( int a_Health );													//tolua_export
	virtual void TakeDamage( int a_Damage, cEntity* a_Instigator );				//tolua_export
	virtual void KilledBy( cEntity* a_Killer );									//tolua_export
	int GetHealth() { return m_Health; }										//tolua_export

protected:
	short m_Health;

	double m_LastPosX, m_LastPosY, m_LastPosZ;
	float m_TimeLastTeleportPacket;
}; //tolua_export
