#include "Globals.h"
#include "Protocol_1_21.h"
#include "Packetizer.h"
#include <ClientHandle.h>





////////////////////////////////////////////////////////////////////////////////
//  cProtocol_1_21:

cProtocol::Version cProtocol_1_21::GetProtocolVersion() const
{
	return Version::v1_21;
}





UInt32 cProtocol_1_21::GetPacketID(ePacketType a_PacketType) const
{
	switch (m_State)
	{
		case cProtocol::Status:
		case cProtocol::Login: return Super::GetPacketID(a_PacketType);
		case cProtocol::Game:
			switch (a_PacketType)
			{
				default: return Super::GetPacketID(a_PacketType); 
				// CustomReportDetailsS2CPacket 0x0F
				// ServerLinksS2CPacket 0x10
			}
			break;
		case cProtocol::Configuration:
			switch (a_PacketType)
			{
				default: return Super::GetPacketID(a_PacketType); 
				// CustomReportDetailsS2CPacket 0x7A
				// ServerLinksS2CPacket 0x7B
			}
			break;
		default: UNREACHABLE("Invalid m_State: %d", m_State); break;
	}
}
