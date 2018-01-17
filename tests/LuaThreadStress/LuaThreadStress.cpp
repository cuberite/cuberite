// LuaThreadStress.cpp

// Implements a stress-test of cLuaState under several threads

#include "Globals.h"
#include "Bindings/LuaState.h"
#include <thread>
#include <random>





/** How long the threading test should run. */
static const int NUM_SECONDS_TO_TEST = 10;





/** Retrieves a callback from the Lua state that can be later called.
Calls the Lua function getCallback with a_Seed param to retrieve the callback. */
static cLuaState::cCallbackPtr getCallback(cLuaState & a_LuaState, unsigned a_Seed)
{
	cLuaState::cLock lock(a_LuaState);
	cLuaState::cCallbackPtr res;
	a_LuaState.Call("getCallback", a_Seed, cLuaState::Return, res);
	return res;
}





/** Runs a single thread that stress-tests the cLuaState object.
a_LuaState is the Lua state on which to operate.
a_Seed is the seed for the random number generator for this thread.
a_ShouldTerminate is a bool flag that another thread sets to ask this thread to terminate.
a_FailResult is a shared result state that is written by any thread upon failure (so if it contains nonzero, at least one thread has failed). */
static void runStress(cLuaState * a_LuaState, unsigned a_Seed, std::atomic<bool> * a_ShouldTerminate, std::atomic<int> * a_FailResult)
{
	std::minstd_rand rnd;
	rnd.seed(a_Seed);
	auto callbackSeed = static_cast<unsigned>(rnd());
	auto callback = getCallback(*a_LuaState, callbackSeed);
	while (!a_ShouldTerminate->load())
	{
		// Pick a random operation on the Lua state and peform it:
		switch (rnd() % 4)
		{
			case 0:
			{
				// Get a new callback:
				callbackSeed = callbackSeed + 1;
				callback = getCallback(*a_LuaState, callbackSeed);
				break;
			}

			default:
			{
				// Call the callback, if still available:
				auto param = static_cast<unsigned>(rnd());
				unsigned returnValue;
				if (callback->Call(param, cLuaState::Return, returnValue))
				{
					if (returnValue != param + callbackSeed)
					{
						LOGWARNING("Bad value returned from the callback");
						*a_FailResult = 2;
						a_ShouldTerminate->store(true);
						return;
					}
				}
				break;
			}
		}  // switch (random)

		// Once in every ~10k operations, reload the lua state completely:
		if ((rnd() % 10000) == 0)
		{
			cLuaState::cLock lock(*a_LuaState);
			a_LuaState->Close();
			a_LuaState->Create();
			if (!a_LuaState->LoadFile("Test.lua"))
			{
				*a_FailResult = 3;
				a_ShouldTerminate->store(true);
				return;
			}
		}
	}  // while (!a_ShouldTerminate)
}





static int DoTest(void)
{
	cLuaState L("LuaThreadStress test");
	L.Create();
	if (!L.LoadFile("Test.lua"))
	{
		return 1;
	}

	// Start the concurrect threads:
	std::atomic<bool> shouldTerminate(false);
	std::atomic<int> failResult(0);
	std::thread threads[] =
	{
		std::thread(runStress, &L, 0, &shouldTerminate, &failResult),
		std::thread(runStress, &L, 1, &shouldTerminate, &failResult),
		std::thread(runStress, &L, 2, &shouldTerminate, &failResult),
		std::thread(runStress, &L, 3, &shouldTerminate, &failResult),
	};

	// Let the threads run wild:
	for (int i = 1; i <= NUM_SECONDS_TO_TEST; ++i)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
		LOG("Testing (%d out of %d seconds)...", i, NUM_SECONDS_TO_TEST);
	}

	// Terminate everything:
	LOG("Terminating the threads");
	shouldTerminate = true;
	for (auto & t: threads)
	{
		t.join();
	}
	LOG("Threads terminated.");

	return failResult.load();
}





int main()
{
	LOG("LuaThreadStress starting.");

	int res = DoTest();
	LOG("LuaThreadStress test done: %s", (res == 0) ? "success" : "failure");
	if (res != 0)
	{
		return res;
	}

	LOG("LuaThreadStress finished.");
	return 0;
}
