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

#include "Painting.h"
#include "ClientHandle.h"
#include "Player.h"
#include "../Chunk.h"





cPainting::cPainting(const AString & a_Name, int a_Direction, double a_X, double a_Y, double a_Z)
	: cEntity(etPainting, a_X, a_Y, a_Z, 1, 1),
	m_Name(a_Name),
	m_Direction(a_Direction)
{
}






void cPainting::SpawnOn(cClientHandle & a_Client)
{
	a_Client.SendPaintingSpawn(*this);
}





void cPainting::Tick(float a_Dt, cChunk & a_Chunk)
{
	UNUSED(a_Dt);
	UNUSED(a_Chunk);
}





void cPainting::GetDrops(cItems & a_Items, cEntity * a_Killer)
{
	if ((a_Killer != NULL) && a_Killer->IsPlayer() && !((cPlayer *)a_Killer)->IsGameModeCreative())
	{
		a_Items.push_back(cItem(E_ITEM_PAINTING));
	}
}




