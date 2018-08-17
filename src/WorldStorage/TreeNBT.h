
#pragma once

/** Declares classes to represent the full NBT tree structure dynamically.

For (de)serialization purposes, cFastNBT and cParsedNBT are much more efficient
but these don't allow both reading and modifying NBT at the same time.

The structure consists of 3 main classes:

	* `cCompound` is a map from string to NBT tag (`cTag`) that gives the tag
	its name. As in the file format, all uses of TreeNBT begin with a
	`cCompound` at top level.

	* `cTag` is a tagged union of all NBT tag types, including `cCompound` which
	may contain more `cTag`s recursively.

	* `cList` holds multiple tags of the same type, without associating them
	with a name. There are no restrictions as to what tags a list can hold.

There is also the `cByteArray` and `cIntArray` which correspond to the similarly
named NBT types (TAG_ByteArray and TAG_IntArray). These are simply aliases to
the appropriate vector type.
*/

#include "NBTDef.h"

#pragma push_macro("new")
#undef new

namespace TreeNBT
{

// fwd:
class cList;
class cTag;

/** cCompound represents NBT's TAG_Compound.
A compound tag is just a map that associates a tag with a name. */
using cCompound = std::map<AString, cTag>;

using cByteArray = std::vector<Int8>;
using cIntArray = std::vector<Int32>;

/** Type contained by an empty cTag. */
class cEmptyTag {};

/** Calls the given macro with each TAG name and the associated type */
#define FOR_EACH_NBT_TAG(MACRO_NAME) \
	MACRO_NAME(TAG_End,       cEmptyTag) \
	MACRO_NAME(TAG_Byte,      Int8) \
	MACRO_NAME(TAG_Short,     Int16) \
	MACRO_NAME(TAG_Int,       Int32) \
	MACRO_NAME(TAG_Long,      Int64) \
	MACRO_NAME(TAG_Float,     float) \
	MACRO_NAME(TAG_Double,    double) \
	MACRO_NAME(TAG_String,    AString) \
	MACRO_NAME(TAG_List,      cList) \
	MACRO_NAME(TAG_Compound,  cCompound) \
	MACRO_NAME(TAG_ByteArray, cByteArray) \
	MACRO_NAME(TAG_IntArray,  cIntArray)


namespace Detail
{

template <eTagType TagID> struct sTypeFromTagId  {};

#define CREATE_TYPE_MAPPING(TAG_ID, TYPE) \
	template <> struct sTypeFromTagId<TAG_ID> { using type = TYPE; };
FOR_EACH_NBT_TAG(CREATE_TYPE_MAPPING)
#undef CREATE_TYPE_MAPPING

template <eTagType TagId>
using TypeFromTagId = typename sTypeFromTagId<TagId>::type;


// Reimplementation of std::aligned_union

template <size_t... Values> struct Maximum;

template <size_t N1, size_t N2, size_t... Tail>
struct Maximum<N1, N2, Tail...>:
	Maximum<(N1 > N2) ? N1 : N2, Tail...>
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

using TagStorage = aligned_union<1,
		cEmptyTag,     // TAG_End
		Int8,          // TAG_Byte
		Int16,         // TAG_Short
		Int32,         // TAG_Int
		Int64,         // TAG_Long
		float,         // TAG_Float
		double,        // TAG_Double
		AString,       // TAG_String
		// cCompound and cList must be pointers as they are incomplete types
		cCompound *,   // TAG_Compound
		cList *,       // TAG_List
		cByteArray,    // TAG_ByteArray
		cIntArray      // TAG_IntArray
	>::type;

/** Get a member from cTag::cPayload storage */
template <typename T>
struct PayloadGetter
{
	static_assert(std::is_same<typename std::decay<T>::type, T>::value, "Getter should not be cv-qualified");
	static T & Get(TagStorage & a_TagStorage)
	{
		return reinterpret_cast<T &>(a_TagStorage);
	}

	static const T & Get(const TagStorage & a_TagStorage)
	{
		return reinterpret_cast<const T &>(a_TagStorage);
	}
};

template <>
struct PayloadGetter<cCompound>
{
	static cCompound & Get(TagStorage & a_TagStorage)
	{
		return *reinterpret_cast<cCompound *&>(a_TagStorage);
	}

	static const cCompound & Get(const TagStorage & a_TagStorage)
	{
		using PtrToConst = const cCompound *;
		return *reinterpret_cast<const PtrToConst &>(a_TagStorage);
	}
};

template <>
struct PayloadGetter<cList>
{
	static cList & Get(TagStorage & a_TagStorage)
	{
		return *reinterpret_cast<cList *&>(a_TagStorage);
	}

