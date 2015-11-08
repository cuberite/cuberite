
#pragma once
#include "Protocol.h"


class cDataSender;
extern void undefined();

class cLengthenedProtocol : public cProtocol
{
public:
	cLengthenedProtocol(AString a_LogID) :
		cProtocol(a_LogID),
		m_ReceivedData(32 KiB)
	{
		undefined();
	}

	virtual ~cLengthenedProtocol() = default;

	cProtocolError HandleHandshake(cByteBuffer & a_ByteBuffer, std::vector<std::unique_ptr<cClientAction>> & a_Actions) override WARN_UNUSED;

protected:


	/** State of the protocol. 1 = status, 2 = login, 3 = game */
	UInt32 m_State;

	virtual cProtocolError DataReceived(const char * a_Data, size_t a_Size, std::vector<std::unique_ptr<cClientAction>> & a_Actions) override WARN_UNUSED;
	/** This method should append the actions from incoming packets to a_Action */
	virtual cProtocolError OnDataAddedToBuffer(cByteBuffer & a_Buffer, std::vector<std::unique_ptr<cClientAction>> & a_Action) WARN_UNUSED = 0;

	virtual void SendData(const char * a_Data, size_t a_Size) = 0;
	
	cDataSender * m_Sender;

private:
	
	AString m_ServerAddress;
	
	UInt16 m_ServerPort;
	
	bool m_ShouldAllowBungeeCord;

	/** Buffer for the received data */
	cByteBuffer m_ReceivedData;

	/** Adds the received (unencrypted) data to m_ReceivedData, parses complete packets, appends to a_Actions */
	cProtocolError AddReceivedData(const char * a_Data, size_t a_Size, std::vector<std::unique_ptr<cClientAction>> & a_Actions) WARN_UNUSED;
};
