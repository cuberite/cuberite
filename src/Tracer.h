
#pragma once





// fwd:
class cWorld;





// tolua_begin

class cTracer
{
public:

	/** Contains the position of the block that caused the collision */
	Vector3f BlockHitPosition;

	/** Contains which face was hit */
	Vector3f HitNormal;

	/** Contains the exact position where a collision occured. (BlockHitPosition + Offset on block) */
	Vector3f RealHit;


	cTracer(cWorld * a_World);
	~cTracer();

	// tolua_end

	/** Determines if a collision occures along a line. Returns true if a collision occurs.
	Exported manually to add deprecation warnings. */
	bool Trace(const Vector3f & a_Start, const Vector3f & a_Direction, int a_Distance)
	{
		return Trace(a_Start, a_Direction, a_Distance, false);
	}

	/** Determines if a collision occures along a line. Returns true if a collision occurs.
	When a_LineOfSight is true, we don't use the standard collision detection rules. Instead we use
	the rules for monster vision. E.g. Only water and air do not block vision.
	a_Distance is the number of iterations (blocks hits) that are tested.
	Exported manually to add deprecation warnings. */
	bool Trace(const Vector3f & a_Start, const Vector3f & a_Direction, int a_Distance, bool a_LineOfSight);

	// tolua_begin

private:

	/** Preps Tracer object for call of Trace function. Only used internally. */
	void SetValues(const Vector3f & a_Start, const Vector3f & a_Direction);

	/** Calculates where on the block a collision occured, if it does occur
	Returns 0 if no intersection occured
	Returns 1 if an intersection occured at a single point
	Returns 2 if the line segment lies in the plane being checked */
	int intersect3D_SegmentPlane(const Vector3f & a_Origin, const Vector3f & a_End, const Vector3f & a_PlanePos, const Vector3f & a_PlaneNormal);

	/** Determines which face on the block a collision occured, if it does occur
	Returns 0 if the block is air, water or no collision occured
	Return 1 through 6 for the following block faces, repectively: -x, -z, x, z, y, -y */
	int GetHitNormal(const Vector3f & start, const Vector3f & end, const Vector3i &  a_BlockPos);

	/** Signum function */
	int SigNum(float a_Num);

	cWorld * m_World;

	static const std::array<const Vector3f, 6> & m_NormalTable(void);

	Vector3f dir;
	Vector3f tDelta;
	Vector3i pos;
	Vector3i end1;
	Vector3i step;
	Vector3f tMax;
};

// tolua_end




