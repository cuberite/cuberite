
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_UseEntity.h"





bool cPacket_UseEntity::Parse(cSocket & a_Socket)
{
	m_Socket = a_Socket;
	if( !ReadInteger(m_UniqueID) ) return false;
	if( !ReadInteger(m_TargetID) ) return false;
	if( !ReadBool (m_bLeftClick) ) return false;
	return true;
}