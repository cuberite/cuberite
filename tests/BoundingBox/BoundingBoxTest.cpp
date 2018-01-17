
// ByteBufferTest.cpp

// Implements the main app entrypoint for the cByteBuffer class test

#include "Globals.h"
#include "BoundingBox.h"





/** Runs the tests, returns the number of failed tests. */
static int Test(void)
{
	int NumFailed = 0;
	Vector3d Min(1, 1, 1);
	Vector3d Max(2, 2, 2);
	Vector3d LineDefs[] =
	{
		Vector3d(1.5,   4, 1.5), Vector3d(1.5,   3, 1.5),  // Should intersect at 2,    face 1 (YP)
		Vector3d(1.5,   0, 1.5), Vector3d(1.5,   4, 1.5),  // Should intersect at 0.25, face 0 (YM)
		Vector3d(0,     0,   0), Vector3d(2,     2,   2),  // Should intersect at 0.5,  face 0, 3 or 5 (anyM)
		Vector3d(0.999, 0, 1.5), Vector3d(0.999, 4, 1.5),  // Should not intersect
		Vector3d(1.999, 0, 1.5), Vector3d(1.999, 4, 1.5),  // Should intersect at 0.25, face 0 (YM)
		Vector3d(2.001, 0, 1.5), Vector3d(2.001, 4, 1.5),  // Should not intersect
	} ;
	bool Results[] = {true, true, true, false, true, false};
	double LineCoeffs[] = {2, 0.25, 0.5, 0, 0.25, 0};

	for (size_t i = 0; i < ARRAYCOUNT(LineDefs) / 2; i++)
	{
		double LineCoeff;
		eBlockFace Face;
		Vector3d Line1 = LineDefs[2 * i];
		Vector3d Line2 = LineDefs[2 * i + 1];
		bool res = cBoundingBox::CalcLineIntersection(Min, Max, Line1, Line2, LineCoeff, Face);
		if (res != Results[i])
		{
			LOGERROR("LineIntersection({%.02f, %.02f, %.02f}, {%.02f, %.02f, %.02f}) -> %d, %.05f, %d",
				Line1.x, Line1.y, Line1.z,
				Line2.x, Line2.y, Line2.z,
				res ? 1 : 0, LineCoeff, Face
			);
			NumFailed += 1;
		}
		if (res)
		{
			if (std::abs(LineCoeff - LineCoeffs[i]) > 0.0000001)
			{
				LOGERROR("LineIntersection({%.02f, %.02f, %.02f}, {%.02f, %.02f, %.02f}) -> %d, %.05f, %d",
					Line1.x, Line1.y, Line1.z,
					Line2.x, Line2.y, Line2.z,
					res ? 1 : 0, LineCoeff, Face
				);
				NumFailed += 1;
			}
		}
	}  // for i - LineDefs[]
	return 0;
}





int main(int argc, char * argv[])
{
	LOGD("Test started");

	LOGD("Running test");
	auto NumFailed = Test();
	LOG("BoundingBox test finished, number of failed tests: %d", NumFailed);
	return NumFailed;
}





