
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





class cNoteEntity : 
	public cBlockEntity
{
public:
	cNoteEntity(int a_X, int a_Y, int a_Z, cWorld * a_World);
	virtual ~cNoteEntity();

	bool LoadFromJson( const Json::Value& a_Value );
	virtual void SaveToJson( Json::Value& a_Value ) override;

	char GetPitch( void );
	void SetPitch( char a_Pitch );
	void IncrementPitch( void );
	void MakeSound( void );
	virtual void UsedBy( cPlayer * a_Player ) override;
	virtual void SendTo(cClientHandle & a_Client) override { };

private:
	unsigned char m_Pitch;
};




