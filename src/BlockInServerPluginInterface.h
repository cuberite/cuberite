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

#include "Blocks/BlockPluginInterface.h"
#include "World.h"
#include "Root.h"
#include "Bindings/PluginManager.h"

class cBlockInServerPluginInterface : public cBlockPluginInterface
{
public:
	cBlockInServerPluginInterface(cWorld & a_World) : m_World(a_World) {}
	virtual bool CallHookBlockToPickups(cEntity * a_Digger, int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, cItems & a_Pickups) override
	{
		return cRoot::Get()->GetPluginManager()->CallHookBlockToPickups(&m_World, a_Digger, a_BlockX, a_BlockY, a_BlockZ, a_BlockType, a_BlockMeta, a_Pickups);
	}
private:
	cWorld & m_World;
};
