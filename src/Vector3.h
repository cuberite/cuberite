
#pragma once





template <typename T>
// tolua_begin
class Vector3
{

	TOLUA_TEMPLATE_BIND((T, int, float, double))

public:

	T x, y, z;


	constexpr Vector3(void) : x(0), y(0), z(0) {}
	constexpr Vector3(T a_x, T a_y, T a_z) : x(a_x), y(a_y), z(a_z) {}


	#ifdef TOLUA_EXPOSITION  // Hardcoded copy constructors (tolua++ does not support function templates .. yet)
		Vector3(const Vector3<float>  & a_Rhs);
		Vector3(const Vector3<double> & a_Rhs);
		Vector3(const Vector3<int>    & a_Rhs);
	#endif


	// tolua_end
	// Conversion constructors where U is not the same as T leaving the copy-constructor implicitly generated
	template <typename U, typename = typename std::enable_if<!std::is_same<U, T>::value>::type>
	constexpr Vector3(const Vector3<U> & a_Rhs):
			x(static_cast<T>(a_Rhs.x)),
			y(static_cast<T>(a_Rhs.y)),
			z(static_cast<T>(a_Rhs.z))
	{
	}
	// tolua_begin

	inline void Set(T a_x, T a_y, T a_z)
	{
		x = a_x;
		y = a_y;
		z = a_z;
	}

	inline void Normalize(void)
	{
		double Len = 1.0 / Length();

		x = static_cast<T>(x * Len);
		y = static_cast<T>(y * Len);
		z = static_cast<T>(z * Len);
	}

	inline Vector3<T> NormalizeCopy(void) const
	{
		double Len = 1.0 / Length();

		return Vector3<T>(
			static_cast<T>(x * Len),
			static_cast<T>(y * Len),
			static_cast<T>(z * Len)
		);
	}

	// tolua_end

	/** Sets the given vector to the normalized version of this vector.
	Removed from LuaAPI, because Lua doesn't need distinguishing from the other overload. */
	inline void NormalizeCopy(Vector3<T> & a_Rhs) const
	{
		double Len = 1.0 / Length();

		a_Rhs.Set(
			static_cast<T>(x * Len),
			static_cast<T>(y * Len),
			static_cast<T>(z * Len)
		);
	}

	// tolua_begin

	inline bool HasNonZeroLength(void) const
	{
		#ifdef __clang__
			#pragma clang diagnostic push
			#pragma clang diagnostic ignored "-Wfloat-equal"
		#endif

		return ((x != 0) || (y != 0) || (z != 0));

		#ifdef __clang__
			#pragma clang diagnostic pop
		#endif
	}

	inline double Length(void) const
	{
		return sqrt(static_cast<double>(x * x + y * y + z * z));
	}

	inline double SqrLength(void) const
	{
		return x * x + y * y + z * z;
	}

	inline T Dot(const Vector3<T> & a_Rhs) const
	{
		return x * a_Rhs.x + y * a_Rhs.y + z * a_Rhs.z;
	}

	/** Updates each coord to its absolute value */
	inline void Abs()
	{
		x = std::abs(x);
		y = std::abs(y);
		z = std::abs(z);
	}

	/** Clamps each coord into the specified range. */
	inline void Clamp(T a_Min, T a_Max)
	{
		x = ::Clamp(x, a_Min, a_Max);
		y = ::Clamp(y, a_Min, a_Max);
		z = ::Clamp(z, a_Min, a_Max);
	}

	inline Vector3<T> Cross(const Vector3<T> & a_Rhs) const
	{
		return Vector3<T>(
			y * a_Rhs.z - z * a_Rhs.y,
			z * a_Rhs.x - x * a_Rhs.z,
			x * a_Rhs.y - y * a_Rhs.x
		);
	}

	inline bool Equals(const Vector3<T> & a_Rhs) const
	{
		// Perform a strict comparison of the contents - we want to know whether this object is exactly equal
		// To perform EPS-based comparison, use the EqualsEps() function

		#ifdef __clang__
			#pragma clang diagnostic push
			#pragma clang diagnostic ignored "-Wfloat-equal"
		#endif

		return !((x != a_Rhs.x) || (y != a_Rhs.y) || (z != a_Rhs.z));

		#ifdef __clang__
			#pragma clang diagnostic pop
		#endif
	}

	inline bool EqualsEps(const Vector3<T> & a_Rhs, T a_Eps) const
	{
		return (std::abs(x - a_Rhs.x) < a_Eps) && (std::abs(y - a_Rhs.y) < a_Eps) && (std::abs(z - a_Rhs.z) < a_Eps);
	}

