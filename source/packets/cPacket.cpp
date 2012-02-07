
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket.h"
#include "../Endianness.h"





/*
// These checks cannot be done in preprocessor, since sizeof() is evaluated while compiling, so in preprocessing it's unknown.
// Check some basic type assumptions:
#if (sizeof(int) != 4)
	#error "Bad size for int, protocol won't work"
#endif

#if (sizeof(float) != 4)
	#error "Bad size for float, protocol won't work"
#endif

#if (sizeof(double) != 8)
	#error "Bad size for double, protocol won't work"
#endif
*/





int cPacket::ReadString16(const char * a_Data, int a_Size, AString & a_OutString )
{
	int TotalBytes = 0;
	short StrLen;
	HANDLE_PACKET_READ(ReadShort, StrLen, TotalBytes);

	if (2 * StrLen > a_Size - TotalBytes)
	{
		// The string is not yet complete in the buffer
		return PACKET_INCOMPLETE;
	}

	// Simple UTF-16 to UTF-8 conversion - discard higher bits, ignore multishort sequences:
	a_OutString.clear();
	a_OutString.reserve(StrLen);
	short * UTF16 = (short *)(a_Data + TotalBytes);
	for ( int i = 0; i < StrLen; ++i )
	{
		a_OutString.push_back( (char)ntohs(UTF16[i]) );
	}

	return TotalBytes + StrLen * sizeof(short);
}





int cPacket::ReadShort(const char * a_Data, int a_Size, short & a_OutShort )
{
	if (a_Size < 2)
	{
		return PACKET_INCOMPLETE;
	}
	a_OutShort = ntohs(*((short *)a_Data));
	return 2;
}





int cPacket::ReadInteger(const char * a_Data, int a_Size, int & a_OutInteger )
{
	if (a_Size < 4)
	{
		return PACKET_INCOMPLETE;
	}
	a_OutInteger = ntohl(*((int *)a_Data));
	return 4;
}





int cPacket::ReadInteger(const char * a_Data, int a_Size, unsigned int & a_OutInteger )
{
	if (a_Size < 4)
	{
		return PACKET_INCOMPLETE;
	}
	a_OutInteger = ntohl(*((unsigned int *)a_Data));
	return 4;
}





int cPacket::ReadFloat(const char * a_Data, int a_Size, float & a_OutFloat )
{
	if (a_Size < sizeof(float))
	{
		return PACKET_INCOMPLETE;
	}
	a_OutFloat = NetworkToHostFloat4(a_Data);
	return sizeof(float);
}





int cPacket::ReadDouble(const char * a_Data, int a_Size, double & a_OutDouble )
{
	if (a_Size < sizeof(double))
	{
		return PACKET_INCOMPLETE;
	}
	a_OutDouble = NetworkToHostDouble8(a_Data);
	return sizeof(double);
}





int cPacket::ReadByte(const char * a_Data, int a_Size, char & a_OutByte )
{
	if (a_Size < 1)
	{
		return PACKET_INCOMPLETE;
	}
	a_OutByte = *a_Data;
	return 1;
}





int cPacket::ReadByte(const char * a_Data, int a_Size, unsigned char & a_OutByte )
{
	if (a_Size < 1)
	{
		return PACKET_INCOMPLETE;
	}
	a_OutByte = *((unsigned char *)a_Data);
	return 1;
}





int cPacket::ReadLong(const char * a_Data, int a_Size, long long & a_OutLong )
{
	if (a_Size < sizeof(a_OutLong))
	{
		return PACKET_INCOMPLETE;
	}
	a_OutLong = NetworkToHostLong8(a_Data);
	return sizeof(a_OutLong);
}





int cPacket::ReadBool(const char * a_Data, int a_Size, bool & a_OutBool )
{
	if (a_Size < sizeof(bool))
	{
		return PACKET_INCOMPLETE;
	}
	a_OutBool = (*a_Data != 0);
	return sizeof(bool);
}





void cPacket::AppendString(AString & a_Dst, const AString & a_String)
{
	AppendShort(a_Dst, (unsigned short)a_String.size());
	a_Dst.append(a_String);
}





void cPacket::AppendString16(AString & a_Dst, const AString & a_String)
{
	AppendShort(a_Dst, (unsigned short)a_String.size());
	std::auto_ptr<char> UTF16(new char[a_String.size() * sizeof( short ) ]);
	for( unsigned int i = 0; i < a_String.size(); ++i )
	{
		UTF16.get()[i * sizeof( short )]     = 0x00;
		UTF16.get()[i * sizeof( short ) + 1] = a_String[i];
	}
	a_Dst.append(UTF16.get(), a_String.size() * sizeof(short));
}





void cPacket::AppendShort(AString & a_Dst, short a_Short)
{
	short ConvertedShort = htons( a_Short );
	a_Dst.append((const char *)&ConvertedShort, sizeof(short));
}





void cPacket::AppendShort(AString & a_Dst, unsigned short a_Short)
{
	short ConvertedShort = htons( a_Short );
	a_Dst.append((const char *)&ConvertedShort, sizeof(short));
}





void cPacket::AppendInteger(AString & a_Dst, int a_Integer)
{
	int ConvertedInt = htonl( a_Integer );
	a_Dst.append((const char *)&ConvertedInt, sizeof(int));
}





void cPacket::AppendInteger(AString & a_Dst, unsigned int a_Integer)
{
	unsigned int ConvertedInt = htonl( a_Integer );
	a_Dst.append((const char *)&ConvertedInt, sizeof(int));
}





void cPacket::AppendFloat(AString & a_Dst, float a_Float)
{
	unsigned int ConvertedFloat = HostToNetwork4(&a_Float);
	a_Dst.append((const char *)&ConvertedFloat, sizeof(int));
}





void cPacket::AppendDouble(AString & a_Dst, const double & a_Double)
{
	unsigned long long ConvertedDouble = HostToNetwork8(&a_Double);
	a_Dst.append((const char *)&ConvertedDouble, 8);
}





void cPacket::AppendByte(AString & a_Dst, char a_Byte)
{
	a_Dst.append(&a_Byte, 1);
}





void cPacket::AppendLong(AString & a_Dst, const long long & a_Long)
{
	unsigned long long ConvertedLong = HostToNetwork8(&a_Long);
	a_Dst.append((const char *)&ConvertedLong, sizeof(a_Long));
}





void cPacket::AppendBool(AString & a_Dst, bool a_Bool)
{
	a_Dst.append((const char *)&a_Bool, 1);
}





void cPacket::AppendData(AString & a_Dst, const char * a_Data, unsigned int a_Size)
{
	a_Dst.append(a_Data, a_Size);
}




