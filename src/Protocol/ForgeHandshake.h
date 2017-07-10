
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
	
	void augmentServerListPing(Json::Value & ResponseValue);
	void BeginForgeHandshake(const AString & a_Name, const AString & a_UUID, const Json::Value & a_Properties);
	
	void SendServerHello();
	void DataReceived(const char * a_Data, size_t a_Size);
	
	bool m_isForgeClient;
	
private:
	void SetError();
	
	cClientHandle * m_Client;
	
	const AString * m_Name;
	const AString * m_UUID;
	const Json::Value * m_Properties;
	
	
	enum Stage {
		UNKNOWN,
		START,
		HELLO,
		WAITINGCACK,
		COMPLETE,
		DONE,
		ERROR,
	} m_stage;
	
	enum {
		Discriminator_ServerHello = 0,
		Discriminator_ClientHello = 1,
		Discriminator_ModList = 2,
		Discriminator_RegistryData = 3,
		Discriminator_HandshakeReset = -2,
		Discriminator_HandshakeAck = -1,
	};
};
