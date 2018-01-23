
#pragma once

#include "NBTDef.h"

namespace TreeNBT
{


template <typename T>
class cArray
{
	static_assert(
		std::is_same<T, Int8>::value ||
		std::is_same<T, Int32>::value,
		"NBT array doesn't exist for this type, are you looking for cNBTList?"
	);
public:



private:
	std::vector<T> m_Elements;
};



class cList
{
public:
	cList(eTagType a_ListType);
	cList(const cList & a_CopyFrom);
	cList(cList && a_MoveFrom);
	cList & operator = (const cList & a_CopyFrom);
	cList & operator = (cList && a_MoveFrom) NOEXCEPT;

	void swap(cList & a_Other) NOEXCEPT
	{
		std::swap(m_Impl, a_Other.m_Impl);
	}

	friend void swap(cList & a_Lhs, cList & a_Rhs) NOEXCEPT
	{
		a_Lhs.swap(a_Rhs);
	}

private:
	// Pimpl needed because of circlular dependence with cTag
	struct sImpl;
	std::unique_ptr<sImpl> m_Impl;

	sImpl & Impl() { return *m_Impl; }
	const sImpl & Impl() const { return *m_Impl; }
};



class cCompound
{
public:
	cCompound();
	cCompound(const cCompound & a_CopyFrom);
	cCompound(cCompound && a_MoveFrom);
	cCompound & operator = (const cCompound & a_CopyFrom);
	cCompound & operator = (cCompound && a_MoveFrom) NOEXCEPT;

	void swap(cCompound & a_Other) NOEXCEPT
	{
		std::swap(m_Impl, a_Other.m_Impl);
	}

	friend void swap(cCompound & a_Lhs, cCompound & a_Rhs) NOEXCEPT
	{
		a_Lhs.swap(a_Rhs);
	}

private:
	// Pimpl needed because of circlular dependence with cTag
	struct sImpl;
	std::unique_ptr<sImpl> m_Impl;

	sImpl & Impl() { return *m_Impl; }
	const sImpl & Impl() const { return *m_Impl; }
};



namespace Detail
{

template <eTagType TagID> struct TypeFromTagId;
template <> struct TypeFromTagId<TAG_Byte>      { using type = Int8; };
template <> struct TypeFromTagId<TAG_Short>     { using type = Int16; };
template <> struct TypeFromTagId<TAG_Int>       { using type = Int32; };
template <> struct TypeFromTagId<TAG_Long>      { using type = Int64; };
template <> struct TypeFromTagId<TAG_Float>     { using type = float; };
template <> struct TypeFromTagId<TAG_Double>    { using type = double; };
template <> struct TypeFromTagId<TAG_String>    { using type = AString; };
template <> struct TypeFromTagId<TAG_List>      { using type = cList; };
template <> struct TypeFromTagId<TAG_Compound>  { using type = cCompound; };
template <> struct TypeFromTagId<TAG_ByteArray> { using type = cArray<Int8>; };
template <> struct TypeFromTagId<TAG_IntArray>  { using type = cArray<Int32>; };

}  // namespace Detail



class cTag
{
public:

	~cTag() { Destroy(); }

	cTag(Int8 a_Value):          m_TagId{TAG_Byte},      m_Payload(a_Value) {}
	cTag(Int16 a_Value):         m_TagId{TAG_Short},     m_Payload(a_Value) {}
	cTag(Int32 a_Value):         m_TagId{TAG_Int},       m_Payload(a_Value) {}
	cTag(Int64 a_Value):         m_TagId{TAG_Long},      m_Payload(a_Value) {}
	cTag(float a_Value):         m_TagId{TAG_Float},     m_Payload(a_Value) {}
	cTag(double a_Value):        m_TagId{TAG_Double},    m_Payload(a_Value) {}
	cTag(AString a_Value):       m_TagId{TAG_String},    m_Payload(std::move(a_Value)) {}
	cTag(cCompound a_Value):     m_TagId{TAG_Compound},  m_Payload(std::move(a_Value)) {}
	cTag(cList a_Value):         m_TagId{TAG_List},      m_Payload(std::move(a_Value)) {}
	cTag(cArray<Int8> a_Value):  m_TagId{TAG_ByteArray}, m_Payload(std::move(a_Value)) {}
	cTag(cArray<Int32> a_Value): m_TagId{TAG_IntArray},  m_Payload(std::move(a_Value)) {}

	cTag(const cTag & a_Copy);
	cTag(cTag && a_Move);
	cTag & operator = (const cTag & a_Copy);
	cTag & operator = (cTag && a_Move);

	template <typename F>
	void Visit(F && a_Visitor) const &
	{
		switch (m_TagId)
		{
			case TAG_Byte:
			{
				a_Visitor(m_Payload.Byte);
				return;
			}
			case TAG_Short:
			{
				a_Visitor(m_Payload.Short);
				return;
			}
			case TAG_Int:
			{
				a_Visitor(m_Payload.Int);
				return;
			}
			case TAG_Long:
			{
				a_Visitor(m_Payload.Long);
				return;
			}
			case TAG_Float:
			{
				a_Visitor(m_Payload.Float);
				return;
			}
			case TAG_Double:
			{
				a_Visitor(m_Payload.Double);
				return;
			}
			case TAG_ByteArray:
			{
				a_Visitor(m_Payload.ByteArray);
				return;
			}
			case TAG_String:
			{
				a_Visitor(m_Payload.String);
				return;
			}
			case TAG_List:
			{
				a_Visitor(m_Payload.List);
				return;
			}
			case TAG_Compound:
			{
				a_Visitor(m_Payload.Compound);
				return;
			}
			case TAG_IntArray:
			{
				a_Visitor(m_Payload.IntArray);
				return;
			}


			case TAG_End:
			{
				FLOGERROR("Tree NBT tag in invalid state");
				std::terminate();
			}
		}
	}

