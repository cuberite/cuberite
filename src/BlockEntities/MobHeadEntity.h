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
// MobHeadEntity.h

// Declares the cMobHeadEntity class representing a single skull/head in the world





#pragma once

#include "BlockEntity.h"





namespace Json
{
	class Value;
}





// tolua_begin

class cMobHeadEntity : 
	public cBlockEntity
{
	typedef cBlockEntity super;
	
public:

	// tolua_end
	
	/** Creates a new mob head entity at the specified block coords. a_World may be NULL */
	cMobHeadEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World);

	bool LoadFromJson( const Json::Value& a_Value );
	virtual void SaveToJson(Json::Value& a_Value ) override;

	// tolua_begin
	
	/** Set the Type */
	void SetType(const eMobHeadType & a_SkullType);
	
	/** Set the Rotation */
	void SetRotation(eMobHeadRotation a_Rotation);
	
	/** Set the Player Name for Mobheads with Player type */
	void SetOwner(const AString & a_Owner);
	
	/** Get the Type */
	eMobHeadType GetType(void) const { return m_Type; }
	
	/** Get the Rotation */
	eMobHeadRotation GetRotation(void) const { return m_Rotation; }
	
	/** Get the setted Player Name */
	AString GetOwner(void) const { return m_Owner; }
	
	// tolua_end
	
	virtual void UsedBy(cPlayer * a_Player) override;
	virtual void SendTo(cClientHandle & a_Client) override;

	static const char * GetClassStatic(void) { return "cMobHeadEntity"; }
	
private:

	eMobHeadType m_Type;
	eMobHeadRotation m_Rotation;
	AString m_Owner;
} ;  // tolua_export




