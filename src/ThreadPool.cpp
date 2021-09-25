#include "Globals.h"

#include "ThreadPool.h"





std::size_t cThreadPool::GenThreadCount()
{
	const auto TotalThreadsCount = std::thread::hardware_concurrency();

	// If the hardware we are running on does not have more than m_MinHwThreads hardware logical threads than
	// only make the thread pool be the size of m_MinPoolThreads to optimize performance
	//  of threads running outside the pool
	return TotalThreadsCount > m_MinHwThreads ? static_cast<std::size_t>(static_cast<float>(TotalThreadsCount) * m_ThreadCountFactor) : m_MinPoolThreads;
}





cThreadPool::cThreadPool() : m_ThreadsCount{GenThreadCount()}, m_Control{tbb::global_control::max_allowed_parallelism, m_ThreadsCount} {}





std::size_t cThreadPool::GetThreadsCount() const { return m_ThreadsCount; }
