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

#include "BlockEntityWithItems.h"





namespace Json
{
	class Value;
};

class cClientHandle;
class cServer;
class cNBTData;





// tolua_begin
class cEnderChestEntity :
	public cBlockEntityWithItems
{
	typedef cBlockEntityWithItems super;
	
public:
	enum {
		ContentsHeight = 3,
		ContentsWidth  = 9,
	} ;
	
	// tolua_end
	
	/// Constructor used for normal operation
	cEnderChestEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World);
	
	virtual ~cEnderChestEntity();

	static const char * GetClassStatic(void) { return "cEnderChestEntity"; }

	bool LoadFromJson(const Json::Value & a_Value);
	
	// cBlockEntity overrides:
	virtual void SaveToJson(Json::Value & a_Value) override;
	virtual void SendTo(cClientHandle & a_Client) override;
	virtual void UsedBy(cPlayer * a_Player) override;
	
	/// Opens a new chest window for this chest. Scans for neighbors to open a double chest window, if appropriate.
	void OpenNewWindow(void);
} ;  // tolua_export




