
#pragma once

#include "../cSocket.h"
#include "../PacketID.h"





#define PACKET_INCOMPLETE -2
#define PACKET_ERROR      -1





// Use this macro to simplify handling several ReadXXX in a row. It assumes that you want [a_Data, a_Size] parsed (which is true for all Parse() functions)
#define HANDLE_PACKET_READ(Proc, Var, TotalBytes) \
	{ \
		int res = Proc(a_Data + TotalBytes, a_Size - TotalBytes, Var); \
		if (res < 0) \
		{ \
			return res; \
		} \
		TotalBytes += res; \
	}





class cPacket
{
public:
	cPacket()
		: m_PacketID( 0 )
	{}
	virtual ~cPacket() {}

	/// Called to parse the packet. Packet type has already been read and the correct packet type created. Return the number of characters processed, PACKET_INCOMPLETE for incomplete data, PACKET_ERROR for error
	virtual int Parse(const char * a_Data, int a_Size)
	{
		LOGERROR("Undefined Parse function for packet type 0x%x\n", m_PacketID );
		ASSERT(!"Undefined Parse function");
		return -1;
	}
	
	/// Called to serialize the packet into a string. Append all packet data to a_Data, including the packet type!
	virtual void Serialize(AString & a_Data) const
	{
		LOGERROR("Undefined Serialize function for packet type 0x%x\n",  m_PacketID );
		ASSERT(!"Undefined Serialize function");
	}
	
	virtual cPacket * Clone() const = 0;

	unsigned char m_PacketID;

protected:

	// These return the number of characters processed, PACKET_INCOMPLETE for incomplete data, PACKET_ERROR for error:
	static int ReadString16(const char * a_Data, int a_Size, AString & a_OutString );
	static int ReadShort   (const char * a_Data, int a_Size, short & a_Short );
	static int ReadInteger (const char * a_Data, int a_Size, int & a_OutInteger );
	static int ReadInteger (const char * a_Data, int a_Size, unsigned int & a_OutInteger );
	static int ReadFloat   (const char * a_Data, int a_Size, float & a_OutFloat );
	static int ReadDouble  (const char * a_Data, int a_Size, double & a_OutDouble );
	static int ReadByte    (const char * a_Data, int a_Size, char & a_OutByte );
	static int ReadByte    (const char * a_Data, int a_Size, unsigned char & a_OutByte );
	static int ReadLong    (const char * a_Data, int a_Size, long long & a_OutLong );
	static int ReadBool    (const char * a_Data, int a_Size, bool & a_OutBool );

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




