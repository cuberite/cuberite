#pragma once

#include <QObject>
#include <QRunnable>
#include <memory>




// fwd:
class Region;
typedef std::shared_ptr<Region> RegionPtr;

class ChunkSource;
typedef std::shared_ptr<ChunkSource> ChunkSourcePtr;





class RegionLoader :
	public QObject,
	public QRunnable
{
	Q_OBJECT

public:
	RegionLoader(int a_RegionX, int a_RegionZ, RegionPtr a_Region, ChunkSourcePtr a_ChunkSource);
	virtual ~RegionLoader() {}

	/** Signals to all loaders that the app is shutting down and the loading should be aborted. */
	static void shutdown() { m_IsShuttingDown = true; }

signals:
	void loaded(int a_RegionX, int a_RegionZ);

protected:
	virtual void run() override;

private:
	/** Coords of the region to be loaded. */
	int m_RegionX, m_RegionZ;

	/** The region to be loaded. */
	RegionPtr m_Region;

	/** The chunk source to be used for individual chunks within the region. */
	ChunkSourcePtr m_ChunkSource;

	/** Flag that is set upon app exit to terminate the queued loaders faster. */
	static volatile bool m_IsShuttingDown;
};




