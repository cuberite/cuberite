
// Packetizer.cpp

// Implements the cPacketizer class representing a wrapper for sending a single packet over a protocol.

#include "Globals.h"
#include "Packetizer.h"





/** Converts the hex digit character to its value. */
static UInt8 HexDigitValue(char a_Character)
{
	switch (a_Character)
	{
		case '0': return 0;
		case '1': return 1;
		case '2': return 2;
		case '3': return 3;
		case '4': return 4;
		case '5': return 5;
		case '6': return 6;
		case '7': return 7;
		case '8': return 8;
		case '9': return 9;
		case 'a': return 10;
		case 'b': return 11;
		case 'c': return 12;
		case 'd': return 13;
		case 'e': return 14;
		case 'f': return 15;
		case 'A': return 10;
		case 'B': return 11;
		case 'C': return 12;
		case 'D': return 13;
		case 'E': return 14;
		case 'F': return 15;
		default:
		{
			LOGWARNING("Bad hex digit: %c", a_Character);
			ASSERT(!"Bad hex digit");
			return 0;
		}
	}
}





////////////////////////////////////////////////////////////////////////////////
// cPacketizer:

cPacketizer::~cPacketizer()
{
	m_Protocol.SendPacket(*this);
}





void cPacketizer::WriteByteAngle(double a_Angle)
{
	WriteChar(static_cast<char>(255 * a_Angle / 360));
}





void cPacketizer::WriteFPInt(double a_Value)
{
	Int32 Value = static_cast<Int32>(a_Value * 32);
	WriteInt(Value);
}





void cPacketizer::WriteUUID(const AString & a_UUID)
{
	if (a_UUID.length() != 32)
	{
		LOGWARNING("%s: Attempt to send a bad uuid (length isn't 32): %s", __FUNCTION__, a_UUID.c_str());
		ASSERT(!"Wrong uuid length!");
		return;
	}

	for (size_t i = 0; i < 32; i += 2)
	{
		auto val = static_cast<UInt8>(HexDigitValue(a_UUID[i]) << 4 | HexDigitValue(a_UUID[i + 1]));
		VERIFY(m_Out.WriteBEUInt8(val));
	}
}




