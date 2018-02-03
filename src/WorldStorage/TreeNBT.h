
#pragma once

#include "NBTDef.h"

#include <unordered_map>

#if (!defined(_MSC_VER) || (_MSC_VER >= 1900))
	#define HAS_REFERENCE_QUALIFIERS 1
#else
	#define HAS_REFERENCE_QUALIFIERS 0
#endif

#if HAS_REFERENCE_QUALIFIERS
	#define LVALUE_REF_QUALIFIER &
#else
	#define LVALUE_REF_QUALIFIER
#endif

#pragma push_macro("new")
#undef new

namespace TreeNBT
{

// fwd:
class cCompound;
class cList;
class cTag;
template <typename T> class cArray;



namespace Detail
{

template <eTagType TagID> struct TypeFromTagId;
template <> struct TypeFromTagId<TAG_End>       { using type = void; };
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

template <bool Value, typename T>
using enable_if_t = typename std::enable_if<Value, T>::type;

template <typename T>
using decay_t = typename std::decay<T>::type;

// Reimplementation of std::aligned_union

template <size_t... Values> struct Maximum;

template <size_t N1, size_t N2, size_t... Tail>
struct Maximum<N1, N2, Tail...>:
	std::integral_constant<size_t,
		(N1 > N2) ? Maximum<N1, Tail...>::value : Maximum<N2, Tail...>::value
	>
{
};

template <size_t N>
struct Maximum<N>:
	std::integral_constant<size_t, N>
{
};

template<size_t MinLength, class... Ts>
struct aligned_union:
	std::aligned_storage<
		Maximum<MinLength, sizeof(Ts)...>::value,
		Maximum<std::alignment_of<Ts>::value...>::value
	>
{
};

}  // namespace Detail



/** cArray represents NBT's TAG_ByteArray and TAG_IntArray. */
template <typename T>
class cArray:
	public std::vector<T>
{
	static_assert(
		std::is_same<T, Int8>::value ||
		std::is_same<T, Int32>::value,
		"NBT array doesn't exist for this type, are you looking for cNBTList?"
	);
};





/** Type used when visiting an empty tag. */
class cEmptyTag {};


class cTag
{
	// Note that as TAG_End is abused as the representation of an empty tag
public:

	~cTag() { Destroy(); }

	cTag():                      m_TagId{TAG_End},       m_Payload(cEmptyTag{}) {}
	cTag(Int8 a_Value):          m_TagId{TAG_Byte},      m_Payload(a_Value) {}
	cTag(Int16 a_Value):         m_TagId{TAG_Short},     m_Payload(a_Value) {}
	cTag(Int32 a_Value):         m_TagId{TAG_Int},       m_Payload(a_Value) {}
	cTag(Int64 a_Value):         m_TagId{TAG_Long},      m_Payload(a_Value) {}
	cTag(float a_Value):         m_TagId{TAG_Float},     m_Payload(a_Value) {}
	cTag(double a_Value):        m_TagId{TAG_Double},    m_Payload(a_Value) {}
	cTag(AString a_Value):       m_TagId{TAG_String},    m_Payload(std::move(a_Value)) {}
	cTag(cArray<Int8> a_Value):  m_TagId{TAG_ByteArray}, m_Payload(std::move(a_Value)) {}
	cTag(cArray<Int32> a_Value): m_TagId{TAG_IntArray},  m_Payload(std::move(a_Value)) {}

	cTag(const char * a_String): cTag(AString(a_String)) {}
	cTag(cCompound a_Value);
	cTag(cList a_Value);

	cTag(const cTag & a_Copy);
	cTag(cTag && a_Move);
	cTag & operator = (const cTag & a_Copy);
	cTag & operator = (cTag && a_Move);

