// PieceRotation.cpp

// Implements the tests for cPiece::cConnector::eDirection rotations

#include "Globals.h"
#include <exception>
#include <map>
#include "Generating/PiecePool.h"





static cPiece::cConnector::eDirection g_AllDirections[] =
{
	cPiece::cConnector::dirXM,
	cPiece::cConnector::dirXP,
	cPiece::cConnector::dirYM,
	cPiece::cConnector::dirYP,
	cPiece::cConnector::dirZM,
	cPiece::cConnector::dirZP,
	cPiece::cConnector::dirYM_XM_ZM,
	cPiece::cConnector::dirYM_XM_ZP,
	cPiece::cConnector::dirYM_XP_ZM,
	cPiece::cConnector::dirYM_XP_ZP,
	cPiece::cConnector::dirYP_XM_ZM,
	cPiece::cConnector::dirYP_XM_ZP,
	cPiece::cConnector::dirYP_XP_ZM,
	cPiece::cConnector::dirYP_XP_ZP,
};





class cTestFailure: public std::exception
{
public:
	cTestFailure(const char * a_Expression, const char * a_FileName, const int a_Line):
		std::exception(),
		m_Expression(a_Expression),
		m_FileName(a_FileName),
		m_Line(a_Line)
	{
	}

	const std::string m_Expression;
	const std::string m_FileName;
	const int m_Line;
};





#define EXPECT(X) \
	do { \
		if (!(X)) \
		{ \
			ASSERT(X); \
			throw cTestFailure(#X, __FILE__, __LINE__); \
		} \
	} while (false)





/** Tests that rotating a direction in one way and then the opposite way produces the original direction. */
static void testBackAndForth()
{
	for (size_t i = 0; i < ARRAYCOUNT(g_AllDirections); ++i)
	{
		auto rotated = cPiece::cConnector::RotateDirectionCW(g_AllDirections[i]);
		auto back = cPiece::cConnector::RotateDirectionCCW(rotated);
		EXPECT(back == g_AllDirections[i]);
	}
	for (size_t i = 0; i < ARRAYCOUNT(g_AllDirections); ++i)
	{
		auto rotated = cPiece::cConnector::RotateDirectionCCW(g_AllDirections[i]);
		auto back = cPiece::cConnector::RotateDirectionCW(rotated);
		EXPECT(back == g_AllDirections[i]);
	}
}





/** Tests rotating the direction 360 degrees. */
static void testFullRotation()
{
	// Rotate 90 degrees CCW four times:
	for (size_t i = 0; i < ARRAYCOUNT(g_AllDirections); ++i)
	{
		auto d = g_AllDirections[i];
		for (int j = 0; j < 4; ++j)
		{
			d = cPiece::cConnector::RotateDirectionCCW(d);
		}
		EXPECT(d == g_AllDirections[i]);
	}

	// Rotate 90 degrees CW four times:
	for (size_t i = 0; i < ARRAYCOUNT(g_AllDirections); ++i)
	{
		auto d = g_AllDirections[i];
		for (int j = 0; j < 4; ++j)
		{
			d = cPiece::cConnector::RotateDirectionCW(d);
		}
		EXPECT(d == g_AllDirections[i]);
	}

	// Rotate 180 degrees twice:
	for (size_t i = 0; i < ARRAYCOUNT(g_AllDirections); ++i)
	{
		auto d = g_AllDirections[i];
		d = cPiece::cConnector::RotateDirection(d);
		d = cPiece::cConnector::RotateDirection(d);
		EXPECT(d == g_AllDirections[i]);
	}
}





/** Tests that no two values are rotated onto the same destination value. */
template <class Fn>
static void testPermutation(Fn & a_Fn)
{
	std::map<cPiece::cConnector::eDirection, int> numDest;
	for (size_t i = 0; i < ARRAYCOUNT(g_AllDirections); ++i)
	{
		auto d = a_Fn(g_AllDirections[i]);
		EXPECT(numDest[d] == 0);
		numDest[d] = 1;
	}
}





int main(void)
{
	LOG("PieceRotation test starting.");
	try
	{
		testBackAndForth();
		testFullRotation();
		testPermutation(cPiece::cConnector::RotateDirection);
		testPermutation(cPiece::cConnector::RotateDirectionCW);
		testPermutation(cPiece::cConnector::RotateDirectionCCW);
	}
	catch (const cTestFailure & f)
	{
		LOGERROR("Test failed:\tExpression: %s\n\tFile: %s (%d)", f.m_Expression.c_str(), f.m_FileName.c_str(), f.m_Line);
		return 1;
	}
	catch (const std::exception & exc)
	{
		LOGERROR("Exception caught: %s", exc.what());
		return 2;
	}
	LOG("PieceRotation test succeeded.");
	return 0;
}




