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

// CommandBlockEntity.h

// Declares the cCommandBlockEntity class representing a single command block in the world





#pragma once

#include "BlockEntity.h"





namespace Json
{
	class Value;
}





// tolua_begin

class cCommandBlockEntity : 
	public cBlockEntity
{
	typedef cBlockEntity super;
	
public:

	// tolua_end
	
	/// Creates a new empty command block entity
	cCommandBlockEntity(int a_X, int a_Y, int a_Z, cWorld * a_World);

	bool LoadFromJson( const Json::Value& a_Value );
	virtual void SaveToJson(Json::Value& a_Value ) override;

	virtual bool Tick(float a_Dt, cChunk & a_Chunk) override;
	virtual void SendTo(cClientHandle & a_Client) override;
	virtual void UsedBy(cPlayer * a_Player) override;

	void SetLastOutput(const AString & a_LastOut);

	void SetResult(const NIBBLETYPE a_Result);

	// tolua_begin

	/// Sets the internal redstone power flag to "on" or "off", depending on the parameter. Calls Activate() if appropriate
	void SetRedstonePower(bool a_IsPowered);

	/// Sets the command block to execute a command in the next tick
	void Activate(void);
	
	/// Sets the command
	void SetCommand(const AString & a_Cmd);

	/// Retrieves stored command
	const AString & GetCommand(void) const;

	/// Retrieves the last line of output generated by the command block
	const AString & GetLastOutput(void) const;

	/// Retrieves the result (signal strength) of the last operation
	NIBBLETYPE GetResult(void) const;
	
	// tolua_end
	
private:

	/// Executes the associated command
	void Execute();

	bool m_ShouldExecute;
	bool m_IsPowered;

	AString m_Command;

	AString m_LastOutput;

	NIBBLETYPE m_Result;
} ;  // tolua_export




