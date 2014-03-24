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

#include "Magmacube.h"





cMagmaCube::cMagmaCube(int a_Size) :
	super("MagmaCube", mtMagmaCube, "mob.MagmaCube.big", "mob.MagmaCube.big", 0.6 * a_Size, 0.6 * a_Size),
	m_Size(a_Size)
{
}





void cMagmaCube::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	UNUSED(a_Killer);
	if (GetSize() > 1)
	{
		AddRandomUncommonDropItem(a_Drops, 25.0f, E_ITEM_MAGMA_CREAM);
	}
}




