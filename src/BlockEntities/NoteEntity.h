
#pragma once

#include "BlockEntity.h"





enum ENUM_NOTE_INSTRUMENTS
{
	E_INST_HARP_PIANO  = 0,
	E_INST_DOUBLE_BASS = 1,
	E_INST_SNARE_DRUM  = 2,
	E_INST_CLICKS      = 3,
	E_INST_BASS_DRUM   = 4,
	E_INST_FLUTE       = 5,
	E_INST_BELL        = 6,
	E_INST_GUITAR      = 7,
	E_INST_CHIME       = 8,
	E_INST_XYLOPHONE   = 9
};





// tolua_begin

class cNoteEntity :
	public cBlockEntity
{
	typedef cBlockEntity Super;
public:

	// tolua_end

	BLOCKENTITY_PROTODEF(cNoteEntity)

	/** Creates a new note entity. a_World may be nullptr */
	cNoteEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World);
	virtual ~cNoteEntity() override {}

	// tolua_begin

	char GetPitch(void);
	void SetPitch(char a_Pitch);
	void IncrementPitch(void);
	void MakeSound(void);

	// tolua_end

	// cBlockEntity overrides:
	virtual void CopyFrom(const cBlockEntity & a_Src) override;
	virtual bool UsedBy(cPlayer * a_Player) override;
	virtual void SendTo(cClientHandle &) override {}

private:
	char m_Pitch;
} ;  // tolua_export




