
// StressEvent.cpp

// Stress-tests the cEvent implementation

#include "Globals.h"
#include <thread>





/** Number of repetitions of the thread loops. */
const int NUM_REPETITIONS = 5000;




// Forward declarations are needed for clang
void runThread(cEvent * a_Event1, cEvent * a_Event2, const char * a_ThreadName);





/** Function that runs in a separate thread, notifies event1 and waits for event2, in a loop, NUM_REPETITIONS times.
This basically simulates a producer / consumer pattern with 2 events, one for "queue empty", the other for "queue full". */
void runThread(cEvent * a_Event1, cEvent * a_Event2, const char * a_ThreadName)
{
	LOG("Thread %s started", a_ThreadName);
	for (int i = 0; i < NUM_REPETITIONS; ++i)
	{
		// LOGD("%s: Waiting for event %p (%d)", a_ThreadName, a_Event2, i);
		a_Event2->Wait();
		// LOGD("%s: Setting event %p (%d)", a_ThreadName, a_Event1, i);
		a_Event1->SetAll();
	}
	LOG("Thread %s finished", a_ThreadName);
}





int main()
{
	LOG("Test started");
	cEvent event1, event2;
	event1.Set();
	std::thread thread1(&runThread, &event1, &event2, "A");
	std::thread thread2(&runThread, &event2, &event1, "B");
	thread1.join();
	thread2.join();
	LOG("Test finished");
	return 0;
}