	static const cList & Get(const TagStorage & a_TagStorage)
	{
		using PtrToConst = const cList *;
		return *reinterpret_cast<const PtrToConst &>(a_TagStorage);
	}
};

}  // namespace Detail





class cTag
{
	// Note that as TAG_End is used to represent a defaulted tag
public:

	~cTag() { Destroy(); }

	#define DECLARE_CONSTRUCTOR(TAG_ID, TYPE) \
		cTag(TYPE a_Value);
	FOR_EACH_NBT_TAG(DECLARE_CONSTRUCTOR)
	#undef DECLARE_CONSTRUCTOR

	cTag(): cTag(cEmptyTag{}) {}
	cTag(const char * a_String): cTag(AString(a_String)) {}

	cTag(const cTag & a_Copy);
	cTag(cTag && a_Move);
	cTag & operator = (const cTag & a_Copy);
	cTag & operator = (cTag && a_Move);

	template <typename F>
	void Visit(F && a_Visitor) const
	{
		switch (m_TagId)
		{
			#define VISIT_TAG(TAG_ID, TYPE) \
				case TAG_ID: a_Visitor(m_Payload.As<TYPE>()); return;
			FOR_EACH_NBT_TAG(VISIT_TAG)
			#undef VISIT_TAG
		}
	}

	template <typename F>
	void Visit(F && a_Visitor)
	{
		switch (m_TagId)
		{
			#define VISIT_TAG(TAG_ID, TYPE) \
				case TAG_ID: a_Visitor(m_Payload.As<TYPE>()); return;
			FOR_EACH_NBT_TAG(VISIT_TAG)
			#undef VISIT_TAG
		}
	}


	eTagType TypeId() const
	{
		return m_TagId;
	}

	template <eTagType TagId>
	const Detail::TypeFromTagId<TagId> * GetAs() const
	{
		return (m_TagId == TagId) ? &m_Payload.As<Detail::TypeFromTagId<TagId>>() : nullptr;
	}

	template <eTagType TagId>
	Detail::TypeFromTagId<TagId> * GetAs()
	{
		return (m_TagId == TagId) ? &m_Payload.As<Detail::TypeFromTagId<TagId>>() : nullptr;
	}

	bool IsEmpty() const
	{
		return (m_TagId == TAG_End);
	}


private:

	/** Union that holds the actual tag value. */
	class cPayload
	{
	public:

		template <typename T>
		cPayload(T && a_Value)
		{
			Construct(std::forward<T>(a_Value));
		}

		template <typename T>
		void Construct(T a_Value) { new (&m_Storage) T(std::move(a_Value)); }

		void Construct(cCompound a_Compound);
		void Construct(cList a_List);

		template <typename T>
		T & As()
		{
			return Detail::PayloadGetter<T>::Get(m_Storage);
		}

		template <typename T>
		const T & As() const
		{
			return Detail::PayloadGetter<T>::Get(m_Storage);
		}

		~cPayload() {}  // Members are destoyed by cTag

	private:

		/** Storage for the contained value */
		Detail::TagStorage m_Storage;
	};

	/** Visitor that copy constructs a new payload from the value. */
	struct sCopyConstructor
	{
		cPayload & Payload;
		template <typename T>
		void operator () (const T & a_Value)
		{
			Payload.Construct(a_Value);
		}
	};

	/** Visitor that move construct a new payload from the value. */
	struct sMoveConstructor
	{
		cPayload & Payload;
		template <typename T>
		void operator () (T & a_Value)
		{
			Payload.Construct(std::move(a_Value));
		}
	};

	/** Visitor that copies the value into an existing payload */
	struct sCopyAssigner
	{
		cPayload & Payload;
		template <typename T>
		void operator () (const T & a_Value)
		{
			Payload.As<T>() = a_Value;
		}
	};

	/** Virtor that moves the value into an existing payload */
	struct sMoveAssigner
	{
		cPayload & Payload;
		template <typename T>
		void operator () (T & a_Value)
		{
			Payload.As<T>() = std::move(a_Value);
		}
	};

	// TAG_End is used to signal the valueless state
	eTagType m_TagId;
	cPayload m_Payload;

	void Destroy();
};


/** cList represents NBT's TAG_List.
A list tag is a dynamic array of tags that are all of the same type.
To maintain this property, the individual elements can't be modified directly.
Instead, elements must be modified via the `Visit` function. */
class cList
{
	using cTagArray = std::vector<cTag>;
public:
	using value_type      = cTagArray::value_type;
	using size_type       = cTagArray::size_type;
	using difference_type = cTagArray::difference_type;
	using const_reference = cTagArray::const_reference;
	using const_pointer   = cTagArray::const_pointer;
	using const_iterator  = cTagArray::const_iterator;

