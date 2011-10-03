#include "cPacket_BlockPlace.h"

bool cPacket_BlockPlace::Parse(cSocket & a_Socket)
{
	m_Socket = a_Socket;
	if( !ReadInteger( m_PosX ) ) return false;
	if( !ReadByte	( m_PosY ) ) return false;
	if( !ReadInteger( m_PosZ ) ) return false;
	if( !ReadByte	( m_Direction ) ) return false;

	if( !ReadShort	( m_ItemType ) ) return false;
	if( m_ItemType > -1 )
	{
		if( !ReadByte	( m_Count ) ) return false;
		if( !ReadShort	( m_Uses ) ) return false;
	}
	return true;
}