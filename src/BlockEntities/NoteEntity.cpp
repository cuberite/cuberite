
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "NoteEntity.h"
#include "../World.h"
#include "json/value.h"





cNoteEntity::cNoteEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World) :
	super(E_BLOCK_NOTE_BLOCK, a_BlockX, a_BlockY, a_BlockZ, a_World),
	m_Pitch(0)
{
}





void cNoteEntity::UsedBy(cPlayer * a_Player)
{
	UNUSED(a_Player);
	IncrementPitch();
	MakeSound();
}





void cNoteEntity::MakeSound(void)
{
	char instrument;
	AString sampleName;
	
	switch (m_World->GetBlock(m_PosX, m_PosY - 1, m_PosZ))
	{
		case E_BLOCK_PLANKS:
		case E_BLOCK_LOG:
		case E_BLOCK_NOTE_BLOCK:
		{
			// TODO: add other wood-based blocks if needed
			instrument = E_INST_DOUBLE_BASS;
			sampleName = "note.db";
			break;
		}
		
		case E_BLOCK_SAND:
		case E_BLOCK_GRAVEL:
		case E_BLOCK_SOULSAND:
		{
			instrument = E_INST_SNARE_DRUM;
			sampleName = "note.snare";
			break;
		}
		
		case E_BLOCK_GLASS:
		case E_BLOCK_GLASS_PANE:
		case E_BLOCK_GLOWSTONE:
		{
			instrument = E_INST_CLICKS;
			sampleName = "note.hat";
			break;
		}

		case E_BLOCK_STONE:
		case E_BLOCK_STONE_BRICKS:
		case E_BLOCK_COBBLESTONE:
		case E_BLOCK_OBSIDIAN:
		case E_BLOCK_NETHERRACK:
		case E_BLOCK_BRICK:
		case E_BLOCK_NETHER_BRICK:
		{
			// TODO: add other stone-based blocks if needed
			instrument = E_INST_BASS_DRUM;
			sampleName = "note.bassattack";
			break;
		}

		default:
		{
			instrument = E_INST_HARP_PIANO;
			sampleName = "note.harp";
			break;
		}
	}

	m_World->BroadcastBlockAction(m_PosX, m_PosY, m_PosZ, static_cast<Byte>(instrument), static_cast<Byte>(m_Pitch), E_BLOCK_NOTE_BLOCK);
	
	// TODO: instead of calculating the power function over and over, make a precalculated table - there's only 24 pitches after all
	float calcPitch = static_cast<float>(pow(2.0f, static_cast<float>(m_Pitch - 12.0f) / 12.0f));
	m_World->BroadcastSoundEffect(
		sampleName,
		static_cast<double>(m_PosX),
		static_cast<double>(m_PosY),
		static_cast<double>(m_PosZ),
		3.0f,
		calcPitch
	);
}





char cNoteEntity::GetPitch(void)
{
	return m_Pitch;
}





void cNoteEntity::SetPitch(char a_Pitch)
{
	m_Pitch = a_Pitch % 25;
}





void cNoteEntity::IncrementPitch(void)
{
	SetPitch(m_Pitch + 1);
}




