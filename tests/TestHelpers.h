// Helper macros for writing exception-based tests

/*
The tests are supposed to be contained in small static functions that get called from a main function provided by this framework:
static void test1()
{
	// Perform the test
}

...

IMPLEMENT_TEST_MAIN("TestName",
	test1();
	...
)
*/




/** The exception that is thrown if a test fails.
It doesn't inherit from any type so that it is not possible to catch it by a mistake,
it needs to be caught explicitly (used in the TEST_THROWS).
It bears a single message that is to be displayed to stderr. */
class TestException
{
public:
	TestException(const AString & aFileName, int aLineNumber, const AString & aFunctionName, const AString & aMessage):
		mFileName(aFileName),
		mLineNumber(aLineNumber),
		mFunctionName(aFunctionName),
		mMessage(aMessage)
	{
	}

	AString mFileName;
	int mLineNumber;
	AString mFunctionName;
	AString mMessage;
};





/** Checks that the two values are equal; if not, throws a TestException. */
#define TEST_EQUAL(VAL1, VAL2) \
	do { \
		if (VAL1 != VAL2) \
		{ \
			throw TestException( \
				__FILE__, __LINE__, __FUNCTION__, \
				Printf("Equality test failed: %s != %s", #VAL1, #VAL2)); \
		} \
	}	while (false)





/** Checks that the two values are equal; if not, throws a TestException, includes the specified message. */
#define TEST_EQUAL_MSG(VAL1, VAL2, MSG) \
	do { \
		if (VAL1 != VAL2) \
		{ \
			throw TestException( \
				__FILE__, __LINE__, __FUNCTION__, \
				Printf("Equality test failed: %s != %s (%s)", #VAL1, #VAL2, MSG)); \
		} \
	} while (false)





/** Checks that the two values are not equal; if they are, throws a TestException. */
#define TEST_NOTEQUAL(VAL1, VAL2) \
	do { \
		if (VAL1 == VAL2) \
		{ \
			throw TestException( \
				__FILE__, __LINE__, __FUNCTION__, \
				Printf("Inequality test failed: %s == %s", #VAL1, #VAL2) \
				); \
		} \
	} while(false)





/** Checks that the statement evaluates to true. */
#define TEST_TRUE(X) TEST_EQUAL(X, true)





/** Checks that the statement evaluates to false. */
#define TEST_FALSE(X) TEST_EQUAL(X, false)





/** Checks that the statement returns a value greater than or equal to the specified value. */
#define TEST_GREATER_THAN_OR_EQUAL(Stmt, Val) \
	do { \
		if (Stmt < Val) \
		{ \
			throw TestException(__FILE__, __LINE__, __FUNCTION__, Printf("Comparison failed: %s < %s", #Stmt, #Val)); \
		} \
	} while (false)





/** Checks that the statement returns a value less than or equal to the specified value. */
#define TEST_LESS_THAN_OR_EQUAL(Stmt, Val) \
	do { \
		if (Stmt > Val) \
		{ \
			throw TestException(__FILE__, __LINE__, __FUNCTION__, Printf("Comparison failed: %s > %s", #Stmt, #Val)); \
		} \
	} while (false)





/** Checks that the statement throws an exception of the specified class. */
#define TEST_THROWS(Stmt, ExcClass) \
	do { \
		try \
		{ \
			Stmt; \
			throw TestException( \
				__FILE__, __LINE__, __FUNCTION__, \
				Printf("Failed to throw an exception of type %s", #ExcClass) \
				); \
		} \
		catch (const ExcClass &) \
		{ \
			/* This is the expected case. */ \
		} \
		catch (const std::exception & exc) \
		{ \
			throw TestException( \
				__FILE__, __LINE__, __FUNCTION__, \
				Printf("An unexpected std::exception descendant was thrown, was expecting type %s. Message is: %s", \
					#ExcClass, exc.what() \
				)); \
		} \
		catch (...)\
		{ \
			throw TestException( \
				__FILE__, __LINE__, __FUNCTION__, \
				Printf("An unexpected unknown exception object was thrown, was expecting type %s", \
							 #ExcClass \
					)); \
		} \
	} while (false)





/** Checks that the statement throws an exception of any type. */
#define TEST_THROWS_ANY(Stmt) \
	do { \
		try \
		{ \
			Stmt; \
			throw TestException( \
				__FILE__, __LINE__, __FUNCTION__, \
				"Failed to throw an exception of any type"); \
		} \
		catch (const TestException & exc) \
		{ \
			throw exc; \
		} \
		catch (...)\
		{ \
			/* This is the expected case */ \
		} \
	} while (false)





/** Fails the test unconditionally, with the specified message. */
#define TEST_FAIL(MSG) \
	throw TestException(__FILE__, __LINE__, __FUNCTION__, MSG)





/** Checks that the statement causes an ASSERT trigger. */
#ifdef _DEBUG
	#define TEST_ASSERTS(Stmt) TEST_THROWS(Stmt, cAssertFailure)
#else
	#define TEST_ASSERTS(Stmt) LOG("Skipped, cannot test in Release mode: TEST_ASSERT(%s); (%s:%d)", #Stmt, __FILE__, __LINE__)
#endif  // else _DEBUG





/** Used to implement the main() function for tests. */
#define IMPLEMENT_TEST_MAIN(TestName, TestCode) \
int main() \
{ \
	LOG("Test started: %s", TestName); \
	\
	try \
	{ \
		TestCode; \
	} \
	catch (const TestException & exc) \
	{ \
		LOGERROR("Test has failed at file %s, line %d, function %s: %s", \
			exc.mFileName.c_str(), \
			exc.mLineNumber, \
			exc.mFunctionName.c_str(), \
			exc.mMessage.c_str() \
		); \
		return 1; \
	} \
	catch (const std::exception & exc) \
	{ \
		LOGERROR("Test has failed, an exception was thrown: %s", exc.what()); \
		return 1; \
	} \
	catch (const cAssertFailure & exc) \
	{ \
		LOGERROR("Test has failed, an unexpected ASSERT was triggered at %s:%d: %s", \
			exc.fileName().c_str(), exc.lineNumber(), exc.expression().c_str() \
		); \
		return 1; \
	} \
	catch (...) \
	{ \
		LOGERROR("Test has failed, an unhandled exception was thrown."); \
		return 1; \
	} \
	\
	LOG("Test finished"); \
	return 0; \
}
