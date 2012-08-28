
#pragma once

#include "../cSocket.h"
#include "../PacketID.h"
#include "../ByteBuffer.h"





enum
{
	PACKET_INCOMPLETE = -2,
	PACKET_ERROR      = -1,
	PACKET_OK         =  1,
} ;





// Use this macro to simplify handling several ReadXXX in a row. It assumes that you want [a_Data, a_Size] parsed (which is true for all Parse() functions)
#define HANDLE_PACKET_READ(Proc, Var, TotalBytes) \
	{ \
		if (!a_Buffer.Proc(Var)) \
		{ \
			return PACKET_INCOMPLETE; \
		} \
		TotalBytes = PACKET_OK; \
	}





class cPacket
{
public:
	cPacket()
		: m_PacketID( 0 )
	{}
	virtual ~cPacket() {}

	/// Called to parse the packet. Packet type has already been read and the correct packet type created. Return PACKET_INCOMPLETE for incomplete data, PACKET_ERROR for error, any positive number for success
	virtual int Parse(cByteBuffer & a_Buffer)
	{
		// There are packets that are sent S->C only, those don't have a parsing function
		UNUSED(a_Buffer);
		LOGERROR("Packet type 0x%02x has no parser defined!", m_PacketID);
		ASSERT(!"Unparsed packet type!");
		return PACKET_ERROR;
	}
	
	virtual cPacket * Clone() const = 0;

	unsigned char m_PacketID;

protected:
	// These append the data into the a_Dst string:
	static void AppendString  ( AString & a_Dst, const AString & a_String);
	static void AppendString16( AString & a_Dst, const AString & a_String);
	static void AppendShort   ( AString & a_Dst, short a_Short);
	static void AppendShort   ( AString & a_Dst, unsigned short a_Short);
	static void AppendInteger ( AString & a_Dst, int a_Integer);
	static void AppendInteger ( AString & a_Dst, unsigned int a_Integer);
	static void AppendFloat   ( AString & a_Dst, float a_Float);
	static void AppendDouble  ( AString & a_Dst, const double & a_Double);
	static void AppendByte    ( AString & a_Dst, char a_Byte);
	static void AppendLong    ( AString & a_Dst, const long long & a_Long);
	static void AppendBool    ( AString & a_Dst, bool a_Bool);
	static void AppendData    ( AString & a_Dst, const char * a_Data, unsigned int a_Size);
};

typedef std::list <cPacket*>  PacketList;
typedef std::deque<cPacket *> PacketQueue;




