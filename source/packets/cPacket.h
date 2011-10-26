#pragma once

#include "cSocket.h"
#ifdef _WIN32
#include <xstring>
#else
#include <cstring> // Silly Linux doesn't have xstring...
#include <string>
#include <cstdio>
#endif

class cSocket;
class cPacket
{
public:
	cPacket()
		: m_PacketID( 0 )
	{}
	virtual ~cPacket() {}

	virtual bool Parse( cSocket & a_Socket) { a_Socket=0; printf("ERROR: Undefined NEW Parse function %x\n", m_PacketID ); return false; }
	virtual bool Send( cSocket & a_Socket) { a_Socket=0; printf("ERROR: Undefined NEW Send function %x\n", m_PacketID ); return false; }
	virtual cPacket* Clone() const = 0;

        static const int GAMEMODE = 1; //0 = Survival, 1 = Creative;

	unsigned char m_PacketID;
	cSocket m_Socket; // Current socket being used
protected:
	bool ReadString	( std::string & a_OutString );
	bool ReadString16( std::string & a_OutString );
	bool ReadShort	( short & a_Short );
	bool ReadInteger(int & a_OutInteger );
	bool ReadInteger(unsigned int & a_OutInteger );
	bool ReadFloat	( float & a_OutFloat );
	bool ReadDouble	( double & a_OutDouble );
	bool ReadByte	( char & a_OutByte );
	bool ReadByte	( unsigned char & a_OutByte );
	bool ReadLong	( long long & a_OutLong );
	bool ReadBool	( bool & a_OutBool );

	void AppendString	( std::string & a_String,	char* a_Dst, unsigned int & a_Iterator );
	void AppendString16 ( std::string & a_String,	char* a_Dst, unsigned int & a_Iterator );
	void AppendShort	( short a_Short,			char* a_Dst, unsigned int & a_Iterator );
	void AppendShort	( unsigned short a_Short,	char* a_Dst, unsigned int & a_Iterator );
	void AppendInteger	( int a_Integer,			char* a_Dst, unsigned int & a_Iterator );
	void AppendInteger	( unsigned int a_Integer,	char* a_Dst, unsigned int & a_Iterator );
	void AppendFloat	( float a_Float,			char* a_Dst, unsigned int & a_Iterator );
	void AppendDouble	( double & a_Double,		char* a_Dst, unsigned int & a_Iterator );
	void AppendByte		( char a_Byte,				char* a_Dst, unsigned int & a_Iterator );
	void AppendLong		( long long & a_Long,		char* a_Dst, unsigned int & a_Iterator );
	void AppendBool		( bool a_Bool,				char* a_Dst, unsigned int & a_Iterator );
	void AppendData		( char* a_Data, unsigned int a_Size, char* a_Dst, unsigned int & a_Iterator );

public:
	static int SendData( cSocket & a_Socket, const char* a_Message, unsigned int a_Size, int a_Options );
	static int RecvAll( cSocket & a_Socket, char* a_Data, unsigned int a_Size, int a_Options );
};
