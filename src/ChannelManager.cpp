#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include <algorithm>

#include "ChannelManager.h"
#include "ChannelCallback.h"
#include "ClientHandle.h"
#include "Entities/Player.h"
#include "OSSupport/CriticalSection.h"
#include "World.h"


class cChannelBroadcastCallback : public cPlayerListCallback
{
	AString m_Channel;
	AString m_Data;
public:
	cChannelBroadcastCallback(const AString & a_Channel, const AString & a_Data)
	: m_Channel(a_Channel), m_Data(a_Data){}

	virtual bool Item(cPlayer * a_Type)
	{
		cClientHandle * Handle = a_Type->GetClientHandle();
		if (Handle->HasPluginChannel(m_Channel))
		{
			Handle->SendPluginMessage(m_Channel, m_Data);
		}
		return false;
	}
};





bool cChannelManager::RegisterChannel(const AString & a_Channel, cChannelCallbackPtr a_Callback)
{
	cCSLock Lock(m_CSCallbacks);

	if (m_ChannelCallbacks.find(a_Channel) == m_ChannelCallbacks.end())
	{
		m_ChannelCallbacks.insert(std::make_pair(a_Channel, a_Callback));
		return true;
	}

	return false;
}





bool cChannelManager::RemoveChannel(const AString & a_Channel)
{
	cCSLock Lock(m_CSCallbacks);
	auto callback = m_ChannelCallbacks.find(a_Channel);
	if (callback != m_ChannelCallbacks.end())
	{
		m_ChannelCallbacks.erase(callback);
		return true;
	}
	return false;
}





void cChannelManager::AddClientToChannel(cClientHandle & a_Client, const AString & a_Channel)
{
	a_Client.RegisterChannel(a_Channel);
}





void cChannelManager::AddClientToChannels(cClientHandle & a_Client, const AStringVector & a_Channels)
{
	for (size_t i = 0; i < a_Channels.size(); ++i)
	{
		AddClientToChannel(a_Client, a_Channels[i]);
	}
}





void cChannelManager::RemoveClientFromChannel(cClientHandle & a_Client, const AString & a_Channel)
{
	a_Client.RemoveChannel(a_Channel);
}





void cChannelManager::RemoveClientFromChannels(cClientHandle & a_Client, const AStringVector & a_Channels)
{
	for (size_t i = 0; i < a_Channels.size(); ++i)
	{
		RemoveClientFromChannel(a_Client, a_Channels[i]);
	}
}





bool cChannelManager::ClientHasChannel(cClientHandle & a_Client, const AString & a_Channel)
{
	return a_Client.HasPluginChannel(a_Channel);
}





void cChannelManager::HandleChannelMessage(cClientHandle & a_Client, const AString & a_Channel,  const AString & a_Data)
{
	bool has_channel = ClientHasChannel(a_Client, a_Channel);

	// If the client has not registered to send and recieve this channel, log it and send an unregister message.
	// Otherwise try to use it.
	if (has_channel)
	{
		cCSLock Lock(m_CSCallbacks);

		auto callback = m_ChannelCallbacks.find(a_Channel);

		// If a callback has been registered for this channel, call it.
		// Otherwise, call the hook
		if (has_channel && (callback != m_ChannelCallbacks.end()))
		{
			callback->second->Call(a_Client, a_Data);
		}
		else
		{
			cPluginManager::Get()->CallHookPluginMessage(a_Client, a_Channel, a_Data);
		}
	}
	else
	{
		// Ignore if client sent something but didn't register the channel first
		LOGD("Player %s sent a plugin message on channel \"%s\", but didn't REGISTER it first", a_Client.GetUsername().c_str(), a_Channel.c_str());
		a_Client.SendPluginMessage("UNREGISTER", a_Channel);
	}
	
}
void cChannelManager::HandlePluginUnloading(const cPluginLua * a_Plugin)
{
	cCSLock Lock(m_CSCallbacks);

	for (auto it = m_ChannelCallbacks.begin(); it != m_ChannelCallbacks.end(); ++it)
	{
		if (it->second->BelongsTo(a_Plugin))
		{
			m_ChannelCallbacks.erase(it);
		}
	}
}





void cChannelManager::BroadcastChannelMessage(const AString & a_Channel,  const AString & a_Data, cWorld * a_World)
{
	if (a_World != nullptr)
	{
		cChannelBroadcastCallback callback(a_Channel, a_Data);
		a_World->ForEachPlayer(callback);
	}
	else
	{
		// TODO: Implement server-wide broadcast
	}
}
