
// ByteBufferTest.cpp

// Implements the main app entrypoint for the cByteBuffer class test

#include <gtest/gtest.h>

#include "Globals.h"
#include "BoundingBox.h"

constexpr Vector3d Min(1, 1, 1);
constexpr Vector3d Max(2, 2, 2);
struct LineTestCase
{
	Vector3d start;
	Vector3d end;
	bool expectedRes;
	double expectedLineCoeff;
};

#define VECTOR_ASSERT(a, b) \
do {\
    EXPECT_NEAR((a).x, (b).x, 0.00000001); \
    EXPECT_NEAR((a).y, (b).y, 0.00000001); \
    EXPECT_NEAR((a).z, (b).z, 0.00000001); \
} while (0)


class BoundingBoxLineIntersect : public ::testing::TestWithParam<LineTestCase> {
};


INSTANTIATE_TEST_SUITE_P(
	IntersectVariants, BoundingBoxLineIntersect,
	::testing::Values(
		LineTestCase{Vector3d(1.5,   4, 1.5), Vector3d(1.5,   3, 1.5), true, 2},
		LineTestCase{Vector3d(1.5,   0, 1.5), Vector3d(1.5,   4, 1.5), true, 0.25},
		LineTestCase{Vector3d(0,     0,   0), Vector3d(2,     2,   2), true, 0.5},
		LineTestCase{Vector3d(0.999, 0, 1.5), Vector3d(0.999, 4, 1.5), false, 0},
		LineTestCase{Vector3d(1.999, 0, 1.5), Vector3d(1.999, 4, 1.5), true, 0.25},
		LineTestCase{Vector3d(2.001, 0, 1.5), Vector3d(2.001, 4, 1.5), false, 0}
		)
);


TEST_P(BoundingBoxLineIntersect, IntersectTest) {
	LineTestCase const & params = GetParam();
	double LineCoeff;
	eBlockFace Face;
	auto res = cBoundingBox::CalcLineIntersection(
		Min, Max, params.start, params.end, LineCoeff, Face);
	EXPECT_EQ(res, params.expectedRes);
	if (res)
	{
		EXPECT_NEAR(LineCoeff, params.expectedLineCoeff, 0.00000001);
	}
}


TEST(BoundingBoxConstructor, Vector_DoubleConstructor) {
	Vector3d Pos1(1, 2, 3);
	Vector3d Pos2(4, 5, 6);

	cBoundingBox Box1(Pos1, Pos2);
	cBoundingBox Box2(Pos1.x, Pos2.x, Pos1.y, Pos2.y, Pos1.z, Pos2.z);

	VECTOR_ASSERT(Box1.GetMin(), Box2.GetMin());
	VECTOR_ASSERT(Box1.GetMax(), Box2.GetMax());
}


TEST(BoundingBoxConstructor, Vector_RangeConstructor) {
	Vector3d Start(0, 0, 0);
	Vector3d End(20, 5, 20);
	Vector3d Center(10, 0, 10);
	constexpr double Radius = 10;
	constexpr double Height = 5;

	cBoundingBox Box1(Start, End);
	cBoundingBox Box2(Center, Radius, Height);

	VECTOR_ASSERT(Box1.GetMin(), Box2.GetMin());
	VECTOR_ASSERT(Box1.GetMax(), Box2.GetMax());
}


TEST(BoundingBoxConstructor, Vector_RangeOffsetConstructor) {
	Vector3d Start(0, -5, 0);
	Vector3d End(20, 0, 20);
	Vector3d Center(10, 0, 10);
	constexpr double Radius = 10;
	constexpr double Height = 5;
	constexpr double Offset = -5;

	cBoundingBox Box1(Start, End);
	cBoundingBox Box2(Center, Radius, Height, Offset);

	VECTOR_ASSERT(Box1.GetMin(), Box2.GetMin());
	VECTOR_ASSERT(Box1.GetMax(), Box2.GetMax());
}


TEST(BoundingBoxAccessor, GetMin) {
	cBoundingBox Box(Min, Max);

	VECTOR_ASSERT(Box.GetMin(), Min);
	EXPECT_EQ(Box.GetMinX(), Min.x);
	EXPECT_EQ(Box.GetMinY(), Min.y);
	EXPECT_EQ(Box.GetMinZ(), Min.z);
}


TEST(BoundingBoxAccessor, GetMax) {
	cBoundingBox Box(Min, Max);

	VECTOR_ASSERT(Box.GetMax(), Max);
	EXPECT_EQ(Box.GetMaxX(), Max.x);
	EXPECT_EQ(Box.GetMaxY(), Max.y);
	EXPECT_EQ(Box.GetMaxZ(), Max.z);
}


