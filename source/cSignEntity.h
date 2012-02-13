
#pragma once

#include "cBlockEntity.h"





namespace Json
{
	class Value;
}





class cSignEntity : 
	public cBlockEntity
{
public:
	cSignEntity(ENUM_BLOCK_ID a_BlockType, int a_X, int a_Y, int a_Z, cWorld * a_World);
	virtual ~cSignEntity();

	bool LoadFromFile(cFile & a_File);  // deprecated format

	bool LoadFromJson( const Json::Value& a_Value );
	void SaveToJson( Json::Value& a_Value );

	void SetLines( const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4 );
	void SetLine( int a_Index, const AString & a_Line );

	AString GetLine( int a_Index ) const;

	virtual void UsedBy( cPlayer & a_Player );
	virtual void SendTo( cClientHandle* a_Client );
	
private:

	AString  m_Line[4];
};




