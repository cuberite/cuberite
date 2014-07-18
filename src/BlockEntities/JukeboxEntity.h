
#pragma once

#include "BlockEntity.h"
#include "../Entities/Player.h"





namespace Json
{
	class Value;
}





// tolua_begin

class cJukeboxEntity :
	public cBlockEntity
{
	typedef cBlockEntity super;
public:

	// tolua_end
	
	cJukeboxEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World);
	virtual ~cJukeboxEntity();

	bool LoadFromJson(const Json::Value & a_Value);
	virtual void SaveToJson(Json::Value & a_Value) override;

	// tolua_begin
	
	int  GetRecord(void);
	void SetRecord(int a_Record);
	
	/** Plays the specified Record. Return false if a_Record isn't a playable Record (E_ITEM_XXX_DISC).
	If there is a record already playing, ejects it first. */
	bool PlayRecord(int a_Record);
	
	/** Ejects the currently held record as a pickup. Return false when no record had been inserted. */
	bool EjectRecord(void);
	
	/** Is in the Jukebox a Record? */
	bool IsPlayingRecord(void);
	
	static bool IsRecordItem(int a_Item)
	{
		return ((a_Item >= E_ITEM_FIRST_DISC) && (a_Item <= E_ITEM_LAST_DISC));
	}
	
	// tolua_end

	static const char * GetClassStatic(void) { return "cJukeboxEntity"; }
	
	virtual void UsedBy(cPlayer * a_Player) override;
	virtual void SendTo(cClientHandle &) override { };

private:
	int m_Record;
} ;  // tolua_end