	template <typename F>
	void Visit(F && a_Visitor) const LVALUE_REF_QUALIFIER
	{
		switch (m_TagId)
		{
			case TAG_Byte:
			{
				a_Visitor(m_Payload.As<Int8>());
				return;
			}
			case TAG_Short:
			{
				a_Visitor(m_Payload.As<Int16>());
				return;
			}
			case TAG_Int:
			{
				a_Visitor(m_Payload.As<Int32>());
				return;
			}
			case TAG_Long:
			{
				a_Visitor(m_Payload.As<Int64>());
				return;
			}
			case TAG_Float:
			{
				a_Visitor(m_Payload.As<float>());
				return;
			}
			case TAG_Double:
			{
				a_Visitor(m_Payload.As<double>());
				return;
			}
			case TAG_ByteArray:
			{
				a_Visitor(m_Payload.As<cArray<Int8>>());
				return;
			}
			case TAG_String:
			{
				a_Visitor(m_Payload.As<AString>());
				return;
			}
			case TAG_List:
			{
				a_Visitor(*m_Payload.As<const cList *>());
				return;
			}
			case TAG_Compound:
			{
				a_Visitor(*m_Payload.As<const cCompound *>());
				return;
			}
			case TAG_IntArray:
			{
				a_Visitor(m_Payload.As<cArray<Int32>>());
				return;
			}
			case TAG_End:
			{
				const cEmptyTag Empty{};
				a_Visitor(Empty);
				return;
			}
		}
	}

	template <typename F>
	void Visit(F && a_Visitor) LVALUE_REF_QUALIFIER
	{
		switch (m_TagId)
		{
			case TAG_Byte:
			{
				a_Visitor(m_Payload.As<Int8>());
				return;
			}
			case TAG_Short:
			{
				a_Visitor(m_Payload.As<Int16>());
				return;
			}
			case TAG_Int:
			{
				a_Visitor(m_Payload.As<Int32>());
				return;
			}
			case TAG_Long:
			{
				a_Visitor(m_Payload.As<Int64>());
				return;
			}
			case TAG_Float:
			{
				a_Visitor(m_Payload.As<float>());
				return;
			}
			case TAG_Double:
			{
				a_Visitor(m_Payload.As<double>());
				return;
			}
			case TAG_ByteArray:
			{
				a_Visitor(m_Payload.As<cArray<Int8>>());
				return;
			}
			case TAG_String:
			{
				a_Visitor(m_Payload.As<AString>());
				return;
			}
			case TAG_List:
			{
				a_Visitor(*m_Payload.As<cList *>());
				return;
			}
			case TAG_Compound:
			{
				a_Visitor(*m_Payload.As<cCompound *>());
				return;
			}
			case TAG_IntArray:
			{
				a_Visitor(m_Payload.As<cArray<Int32>>());
				return;
			}
			case TAG_End:
			{
				cEmptyTag Empty{};
				a_Visitor(Empty);
				return;
			}
		}
	}

	#if HAS_REFERENCE_QUALIFIERS
		template <typename F>
		void Visit(F && a_Visitor) &&
		{
			switch (m_TagId)
			{
				case TAG_Byte:
				{
					a_Visitor(std::move(m_Payload.As<Int8>()));
					return;
				}
				case TAG_Short:
				{
					a_Visitor(std::move(m_Payload.As<Int16>()));
					return;
				}
				case TAG_Int:
				{
					a_Visitor(std::move(m_Payload.As<Int32>()));
					return;
				}
				case TAG_Long:
				{
					a_Visitor(std::move(m_Payload.As<Int64>()));
					return;
				}
				case TAG_Float:
				{
					a_Visitor(std::move(m_Payload.As<float>()));
					return;
				}
				case TAG_Double:
				{
					a_Visitor(std::move(m_Payload.As<double>()));
					return;
				}
				case TAG_ByteArray:
				{
					a_Visitor(std::move(m_Payload.As<cArray<Int8>>()));
					return;
				}
				case TAG_String:
				{
					a_Visitor(std::move(m_Payload.As<AString>()));
					return;
				}
				case TAG_List:
				{
					a_Visitor(std::move(*m_Payload.As<cList *>()));
					return;
				}
				case TAG_Compound:
				{
					a_Visitor(std::move(*m_Payload.As<cCompound *>()));
					return;
				}
				case TAG_IntArray:
				{
					a_Visitor(std::move(m_Payload.As<cArray<Int32>>()));
					return;
				}
				case TAG_End:
				{
					a_Visitor(cEmptyTag{});
					return;
				}
			}
		}
	#endif  // HAS_REFERENCE_QUALIFIERS

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
		sValueGetter<const ValueType> Getter;
		Visit(Getter);
		return Getter.m_Result;
	}