	inline void Move(T a_X, T a_Y, T a_Z)
	{
		x += a_X;
		y += a_Y;
		z += a_Z;
	}

	inline void Move(const Vector3<T> & a_Diff)
	{
		x += a_Diff.x;
		y += a_Diff.y;
		z += a_Diff.z;
	}

	/** Returns a new Vector3i with coords set to std::floor() of this vector's coords. */
	inline Vector3<int> Floor(void) const
	{
		return Vector3<int>(
			FloorC(x),
			FloorC(y),
			FloorC(z)
		);
	}

	/** Returns a new Vector3i with coords set to std::ceil() of this vector's coords. */
	inline Vector3<int> Ceil() const
	{
		return Vector3<int>(
			CeilC(x),
			CeilC(y),
			CeilC(z)
		);
	}

	// tolua_end

	inline bool operator != (const Vector3<T> & a_Rhs) const
	{
		return !Equals(a_Rhs);
	}

	inline bool operator == (const Vector3<T> & a_Rhs) const
	{
		return Equals(a_Rhs);
	}

	inline bool operator > (const Vector3<T> & a_Rhs) const
	{
		return (SqrLength() > a_Rhs.SqrLength());
	}

	inline bool operator < (const Vector3<T> & a_Rhs) const
	{
		return (SqrLength() < a_Rhs.SqrLength());
	}

	inline void operator += (const Vector3<T> & a_Rhs)
	{
		x += a_Rhs.x;
		y += a_Rhs.y;
		z += a_Rhs.z;
	}

	inline void operator -= (const Vector3<T> & a_Rhs)
	{
		x -= a_Rhs.x;
		y -= a_Rhs.y;
		z -= a_Rhs.z;
	}

	inline void operator *= (const Vector3<T> & a_Rhs)
	{
		x *= a_Rhs.x;
		y *= a_Rhs.y;
		z *= a_Rhs.z;
	}

	inline void operator *= (T a_v)
	{
		x *= a_v;
		y *= a_v;
		z *= a_v;
	}

	// tolua_begin

	inline Vector3<T> operator + (const Vector3<T>& a_Rhs) const
	{
		return Vector3<T>(
			x + a_Rhs.x,
			y + a_Rhs.y,
			z + a_Rhs.z
		);
	}

	inline Vector3<T> operator - (const Vector3<T>& a_Rhs) const
	{
		return Vector3<T>(
			x - a_Rhs.x,
			y - a_Rhs.y,
			z - a_Rhs.z
		);
	}

	inline Vector3<T> operator - (void) const
	{
		return Vector3<T>(-x, -y, -z);
	}

	inline Vector3<T> operator * (const Vector3<T>& a_Rhs) const
	{
		return Vector3<T>(
			x * a_Rhs.x,
			y * a_Rhs.y,
			z * a_Rhs.z
		);
	}

	inline Vector3<T> operator / (const Vector3<T> & a_Rhs)
	{
		return Vector3<T>(
			x / a_Rhs.x,
			y / a_Rhs.y,
			z / a_Rhs.z
		);
	}

	inline Vector3<T> operator * (T a_v) const
	{
		return Vector3<T>(
			x * a_v,
			y * a_v,
			z * a_v
		);
	}

	inline Vector3<T> operator / (T a_v) const
	{
		return Vector3<T>(
			x / a_v,
			y / a_v,
			z / a_v
		);
	}

	/** Returns a copy of this vector moved by the specified amount on the X axis. */
	inline Vector3<T> addedX(T a_AddX) const
	{
		return Vector3<T>(x + a_AddX, y, z);
	}

	/** Returns a copy of this vector moved by the specified amount on the y axis. */
	inline Vector3<T> addedY(T a_AddY) const
	{
		return Vector3<T>(x, y + a_AddY, z);
	}

	/** Returns a copy of this vector moved by the specified amount on the Z axis. */
	inline Vector3<T> addedZ(T a_AddZ) const
	{
		return Vector3<T>(x, y, z + a_AddZ);
	}

	/** Returns a copy of this vector moved by the specified amount on the X and Z axes. */
	inline Vector3<T> addedXZ(T a_AddX, T a_AddZ) const
	{
		return Vector3<T>(x + a_AddX, y, z + a_AddZ);
	}

