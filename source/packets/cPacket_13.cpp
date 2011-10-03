#include "cPacket_13.h"

bool cPacket_13::Parse(cSocket & a_Socket)
{
	m_Socket = a_Socket;

	if( !ReadInteger( m_EntityID ) )	return false;
	if( !ReadByte	( m_ActionID ) )	return false;
	return true;
}