TEST(BoundingBoxConstructor, Vector_StartLength) {
	Vector3d Start(0, 0, 0);
	Vector3d End(20, 20, 20);
	Vector3d Center(10, 10, 10);
	constexpr double Length = 20;

	cBoundingBox Box1(Start, End);
	cBoundingBox Box2(Center, Length);

	VECTOR_ASSERT(Box1.GetMin(), Box2.GetMin());
	VECTOR_ASSERT(Box1.GetMax(), Box2.GetMax());
}


TEST(BoundingBoxMove, Double) {
	cBoundingBox Box1(Min, Max);
	cBoundingBox Box2(Min, Max);

	Box1.Move(1, 2, 3);

	VECTOR_ASSERT(Box1.GetMin(), Vector3d(2, 3, 4));
	VECTOR_ASSERT(Box1.GetMax(), Vector3d(3, 4, 5));
}


TEST(BoundingBoxMove, Vector) {
	cBoundingBox Box1(Min, Max);
	cBoundingBox Box2(Min, Max);

	Box1.Move(Vector3d(1, 2, 3));

	VECTOR_ASSERT(Box1.GetMin(), Vector3d(2, 3, 4));
	VECTOR_ASSERT(Box1.GetMax(), Vector3d(3, 4, 5));
}


TEST(BoundingBoxExpand, Double) {
	cBoundingBox Box1(Min, Max);
	cBoundingBox Box2(Min, Max);

	Box1.Expand(1, 2, 3);

	VECTOR_ASSERT(Box1.GetMin(), Vector3d(0, -1, -2));
	VECTOR_ASSERT(Box1.GetMax(), Vector3d(3, 4, 5));
}


TEST(BoundingBoxIntersect, Intersect) {
	cBoundingBox Box1(Vector3d(0, 0, 0), Vector3d(10, 10, 10));
	cBoundingBox Box2(Vector3d(5, 5, 5), Vector3d(15, 15, 15));

	EXPECT_TRUE(Box1.DoesIntersect(Box2));
	EXPECT_TRUE(Box2.DoesIntersect(Box1));
}


TEST(BoundingBoxIntersect, NoIntersect) {
	cBoundingBox Box1(Vector3d(0, 0, 0), Vector3d(10, 10, 10));
	cBoundingBox Box2(Vector3d(11, 11, 11), Vector3d(15, 15, 15));

	EXPECT_FALSE(Box1.DoesIntersect(Box2));
	EXPECT_FALSE(Box2.DoesIntersect(Box1));
}


TEST(BoundingBoxIntersect, IntersectEdge) {
	cBoundingBox Box1(Vector3d(0, 0, 0), Vector3d(10, 10, 10));
	cBoundingBox Box2(Vector3d(10, 10, 10), Vector3d(15, 15, 15));

	EXPECT_TRUE(Box1.DoesIntersect(Box2));
	EXPECT_TRUE(Box2.DoesIntersect(Box1));
}


TEST(BoundingBoxUnion, Union) {
	cBoundingBox Box1(Vector3d(0, 0, 0), Vector3d(10, 10, 10));
	cBoundingBox Box2(Vector3d(5, 5, 5), Vector3d(15, 15, 15));

	cBoundingBox Box3 = Box1.Union(Box2);
	cBoundingBox Box4 = Box2.Union(Box1);

	VECTOR_ASSERT(Box3.GetMin(), Vector3d(0, 0, 0));
	VECTOR_ASSERT(Box3.GetMax(), Vector3d(15, 15, 15));

	VECTOR_ASSERT(Box4.GetMin(), Vector3d(0, 0, 0));
	VECTOR_ASSERT(Box4.GetMax(), Vector3d(15, 15, 15));
}


TEST(BoundingBoxUnion, UnionEdge) {
	cBoundingBox Box1(Vector3d(0, 0, 0), Vector3d(10, 10, 10));
	cBoundingBox Box2(Vector3d(10, 10, 10), Vector3d(15, 15, 15));

	cBoundingBox Box3 = Box1.Union(Box2);
	cBoundingBox Box4 = Box2.Union(Box1);

	VECTOR_ASSERT(Box3.GetMin(), Vector3d(0, 0, 0));
	VECTOR_ASSERT(Box3.GetMax(), Vector3d(15, 15, 15));

	VECTOR_ASSERT(Box4.GetMin(), Vector3d(0, 0, 0));
	VECTOR_ASSERT(Box4.GetMax(), Vector3d(15, 15, 15));
}