	template <eTagType TagId>
	auto GetAs()
		-> typename Detail::TypeFromTagId<TagId>::type *
	{
		static_assert(TagId != TAG_End, "Trying to GetAs invalid tag type");
		using ValueType = typename Detail::TypeFromTagId<TagId>::type;
		sValueGetter<ValueType> Getter;
		Visit(Getter);
		return Getter.m_Result;
	}

	bool IsEmpty() const
	{
		return (m_TagId == TAG_End);
	}


private:

	/** Union that holds the actual tag value. */
	union uPayload
	{
		using Storage = Detail::aligned_union<1,
			Int8,          // TAG_Byte
			Int16,         // TAG_Short
			Int32,         // TAG_Int
			Int64,         // TAG_Long
			float,         // TAG_Float
			double,        // TAG_Double
			AString,       // TAG_String
			cCompound *,   // TAG_Compound
			cList *,       // TAG_List
			cArray<Int8>,  // TAG_ByteArray
			cArray<Int32>  // TAG_IntArray
		>::type;

		Storage m_Storage;

		template <typename T>
		T & As() LVALUE_REF_QUALIFIER
		{
			return reinterpret_cast<T &>(m_Storage);
		}

		template <typename T>
		const T & As() const LVALUE_REF_QUALIFIER
		{
			return reinterpret_cast<const T &>(m_Storage);
		}

		#if HAS_REFERENCE_QUALIFIERS
			template <typename T>
			T && As() &&
			{
				using RValue = T && ;
				return reinterpret_cast<RValue>(m_Storage);
			}
		#endif  // HAS_REFERENCE_QUALIFIERS


		template <typename T>
		uPayload(T && a_Value)
		{
			Construct(std::forward<T>(a_Value));
		}

		// Even trivial constructors are given functions so
		// that templates can rely on overloading.

		void Construct(cEmptyTag) {}

		void Construct(Int8   a_Byte)   { new(&m_Storage) Int8{a_Byte};     }
		void Construct(Int16  a_Short)  { new(&m_Storage) Int16{a_Short};   }
		void Construct(Int32  a_Int)    { new(&m_Storage) Int32{a_Int};     }
		void Construct(Int64  a_Long)   { new(&m_Storage) Int64{a_Long};    }
		void Construct(float  a_Float)  { new(&m_Storage) float{a_Float};   }
		void Construct(double a_Double) { new(&m_Storage) double{a_Double}; }

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

		void Assign(cEmptyTag) {}
		void Assign(Int8   a_Byte)   { As<Int8>()   = a_Byte;   }
		void Assign(Int16  a_Short)  { As<Int16>()  = a_Short;  }
		void Assign(Int32  a_Int)    { As<Int32>()  = a_Int;    }
		void Assign(Int64  a_Long)   { As<Int64>()  = a_Long;   }
		void Assign(float  a_Float)  { As<float>()  = a_Float;  }
		void Assign(double a_Double) { As<double>() = a_Double; }

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

	/** Implementation of GetAs. */
	template <typename ValueType>
	struct sValueGetter
	{
		ValueType * m_Result = nullptr;

		// Called for values matching the tag id
		void operator () (ValueType & a_Value)
		{
			m_Result = &a_Value;
		}

		// Called for non-matches
		template <typename T> void operator () (const T &) {}
	};


	// TAG_End is used to signal the valueless state
	eTagType m_TagId;
	uPayload m_Payload;

