
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
	return IsPlayingRecord() ? cItem(PaletteUpgrade::FromItem(static_cast<short>(m_Record), 0)) : cItems();
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
	m_World->BroadcastSoundParticleEffect(EffectID::SFX_RANDOM_PLAY_MUSIC_DISC, GetPos(), PaletteUpgrade::ToItem(m_Record).first);
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

	m_World->SpawnItemPickups(cItem(PaletteUpgrade::FromItem(static_cast<short>(m_Record), 0)), Vector3d(0.5, 0.5, 0.5) + m_Pos, 10);
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
