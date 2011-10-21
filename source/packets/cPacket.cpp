#include "cPacket.h"
#include "cMCLogger.h"
#include "Endianness.h"

#ifdef _WIN32
#define MSG_NOSIGNAL (0)
#endif

#ifdef __MAC_NA
#define MSG_NOSIGNAL (0)
#endif

//*****************************************************************************
// Blocking receive all function
//*****************************************************************************
int cPacket::RecvAll( cSocket & a_Socket, char* a_Data, unsigned int a_Size, int a_Options )
{
	unsigned int RequestSize = a_Size;
	while(a_Size != 0)
	{
		int Num = recv(a_Socket, a_Data, a_Size, a_Options);
		if( cSocket::IsSocketError( Num ) )
			return Num;
		a_Size -= Num;
		a_Data += Num;
	}
	return RequestSize - a_Size;
}

//*****************************************************************************
// Own implementation of send()
//*****************************************************************************
int cPacket::SendData( cSocket & a_Socket, const char* a_Message, unsigned int a_Size, int a_Options )
{
	return send(a_Socket, a_Message, a_Size, a_Options | MSG_NOSIGNAL );
}


//*****************************************************************************
// New packets
//*****************************************************************************

bool cPacket::ReadString( std::string & a_OutString )
{
	short StrLen;
	if(!ReadShort( StrLen )) return false;

	if( StrLen == 0 )
	{
		a_OutString.clear();
		return true;
	}

	char* cString = new char[StrLen];
	if( cSocket::IsSocketError( RecvAll( m_Socket, cString, StrLen, 0 ) ) ) return false;

	a_OutString.assign( cString, StrLen );

	//printf("Discoved string: %s size: %i\n", a_OutString.c_str(), a_OutString.size() );
	delete [] cString;
	return true;
}

bool cPacket::ReadString16( std::string & a_OutString )
{
	short StrLen;
	if(!ReadShort( StrLen )) return false;

	a_OutString.clear();
	if( StrLen == 0 )
	{
		return true;
	}

	char* UTF16 = new char[StrLen*sizeof( short )];
	if( cSocket::IsSocketError( RecvAll( m_Socket, UTF16, StrLen * sizeof( short ), 0 ) ) ) return false;

	for( int i = 0; i < StrLen; ++i )
		a_OutString.push_back( (char)UTF16[i*sizeof( short )+1] );

	//printf("Discoved string: %s size: %i\n", a_OutString.c_str(), a_OutString.size() );
	delete [] UTF16;
	return true;
}

bool cPacket::ReadShort( short & a_OutShort )
{
	if( cSocket::IsSocketError( RecvAll( m_Socket, (char*)&a_OutShort, sizeof(short), 0 ) ) ) return false;
	a_OutShort = ntohs(a_OutShort);
	return true;
}

bool cPacket::ReadInteger( int & a_OutInteger )
{
	if( cSocket::IsSocketError( RecvAll( m_Socket, (char*)&a_OutInteger, sizeof(int), 0 ) ) ) return false;
	a_OutInteger = ntohl(a_OutInteger);
	return true;
}

bool cPacket::ReadInteger( unsigned int & a_OutInteger )
{
	if( cSocket::IsSocketError( RecvAll( m_Socket, (char*)&a_OutInteger, sizeof(unsigned int), 0 ) ) ) return false;
	a_OutInteger = ntohl(a_OutInteger);
	return true;
}

bool cPacket::ReadFloat( float & a_OutFloat )
{
	if( cSocket::IsSocketError( RecvAll( m_Socket, (char*)&a_OutFloat, sizeof(float), 0 ) ) ) return false;
	a_OutFloat = NetworkToHostFloat4( &a_OutFloat );
	return true;
}

bool cPacket::ReadDouble( double & a_OutDouble )
{
	if( cSocket::IsSocketError( RecvAll( m_Socket, (char*)&a_OutDouble, sizeof(double), 0 ) ) ) return false;
	a_OutDouble = NetworkToHostDouble8( &a_OutDouble );
	return true;
}

bool cPacket::ReadByte( char & a_OutByte )
{
	return !cSocket::IsSocketError( RecvAll( m_Socket, (char*)&a_OutByte, sizeof(char), 0 ) );
}

bool cPacket::ReadByte( unsigned char & a_OutByte )
{
	return !cSocket::IsSocketError(RecvAll( m_Socket, (char*)&a_OutByte, sizeof(char), 0 ) );
}

