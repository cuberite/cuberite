
// SelfTests.h

// Declares the cSelfTests class representing the singleton used for registering self-tests
// This class is only declared if SELF_TEST macro is defined.





#pragma once





#ifdef SELF_TEST
	/** Singleton containing registered self-tests.
	Used to schedule self-tests to run after the logging framework is initialized (#2228). */
	class cSelfTests
	{
	public:
		/** Returns the singleton instance of this class. */
		static cSelfTests & Get(void);

		// typedef void (* SelfTestFunction)(void);
		typedef std::function<void(void)> SelfTestFunction;

		/** Registers a self-test to be executed once the logging framework is initialized. */
		static void Register(SelfTestFunction a_FnToExecute, const AString & a_TestName);

		/** Executes all the registered self-tests. */
		static void ExecuteAll(void);

	protected:
		typedef std::vector<std::pair<SelfTestFunction, AString>> SelfTestFunctions;

		/** Functions (registered self-tests) to call once the logging framework is initialized. */
		SelfTestFunctions m_SelfTests;

		/** If true, tests may be registered. Set to false once the tests are executed, to detect tests that are registered too late. */
		bool m_AllowRegistering;


		cSelfTests(void);
	};
#endif  // SELF_TEST




