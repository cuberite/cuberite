#pragma once

// contexpr copies of std:: algorithms. Remove once we're using C++20.
namespace ConstexprAlgorithms
{
template <typename InIt, typename OutIt>
constexpr OutIt copy(InIt a_First, InIt a_Last, OutIt a_DestFirst)
{
	for (; a_First != a_Last; ++a_First, ++a_DestFirst)
	{
		*a_DestFirst = *a_First;
	}
	return a_DestFirst;
}

template <typename T>
constexpr void swap(T & a, T & b)
{
	T Temp = std::move(a);
	a = std::move(b);
	b = std::move(Temp);
}

template <typename It, typename Pred>
constexpr It adjacent_find(It a_First, It a_Last, Pred a_Pred = std::equal_to<>{})
{
	if (a_First == a_Last)
	{
		return a_Last;
	}

	auto Trailer = a_First;
	++a_First;
	for (; a_First != a_Last; ++a_First, ++Trailer)
	{
		if (a_Pred(*Trailer, *a_First))
		{
			return Trailer;
		}
	}
	return a_Last;
}

template <typename It, typename T, typename Pred>
constexpr It lower_bound(It a_First, It a_Last, T a_Val, Pred a_Less = std::less<>{})
{
	while (a_First != a_Last)
	{
		const auto Dist = a_Last - a_First;
		const auto Middle = a_First + Dist / 2;
		if (a_Less(*Middle, a_Val))
		{
			a_First = Middle + 1;
		}
		else
		{
			a_Last = Middle;
		}
	}
	return a_First;
}

template <typename It, typename Pred>
constexpr It partition(It a_First, It a_Last, Pred a_Pred)
{

	for (;;)
	{
		for (;; ++a_First)
		{
			if (a_First == a_Last)
			{
				return a_First;
			}

			if (!a_Pred(*a_First))
			{
				break;
			}
		}

		for (;;)
		{
			--a_Last;
			if (a_First == a_Last)
			{
				return a_First;
			}

			if (a_Pred(*a_Last))
			{
				break;
			}
		}

		ConstexprAlgorithms::swap(*a_First, *a_Last);
		++a_First;
	}
}

template <typename It, typename Comp>
constexpr void InsertionSort(It a_First, It a_Last, Comp a_Less = std::less<>{})
{
	for (auto i = a_First; i != a_Last; ++i)
	{
		for (auto j = i; j != a_First;)
		{
			auto Prev = std::prev(j);
			if (!a_Less(*j, *Prev))
			{
				break;
			}
			ConstexprAlgorithms::swap(*j, *Prev);
			j = Prev;
		}
	}
}

template <typename It, typename Pred>
constexpr void sort(It a_First, It a_Last, Pred a_Less = std::less<>{})
{
	// Insertion sort for small ranges
	auto Dist = a_Last - a_First;
	if (Dist <= 8)
	{
		return ConstexprAlgorithms::InsertionSort(a_First, a_Last, a_Less);
	}

	// Recursive quicksort for larger ranges
	auto Pivot = [&]  // Select "median of 3" as pivot
		{
			auto Mid = a_First + Dist / 2;
			std::array<decltype(Mid), 3> Samples{{a_First, Mid, std::prev(a_Last)}};
			ConstexprAlgorithms::InsertionSort(std::begin(Samples), std::end(Samples),
				[&](const auto & a_Lhs, const auto & a_Rhs)
				{
					return a_Less(*a_Lhs, *a_Rhs);
				}
			);
			return *Samples[1];
		}();

	const auto LowerBound = ConstexprAlgorithms::partition(a_First, a_Last,
		[&](const auto & a_Val) { return a_Less(a_Val, Pivot); }
	);
	const auto UpperBound = ConstexprAlgorithms::partition(LowerBound, a_Last,
		[&](const auto & a_Val) { return !a_Less(Pivot, a_Val); }
	);
	ConstexprAlgorithms::sort(a_First, LowerBound, a_Less);
	ConstexprAlgorithms::sort(UpperBound, a_Last, a_Less);
}
}  // namespace ConstexprAlgorithms




// NOTE: Some supported compilers won't allow throw in a constexpr function.
// Skipping is okay though; error checking is still done on all other compilers.
#ifdef _MSC_VER
	#define CAN_THROW_IN_CONSTEXPR (_MSC_VER >= 1923)
#elif defined(__clang__)
	#define CAN_THROW_IN_CONSTEXPR 1
#elif defined(__GNUC__)
	#define CAN_THROW_IN_CONSTEXPR (__GNUC__ >= 8)
#else
	#define CAN_THROW_IN_CONSTEXPR 1
#endif

// A flat-map intended for constexpr lookup tables
template <typename K, typename V, size_t Size, typename Comp=std::less<>>
class cConstexprMap
{
	// NOTE: std::pair assignment isn't constexpr, so need our own type
	struct sKVPair
	{
		K first = {};
		V second = {};
	};
	using cStorage = std::array<sKVPair, Size>;
public:

	using value_type = sKVPair;
	using size_type = size_t;
	using const_iterator = typename cStorage::const_iterator;
	using iterator = const_iterator;

	constexpr cConstexprMap(std::initializer_list<value_type> a_IL):
		m_Storage{}
	{
		if (a_IL.size() != Size)
		{
			#if CAN_THROW_IN_CONSTEXPR
				throw std::invalid_argument("Wrong number of elements");
			#endif
		}

		ConstexprAlgorithms::copy(a_IL.begin(), a_IL.end(), std::begin(m_Storage));
		ConstexprAlgorithms::sort(std::begin(m_Storage), std::end(m_Storage),
			[](const auto & a_Lhs, const auto & a_Rhs)
			{
				return Comp{}(a_Lhs.first, a_Rhs.first);
			}
		);

		#if CAN_THROW_IN_CONSTEXPR
			for (size_t i = 0; i + 1 < Size; ++i)
			{
				// Assert m_Storage is sorted
				if (Comp{}(m_Storage[i + 1].first, m_Storage[i].first))
				{
					throw std::runtime_error("Sorting failed");
				}
			}
		#endif

		#if CAN_THROW_IN_CONSTEXPR
			auto It = ConstexprAlgorithms::adjacent_find(begin(), end(),
				[](const auto & a_Lhs, const auto & a_Rhs)
				{
					return a_Lhs.first == a_Rhs.first;
				}
			);

			if (It != end())
			{
				throw std::invalid_argument("Duplicate keys in initializer_list");
			}
		#endif
	}

	constexpr iterator cbegin() const { return std::cbegin(m_Storage); }
	constexpr iterator cend() const { return std::cend(m_Storage); }
	constexpr iterator begin() const { return cbegin(); }
	constexpr iterator end() const { return cend(); }
	constexpr size_type size() const { return Size; }

	template <typename K2>
	constexpr iterator lower_bound(const K2 & a_Key) const
	{
		return ConstexprAlgorithms::lower_bound(begin(), end(), a_Key,
			[&](const value_type & a_Lhs, const auto & a_Rhs)
			{
				return Comp{}(a_Lhs.first, a_Rhs);
			}
		);
	}

	template <typename K2>
	constexpr iterator find(const K2 & a_Key) const
	{
		auto it = lower_bound(a_Key);
		return (it->first == a_Key) ? it : end();
	}

	template <typename K2>
	const V & at(const K2 & a_Key) const
	{
		auto It = find(a_Key);
		if (It == end())
		{
			throw std::out_of_range("");
		}
		return It->second;
	}

private:
	cStorage m_Storage;
};
