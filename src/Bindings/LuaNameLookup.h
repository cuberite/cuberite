
// LuaNameLookup.h

// Declares the cLuaNameLookup class used as the cNetwork API callbacks for name and IP lookups from Lua





#pragma once

#include "../OSSupport/Network.h"
#include "LuaState.h"





class cLuaNameLookup:
	public cNetwork::cResolveNameCallbacks
{
public:
	/** Creates a new instance of the lookup callbacks for the specified query,
	using the callbacks that are in the specified table. */
	cLuaNameLookup(const AString & a_Query, cLuaState::cTableRefPtr && a_Callbacks);

protected:
	/** The Lua table that holds the callbacks to be invoked. */
	cLuaState::cTableRefPtr m_Callbacks;

	/** The query used to start the lookup (either hostname or IP). */
	AString m_Query;


	// cNetwork::cResolveNameCallbacks overrides:
	virtual void OnNameResolved(const AString & a_Name, const AString & a_IP) override;
	virtual void OnError(int a_ErrorCode, const AString & a_ErrorMsg) override;
	virtual void OnFinished(void) override;
};




