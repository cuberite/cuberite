#pragma once

// The new and free macros break tbb
#pragma push_macro("new")
#undef new
#pragma push_macro("free")
#undef free
#include <oneapi/tbb/global_control.h>
#pragma pop_macro("free")
#pragma pop_macro("new")





class cThreadPool
{
	/** Factor of the amount of hardware threads compared to the pool size. */
	constexpr static float m_ThreadCountFactor{0.8F};

	/** Minimum amount of thread to start using m_ThreadCountFactor for pool size. */
	constexpr static std::size_t m_MinHwThreads{4};

	/** Pool size when hardware does not have enough threads. */
	constexpr static std::size_t m_MinPoolThreads{1};

	/** Thread pool size. */
	std::size_t m_ThreadsCount;

	/** OneAPI TBB global control. */
	tbb::global_control m_Control;

	/** Calculate the thread pool size. */
	static std::size_t GenThreadCount();

public:
	/** Constructor. */
	cThreadPool();

	/** Calculate the thread pool size. */
	[[nodiscard]] std::size_t GetThreadsCount() const;
};
