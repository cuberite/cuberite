
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "JukeboxEntity.h"
#include "../World.h"
#include "../EffectID.h"
#include "json/value.h"
#include "Entities/Player.h"




cJukeboxEntity::cJukeboxEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World):
	Super(a_BlockType, a_BlockMeta, a_BlockX, a_BlockY, a_BlockZ, a_World),
	m_Record(0)
{
	ASSERT(a_BlockType == E_BLOCK_JUKEBOX);
}





cJukeboxEntity::~cJukeboxEntity()
{
}





void cJukeboxEntity::CopyFrom(const cBlockEntity & a_Src)
{
	Super::CopyFrom(a_Src);
	auto & src = reinterpret_cast<const cJukeboxEntity &>(a_Src);
	m_Record = src.m_Record;
}





bool cJukeboxEntity::UsedBy(cPlayer * a_Player)
{
	if (IsPlayingRecord())
	{
		EjectRecord();
		return true;
	}
	else
	{
		const cItem & HeldItem = a_Player->GetEquippedItem();
		if (PlayRecord(HeldItem.m_ItemType))
		{
			a_Player->GetInventory().RemoveOneEquippedItem();
			return true;
		}
	}
	return false;
}





bool cJukeboxEntity::PlayRecord(int a_Record)
{
	if (!IsRecordItem(a_Record))
	{
		// This isn't a Record Item
		return false;
	}
	if (IsPlayingRecord())
	{
		// A Record is already in the Jukebox.
		EjectRecord();
	}
	m_Record = a_Record;
	m_World->BroadcastSoundParticleEffect(EffectID::SFX_RANDOM_PLAY_MUSIC_DISC, m_PosX, m_PosY, m_PosZ, m_Record);
	m_World->SetBlockMeta(m_PosX, m_PosY, m_PosZ, E_META_JUKEBOX_ON);
	return true;
}





bool cJukeboxEntity::EjectRecord(void)
{
	if (!IsPlayingRecord())
	{
		// There's no record here
		return false;
	}

	cItems Drops;
	Drops.push_back(cItem(static_cast<short>(m_Record), 1, 0));
	m_Record = 0;
	m_World->SpawnItemPickups(Drops, m_PosX + 0.5, m_PosY + 1, m_PosZ + 0.5, 8);
	m_World->BroadcastSoundParticleEffect(EffectID::SFX_RANDOM_PLAY_MUSIC_DISC, m_PosX, m_PosY, m_PosZ, 0);
	m_World->SetBlockMeta(m_PosX, m_PosY, m_PosZ, E_META_JUKEBOX_OFF);
	return true;
}





bool cJukeboxEntity::IsPlayingRecord(void)
{
	return (m_Record != 0);
}





int cJukeboxEntity::GetRecord(void)
{
	return m_Record;
}





void cJukeboxEntity::SetRecord(int a_Record)
{
	m_Record = a_Record;
}
