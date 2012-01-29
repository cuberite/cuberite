
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_PreChunk.h"





bool cPacket_PreChunk::Send(cSocket & a_Socket)
{
	unsigned int TotalSize = c_Size;
	char* Message = new char[TotalSize];

	unsigned int i = 0;
	AppendByte	 ( (char)m_PacketID,	Message, i );
	AppendInteger( m_PosX,		Message, i );
	AppendInteger( m_PosZ,		Message, i );
	AppendBool   ( m_bLoad,		Message, i );

	bool RetVal = !cSocket::IsSocketError( SendData( a_Socket, Message, TotalSize, 0 ) );
	delete [] Message;
	return RetVal;
}