	template <typename F>
	void Visit(F && a_Visitor) &
	{
		const auto * CThis = this;
		CThis->Visit(LValueVisitWrapper<F>{std::forward<F>(a_Visitor)});
	}

	template <typename F>
	void Visit(F && a_Visitor) &&
	{
		const auto * CThis = this;
		CThis->Visit(RValueVisitWrapper<F>{std::forward<F>(a_Visitor)});
	}

	eTagType TypeId() const
	{
		return m_TagId;
	}

	template <eTagType TagId>
	auto GetAs() const
		-> const typename Detail::TypeFromTagId<TagId>::type *
	{
		static_assert(TagId != TAG_End, "Trying to GetAs invalid tag type");
		using ValueType = typename Detail::TypeFromTagId<TagId>::type;
		ValueGetter<ValueType> Getter;
		Visit(Getter);
		return Getter.m_Result;
	}

	template <eTagType TagId>
	auto GetAs()
		-> typename Detail::TypeFromTagId<TagId>::type *
	{
		using ValueType = typename Detail::TypeFromTagId<TagId>::type;
		const auto * CThis = this;
		return const_cast<ValueType *>(CThis->GetAs<TagId>());
	}


private:
	union uPayload
	{
		Int8          Byte;
		Int16         Short;
		Int32         Int;
		Int64         Long;
		float         Float;
		double        Double;
		AString       String;
		cCompound     Compound;
		cList         List;
		cArray<Int8>  ByteArray;
		cArray<Int32> IntArray;

		template <typename T>
		uPayload(T && a_Value)
		{
			Construct(std::forward<T>(a_Value));
		}

		// Even trivial constructors are given functions so
		// then templates can rely on overloading.

		void Construct(Int8   a_Byte)   { Byte   = a_Byte;   }
		void Construct(Int16  a_Short)  { Short  = a_Short;  }
		void Construct(Int32  a_Int)    { Int    = a_Int;    }
		void Construct(Int64  a_Long)   { Long   = a_Long;   }
		void Construct(float  a_Float)  { Float  = a_Float;  }
		void Construct(double a_Double) { Double = a_Double; }

		void Construct(const AString & a_String);
		void Construct(const cCompound & a_Compound);
		void Construct(const cList & a_List);
		void Construct(const cArray<Int8> & a_ByteArray);
		void Construct(const cArray<Int32> & a_IntArray);

		void Construct(AString && a_String);
		void Construct(cCompound && a_Compound);
		void Construct(cList && a_List);
		void Construct(cArray<Int8> && a_ByteArray);
		void Construct(cArray<Int32> && a_IntArray);

		void Assign(Int8   a_Byte)   { Byte   = a_Byte;   }
		void Assign(Int16  a_Short)  { Short  = a_Short;  }
		void Assign(Int32  a_Int)    { Int    = a_Int;    }
		void Assign(Int64  a_Long)   { Long   = a_Long;   }
		void Assign(float  a_Float)  { Float  = a_Float;  }
		void Assign(double a_Double) { Double = a_Double; }

		void Assign(AString && a_String);
		void Assign(cCompound && a_Compound);
		void Assign(cList && a_List);
		void Assign(cArray<Int8> && a_ByteArray);
		void Assign(cArray<Int32> && a_IntArray);

		void Assign(const AString & a_String);
		void Assign(const cCompound & a_Compound);
		void Assign(const cList & a_List);
		void Assign(const cArray<Int8> & a_ByteArray);
		void Assign(const cArray<Int32> & a_IntArray);

		~uPayload() {}  // Members are destoyed by cTag

	};


	/** Visitors to construct the payload from a value. */
	struct sConstructor
	{
		uPayload & Payload;
		template <typename T>
		void operator () (T && a_Value)
		{
			Payload.Construct(std::forward<T>(a_Value));
		}
	};

	/** Visitors to assign to the payload from a value. */
	struct sAssigner
	{
		uPayload & Payload;
		template <typename T>
		void operator () (T && a_Value)
		{
			Payload.Assign(std::forward<T>(a_Value));
		}
	};


	/** Visitor wrappers used to implement Visit. */
	template <typename F>
	struct LValueVisitWrapper
	{
		F && Visitor;
		template <typename T>
		void operator () (T & a_Value)
		{
			using NCT = typename std::remove_const<T>::type;
			auto && NCRef = const_cast<NCT &>(a_Value);
			Visitor(NCRef);
		}
	};

	template <typename F>
	struct RValueVisitWrapper
	{
		F && Visitor;
		template <typename T>
		void operator () (T & a_Value)
		{
			using NCT = typename std::remove_const<T>::type;
			auto && NCRef = const_cast<NCT &&>(a_Value);
			Visitor(NCRef);
		}
	};

	/** Implementation of GetAs. */
	template <typename ValueType>
	struct ValueGetter
	{
		const ValueType * m_Result = nullptr;

		// Called for values matching the tag id
		void operator () (const ValueType & a_Value)
		{
			m_Result = &a_Value;
		}

		// Called for non-matches
		template <typename T> void operator () (const T &) {}
	};

	eTagType m_TagId;
	uPayload m_Payload;

	void Destroy();
};

}  // namespace TreeNBT
