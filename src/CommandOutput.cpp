
// CommandOutput.cpp

// Implements the various classes that process command output

#include "Globals.h"
#include "CommandOutput.h"





////////////////////////////////////////////////////////////////////////////////
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





////////////////////////////////////////////////////////////////////////////////
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




