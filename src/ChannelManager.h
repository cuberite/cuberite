#pragma once

#include <unordered_map>
#include <set>

#include "StringUtils.h"


class cClientHandle;
class cCriticalSection;
class cPluginLua;
class cWorld;
class cChannelCallback;
class cByteBuffer;

// tolua_begin
class cChannelManager
{
// tolua_end
public:
	typedef SharedPtr<cChannelCallback> cChannelCallbackPtr;
	typedef std::unordered_map<AString, cChannelCallbackPtr> CallbackMap;

	/** Adds a client to a specific channel. */
	void AddClientToChannel      (cClientHandle & a_Client, const AString & a_Channel);  // tolua_export

	/** Adds a client to multiple channels at once. */
	void AddClientToChannels     (cClientHandle & a_Client, const AString & a_Channels);

	/** Removes a client from a specific channel. */
	void RemoveClientFromChannel (cClientHandle & a_Client, const AString & a_Channel);  // tolua_export

	/** Removes a client from multiple channels at once. */
	void RemoveClientFromChannels(cClientHandle & a_Client, const AString & a_Channels);

	/** Returns true if the client is registered for the specified channel. */
	bool ClientHasChannel        (cClientHandle & a_Client, const AString & a_Channel);

	/** Broadcasts a channel message to all clients.
	If a_World is provided, broadcast is limited to that world. */
	void BroadcastChannelMessage (const AString & a_Channel, cByteBuffer & a_Data, cWorld * a_World = nullptr);  // tolua_export

	/** Registers a handler for a specific channel.
	Returns true if registration was successful. */
	bool RegisterChannel         (const AString & a_Channel, cChannelCallbackPtr a_Callback);  // Exported in manual bindings

	/** Removes the handler for the specified channel.
	Returns true if the handler was removed. */
	bool RemoveChannel           (const AString & a_Channel);  // tolua_export


	void HandleChannelMessage    (cClientHandle & a_Client, const AString & a_Channel, cByteBuffer & a_Data);


	void HandlePluginUnloading   (const cPluginLua * a_Plugin);

private:

	AStringVector BreakApartChannels(const AString & a_Channels);

	/** This protects m_ChannelCallbacks */
	cCriticalSection m_CSCallbacks;

	CallbackMap m_ChannelCallbacks;

};  // tolua_export
