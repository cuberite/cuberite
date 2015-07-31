
#pragma once

#include "BlockEntity.h"
#include "RedstonePoweredEntity.h"





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
	// tolua_end
	, public cRedstonePoweredEntity
	// tolua_begin
{
	typedef cBlockEntity super;
public:

	// tolua_end

	BLOCKENTITY_PROTODEF(cNoteEntity)
	
	/** Creates a new note entity. a_World may be nullptr */
	cNoteEntity(int a_X, int a_Y, int a_Z, cWorld * a_World);
	virtual ~cNoteEntity() {}

	// tolua_begin
	
	char GetPitch(void);
	void SetPitch(char a_Pitch);
	void IncrementPitch(void);
	void MakeSound(void);
	
	// tolua_end
	
	virtual void UsedBy(cPlayer * a_Player) override;
	virtual void SendTo(cClientHandle &) override {}
	
	virtual void SetRedstonePower(bool a_Value) override
	{
		if (a_Value)
		{
			MakeSound();
		}
	}

private:
	char m_Pitch;
} ;  // tolua_export