	/** Returns the coefficient for the (a_OtherEnd - this) line to reach the specified Z coord.
	The result satisfies the following equation:
	(*this + Result * (a_OtherEnd - *this)).z = a_Z
	If the line is too close to being parallel, this function returns NO_INTERSECTION
	*/
	inline double LineCoeffToXYPlane(const Vector3<T> & a_OtherEnd, T a_Z) const
	{
		if (std::abs(z - a_OtherEnd.z) < EPS)
		{
			return NO_INTERSECTION;
		}

		return (a_Z - z) / (a_OtherEnd.z - z);
	}

	/** Returns the coefficient for the (a_OtherEnd - this) line to reach the specified Y coord.
	The result satisfies the following equation:
	(*this + Result * (a_OtherEnd - *this)).y = a_Y
	If the line is too close to being parallel, this function returns NO_INTERSECTION
	*/
	inline double LineCoeffToXZPlane(const Vector3<T> & a_OtherEnd, T a_Y) const
	{
		if (std::abs(y - a_OtherEnd.y) < EPS)
		{
			return NO_INTERSECTION;
		}

		return (a_Y - y) / (a_OtherEnd.y - y);
	}

	/** Returns the coefficient for the (a_OtherEnd - this) line to reach the specified X coord.
	The result satisfies the following equation:
	(*this + Result * (a_OtherEnd - *this)).x = a_X
	If the line is too close to being parallel, this function returns NO_INTERSECTION
	*/
	inline double LineCoeffToYZPlane(const Vector3<T> & a_OtherEnd, T a_X) const
	{
		if (std::abs(x - a_OtherEnd.x) < EPS)
		{
			return NO_INTERSECTION;
		}

		return (a_X - x) / (a_OtherEnd.x - x);
	}

	/** Rotates the vector 90 degrees clockwise around the vertical axis.
	Note that this is specific to minecraft's axis ordering, which is X+ left, Z+ down. */
	inline void TurnCW(void)
	{
		std::swap(x, z);
		x = -x;
	}

	/** Rotates the vector 90 degrees counterclockwise around the vertical axis.
	Note that this is specific to minecraft's axis ordering, which is X+ left, Z+ down. */
	inline void TurnCCW(void)
	{
		std::swap(x, z);
		z = -z;
	}

	/** The max difference between two coords for which the coords are assumed equal. */
	static const double EPS;

	/** Return value of LineCoeffToPlane() if the line is parallel to the plane. */
	static const double NO_INTERSECTION;
};
// tolua_end





/** Allows formatting a Vector<T> using the same format specifiers as for T
e.g. `fmt::format("{0:0.2f}", Vector3f{0.0231f, 1.2146f, 1.0f}) == "{0.02, 1.21, 1.00}"` */
template <typename What>
class fmt::formatter<Vector3<What>> : public fmt::formatter<What>
{
	using Super = fmt::formatter<What>;

	template <typename FormatContext, size_t Len>
	void Write(FormatContext & a_Ctx, const char (& a_Str)[Len])
	{
		const auto Itr = std::copy_n(&a_Str[0], Len - 1, a_Ctx.out());
		a_Ctx.advance_to(Itr);
	}

	template <typename FormatContext>
	void Write(FormatContext & a_Ctx, const What & a_Arg)
	{
		const auto Itr = Super::format(a_Arg, a_Ctx);
		a_Ctx.advance_to(Itr);
	}

public:

	template <typename FormatContext>
	auto format(const Vector3<What> & a_Vec, FormatContext & a_Ctx)
	{
		Write(a_Ctx, "{");
		Write(a_Ctx, a_Vec.x);
		Write(a_Ctx, ", ");
		Write(a_Ctx, a_Vec.y);
		Write(a_Ctx, ", ");
		Write(a_Ctx, a_Vec.z);
		Write(a_Ctx, "}");
		return a_Ctx.out();
	}
};





template <> inline Vector3<int> Vector3<int>::Floor(void) const
{
	return *this;
}





template <typename What>
class VectorHasher
{
public:
	/** Provides a hash of a vector's contents */
	size_t operator()(const Vector3<What> & a_Vector) const
	{
		// Guaranteed to have non repeating hashes for any 128x128x128 area
		size_t Hash = static_cast<size_t>(a_Vector.y);
		Hash <<= 16;
		Hash ^= static_cast<size_t>(a_Vector.x);
		Hash ^= static_cast<size_t>(a_Vector.z) << 8;
		return Hash;
	}
};





template <typename T>
const double Vector3<T>::EPS = 0.000001;

template <typename T>
const double Vector3<T>::NO_INTERSECTION = 1e70;





// tolua_begin
typedef Vector3<double> Vector3d;
typedef Vector3<float>  Vector3f;
typedef Vector3<int>    Vector3i;
// tolua_end





typedef std::vector<Vector3i> cVector3iArray;
