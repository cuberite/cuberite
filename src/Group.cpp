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

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Group.h"





void cGroup::AddCommand( AString a_Command )
{
	m_Commands[ a_Command ] = true;
}





void cGroup::AddPermission( AString a_Permission )
{
	m_Permissions[ a_Permission ] = true;
}





void cGroup::InheritFrom( cGroup* a_Group )
{
	m_Inherits.remove( a_Group );
	m_Inherits.push_back( a_Group );
}





void cGroup::ClearPermission()
{
	m_Permissions.clear();
}