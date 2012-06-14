
#pragma once

#include "cTCPLink.h"





class cHeartBeat : public cTCPLink
{
public:
	cHeartBeat();
	~cHeartBeat();
private:
	virtual void ReceivedData( char a_Data[256], int a_Size );

	void Authenticate();
	int m_State;

	void SendUpdate();

	std::string m_ServerID;
};




