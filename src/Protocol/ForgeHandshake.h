
// ForgeHandshake.h

// Implements Forge protocol handshaking

#pragma once

#include <stddef.h>
#include "../UUID.h"
#include "json/json.h"

// fwd:
class cClientHandle;





class cForgeHandshake
{
public:
	/** True if the client advertised itself as a Forge client. */
	bool m_IsForgeClient;

	cForgeHandshake(cClientHandle * client);

	/** Add the registered Forge mods to the server ping list packet. */
	void AugmentServerListPing(Json::Value & ResponseValue);

	/** Begin the Forge Modloader Handshake (FML|HS) sequence. */
	void BeginForgeHandshake(const AString & a_Name, const cUUID & a_UUID, const Json::Value & a_Properties);

	/** Send the ServerHello packet in the Forge handshake. */
	void SendServerHello();

	/** Process received data from the client advancing the Forge handshake. */
	void DataReceived(cClientHandle * a_Client, const char * a_Data, size_t a_Size);

private:
	/** True if the Forge handshake is in an errored state. */
	bool m_Errored;

	/** The client handle undergoing this Forge handshake. */
	cClientHandle * m_Client;

	/** Values saved from BeginForgeHandshake() for continuing the normal handshake after Forge completes. */
	AString m_Name;
	cUUID m_UUID;
	Json::Value m_Properties;

	void HandleClientHello(cClientHandle * a_Client, const char * a_Data, size_t a_Size);
	void HandleModList(cClientHandle * a_Client, const char * a_Data, size_t a_Size);
	void HandleHandshakeAck(cClientHandle * a_Client, const char * a_Data, size_t a_Size);

	/** Set errored state to prevent further handshake message processing. */
	void SetError(const AString & message);

	/** Parse the client ModList packet of installed Forge mods and versions. */
	AStringMap ParseModList(const char * a_Data, size_t a_Size);
};
