
// Stopwatch.h

// Implements the cStopwatch class that measures and logs time between its creation and destruction





#pragma once





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
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - m_StartTime).count();
		LOG("Stopwatch: %s took %.03f sec", m_Name, static_cast<double>(duration) / 1000);
	}

protected:
	AString m_Name;
	std::chrono::high_resolution_clock::time_point m_StartTime;
};