	void Destroy();
};


/** cList represents NBT's TAG_List.
A list tag is a dynamic array of tags that are all of the same type.
To maintain this property, the individual elements can't be modified directly.
Instead, elements must be modified via the `Visit` function. */
class cList
{
	template <typename Tag, typename Result = void>
	using IsTag = Detail::enable_if_t<
		std::is_same<Detail::decay_t<Tag>, cTag>::value, Result>;
public:
	using value_type             = std::vector<cTag>::value_type;
	using size_type              = std::vector<cTag>::size_type;
	using difference_type        = std::vector<cTag>::difference_type;

	// No modifiable references can be handed out in order to maintain
	// the invariant that all tags are of the same type
	using reference              = std::vector<cTag>::const_reference;
	using const_reference        = std::vector<cTag>::const_reference;
	using pointer                = std::vector<cTag>::const_pointer;
	using const_pointer          = std::vector<cTag>::const_pointer;
	using iterator               = std::vector<cTag>::const_iterator;
	using const_iterator         = std::vector<cTag>::const_iterator;

	cList(eTagType a_ListType):
		m_TypeId(a_ListType)
	{
		VERIFY(a_ListType != TAG_End);
	}

	/** Try to insert a new tag at the given position.
	Returns false if the tag is of the wrong type and cannot be inserted. */
	template <typename Tag, typename = IsTag<Tag>>
	std::pair<iterator, bool> TryInsert(iterator a_Pos, Tag && a_Tag)
	{
		if (a_Tag.TypeId() == m_TypeId)
		{
			return {m_Tags.insert(std::forward<Tag>(a_Tag)), true};
		}
		else
		{
			return {a_Pos, false};
		}
	}

	/** Try to insert a new tag at the end of the list.
	Returns false if the tag is of the wrong type and cannot be inserted. */
	template <typename Tag, typename = IsTag<Tag>>
	bool TryPushBack(Tag && a_Tag)
	{
		if (a_Tag.TypeId() == m_TypeId)
		{
			m_Tags.push_back(std::forward<Tag>(a_Tag));
			return true;
		}
		else
		{
			return false;
		}
	}

	/** Visit the element of the list at the given position. */
	template <typename Func>
	void Visit(iterator a_Pos, Func a_Visitor) LVALUE_REF_QUALIFIER
	{
		auto Idx = std::distance(begin(), a_Pos);
		m_Tags[Idx].Visit(std::move(a_Visitor));
	}

	#if HAS_REFERENCE_QUALIFIERS
		/** Visit the element of the list at the given position. */
		template <typename Func>
		void Visit(iterator a_Pos, Func a_Visitor) &&
		{
			auto Idx = std::distance(begin(), a_Pos);
			std::move(m_Tags[Idx]).Visit(std::move(a_Visitor));
		}
	#endif

	/** Visit the element of the list at the given position. */
	template <typename Func>
	void Visit(iterator a_Pos, Func a_Visitor) const LVALUE_REF_QUALIFIER
	{
		auto Idx = std::distance(begin(), a_Pos);
		m_Tags[Idx].Visit(std::move(a_Visitor));
	}

	/** Clear the list and rebind to store a new tag type. */
	void Reset(eTagType a_NewListType)
	{
		clear();
		m_TypeId = a_NewListType;
		VERIFY(a_NewListType != TAG_End);
	}

	// Standard STL style interface

	const cTag & operator [] (size_type a_Idx) const
	{
		return m_Tags[a_Idx];
	}

	iterator cbegin() const { return m_Tags.cbegin(); }
	iterator begin() const { return m_Tags.cbegin(); }

	iterator cend() const { return m_Tags.cend(); }
	iterator end() const { return m_Tags.cend(); }


