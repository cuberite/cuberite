
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_BlockAction.h"





cPacket_BlockAction::cPacket_BlockAction( const cPacket_BlockAction & a_Copy )
{
        m_PacketID	= E_BLOCK_ACTION;
        m_PosX			= a_Copy.m_PosX;
        m_PosY			= a_Copy.m_PosY;
        m_PosZ			= a_Copy.m_PosZ;
        m_Byte1			= a_Copy.m_Byte1;
        m_Byte2			= a_Copy.m_Byte2;
}

bool cPacket_BlockAction::Send(cSocket & a_Socket)
{
	unsigned int TotalSize = c_Size;
	char* Message = new char[TotalSize];

	unsigned int i = 0;
	AppendByte		( (char)m_PacketID, Message, i );
	AppendInteger	( m_PosX,  Message, i );
	AppendShort		( m_PosY,  Message, i );
	AppendInteger	( m_PosZ,  Message, i );
	AppendByte		( m_Byte1, Message, i );
	AppendByte		( m_Byte2, Message, i );

	bool RetVal = !cSocket::IsSocketError( SendData( a_Socket, Message, TotalSize, 0 ) );
	delete [] Message;
	return RetVal;
}
