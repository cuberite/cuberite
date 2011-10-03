#pragma once

#include "tolua++.h"
#include <string>
#include <map>
// A std::map<string, string> interface for Lua

class cStringMap					// tolua_export
{									// tolua_export
public:								// tolua_export
	cStringMap(std::map< std::string, std::string > a_StringMap) : m_StringMap( a_StringMap ) {}
	void clear();					// tolua_export

	unsigned int size() const;		// tolua_export

	std::string & get( const std::string & index );	//tolua_export
private:
	std::map< std::string, std::string > m_StringMap;
};									// tolua_export