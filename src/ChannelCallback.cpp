
// ChannelCallback.cpp

// Implements the cChannelCallback class representing the callbacks used for plugin channel management

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "ChannelCallback.h"
#include "ByteBuffer.h"
#include "ClientHandle.h"

cChannelCallback::cChannelCallback(cPluginLua & a_Plugin, cLuaState::cCallbackPtr & a_Callback)
{
	m_Plugin = &a_Plugin;
	m_Callback = std::move(a_Callback);
}

void cChannelCallback::Call(cClientHandle & a_Handle, const cByteBuffer & a_Data)
{
	cCSLock Lock(m_CSPlugin);
	if (m_Callback->IsValid())
	{
		m_Callback->Call(a_Handle.GetPlayer(), a_Data);
	}
}





bool cChannelCallback::BelongsTo(const cPluginLua * a_Plugin)
{
	return (a_Plugin == m_Plugin);
}
