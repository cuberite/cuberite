
// CommandOutput.cpp

// Implements the various classes that process command output

#include "Globals.h"
#include "CommandOutput.h"





////////////////////////////////////////////////////////////////////////////////
// cCommandOutputCallback:

void cCommandOutputCallback::vOut(const char * a_Fmt, fmt::printf_args a_ArgList)
{
	AString Output = ::vPrintf(a_Fmt, a_ArgList);
	Output.append("\n");
	Out(Output);
}





////////////////////////////////////////////////////////////////////////////////
// cStringAccumCommandOutputCallback:

void cStringAccumCommandOutputCallback::Out(const AString & a_Text)
{
	m_Accum.append(a_Text);
}





////////////////////////////////////////////////////////////////////////////////
// cLogCommandOutputCallback:

void cLogCommandOutputCallback::Finished(void)
{
	// Log each line separately:
	size_t len = m_Accum.length();
	size_t last = 0;
	for (size_t i = 0; i < len; i++)
	{
		switch (m_Accum[i])
		{
			case '\n':
			{
				LOG("%s", m_Accum.substr(last, i - last).c_str());
				last = i + 1;
				break;
			}
		}
	}  // for i - m_Buffer[]
	if (last < len)
	{
		LOG("%s", m_Accum.substr(last).c_str());
	}

	// Clear the buffer for the next command output:
	m_Accum.clear();
}




