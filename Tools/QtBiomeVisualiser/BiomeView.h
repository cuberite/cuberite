#pragma once

#include <QWidget>
#include "ChunkCache.h"
#include "ChunkSource.h"





class BiomeView :
	public QWidget
{
	typedef QWidget super;
	Q_OBJECT

public:
	explicit BiomeView(QWidget * parent = NULL);

	QSize minimumSizeHint() const;
	QSize sizeHint() const;

	/** Replaces the chunk source used by the biome view to get the chunk biome data.
	The entire view is then invalidated and regenerated. */
	void setChunkSource(std::shared_ptr<ChunkSource> a_ChunkSource);

signals:

public slots:
	/** Redraw the entire widget area. */
	void redraw();

	/** A specified chunk has become available, redraw it. */
	void chunkAvailable(int a_ChunkX, int a_ChunkZ);

protected:
	double m_X, m_Z;
	int m_Zoom;
	ChunkCache m_Cache;
	QImage m_Image;

	/** Data used for rendering a chunk that hasn't been loaded yet */
	uchar m_EmptyChunkImage[16 * 16 * 4];


	/** Draws the specified chunk into m_Image */
	void drawChunk(int a_ChunkX, int a_ChunkZ);

	/** Returns true iff the biome view has been initialized to contain proper biome data. */
	bool hasData(void) const { return m_Cache.hasData(); }

	/** Called when the widget is resized */
	virtual void resizeEvent(QResizeEvent *) override;

	/** Paints the entire widget */
	virtual void paintEvent(QPaintEvent *) override;

	/** Queues the chunk for rendering. */
	void queueChunkRender(ChunkPtr a_Chunk);
};





