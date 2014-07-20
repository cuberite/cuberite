
#pragma once

#include "BlockEntity.h"


namespace Json
{
	class Value;
}





enum ENUM_NOTE_INSTRUMENTS
{
	E_INST_HARP_PIANO  = 0,
	E_INST_DOUBLE_BASS = 1,
	E_INST_SNARE_DRUM  = 2,
	E_INST_CLICKS      = 3,
	E_INST_BASS_DRUM   = 4
};





// tolua_begin

class cNoteEntity :
	public cBlockEntity
{
	typedef cBlockEntity super;
public:

	// tolua_end

	/// Creates a new note entity. a_World may be NULL
	cNoteEntity(int a_X, int a_Y, int a_Z, cWorld * a_World);

	bool LoadFromJson(const Json::Value & a_Value);
	virtual void SaveToJson(Json::Value & a_Value) override;

	// tolua_begin
	
	char GetPitch(void);
	void SetPitch(char a_Pitch);
	void IncrementPitch(void);
	void MakeSound(void);
	
	// tolua_end
	
	virtual void UsedBy(cPlayer * a_Player) override;
	virtual void SendTo(cClientHandle &) override { };

	static const char * GetClassStatic(void) { return "cNoteEntity"; }

private:
	char m_Pitch;
} ;  // tolua_export




