
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
	// tolua_end

	using Super = cBlockEntity;

public:  // tolua_export

	/** Creates a new note entity. a_World may be nullptr */
	cNoteEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World);

	// tolua_begin

	unsigned char GetNote(void);
	void SetNote(unsigned char a_Note);
	void IncrementNote(void);
	void MakeSound(void);

	// tolua_end

	// cBlockEntity overrides:
	virtual void CopyFrom(const cBlockEntity & a_Src) override;
	virtual bool UsedBy(cPlayer * a_Player) override;
	virtual void SendTo(cClientHandle &) override {}

	/** Returns the relative pitch (used in the protocol)
	from a note value between 0 and 24 (used in m_Note). */
	static float PitchFromNote(unsigned char a_Note);

private:

	unsigned char m_Note;
} ;  // tolua_export
