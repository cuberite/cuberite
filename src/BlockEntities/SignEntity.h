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
// SignEntity.h

// Declares the cSignEntity class representing a single sign in the world





#pragma once

#include "BlockEntity.h"





namespace Json
{
	class Value;
}





// tolua_begin

class cSignEntity : 
	public cBlockEntity
{
	typedef cBlockEntity super;
	
public:

	// tolua_end
	
	/// Creates a new empty sign entity at the specified block coords and block type (wall or standing). a_World may be NULL
	cSignEntity(BLOCKTYPE a_BlockType, int a_X, int a_Y, int a_Z, cWorld * a_World);

	bool LoadFromJson( const Json::Value& a_Value );
	virtual void SaveToJson(Json::Value& a_Value ) override;

	// tolua_begin
	
	/// Sets all the sign's lines
	void SetLines(const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4);
	
	/// Sets individual line (zero-based index)
	void SetLine(int a_Index, const AString & a_Line);

	/// Retrieves individual line (zero-based index)
	AString GetLine(int a_Index) const;
	
	// tolua_end
	
	virtual void UsedBy(cPlayer * a_Player) override;
	virtual void SendTo(cClientHandle & a_Client) override;

	static const char * GetClassStatic(void) { return "cSignEntity"; }
	
private:

	AString  m_Line[4];
} ;  // tolua_export




