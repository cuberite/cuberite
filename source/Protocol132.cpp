
// Protocol132.cpp

// Implements the cProtocol132 class representing the release 1.3.2 protocol (#39)

#include "Globals.h"
#include "Protocol132.h"





#define HANDLE_PACKET_READ(Proc, Type, Var) \
	Type Var; \
	{ \
		if (!m_ReceivedData.Proc(Var)) \
		{ \
			return PARSE_INCOMPLETE; \
		} \
	}




typedef unsigned char Byte;





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cProtocol132:

cProtocol132::cProtocol132(cClientHandle * a_Client) :
	super(a_Client)
{
}





void cProtocol132::DataReceived(const char * a_Data, int a_Size)
{
	// TODO: Protocol decryption
	super::DataReceived(a_Data, a_Size);
}





int cProtocol132::ParseHandshake(void)
{
	HANDLE_PACKET_READ(ReadByte,            Byte,    ProtocolVersion);
	HANDLE_PACKET_READ(ReadBEUTF16String16, AString, Username);
	HANDLE_PACKET_READ(ReadBEUTF16String16, AString, ServerHost);
	HANDLE_PACKET_READ(ReadBEInt,           int, ServerPort);
	m_Username = Username;
	return PARSE_OK;
}




