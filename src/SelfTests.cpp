
// SelfTests.h

// Implements the cSelfTests class representing the singleton used for registering self-tests
// This class is only declared if SELF_TEST macro is defined.

#include "Globals.h"
#include "SelfTests.h"





#ifdef SELF_TEST
	cSelfTests::cSelfTests(void):
		m_AllowRegistering(true)
	{
	}





	cSelfTests & cSelfTests::Get(void)
	{
		static cSelfTests singleton;
		return singleton;
	}





	void cSelfTests::Register(cSelfTests::SelfTestFunction a_FnToExecute, const AString & a_TestName)
	{
		ASSERT(Get().m_AllowRegistering);
		Get().m_SelfTests.push_back(std::make_pair(a_FnToExecute, a_TestName));
	}





	void cSelfTests::ExecuteAll(void)
	{
		Get().m_AllowRegistering = false;
		LOG("--- Performing self-tests ---");
		for (auto & test: Get().m_SelfTests)
		{
			LOG("Performing self-test: %s", test.second.c_str());
			try
			{
				test.first();
			}
			catch (const std::exception & exc)
			{
				LOGWARNING("Exception in test %s: %s", test.second.c_str(), exc.what());
			}
			catch (...)
			{
				LOGWARNING("Unknown exception in test %s", test.second.c_str());
			}
		}  // for test - m_SelfTests[]
		LOG("--- Self-tests finished ---");
	}

#endif  // SELF_TEST




