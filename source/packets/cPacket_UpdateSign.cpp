
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_UpdateSign.h"





bool cPacket_UpdateSign::Parse( cSocket & a_Socket )
{
	m_Socket = a_Socket;

	if( !ReadInteger( m_PosX ) ) return false;
	if( !ReadShort	( m_PosY ) ) return false;
	if( !ReadInteger( m_PosZ ) ) return false;
	if( !ReadString16	( m_Line1 ) ) return false;
	if( !ReadString16	( m_Line2 ) ) return false;
	if( !ReadString16	( m_Line3 ) ) return false;
	if( !ReadString16	( m_Line4 ) ) return false;

	return true;
}

bool cPacket_UpdateSign::Send( cSocket & a_Socket )
{
	unsigned int TotalSize = c_Size;
	TotalSize += m_Line1.size() * sizeof( short );
	TotalSize += m_Line2.size() * sizeof( short );
	TotalSize += m_Line3.size() * sizeof( short );
	TotalSize += m_Line4.size() * sizeof( short );

	char* Message = new char[TotalSize];

	unsigned int i = 0;
	AppendByte		( (char)m_PacketID,	Message, i );
	AppendInteger	( m_PosX,		Message, i );
	AppendShort		( m_PosY,		Message, i );
	AppendInteger	( m_PosZ,		Message, i );
	AppendString16	( m_Line1,		Message, i );
	AppendString16	( m_Line2,		Message, i );
	AppendString16	( m_Line3,		Message, i );
	AppendString16	( m_Line4,		Message, i );

	bool RetVal = !cSocket::IsSocketError( SendData( a_Socket, Message, TotalSize, 0 ) );
	delete [] Message;
	return RetVal;
}