
#pragma once

#include "BlockEntity.h"
#include "../Entities/Player.h"





namespace Json
{
	class Value;
}





class cJukeboxEntity : 
	public cBlockEntity
{
	typedef cBlockEntity super;
public:

	/// Creates a new jukebox entity that is not assigned to a world
	cJukeboxEntity(int a_BlockX, int a_BlockY, int a_BlockZ);
	
	cJukeboxEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World);
	virtual ~cJukeboxEntity();

	bool LoadFromJson( const Json::Value& a_Value );
	virtual void SaveToJson( Json::Value& a_Value ) override;

	int  GetRecord( void );
	void SetRecord( int a_Record );
	void PlayRecord( void );
	void EjectRecord( void );
	virtual void UsedBy( cPlayer * a_Player ) override;
	virtual void SendTo(cClientHandle & a_Client) override { };

private:
	int m_Record;
};




