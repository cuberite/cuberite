
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
	bool IsForgeClient;

	cForgeHandshake();

	/** Add the registered Forge mods to the server ping list packet. */
	void AugmentServerListPing(cClientHandle & a_Client, Json::Value & ResponseValue);

	/** Begin the Forge Modloader Handshake (FML|HS) sequence. */
	void BeginForgeHandshake(cClientHandle & a_Client);

	/** Send the ServerHello packet in the Forge handshake. */
	void SendServerHello(cClientHandle & a_Client);

	/** Process received data from the client advancing the Forge handshake. */
	void DataReceived(cClientHandle & a_Client, ContiguousByteBufferView a_Data);

private:

	/** True if the Forge handshake is in an errored state. */
	bool m_Errored;

	void HandleClientHello(cClientHandle & a_Client, ContiguousByteBufferView a_Data);
	void HandleModList(cClientHandle & a_Client, ContiguousByteBufferView a_Data);
	void HandleHandshakeAck(cClientHandle & a_Client, ContiguousByteBufferView a_Data);

	/** Set errored state to prevent further handshake message processing. */
	void SetError(const AString & message);

	/** Parse the client ModList packet of installed Forge mods and versions. */
	AStringMap ParseModList(ContiguousByteBufferView a_Data);
};
