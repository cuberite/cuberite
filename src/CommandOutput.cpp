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

// CommandOutput.cpp

// Implements the various classes that process command output

#include "Globals.h"
#include "CommandOutput.h"





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cCommandOutputCallback:

void cCommandOutputCallback::Out(const char * a_Fmt, ...)
{
	AString Output;
	va_list args;
	va_start(args, a_Fmt);
	AppendVPrintf(Output, a_Fmt, args);
	va_end(args);
	Output.append("\n");
	Out(Output);
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cLogCommandOutputCallback:

void cLogCommandOutputCallback::Out(const AString & a_Text)
{
	m_Buffer.append(a_Text);
}





void cLogCommandOutputCallback::Finished(void)
{
	// Log each line separately:
	size_t len = m_Buffer.length();
	size_t last = 0;
	for (size_t i = 0; i < len; i++)
	{
		switch (m_Buffer[i])
		{
			case '\n':
			{
				LOG("%s", m_Buffer.substr(last, i - last).c_str());
				last = i + 1;
				break;
			}
		}
	}  // for i - m_Buffer[]
	if (last < len)
	{
		LOG("%s", m_Buffer.substr(last).c_str());
	}
	
	// Clear the buffer for the next command output:
	m_Buffer.clear();
}




