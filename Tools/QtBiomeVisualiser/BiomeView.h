#pragma once

#include <QWidget>
#include <memory>
#include "RegionCache.h"
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

	/** Sets the position of the central pixel of the map to the specified point and redraws the view. */
	void setPosition(int a_BlockX, int a_BlockZ);

	/** Sets the zoom level to the specified value and redraws the view. */
	void setZoomLevel(double a_ZoomLevel);

signals:
	/** Signalled when the user uses the wheel to scroll upwards. */
	void wheelUp();

	/** Signalled when the user uses the wheel to scroll downwards. */
	void wheelDown();

	/** Signalled when the user presses a key to increase zoom. */
	void increaseZoom();

	/** Signalled when the user presses a key to decrease zoom. */
	void decreaseZoom();

	/** Emitted when the user moves the mouse, to reflect the current block under the cursor. */
	void hoverChanged(int a_BlockX, int a_BlockZ, int a_Biome);

public slots:
	/** Redraw the entire widget area. */
	void redraw();

	/** A specified region has become available, redraw it. */
	void regionAvailable(int a_RegionX, int a_RegionZ);

	/** Reloads the current chunk source and redraws the entire workspace. */
	void reload();

protected:
	double m_X, m_Z;
	double m_Zoom;

	/** Cache for the loaded chunk data. */
	RegionCache m_Cache;

	/** The entire view's contents in an offscreen image. */
	QImage m_Image;

	/** Coords of the mouse for the previous position, used while dragging. */
	int m_LastX, m_LastY;

	/** Set to true when the user has a mouse button depressed, and is dragging the view. */
	bool m_IsMouseDragging;

	/** Accumulator for the mouse wheel's delta. When the accumulator hits a threshold, the view zooms. */
	int m_MouseWheelDelta;

	/** Data used for rendering a chunk that hasn't been loaded yet */
	uchar m_EmptyChunkImage[16 * 16 * 4];

	/** Data placeholder for chunks that aren't valid. */
	short m_EmptyChunkBiomes[16 * 16];


	/** Draws the specified chunk into m_Image */
	void drawChunk(int a_ChunkX, int a_ChunkZ);

	/** Returns true iff the biome view has been initialized to contain proper biome data. */
	bool hasData(void) const { return m_Cache.hasData(); }

	/** Called when the widget is resized */
	virtual void resizeEvent(QResizeEvent *) override;

	/** Paints the entire widget */
	virtual void paintEvent(QPaintEvent *) override;

	/** Called when the user presses any mouse button. */
	virtual void mousePressEvent(QMouseEvent * a_Event);

	/** Called when the user moves the mouse. */
	virtual void mouseMoveEvent(QMouseEvent * a_Event);

	/** Called when the user releases a previously held mouse button. */
	virtual void mouseReleaseEvent(QMouseEvent * a_Event) override;

	/** Called when the user rotates the mouse wheel. */
	virtual void wheelEvent(QWheelEvent * a_Event) override;

	/** Called when the user presses a key. */
	virtual void keyPressEvent(QKeyEvent * a_Event) override;
};





