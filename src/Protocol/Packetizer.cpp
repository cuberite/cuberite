
// Packetizer.cpp

// Implements the cPacketizer class representing a wrapper for sending a single packet over a protocol.

#include "Globals.h"
#include "Packetizer.h"
#include "UUID.h"





////////////////////////////////////////////////////////////////////////////////
// cPacketizer:

cPacketizer::~cPacketizer()
{
	m_Protocol.SendPacket(*this);
}





void cPacketizer::WriteByteAngle(double a_Angle)
{
	WriteBEInt8(static_cast<Int8>(255 * a_Angle / 360));
}





void cPacketizer::WriteFPInt(double a_Value)
{
	WriteBEInt32(static_cast<Int32>(a_Value * 32));
}





void cPacketizer::WriteUUID(const cUUID & a_UUID)
{
	for (auto val : a_UUID.ToRaw())
	{
		VERIFY(m_Out.WriteBEUInt8(val));
	}
}




