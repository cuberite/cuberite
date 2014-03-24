/**
 * Copyright 2014 MCServer Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#pragma once

struct lua_State;
struct HTTPRequest;





// tolua_begin
class cWebPlugin
{
public:
	// tolua_end
	cWebPlugin();
	virtual ~cWebPlugin();

	// tolua_begin
	virtual const AString GetWebTitle(void) const = 0;
	
	virtual AString HandleWebRequest(const HTTPRequest * a_Request ) = 0;

	static AString SafeString( const AString & a_String );
	// tolua_end

	struct sWebPluginTab
	{
		std::string Title;
		std::string SafeTitle;

		int UserData;
	};

	typedef std::list< sWebPluginTab* > TabList;
	TabList & GetTabs() { return m_Tabs; }

	typedef std::list< std::pair<AString, AString> > TabNameList;
	TabNameList GetTabNames(); // >> EXPORTED IN MANUALBINDINGS <<
	std::pair< AString, AString > GetTabNameForRequest(const HTTPRequest* a_Request );

private:
	TabList m_Tabs;
};  // tolua_export




