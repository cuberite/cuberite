#include "Globals.h"
#include "RegionCache.h"
#include <QMutexLocker>
#include <QThreadPool>
#include "ChunkSource.h"
#include "RegionLoader.h"
#include "Region.h"





RegionCache::RegionCache(QObject * parent) :
	super(parent)
{
	m_Cache.setMaxCost(1024 * 1024 * 1024);  // 1 GiB of memory for the cache
}





RegionPtr RegionCache::fetch(int a_RegionX, int a_RegionZ)
{
	// Retrieve from the cache:
	quint32 hash = getRegionHash(a_RegionX, a_RegionZ);
	RegionPtr * res;
	{
		QMutexLocker lock(&m_Mtx);
		res = m_Cache[hash];
		// If succesful and region loaded, return the retrieved value:
		if ((res != nullptr) && (*res)->isValid())
		{
			return *res;
		}
	}

	// If the region is in cache but not valid, it means it has been already queued for rendering, do nothing now:
	if (res != nullptr)
	{
		return RegionPtr(nullptr);
	}

	// There's no such item in the cache, create it now:
	try
	{
		res = new RegionPtr(new Region);
	}
	catch (const std::bad_alloc &)
	{
		/* Allocation failed (32-bit process hit the 2 GiB barrier?)
		This may happen even with the cache set to 1 GiB, because it contains shared ptrs and so they may be
		held by another place in the code even when they are removed from cache.
		*/
		return RegionPtr(nullptr);
	}
	if (res == nullptr)
	{
		return RegionPtr(nullptr);
	}
	{
		QMutexLocker lock(&m_Mtx);
		m_Cache.insert(hash, res, sizeof(Region));
	}

	// Queue the region for rendering:
	queueRegionRender(a_RegionX, a_RegionZ, *res);

	// Return failure, the region is not yet rendered:
	return RegionPtr(nullptr);
}





void RegionCache::setChunkSource(std::shared_ptr<ChunkSource> a_ChunkSource)
{
	// Replace the chunk source:
	m_ChunkSource = a_ChunkSource;

	// Clear the cache:
	QMutexLocker lock(&m_Mtx);
	m_Cache.clear();
}





void RegionCache::reload()
{
	assert(m_ChunkSource.get() != nullptr);

	// Reload the chunk source:
	m_ChunkSource->reload();

	// Clear the cache:
	QMutexLocker lock(&m_Mtx);
	m_Cache.clear();
}





void RegionCache::gotRegion(int a_RegionX, int a_RegionZ)
{
	emit regionAvailable(a_RegionX, a_RegionZ);
}





quint32 RegionCache::getRegionHash(int a_RegionX, int a_RegionZ)
{
	// Simply join the two coords into a single int
	// The coords will never be larger than 16-bits, so we can do this safely
	return (((static_cast<quint32>(a_RegionX) & 0xffff) << 16) | (static_cast<quint32>(a_RegionZ) & 0xffff));
}





void RegionCache::queueRegionRender(int a_RegionX, int a_RegionZ, RegionPtr & a_Region)
{
	// Create a new loader task:
	RegionLoader * loader = new RegionLoader(a_RegionX, a_RegionZ, a_Region, m_ChunkSource);
	connect(loader, SIGNAL(loaded(int, int)), this, SLOT(gotRegion(int, int)));

	QThreadPool::globalInstance()->start(loader);
}




