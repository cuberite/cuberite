#include "Globals.h"
#include "ChunkCache.h"
#include <QMutexLocker>
#include <QThreadPool>
#include "ChunkSource.h"
#include "ChunkLoader.h"





ChunkCache::ChunkCache(QObject * parent) :
	super(parent)
{
	m_Cache.setMaxCost(1024 * 1024 * 1024);  // 1 GiB of memory for the cache
}





ChunkPtr ChunkCache::fetch(int a_ChunkX, int a_ChunkZ)
{
	// Retrieve from the cache:
	quint32 hash = getChunkHash(a_ChunkX, a_ChunkZ);
	ChunkPtr * res;
	{
		QMutexLocker lock(&m_Mtx);
		res = m_Cache[hash];
		// If succesful and chunk loaded, return the retrieved value:
		if ((res != nullptr) && (*res)->isValid())
		{
			return *res;
		}
	}

	// If the chunk is in cache but not valid, it means it has been already queued for rendering, do nothing now:
	if (res != nullptr)
	{
		return ChunkPtr(nullptr);
	}

	// There's no such item in the cache, create it now:
	res = new ChunkPtr(new Chunk);
	if (res == nullptr)
	{
		return ChunkPtr(nullptr);
	}
	{
		QMutexLocker lock(&m_Mtx);
		m_Cache.insert(hash, res, sizeof(Chunk));
	}

	// Queue the chunk for rendering:
	queueChunkRender(a_ChunkX, a_ChunkZ, *res);

	// Return failure, the chunk is not yet rendered:
	return ChunkPtr(nullptr);
}





void ChunkCache::setChunkSource(std::shared_ptr<ChunkSource> a_ChunkSource)
{
	// Replace the chunk source:
	m_ChunkSource = a_ChunkSource;

	// Clear the cache:
	QMutexLocker lock(&m_Mtx);
	m_Cache.clear();
}





void ChunkCache::gotChunk(int a_ChunkX, int a_ChunkZ)
{
	emit chunkAvailable(a_ChunkX, a_ChunkZ);
}





quint32 ChunkCache::getChunkHash(int a_ChunkX, int a_ChunkZ)
{
	// Simply join the two coords into a single int
	// The coords will never be larger than 16-bits, so we can do this safely
	return (((static_cast<quint32>(a_ChunkX) & 0xffff) << 16) | (static_cast<quint32>(a_ChunkZ) & 0xffff));
}





void ChunkCache::queueChunkRender(int a_ChunkX, int a_ChunkZ, ChunkPtr & a_Chunk)
{
	// Create a new loader task:
	ChunkLoader * loader = new ChunkLoader(a_ChunkX, a_ChunkZ, a_Chunk, m_ChunkSource);
	connect(loader, SIGNAL(loaded(int, int)), this, SLOT(gotChunk(int, int)));

	QThreadPool::globalInstance()->start(loader);
}




