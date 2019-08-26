// Helper macros for writing exception-based tests




/** The exception that is thrown if a test fails.
It doesn't inherit from any type so that it is not possible to catch it by a mistake,
it needs to be caught explicitly (used in the TEST_THROWS).
It bears a single message that is to be displayed to stderr. */
class TestException
{
public:
	TestException(const AString & aMessage):
		mMessage(aMessage)
	{
	}

	AString mMessage;
};





/** Checks that the two values are equal; if not, throws a TestException. */
#define TEST_EQUAL(VAL1, VAL2) \
	if (VAL1 != VAL2) \
	{ \
		throw TestException(Printf("%s (line %d): Equality test failed: %s != %s", \
			__FUNCTION__, __LINE__, \
			#VAL1, #VAL2 \
		)); \
	}



/** Checks that the two values are equal; if not, throws a TestException, includes the specified message. */
#define TEST_EQUAL_MSG(VAL1, VAL2, MSG) \
	if (VAL1 != VAL2) \
	{ \
		throw TestException(Printf("%s (line %d): Equality test failed: %s != %s (%s)", \
			__FUNCTION__, __LINE__, \
			#VAL1, #VAL2, MSG \
		)); \
	}



/** Checks that the two values are not equal; if they are, throws a TestException. */
#define TEST_NOTEQUAL(VAL1, VAL2) \
	if (VAL1 == VAL2) \
	{ \
		throw TestException(Printf("%s (line %d): Inequality test failed: %s == %s", \
			__FUNCTION__, __LINE__, \
			#VAL1, #VAL2 \
		)); \
	}



/** Checks that the statement throws an exception of the specified class. */
#define TEST_THROWS(Stmt, ExcClass) \
	try \
	{ \
		Stmt; \
		throw TestException(Printf("%s (line %d): Failed to throw an exception of type %s", \
			__FUNCTION__, __LINE__, \
			#ExcClass \
		)); \
	} \
	catch (const ExcClass &) \
	{ \
		/* This is the expected case. */ \
	} \
	catch (const std::exception & exc) \
	{ \
		throw TestException(Printf("%s (line %d): An unexpected std::exception descendant was thrown, was expecting type %s. Message is: %s", \
			__FUNCTION__, __LINE__, \
			#ExcClass, exc.what() \
		)); \
	} \
	catch (...) \
	{ \
		throw TestException(Printf("%s (line %d): An unexpected unknown exception object was thrown, was expecting type %s", \
			__FUNCTION__, __LINE__, \
			#ExcClass \
		)); \
	}





/** Checks that the statement throws an exception of any type. */
#define TEST_THROWS_ANY(Stmt) \
	try \
	{ \
		Stmt; \
		throw TestException(Printf("%s (line %d): Failed to throw an exception of any type", \
			__FUNCTION__, __LINE__ \
		)); \
	} \
	catch (const TestException & exc) \
	{ \
		throw exc; \
	} \
	catch (...) \
	{ \
		/* This is the expected case */ \
	}




/** Checks that the statement causes an ASSERT trigger. */
#define TEST_ASSERTS(Stmt) TEST_THROWS(Stmt, cAssertFailure)

