
// Stopwatch.h

// Implements the cStopwatch class that measures and logs time between its creation and destruction






#pragma once

#include <chrono>





class cStopwatch
{
public:
	cStopwatch(const AString & a_Name):
		m_Name(a_Name),
		m_StartTime(std::chrono::high_resolution_clock::now())
	{
	}

	~cStopwatch()
	{
		#ifdef _DEBUG
			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - m_StartTime).count();
			LOGD("Stopwatch: %s took %.03f sec", m_Name.c_str(), static_cast<double>(duration) / 1000);
		#endif  // _DEBUG
	}

protected:
	AString m_Name;
	std::chrono::high_resolution_clock::time_point m_StartTime;
};




