
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
	void PlayRecord(void);
	
	/// Ejects the currently held record as a pickup. Does nothing when no record inserted.
	void EjectRecord(void);
	
	// tolua_end
	
	virtual void UsedBy(cPlayer * a_Player) override;
	virtual void SendTo(cClientHandle &) override { };

private:
	int m_Record;
} ;  // tolua_end




