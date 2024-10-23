
#pragma once

#include "BlockEntity.h"
#include "../BlockType.h"





// tolua_begin

class cJukeboxEntity :
	public cBlockEntity
{
	// tolua_end

	using Super = cBlockEntity;

public:  // tolua_export

	cJukeboxEntity(BlockState a_Block, Vector3i a_Pos, cWorld * a_World);

	// tolua_begin

	Item  GetRecord(void);
	void SetRecord(Item a_Record);

	/** Plays the specified Record. Return false if a_Record isn't a playable Record (E_ITEM_XXX_DISC).
	If there is a record already playing, ejects it first. */
	bool PlayRecord(Item a_Record);

	/** Ejects the currently held record as a pickup. Return false when no record had been inserted. */
	bool EjectRecord(void);

	/** Is in the Jukebox a Record? */
	bool IsPlayingRecord(void) const;

	static bool IsRecordItem(Item a_Item)
	{
		switch (a_Item)
		{
			case Item::MusicDiscBlocks:
			case Item::MusicDiscCat:
			case Item::MusicDiscChirp:
			case Item::MusicDiscFar:
			case Item::MusicDiscMall:
			case Item::MusicDiscMellohi:
			case Item::MusicDiscPigstep:
			case Item::MusicDiscStal:
			case Item::MusicDiscStrad:
			case Item::MusicDiscWait:
			case Item::MusicDiscWard:
			case Item::MusicDisc11:
			case Item::MusicDisc13:
			case Item::MusicDisc5:
			case Item::MusicDiscCreator:
			case Item::MusicDiscCreatorMusicBox:
			case Item::MusicDiscOtherside:
			case Item::MusicDiscPrecipice:
			case Item::MusicDiscRelic:
				return true;
			default: return false;
		}
	}

	// tolua_end

	// cBlockEntity overrides:
	virtual void Destroy(void) override;
	virtual cItems ConvertToPickups() const override;
	virtual void CopyFrom(const cBlockEntity & a_Src) override;
	virtual bool UsedBy(cPlayer * a_Player) override;
	virtual void SendTo(cClientHandle &) override {}

private:
	Item m_Record;
} ;  // tolua_end
