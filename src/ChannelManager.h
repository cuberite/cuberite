#pragma once

#include <unordered_map>
#include <set>

#include "StringUtils.h"


class cClientHandle;
class cCriticalSection;
class cPluginLua;
class cWorld;
class cChannelCallback;


// tolua_begin
class cChannelManager
{
// tolua_end
public:
	typedef SharedPtr<cChannelCallback> cChannelCallbackPtr;
	typedef std::unordered_map<AString, cChannelCallbackPtr> CallbackMap;

	void AddClientToChannel      (cClientHandle & a_Client, const AString & a_Channel);
	void AddClientToChannels     (cClientHandle & a_Client, const AStringVector & a_Channels);
	void RemoveClientFromChannel (cClientHandle & a_Client, const AString & a_Channel);
	void RemoveClientFromChannels(cClientHandle & a_Client, const AStringVector & a_Channels);
	bool ClientHasChannel        (cClientHandle & a_Client, const AString & a_Channel);

	void BroadcastChannelMessage (const AString & a_Channel,  const AString & a_Data, cWorld * a_World = nullptr);  // tolua_export

	bool RegisterChannel         (const AString & a_Channel, cChannelCallbackPtr a_Callback);  // Exported in manual bindings
	bool RemoveChannel           (const AString & a_Channel);  // tolua_export

	void HandleChannelMessage    (cClientHandle & a_Client, const AString & a_Channel,  const AString & a_Data);
	void HandlePluginUnloading   (const cPluginLua * a_Plugin);

private:

	/** This protects m_ChannelCallbacks */
	cCriticalSection m_CSCallbacks;

	CallbackMap m_ChannelCallbacks;
};  // tolua_export
