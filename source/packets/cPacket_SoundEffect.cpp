
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_SoundEffect.h"





cPacket_SoundEffect::cPacket_SoundEffect( const cPacket_SoundEffect & a_Copy )
{
        m_PacketID	= E_SOUND_EFFECT;
        m_SoundID		=	a_Copy.m_SoundID;
        m_PosX			= a_Copy.m_PosX;
        m_PosY			= a_Copy.m_PosY;
        m_PosZ			= a_Copy.m_PosZ;
        m_SoundData	=	a_Copy.m_SoundData;
}


bool cPacket_SoundEffect::Send(cSocket & a_Socket)
{
	unsigned int TotalSize = c_Size;
	char* Message = new char[TotalSize];

	unsigned int i = 0;
	AppendByte    ( (char)m_PacketID, Message, i );
	AppendInteger ( m_SoundID, Message, i );
	AppendInteger ( m_PosX, Message, i );
	AppendByte    ( (char)m_PosY, Message, i );
	AppendInteger ( m_PosZ, Message, i );
	AppendInteger ( m_SoundData, Message, i );

	bool RetVal = !cSocket::IsSocketError( SendData( a_Socket, Message, TotalSize, 0 ) );
	delete [] Message;
	return RetVal;
}
