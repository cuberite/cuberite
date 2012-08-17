
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





void cPacket::AppendString(AString & a_Dst, const AString & a_String)
{
	AppendShort(a_Dst, (unsigned short)a_String.size());
	a_Dst.append(a_String);
}





void cPacket::AppendString16(AString & a_Dst, const AString & a_String)
{
	AppendShort(a_Dst, (unsigned short)a_String.size());
	AString UTF16;
	UTF16.resize(a_String.size() * sizeof(short));
	for( unsigned int i = 0; i < a_String.size(); ++i )
	{
		UTF16[i * sizeof( short )]     = 0x00;
		UTF16[i * sizeof( short ) + 1] = a_String[i];
	}
	a_Dst.append(UTF16.data(), a_String.size() * sizeof(short));
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




