
// JukeboxEntity.cpp

// Implements the cJukeboxEntity class representing a single jukebox in the world

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules
#include "JukeboxEntity.h"
#include "../World.h"
#include "../EffectID.h"
#include "json/value.h"
#include "../Entities/Player.h"




cJukeboxEntity::cJukeboxEntity(BlockState a_Block, Vector3i a_Pos, cWorld * a_World):
	Super(a_Block, a_Pos, a_World),
	m_Record(Item::Air)
{
	ASSERT(a_Block.Type() == BlockType::Jukebox);
}





void cJukeboxEntity::Destroy(void)
{
	ASSERT(m_World != nullptr);
	m_World->BroadcastSoundParticleEffect(EffectID::SFX_RANDOM_PLAY_MUSIC_DISC, GetPos(), 0);
}





cItems cJukeboxEntity::ConvertToPickups() const
{
	return IsPlayingRecord() ? cItem(m_Record) : cItems();
}





void cJukeboxEntity::CopyFrom(const cBlockEntity & a_Src)
{
	Super::CopyFrom(a_Src);
	auto & src = static_cast<const cJukeboxEntity &>(a_Src);
	m_Record = src.m_Record;
}





bool cJukeboxEntity::UsedBy(cPlayer * a_Player)
{
	if (IsPlayingRecord())
	{
		EjectRecord();
		return true;
	}

	const cItem & HeldItem = a_Player->GetEquippedItem();
	if (PlayRecord(HeldItem.m_ItemType))
	{
		a_Player->GetStatistics().Custom[CustomStatistic::PlayRecord]++;

		if (!a_Player->IsGameModeCreative())
		{
			a_Player->GetInventory().RemoveOneEquippedItem();
		}

		return true;
	}

	// No state change, continue with block placement:
	return false;
}





bool cJukeboxEntity::PlayRecord(Item a_Record)
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
	int record_id = 0;
	switch (a_Record)
	{
		case Item::MusicDiscBlocks: record_id = 2; break;
		case Item::MusicDiscCat: record_id = 1; break;
		case Item::MusicDiscChirp: record_id = 3; break;
		case Item::MusicDiscFar: record_id = 4; break;
		case Item::MusicDiscMall: record_id = 5; break;
		case Item::MusicDiscMellohi: record_id = 6; break;
		case Item::MusicDiscPigstep: record_id = 12; break;
		case Item::MusicDiscStal: record_id = 7; break;
		case Item::MusicDiscStrad: record_id = 8; break;
		case Item::MusicDiscWait: record_id = 11; break;
		case Item::MusicDiscWard: record_id = 9; break;
		case Item::MusicDisc11: record_id = 10; break;
		case Item::MusicDisc13: record_id = 0; break;
		case Item::MusicDisc5: record_id = 14; break;
		case Item::MusicDiscCreator: record_id = 17; break;
		case Item::MusicDiscCreatorMusicBox: record_id = 18; break;
		case Item::MusicDiscOtherside: record_id = 13; break;
		case Item::MusicDiscPrecipice: record_id = 16; break;
		case Item::MusicDiscRelic: record_id = 15; break;
		default: break;
	}
	m_World->BroadcastSoundParticleEffect(EffectID::SFX_RANDOM_PLAY_MUSIC_DISC, GetPos(), record_id);
	m_World->FastSetBlock(m_Pos, Block::Jukebox::Jukebox(true));
	return true;
}





bool cJukeboxEntity::EjectRecord(void)
{
	if (!IsPlayingRecord())
	{
		// There's no record here
		return false;
	}

	m_World->SpawnItemPickups(cItem(m_Record), Vector3d(0.5, 0.5, 0.5) + m_Pos, 10);
	m_World->SetBlock(m_Pos, Block::Jukebox::Jukebox(false));
	m_World->BroadcastSoundParticleEffect(EffectID::SFX_RANDOM_PLAY_MUSIC_DISC, GetPos(), 0);

	m_Record = Item::Air;
	return true;
}





bool cJukeboxEntity::IsPlayingRecord(void) const
{
	return m_Record != Item::Air;
}





Item cJukeboxEntity::GetRecord(void)
{
	return m_Record;
}





void cJukeboxEntity::SetRecord(Item a_Record)
{
	m_Record = a_Record;
}
