#include "Bindings/PluginLua.h"

class cChannelCallback : public cPluginLua::cResettable
{
	int m_FnRef;
public:
	cChannelCallback(cPluginLua & a_Plugin, int a_FnRef) : cPluginLua::cResettable(a_Plugin), m_FnRef(a_FnRef){}
	void Call(const cClientHandle & a_Handle, const AString & a_Data);
	bool BelongsTo(const cPluginLua * a_Plugin);
};
