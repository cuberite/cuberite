#pragma once

#include "cSocket.h"





class cEvent;
class cBlockingTCPLink															//tolua_export
{																				//tolua_export
public:																			//tolua_export
	cBlockingTCPLink();															//tolua_export
	~cBlockingTCPLink();														//tolua_export

	bool Connect( const char* a_Address, unsigned int a_Port );					//tolua_export
	int Send( char* a_Data, unsigned int a_Size, int a_Flags = 0 );				//tolua_export
	int SendMessage( const char* a_Message, int a_Flags = 0 );					//tolua_export
	void CloseSocket();															//tolua_export
	std::string ReceiveData();													//tolua_export
protected:

	cSocket m_Socket;
};	//tolua_export
