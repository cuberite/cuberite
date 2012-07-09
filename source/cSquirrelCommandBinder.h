#pragma once
#include <string>
#include <sqrat.h>

class cPlugin;
class cPlayer;

class cSquirrelCommandBinder
{
public:
	cSquirrelCommandBinder();
	~cSquirrelCommandBinder();

	bool HandleCommand( const std::string & a_Command, cPlayer* a_Player );

	bool BindCommand( const std::string & a_Command, const std::string & a_Permission, cPlugin* a_Plugin, Sqrat::Function a_Callback);

	void ClearBindings();
	void RemoveBindingsForPlugin( cPlugin* a_Plugin );
private:
	struct BoundFunction
	{
		Sqrat::Function Callback;
		cPlugin *Plugin;
		std::string Permission;
	};

	typedef std::map< std::string, BoundFunction > CommandMap;
	CommandMap m_BoundCommands;
};





