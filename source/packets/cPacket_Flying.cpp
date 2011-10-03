#include "cPacket_Flying.h"

bool cPacket_Flying::Parse(cSocket & a_Socket)
{
	m_Socket = a_Socket;
	if( !ReadBool( m_bFlying ) ) return false;
	return true;
}