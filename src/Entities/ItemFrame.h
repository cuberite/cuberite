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

#include "HangingEntity.h"





// tolua_begin
class cItemFrame :
	public cHangingEntity
{
	// tolua_end
	typedef cHangingEntity super;

public:

	CLASS_PROTODEF(cItemFrame);

	cItemFrame(eBlockFace a_BlockFace, double a_X, double a_Y, double a_Z);

	/** Returns the item in the frame */
	const cItem & GetItem(void) { return m_Item; }                  // tolua_export

	/** Set the item in the frame */
	void SetItem(cItem & a_Item) { m_Item = a_Item; };              // tolua_export

	/** Returns the rotation from the item in the frame */
	Byte GetRotation(void) const { return m_Rotation; }             // tolua_export

	/** Set the rotation from the item in the frame */
	void SetRotation(Byte a_Rotation) { m_Rotation = a_Rotation; }  // tolua_export

private:

	virtual void OnRightClicked(cPlayer & a_Player) override;
	virtual void KilledBy(cEntity * a_Killer) override;
	virtual void GetDrops(cItems & a_Items, cEntity * a_Killer) override;

	cItem m_Item;
	Byte m_Rotation;

};  // tolua_export




