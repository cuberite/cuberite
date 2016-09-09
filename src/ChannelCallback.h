#include "Bindings/PluginLua.h"

class cByteBuffer;

class cChannelCallback
{
	cLuaState::cCallbackPtr m_Callback;
	cPluginLua * m_Plugin;
	cCriticalSection m_CSPlugin;
public:
	cChannelCallback(cPluginLua & a_Plugin, cLuaState::cCallbackPtr & a_Callback);
	void Call(cClientHandle & a_Handle, const cByteBuffer & a_Data);
	bool BelongsTo(const cPluginLua * a_Plugin);
};
