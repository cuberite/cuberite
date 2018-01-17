#pragma once

#include <QObject>
#include <QCache>
#include <QMutex>
#include <memory>





// fwd:
class Region;
typedef std::shared_ptr<Region> RegionPtr;

class ChunkSource;





/** Caches regions' chunk data for reuse */
class RegionCache :
	public QObject
{
	typedef QObject super;
	Q_OBJECT

public:
	explicit RegionCache(QObject * parent = NULL);

	/** Retrieves the specified region from the cache.
	Only returns valid regions; if the region is invalid, queues it for rendering and returns an empty ptr. */
	RegionPtr fetch(int a_RegionX, int a_RegionZ);

	/** Replaces the chunk source used by the biome view to get the chunk biome data.
	The cache is then invalidated. */
	void setChunkSource(std::shared_ptr<ChunkSource> a_ChunkSource);

	/** Returns true iff the chunk source has been initialized. */
	bool hasData() const { return (m_ChunkSource.get() != nullptr); }

	/** Reloads the current chunk source. */
	void reload();

signals:
	void regionAvailable(int a_RegionX, int a_RegionZ);

protected slots:
	void gotRegion(int a_RegionX, int a_RegionZ);

protected:
	/** The cache of the chunks */
	QCache<quint32, RegionPtr> m_Cache;

	/** Locks the cache against multithreaded access */
	QMutex m_Mtx;

	/** The source used to get the biome data. */
	std::shared_ptr<ChunkSource> m_ChunkSource;


	/** Returns the hash used by the chunk in the cache */
	quint32 getRegionHash(int a_RegionX, int a_RegionZ);

	/** Queues the specified region for rendering by m_RegionSource. */
	void queueRegionRender(int a_RegionX, int a_RegionZ, RegionPtr & a_Region);
};