	iterator erase(iterator a_Pos)
	{
		auto Idx = std::distance(m_Tags.cbegin(), a_Pos);
		return m_Tags.erase(m_Tags.begin() + Idx);
	}
	iterator erase(iterator a_First, iterator a_Last)
	{
		auto Begin = m_Tags.begin();
		auto CBegin = m_Tags.cbegin();
		auto FirstIdx = std::distance(CBegin, a_First);
		auto LastIdx = std::distance(CBegin, a_Last);
		return m_Tags.erase(Begin + FirstIdx, Begin + LastIdx);
	}

	size_type size() const { return m_Tags.size(); }

	void resize(size_type a_NewSize);

	void clear() { m_Tags.clear(); }

	void swap(cList & a_Other) NOEXCEPT
	{
		std::swap(m_TypeId, a_Other.m_TypeId);
		std::swap(m_Tags, a_Other.m_Tags);
	}

	friend void swap(cList & a_Lhs, cList & a_Rhs) NOEXCEPT
	{
		a_Lhs.swap(a_Rhs);
	}

private:
	eTagType m_TypeId;
	std::vector<cTag> m_Tags;
};





/** cCompound represents NBT's TAG_Compound.
A compound tag is simply an associative container of name-tag pairs.
As such it provides most of the assiciative container interface. */
class cCompound
{
	using cTagMap = std::unordered_map<AString, cTag>;
public:

	using key_type        = cTagMap::key_type;
	using mapped_type     = cTagMap::mapped_type;
	using value_type      = cTagMap::value_type;
	using size_type       = cTagMap::size_type;
	using difference_type = cTagMap::difference_type;
	using pointer         = cTagMap::pointer;
	using const_pointer   = cTagMap::const_pointer;
	using reference       = cTagMap::reference;
	using const_reference = cTagMap::const_reference;
	using iterator        = cTagMap::iterator;
	using const_iterator  = cTagMap::const_iterator;

	cCompound() = default;
	cCompound(std::initializer_list<value_type> a_Init):
		m_Tags(a_Init)
	{
	}

	cTag & operator [] (const AString & a_Name)
	{
		return m_Tags[a_Name];
	}

	iterator begin() { return m_Tags.begin(); }
	const_iterator cbegin() const { return m_Tags.cbegin(); }
	const_iterator begin() const { return m_Tags.cbegin(); }

	iterator end() { return m_Tags.end(); }
	const_iterator cend() const { return m_Tags.cend(); }
	const_iterator end() const { return m_Tags.cend(); }

	bool empty() const { return m_Tags.empty(); }
	size_type size() const { return m_Tags.size(); }
	void clear() { m_Tags.clear(); }

	std::pair<iterator, bool> insert(value_type && a_Value)
	{
		return m_Tags.insert(std::move(a_Value));
	}

	std::pair<iterator, bool> insert(const value_type & a_Value)
	{
		return m_Tags.insert(a_Value);
	}

	size_type erase(const AString & a_Name) { return m_Tags.erase(a_Name); }
	iterator erase(iterator a_Pos) { return m_Tags.erase(a_Pos); }
	iterator erase(iterator a_First, iterator a_Last)
	{
		return m_Tags.erase(a_First, a_Last);
	}

	void swap(cCompound & a_Other) NOEXCEPT
	{
		m_Tags.swap(a_Other.m_Tags);
	}

	friend void swap(cCompound & a_Lhs, cCompound & a_Rhs) NOEXCEPT
	{
		a_Lhs.swap(a_Rhs);
	}

	iterator find(const AString & a_Name) { return m_Tags.find(a_Name); }
	const_iterator find(const AString & a_Name) const { return m_Tags.find(a_Name); }

private:
	cTagMap m_Tags;
};



////////////////////////////////////////////////////////////////////////////////
// TreeNBT::cTag:

inline cTag::cTag(cCompound a_Value): m_TagId{TAG_Compound}, m_Payload(std::move(a_Value)) {}
inline cTag::cTag(cList a_Value):     m_TagId{TAG_List},     m_Payload(std::move(a_Value)) {}


}  // namespace TreeNBT

#pragma pop_macro("new")
