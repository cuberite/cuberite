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
// FlowerPotEntity.h

// Declares the cFlowerPotEntity class representing a single sign in the world





#pragma once

#include "BlockEntity.h"

class cItem;





namespace Json
{
	class Value;
}





// tolua_begin

class cFlowerPotEntity : 
	public cBlockEntity
{
	typedef cBlockEntity super;
	
public:

	// tolua_end
	
	/** Creates a new flowerpot entity at the specified block coords. a_World may be NULL */
	cFlowerPotEntity(int a_BlocX, int a_BlockY, int a_BlockZ, cWorld * a_World);

	bool LoadFromJson( const Json::Value& a_Value );
	virtual void SaveToJson(Json::Value& a_Value ) override;
	
	virtual void Destroy(void) override;

	// tolua_begin
	
	/** Is a flower in the pot? */
	bool IsItemInPot(void) { return !m_Item.IsEmpty(); }
	
	/** Get the item in the flower pot */
	cItem GetItem(void) const { return m_Item; }
	
	/** Set the item in the flower pot */
	void SetItem(const cItem a_Item) { m_Item = a_Item; }
	
	// tolua_end
	
	virtual void UsedBy(cPlayer * a_Player) override;
	virtual void SendTo(cClientHandle & a_Client) override;

	static bool IsFlower(short m_ItemType, short m_ItemData);

	static const char * GetClassStatic(void) { return "cFlowerPotEntity"; }
	
private:

	cItem m_Item;
} ;  // tolua_export




