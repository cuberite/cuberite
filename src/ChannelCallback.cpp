#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "ChannelCallback.h"

void cChannelCallback::Call(const cClientHandle & a_Handle, const AString & a_Data)
{
	cCSLock Lock(m_CSPlugin);
	if (m_Plugin != nullptr)
	{
		m_Plugin->Call(m_FnRef, a_Handle, a_Data);
	}
}





bool cChannelCallback::BelongsTo(const cPluginLua * a_Plugin)
{
	return (a_Plugin == m_Plugin);
}
