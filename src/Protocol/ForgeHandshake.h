
// ForgeHandshake.h

// Implements Forge protocol handshaking

#pragma once

#include <stddef.h>

// fwd:
namespace Json
{
	class Value;
}
class cClientHandle;

class cForgeHandshake
{
public:
	cForgeHandshake(cClientHandle * client);
	
	void AugmentServerListPing(Json::Value & ResponseValue);
	void BeginForgeHandshake(const AString & a_Name, const AString & a_UUID, const Json::Value & a_Properties);
	
	void SendServerHello();
	void DataReceived(cClientHandle * a_Client, const char * a_Data, size_t a_Size);
	
	AStringMap ParseModList(const char * a_Data, size_t a_Size);
	
	bool m_isForgeClient;
	
private:
	void SetError();
	bool m_Errored;
	
	cClientHandle * m_Client;
	
	const AString * m_Name;
	const AString * m_UUID;
	const Json::Value * m_Properties;
	
	enum
	{
		Discriminator_ServerHello = 0,
		Discriminator_ClientHello = 1,
		Discriminator_ModList = 2,
		Discriminator_RegistryData = 3,
		Discriminator_HandshakeReset = -2,
		Discriminator_HandshakeAck = -1,
	};
	
	enum
	{
		ClientPhase_WAITINGSERVERDATA = 2,
		ClientPhase_WAITINGSERVERCOMPLETE = 3,
		ClientPhase_PENDINGCOMPLETE = 4,
		ClientPhase_COMPLETE = 5,
	};
	
	enum
	{
		ServerPhase_WAITINGCACK = 2,
		ServerPhase_COMPLETE = 3,
	};
};
