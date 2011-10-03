#pragma once

class cTimer
{
public:
	cTimer();
	~cTimer();

	long long GetNowTime();
private:

#ifdef _WIN32
	void* m_TicksPerSecond; // LARGE_INTEGER*
#endif
};