bool cPacket::ReadLong( long long & a_OutLong )
{
	if( cSocket::IsSocketError( RecvAll( m_Socket, (char*)&a_OutLong, sizeof(long long), 0 ) ) ) return false;
	a_OutLong = NetworkToHostLong8( &a_OutLong );
	return true;
}

bool cPacket::ReadBool( bool & a_OutBool )
{
	if( cSocket::IsSocketError(RecvAll( m_Socket, (char*)&a_OutBool, sizeof(bool), 0 ) ) ) return false;
	return true;
}

//*****************************************************************************
// Append variables to a c-String
//*****************************************************************************
void cPacket::AppendString( std::string & a_String, char* a_Dst, unsigned int & a_Iterator )
{
	AppendShort( (unsigned short)a_String.size(), a_Dst, a_Iterator );
	memcpy( a_Dst + a_Iterator, a_String.c_str(), a_String.size() ); a_Iterator += a_String.size();
}

void cPacket::AppendString16( std::string & a_String, char* a_Dst, unsigned int & a_Iterator )
{
	AppendShort( (unsigned short)a_String.size(), a_Dst, a_Iterator );
	char* UTF16 = new char[ a_String.size() * sizeof( short ) ];
	for( unsigned int i = 0; i < a_String.size(); ++i )
	{
		UTF16[i*sizeof( short )]	= 0x00;//a_String[i];
		UTF16[i*sizeof( short )+1]	= a_String[i];
	}
	memcpy( a_Dst + a_Iterator, UTF16, a_String.size() * sizeof( short ) ); a_Iterator += a_String.size() * sizeof( short );
	delete [] UTF16;
}

void cPacket::AppendShort( short a_Short, char *a_Dst, unsigned int &a_Iterator )
{
	short ConvertedShort = htons( a_Short );
	memcpy( a_Dst + a_Iterator, &ConvertedShort, sizeof( short ) ); a_Iterator+=sizeof( short );
}

void cPacket::AppendShort( unsigned short a_Short, char *a_Dst, unsigned int &a_Iterator )
{
	short ConvertedShort = htons( a_Short );
	memcpy( a_Dst + a_Iterator, &ConvertedShort, sizeof( unsigned short ) ); a_Iterator+=sizeof( unsigned short );
}


void cPacket::AppendInteger( int a_Integer, char* a_Dst, unsigned int & a_Iterator )
{
	int ConvertedInt = htonl( a_Integer );
	memcpy( a_Dst + a_Iterator, &ConvertedInt, sizeof( int ) ); a_Iterator+=sizeof( int );
}

void cPacket::AppendInteger( unsigned int a_Integer, char* a_Dst, unsigned int & a_Iterator )
{
	unsigned int ConvertedInt = htonl( a_Integer );
	memcpy( a_Dst + a_Iterator, &ConvertedInt, sizeof( unsigned int ) ); a_Iterator+=sizeof( unsigned int );
}

void cPacket::AppendFloat( float a_Float, char* a_Dst, unsigned int & a_Iterator )
{
	unsigned int ConvertedFloat = HostToNetwork4(&a_Float);
	memcpy( a_Dst + a_Iterator, &ConvertedFloat, sizeof(float) ); a_Iterator += sizeof(float);
}

void cPacket::AppendDouble( double & a_Double, char* a_Dst, unsigned int & a_Iterator )
{
	unsigned long long ConvertedDouble = HostToNetwork8(&a_Double);
	memcpy( a_Dst + a_Iterator, &ConvertedDouble, sizeof(double) ); a_Iterator += sizeof(double);
}

void cPacket::AppendByte( char a_Byte, char* a_Dst, unsigned int & a_Iterator )
{
	a_Dst[a_Iterator] = a_Byte; a_Iterator+=sizeof(char);
}

void cPacket::AppendLong( long long & a_Long, char* a_Dst, unsigned int & a_Iterator )
{
	unsigned long long ConvertedLong = HostToNetwork8(&a_Long);
	memcpy( a_Dst + a_Iterator, &ConvertedLong, sizeof(long long) );
	a_Iterator += sizeof( long long );
}

void cPacket::AppendBool( bool a_Bool, char* a_Dst, unsigned int & a_Iterator )
{
	a_Dst[a_Iterator] = (char)a_Bool; a_Iterator+=sizeof(bool);
}

void cPacket::AppendData( char* a_Data, unsigned int a_Size, char* a_Dst, unsigned int & a_Iterator )
{
	memcpy( a_Dst + a_Iterator, a_Data, a_Size ); a_Iterator += a_Size;
}
