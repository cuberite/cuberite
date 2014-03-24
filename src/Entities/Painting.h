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

#include "Entity.h"





// tolua_begin
class cPainting :
	public cEntity
{
	// tolua_end
	typedef cEntity super;

public:
	CLASS_PROTODEF(cPainting);

	cPainting(const AString & a_Name, int a_Direction, double a_X, double a_Y, double a_Z);
	const AString & GetName(void) const { return m_Name; } // tolua_export
	int GetDirection(void) const { return m_Direction; } // tolua_export

private:

	virtual void SpawnOn(cClientHandle & a_Client) override;
	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;
	virtual void GetDrops(cItems & a_Items, cEntity * a_Killer) override;
	virtual void KilledBy(cEntity * a_Killer) override
	{
		super::KilledBy(a_Killer);
		Destroy();
	}

	AString m_Name;
	int m_Direction;

};  // tolua_export




