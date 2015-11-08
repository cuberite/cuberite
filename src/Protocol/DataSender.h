#pragma once

class cDataSender
{
public:
	virtual ~cDataSender() = default;
	virtual void SendData(const char * a_DataToSend, size_t a_Size) = 0;
};
