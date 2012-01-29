
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_ArmAnim.h"





bool cPacket_ArmAnim::Parse( cSocket & a_Socket )
{
	m_Socket = a_Socket;
	if(!ReadInteger(m_EntityID) ) return false;
	if(!ReadByte(m_Animation) ) return false;
	return true;
}

bool cPacket_ArmAnim::Send( cSocket & a_Socket )
{
	unsigned int TotalSize = c_Size;
	char* Message = new char[TotalSize];

	unsigned int i = 0;
	AppendByte	 ( (char)m_PacketID, Message, i );
	AppendInteger( m_EntityID, Message, i );
	AppendByte   ( m_Animation, Message, i );

	bool RetVal = !cSocket::IsSocketError( SendData( a_Socket, Message, TotalSize, 0 ) );
	delete [] Message;
	return RetVal;
}