#pragma once

#include <QObject>
#include <QCache>
#include <QMutex>





class Chunk;
typedef std::shared_ptr<Chunk> ChunkPtr;

class ChunkSource;





/** Caches chunk data for reuse */
class ChunkCache :
	public QObject
{
	typedef QObject super;
	Q_OBJECT

public:
	explicit ChunkCache(QObject * parent = NULL);

	/** Retrieves the specified chunk from the cache.
	Only returns valid chunks; if the chunk is invalid, queues it for rendering and returns an empty ptr. */
	ChunkPtr fetch(int a_ChunkX, int a_ChunkZ);

	/** Replaces the chunk source used by the biome view to get the chunk biome data.
	The cache is then invalidated. */
	void setChunkSource(std::shared_ptr<ChunkSource> a_ChunkSource);

	/** Returns true iff the chunk source has been initialized. */
	bool hasData(void) const { return (m_ChunkSource.get() != nullptr); }

signals:
	void chunkAvailable(int a_ChunkX, int a_ChunkZ);

protected slots:
	void gotChunk(int a_ChunkX, int a_ChunkZ);

protected:
	/** The cache of the chunks */
	QCache<quint32, ChunkPtr> m_Cache;

	/** Locks te cache against multithreaded access */
	QMutex m_Mtx;

	/** The source used to get the biome data. */
	std::shared_ptr<ChunkSource> m_ChunkSource;


	/** Returns the hash used by the chunk in the cache */
	quint32 getChunkHash(int a_ChunkX, int a_ChunkZ);

	/** Queues the specified chunk for rendering by m_ChunkSource. */
	void queueChunkRender(int a_ChunkX, int a_ChunkZ, ChunkPtr & a_Chunk);
};





