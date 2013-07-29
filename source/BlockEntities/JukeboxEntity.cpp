
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "JukeboxEntity.h"
#include "../World.h"
#include <json/json.h>





cJukeboxEntity::cJukeboxEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World)
	: cBlockEntity(E_BLOCK_JUKEBOX, a_BlockX, a_BlockY, a_BlockZ, a_World)
	, m_Record( 0 )
{
}





cJukeboxEntity::~cJukeboxEntity()
{
	if (m_Record >= 2256 && m_Record <= 2267)
	{
		EjectRecord();
		m_Record = 0;
	}
}





void cJukeboxEntity::UsedBy(cPlayer * a_Player)
{
	if (m_Record == 0)
	{
		const cItem & HeldItem = a_Player->GetEquippedItem();
		if (HeldItem.m_ItemType >= 2256 && HeldItem.m_ItemType <= 2267)
		{
			m_Record = HeldItem.m_ItemType;
			a_Player->GetInventory().RemoveOneEquippedItem();
			PlayRecord();
		}
	}
	else if (m_Record >= 2256 && m_Record <= 2267)
	{
		EjectRecord();
		m_Record = 0;
	}
}





void cJukeboxEntity::PlayRecord( void )
{
	m_World->BroadcastSoundParticleEffect(1005, m_PosX * 8, m_PosY * 8, m_PosZ * 8, m_Record);
}





void cJukeboxEntity::EjectRecord( void )
{
	cItems Drops;
	Drops.push_back(cItem(m_Record, 1, 0));
	m_World->SpawnItemPickups(Drops, m_PosX, m_PosY+1, m_PosZ);
	m_World->BroadcastSoundParticleEffect(1005, m_PosX * 8, m_PosY * 8, m_PosZ * 8, 0);
}





int cJukeboxEntity::GetRecord( void )
{
	return m_Record;
}





void cJukeboxEntity::SetRecord( int a_Record )
{
	m_Record = a_Record;
}





bool cJukeboxEntity::LoadFromJson( const Json::Value & a_Value )
{
	m_PosX = a_Value.get("x", 0).asInt();
	m_PosY = a_Value.get("y", 0).asInt();
	m_PosZ = a_Value.get("z", 0).asInt();

	m_Record = a_Value.get("Record", 0).asInt();

	return true;
}





void cJukeboxEntity::SaveToJson( Json::Value & a_Value )
{
	a_Value["x"] = m_PosX;
	a_Value["y"] = m_PosY;
	a_Value["z"] = m_PosZ;

	a_Value["Record"] = m_Record;
}




