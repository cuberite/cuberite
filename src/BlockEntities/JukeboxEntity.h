
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

	cJukeboxEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World);

	// tolua_begin

	int  GetRecord(void);
	void SetRecord(int a_Record);

	/** Plays the specified Record. Return false if a_Record isn't a playable Record (E_ITEM_XXX_DISC).
	If there is a record already playing, ejects it first. */
	bool PlayRecord(int a_Record);

	/** Ejects the currently held record as a pickup. Return false when no record had been inserted. */
	bool EjectRecord(void);

	/** Is in the Jukebox a Record? */
	bool IsPlayingRecord(void) const;

	static bool IsRecordItem(int a_Item)
	{
		return ((a_Item >= E_ITEM_FIRST_DISC) && (a_Item <= E_ITEM_LAST_DISC));
	}

	// tolua_end

	// cBlockEntity overrides:
	virtual void Destroy(void) override;
	virtual cItems ConvertToPickups() const override;
	virtual void CopyFrom(const cBlockEntity & a_Src) override;
	virtual bool UsedBy(cPlayer * a_Player) override;
	virtual void SendTo(cClientHandle &) override {}

private:
	int m_Record;
} ;  // tolua_end
