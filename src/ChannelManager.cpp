// ChannelManager.cpp

// Implements the cChannelManager class which stores and calls channel callbacks

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include <algorithm>

#include "ByteBuffer.h"
#include "ChannelManager.h"
#include "ChannelCallback.h"
#include "ClientHandle.h"
#include "Entities/Player.h"
#include "OSSupport/CriticalSection.h"
#include "World.h"





class cChannelBroadcastCallback : public cPlayerListCallback
{
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
private:
	AString m_Channel;
	AString m_Data;
};





bool cChannelManager::RegisterChannel(const AString & a_Channel, cChannelCallbackPtr a_Callback)
{
	cCSLock Lock(m_CSCallbacks);
	auto Result = m_ChannelCallbacks.emplace(a_Channel, a_Callback);

	return Result.second;
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
	a_Client.SendPluginMessage("REGISTER", a_Channel);
}





void cChannelManager::AddClientToChannels(cClientHandle & a_Client, const AString & a_Channels)
{
	auto channels = BreakApartChannels(a_Channels);
	a_Client.RegisterChannels(channels);
}





void cChannelManager::RemoveClientFromChannel(cClientHandle & a_Client, const AString & a_Channel)
{
	a_Client.UnregisterChannel(a_Channel);
	a_Client.SendPluginMessage("UNREGISTER", a_Channel);
}





void cChannelManager::RemoveClientFromChannels(cClientHandle & a_Client, const AString & a_Channels)
{
	auto channels = BreakApartChannels(a_Channels);
	a_Client.UnregisterChannels(channels);
}





bool cChannelManager::ClientHasChannel(cClientHandle & a_Client, const AString & a_Channel)
{
	return a_Client.HasPluginChannel(a_Channel);
}





void cChannelManager::HandleChannelMessage(cClientHandle & a_Client, const AString & a_Channel, cByteBuffer & a_Data)
{
	bool HasChannel = ClientHasChannel(a_Client, a_Channel);

	// If the client has not registered to send and recieve this channel, log it and send an unregister message.
	// Otherwise try to use it.
	if (HasChannel)
	{
		cCSLock Lock(m_CSCallbacks);

		auto Callback = m_ChannelCallbacks.find(a_Channel);

		// If a callback has been registered for this channel, call it.
		// Otherwise, call the hook
		if (HasChannel && (Callback != m_ChannelCallbacks.end()))
		{
			Callback->second->Call(a_Client, a_Data);
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

	for (auto it = m_ChannelCallbacks.begin(); it != m_ChannelCallbacks.end();)
	{
		if (!it->second->BelongsTo(a_Plugin))
		{
			++it;
		}
		else
		{
			it = m_ChannelCallbacks.erase(it);
		}
	}
}





void cChannelManager::BroadcastChannelMessage(const AString & a_Channel, cByteBuffer & a_Data, cWorld * a_World)
{
	AString Temp;
	a_Data.ReadAll(Temp);
	if (a_World != nullptr)
	{
		cChannelBroadcastCallback Callback(a_Channel, Temp);
		a_World->ForEachPlayer(Callback);
	}
	else
	{
		// TODO: Implement server-wide broadcast
	}
}





AStringVector cChannelManager::BreakApartChannels(const AString & a_Channels)
{
	// Break the string on each NUL character.
	// Note that StringSplit() doesn't work on this because NUL is a special char - string terminator
	size_t len = a_Channels.size();
	size_t first = 0;
	AStringVector res;
	for (size_t i = 0; i < len; i++)
	{
		if (a_Channels[i] != 0)
		{
			continue;
		}
		if (i > first)
		{
			res.push_back(a_Channels.substr(first, i - first));
		}
		first = i + 1;
	}  // for i - a_PluginChannels[]
	if (first < len)
	{
		res.push_back(a_Channels.substr(first, len - first));
	}
	return res;
}