TEST(BoundingBoxUnion, UnionNoIntersect) {
	cBoundingBox Box1(Vector3d(0, 0, 0), Vector3d(10, 10, 10));
	cBoundingBox Box2(Vector3d(11, 11, 11), Vector3d(15, 15, 15));

	cBoundingBox Box3 = Box1.Union(Box2);
	cBoundingBox Box4 = Box2.Union(Box1);

	VECTOR_ASSERT(Box3.GetMin(), Vector3d(0, 0, 0));
	VECTOR_ASSERT(Box3.GetMax(), Vector3d(15, 15, 15));

	VECTOR_ASSERT(Box4.GetMin(), Vector3d(0, 0, 0));
	VECTOR_ASSERT(Box4.GetMax(), Vector3d(15, 15, 15));
}


TEST(BoundingBoxIsInside, IsInsideVector) {
	cBoundingBox Box1(Vector3d(0, 0, 0), Vector3d(10, 10, 10));

	EXPECT_TRUE(Box1.IsInside(Vector3d(5, 5, 5)));
	EXPECT_TRUE(Box1.IsInside(Vector3d(0, 0, 0)));
	EXPECT_TRUE(Box1.IsInside(Vector3d(10, 10, 10)));
	EXPECT_FALSE(Box1.IsInside(Vector3d(-1, 5, 5)));
	EXPECT_FALSE(Box1.IsInside(Vector3d(5, -1, 5)));
	EXPECT_FALSE(Box1.IsInside(Vector3d(5, 5, -1)));
	EXPECT_FALSE(Box1.IsInside(Vector3d(11, 5, 5)));
	EXPECT_FALSE(Box1.IsInside(Vector3d(5, 11, 5)));
	EXPECT_FALSE(Box1.IsInside(Vector3d(5, 5, 11)));
}


TEST(BoundingBoxIsInside, IsInsideDouble) {
	cBoundingBox Box1(Vector3d(0, 0, 0), Vector3d(10, 10, 10));

	EXPECT_TRUE(Box1.IsInside(5, 5, 5));
	EXPECT_TRUE(Box1.IsInside(0, 0, 0));
	EXPECT_TRUE(Box1.IsInside(10, 10, 10));
	EXPECT_FALSE(Box1.IsInside(-1, 5, 5));
	EXPECT_FALSE(Box1.IsInside(5, -1, 5));
	EXPECT_FALSE(Box1.IsInside(5, 5, -1));
	EXPECT_FALSE(Box1.IsInside(11, 5, 5));
	EXPECT_FALSE(Box1.IsInside(5, 11, 5));
	EXPECT_FALSE(Box1.IsInside(5, 5, 11));
}


TEST(BoundingBoxIsInside, IsInsideBoundingBox) {
	cBoundingBox Box1(Vector3d(0, 0, 0), Vector3d(10, 10, 10));
	cBoundingBox Box2(Vector3d(5, 5, 5), Vector3d(6, 6, 6));
	cBoundingBox Box3(Vector3d(11, 11, 11), Vector3d(15, 15, 15));

	EXPECT_TRUE(Box1.IsInside(Box2));
	EXPECT_FALSE(Box1.IsInside(Box3));
}


TEST(BoundingBoxIsInside, IsInsideBoundingBoxStatic) {
	Vector3d Vec1(0, 0, 0);
	Vector3d Vec2(10, 10, 10);
	Vector3d Vec3(5, 5, 5);

	EXPECT_TRUE(cBoundingBox::IsInside(Vec1, Vec2, Vec3));

	Vec3 = Vector3d(11, 11, 11);
	EXPECT_FALSE(cBoundingBox::IsInside(Vec1, Vec2, Vec3));

	constexpr double x = 5;
	constexpr double y = 5;
	constexpr double z = 5;

	EXPECT_TRUE(cBoundingBox::IsInside(Vec1, Vec2, x, y, z));
}


TEST(BoudingBoxIntersection, Line) {
	Vector3d Start(0, 0, 0);
	Vector3d End(10, 10, 10);

	cBoundingBox Box1(Vector3d(5, 5, 5), Vector3d(6, 6, 6));

	double LineCoeff;
	eBlockFace Face;
	EXPECT_TRUE(Box1.CalcLineIntersection(Start, End, LineCoeff, Face));
	EXPECT_NEAR(LineCoeff, 0.5, 0.00000001);
	EXPECT_EQ(Face, BLOCK_FACE_ZM);
}