	// No modifiable references can be handed out in order to maintain
	// the invariant that all tags are of the same type
	using reference = const_reference;
	using pointer   = const_pointer;
	using iterator  = const_iterator;

	/** Try to insert a new tag at the given position.
	Returns false if the tag is of the wrong type and cannot be inserted. */
	std::pair<iterator, bool> TryInsert(iterator a_Pos, const cTag & a_Tag)
	{
		if (!IsTagCompatible(a_Tag.TypeId()))
		{
			return {a_Pos, false};
		}

		return {m_Tags.insert(MutableIterator(a_Pos), std::move(a_Tag)), true};
	}

	/** Try to insert a new tag at the given position.
	Returns false if the tag is of the wrong type and cannot be inserted. */
	std::pair<iterator, bool> TryInsert(iterator a_Pos, cTag && a_Tag)
	{
		if (!IsTagCompatible(a_Tag.TypeId()))
		{
			return {a_Pos, false};
		}

		return {m_Tags.insert(MutableIterator(a_Pos), std::move(a_Tag)), true};
	}

	/** Try to insert a new tag at the end of the list.
	Returns false if the tag is of the wrong type and cannot be inserted. */
	bool TryPushBack(const cTag & a_Tag)
	{
		if (!IsTagCompatible(a_Tag.TypeId()))
		{
			return false;
		}

		m_Tags.push_back(a_Tag);
		return true;
	}

	/** Try to insert a new tag at the end of the list.
	Returns false if the tag is of the wrong type and cannot be inserted. */
	bool TryPushBack(cTag && a_Tag)
	{
		if (!IsTagCompatible(a_Tag.TypeId()))
		{
			return false;
		}

		m_Tags.push_back(std::move(a_Tag));
		return true;
	}

	/** Visit the element of the list at the given position. */
	template <typename Func>
	void Visit(iterator a_Pos, Func && a_Visitor)
	{
		MutableIterator(a_Pos)->Visit(std::forward<Func>(a_Visitor));
	}

	/** Visit the element of the list at the given position. */
	template <typename Func>
	void Visit(const_iterator a_Pos, Func && a_Visitor) const
	{
		a_Pos->Visit(std::forward<Func>(a_Visitor));
	}

	/** Type of the current elements or TAG_End if empty. */
	eTagType TypeId() const
	{
		return m_Tags.empty() ? TAG_End : m_Tags[0].TypeId();
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
		return m_Tags.erase(MutableIterator(a_Pos));
	}

	iterator erase(iterator a_First, iterator a_Last)
	{
		return m_Tags.erase(MutableIterator(a_First), MutableIterator(a_Last));
	}

	size_type size() const { return m_Tags.size(); }
	bool empty() const { return m_Tags.empty(); }

	void reserve(size_type a_ReserveSize) { m_Tags.reserve(a_ReserveSize); }

	void clear() { m_Tags.clear(); }

	void swap(cList & a_Other) NOEXCEPT
	{
		m_Tags.swap(a_Other.m_Tags);
	}

	friend void swap(cList & a_Lhs, cList & a_Rhs) NOEXCEPT
	{
		a_Lhs.swap(a_Rhs);
	}

private:
	cTagArray m_Tags;

	/** Can the given tag type be added to the list? */
	bool IsTagCompatible(eTagType a_Type) const
	{
		return (
			// If there are elements already, they must be of the same type
			(m_Tags.empty() || (m_Tags[0].TypeId() == a_Type)) &&
			(a_Type != TAG_End)  // TAG_End isn't a valid element
		);
	}

	/** Make a mutable iterator from a const_iterator. */
	cTagArray::iterator MutableIterator(const_iterator a_Itr)
	{
		auto Idx = std::distance(cbegin(), a_Itr);
		return m_Tags.begin() + Idx;
	}
};





////////////////////////////////////////////////////////////////////////////////
// TreeNBT::cTag:

#define DEFINE_CONSTRUCTOR(TAG_ID, TYPE) \
	inline cTag::cTag(TYPE a_Value): m_TagId{TAG_ID}, m_Payload(std::move(a_Value)) {}
FOR_EACH_NBT_TAG(DEFINE_CONSTRUCTOR)
#undef DEFINE_CONSTRUCTOR

#undef FOR_EACH_NBT_TAG

}  // namespace TreeNBT

#pragma pop_macro("new")
