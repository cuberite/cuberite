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

#include "BlockEntity.h"


namespace Json
{
	class Value;
}





enum ENUM_NOTE_INSTRUMENTS
{
	E_INST_HARP_PIANO  = 0,
	E_INST_DOUBLE_BASS = 1,
	E_INST_SNARE_DRUM  = 2,
	E_INST_CLICKS      = 3,
	E_INST_BASS_DRUM   = 4
};





// tolua_begin

class cNoteEntity : 
	public cBlockEntity
{
	typedef cBlockEntity super;
public:

	// tolua_end

	/// Creates a new note entity. a_World may be NULL
	cNoteEntity(int a_X, int a_Y, int a_Z, cWorld * a_World);

	bool LoadFromJson(const Json::Value & a_Value);
	virtual void SaveToJson(Json::Value & a_Value) override;

	// tolua_begin
	
	char GetPitch(void);
	void SetPitch(char a_Pitch);
	void IncrementPitch(void);
	void MakeSound(void);
	
	// tolua_end
	
	virtual void UsedBy(cPlayer * a_Player) override;
	virtual void SendTo(cClientHandle &) override { };

	static const char * GetClassStatic(void) { return "cNoteEntity"; }

private:
	char m_Pitch;
} ;  // tolua_export




