#include "Globals.h"
#include "BiomeView.h"
#include <QPainter>
#include <QResizeEvent>
#include "Region.h"





static const int DELTA_STEP = 120;  // The normal per-notch wheel delta





/** Map for converting biome values to colors. Initialized from biomeColors[]. */
static uchar biomeToColor[256 * 4];

/** Map for converting biome values to colors. Used to initialize biomeToColor[]. */
static struct
{
	EMCSBiome m_Biome;
	uchar m_Color[3];
} biomeColors[] =
{
	{ biOcean,                { 0x00, 0x00, 0x70 }, },
	{ biPlains,               { 0x8d, 0xb3, 0x60 }, },
	{ biDesert,               { 0xfa, 0x94, 0x18 }, },
	{ biExtremeHills,         { 0x60, 0x60, 0x60 }, },
	{ biForest,               { 0x05, 0x66, 0x21 }, },
	{ biTaiga,                { 0x0b, 0x66, 0x59 }, },
	{ biSwampland,            { 0x2f, 0xff, 0xda }, },
	{ biRiver,                { 0x30, 0x30, 0xaf }, },
	{ biHell,                 { 0x7f, 0x00, 0x00 }, },
	{ biSky,                  { 0x00, 0x7f, 0xff }, },
	{ biFrozenOcean,          { 0xa0, 0xa0, 0xdf }, },
	{ biFrozenRiver,          { 0xa0, 0xa0, 0xff }, },
	{ biIcePlains,            { 0xff, 0xff, 0xff }, },
	{ biIceMountains,         { 0xa0, 0xa0, 0xa0 }, },
	{ biMushroomIsland,       { 0xff, 0x00, 0xff }, },
	{ biMushroomShore,        { 0xa0, 0x00, 0xff }, },
	{ biBeach,                { 0xfa, 0xde, 0x55 }, },
	{ biDesertHills,          { 0xd2, 0x5f, 0x12 }, },
	{ biForestHills,          { 0x22, 0x55, 0x1c }, },
	{ biTaigaHills,           { 0x16, 0x39, 0x33 }, },
	{ biExtremeHillsEdge,     { 0x7f, 0x8f, 0x7f }, },
	{ biJungle,               { 0x53, 0x7b, 0x09 }, },
	{ biJungleHills,          { 0x2c, 0x42, 0x05 }, },

	{ biJungleEdge,           { 0x62, 0x8b, 0x17 }, },
	{ biDeepOcean,            { 0x00, 0x00, 0x30 }, },
	{ biStoneBeach,           { 0xa2, 0xa2, 0x84 }, },
	{ biColdBeach,            { 0xfa, 0xf0, 0xc0 }, },
	{ biBirchForest,          { 0x30, 0x74, 0x44 }, },
	{ biBirchForestHills,     { 0x1f, 0x5f, 0x32 }, },
	{ biRoofedForest,         { 0x40, 0x51, 0x1a }, },
	{ biColdTaiga,            { 0x31, 0x55, 0x4a }, },
	{ biColdTaigaHills,       { 0x59, 0x7d, 0x72 }, },
	{ biMegaTaiga,            { 0x59, 0x66, 0x51 }, },
	{ biMegaTaigaHills,       { 0x59, 0x66, 0x59 }, },
	{ biExtremeHillsPlus,     { 0x50, 0x70, 0x50 }, },
	{ biSavanna,              { 0xbd, 0xb2, 0x5f }, },
	{ biSavannaPlateau,       { 0xa7, 0x9d, 0x64 }, },
	{ biMesa,                 { 0xd9, 0x45, 0x15 }, },
	{ biMesaPlateauF,         { 0xb0, 0x97, 0x65 }, },
	{ biMesaPlateau,          { 0xca, 0x8c, 0x65 }, },

	// M variants:
	{ biSunflowerPlains,      { 0xb5, 0xdb, 0x88 }, },
	{ biDesertM,              { 0xff, 0xbc, 0x40 }, },
	{ biExtremeHillsM,        { 0x88, 0x88, 0x88 }, },
	{ biFlowerForest,         { 0x2d, 0x8e, 0x49 }, },
	{ biTaigaM,               { 0x33, 0x8e, 0x81 }, },
	{ biSwamplandM,           { 0x07, 0xf9, 0xb2 }, },
	{ biIcePlainsSpikes,      { 0xb4, 0xdc, 0xdc }, },
	{ biJungleM,              { 0x7b, 0xa3, 0x31 }, },
	{ biJungleEdgeM,          { 0x62, 0x8b, 0x17 }, },
	{ biBirchForestM,         { 0x58, 0x9c, 0x6c }, },
	{ biBirchForestHillsM,    { 0x47, 0x87, 0x5a }, },
	{ biRoofedForestM,        { 0x68, 0x79, 0x42 }, },
	{ biColdTaigaM,           { 0x24, 0x3f, 0x36 }, },
	{ biMegaSpruceTaiga,      { 0x45, 0x4f, 0x3e }, },
	{ biMegaSpruceTaigaHills, { 0x45, 0x4f, 0x4e }, },
	{ biExtremeHillsPlusM,    { 0x78, 0x98, 0x78 }, },
	{ biSavannaM,             { 0xe5, 0xda, 0x87 }, },
	{ biSavannaPlateauM,      { 0xa7, 0x9d, 0x74 }, },
	{ biMesaBryce,            { 0xff, 0x6d, 0x3d }, },
	{ biMesaPlateauFM,        { 0xd8, 0xbf, 0x8d }, },
	{ biMesaPlateauM,         { 0xf2, 0xb4, 0x8d }, },
} ;





static class BiomeColorsInitializer
{
public:
	BiomeColorsInitializer(void)
	{
		// Reset all colors to gray:
		for (size_t i = 0; i < ARRAYCOUNT(biomeToColor); i++)
		{
			biomeToColor[i] = 0x7f;
		}

		// Set known biomes to their colors:
		for (size_t i = 0; i < ARRAYCOUNT(biomeColors); i++)
		{
			uchar * color = &biomeToColor[4 * biomeColors[i].m_Biome];
			color[0] = biomeColors[i].m_Color[2];
			color[1] = biomeColors[i].m_Color[1];
			color[2] = biomeColors[i].m_Color[0];
			color[3] = 0xff;
		}
	}
} biomeColorInitializer;





////////////////////////////////////////////////////////////////////////////////
// BiomeView:

BiomeView::BiomeView(QWidget * parent) :
	super(parent),
	m_X(0),
	m_Z(0),
	m_Zoom(1),
	m_IsMouseDragging(false),
	m_MouseWheelDelta(0)
{
	// Create the image used for undefined chunks:
	int offset = 0;
	for (int y = 0; y < 16; y++)
	{
		for (int x = 0; x < 16; x++)
		{
			uchar color = (((x & 8) ^ (y & 8)) == 0) ? 0x44 : 0x88;
			m_EmptyChunkImage[offset++] = color;
			m_EmptyChunkImage[offset++] = color;
			m_EmptyChunkImage[offset++] = color;
			m_EmptyChunkImage[offset++] = 0xff;
		}
	}

	// Create the startup image:
	redraw();

	// Add a chunk-update callback mechanism:
	connect(&m_Cache, SIGNAL(regionAvailable(int, int)), this, SLOT(regionAvailable(int, int)));

	// Allow mouse and keyboard interaction:
	setFocusPolicy(Qt::StrongFocus);
	setMouseTracking(true);
}




QSize BiomeView::minimumSizeHint() const
{
	return QSize(300, 300);
}





QSize BiomeView::sizeHint() const
{
	return QSize(800, 600);
}





void BiomeView::setChunkSource(std::shared_ptr<ChunkSource> a_ChunkSource)
{
	// Replace the source in the cache:
	m_Cache.setChunkSource(a_ChunkSource);

	// Redraw with the new source:
	redraw();
}





void BiomeView::setPosition(int a_BlockX, int a_BlockZ)
{
	m_X = a_BlockX;
	m_Z = a_BlockZ;
	redraw();
}





void BiomeView::setZoomLevel(double a_ZoomLevel)
{
	m_Zoom = a_ZoomLevel;
	redraw();
}





void BiomeView::redraw()
{
	if (!hasData())
	{
		// No data means no image is displayed, no need to compose:
		update();
		return;
	}

	int chunksize = 16 * m_Zoom;

	// first find the center block position
	int centerchunkx = floor(m_X / 16);
	int centerchunkz = floor(m_Z / 16);
	// and the center of the screen
	int centerx = m_Image.width()  / 2;
	int centery = m_Image.height() / 2;
	// and align for panning
	centerx -= (m_X - centerchunkx * 16) * m_Zoom;
	centery -= (m_Z - centerchunkz * 16) * m_Zoom;
	// now calculate the topleft block on the screen
	int startx = centerchunkx - centerx / chunksize - 1;
	int startz = centerchunkz - centery / chunksize - 1;
	// and the dimensions of the screen in blocks
	int blockswide = m_Image.width()  / chunksize + 3;
	int blockstall = m_Image.height() / chunksize + 3;

	for (int z = startz; z < startz + blockstall; z++)
	{
		for (int x = startx; x < startx + blockswide; x++)
		{
			drawChunk(x, z);
		}
	}
	update();
}





void BiomeView::regionAvailable(int a_RegionX, int a_RegionZ)
{
	for (int z = 0; z < 32; z++)
	{
		for (int x = 0; x < 32; x++)
		{
			drawChunk(a_RegionX * 32 + x, a_RegionZ * 32 + z);
		}
	}
	update();
}





void BiomeView::reload()
{
	if (!hasData())
	{
		return;
	}
	m_Cache.reload();

	redraw();
}





void BiomeView::drawChunk(int a_ChunkX, int a_ChunkZ)
{
	if (!hasData())
	{
		return;
	}

	// Fetch the region:
	int regionX;
	int regionZ;
	Region::chunkToRegion(a_ChunkX, a_ChunkZ, regionX, regionZ);
	RegionPtr region = m_Cache.fetch(regionX, regionZ);

	// Figure out where on the screen this chunk should be drawn:
	// first find the center chunk
	int centerchunkx = floor(m_X / 16);
	int centerchunkz = floor(m_Z / 16);
	// and the center chunk screen coordinates
	int centerx = m_Image.width()  / 2;
	int centery = m_Image.height() / 2;
	// which need to be shifted to account for panning inside that chunk
	centerx -= (m_X - centerchunkx * 16) * m_Zoom;
	centery -= (m_Z - centerchunkz * 16) * m_Zoom;
	// centerx, centery now points to the top left corner of the center chunk
	// so now calculate our x, y in relation
	double chunksize = 16 * m_Zoom;
	centerx += (a_ChunkX - centerchunkx) * chunksize;
	centery += (a_ChunkZ - centerchunkz) * chunksize;

	uchar * bits = m_Image.bits();
	int imgstride = m_Image.bytesPerLine();

	int skipx = 0, skipy = 0;
	int blockwidth = chunksize, blockheight = chunksize;
	// now if we're off the screen we need to crop
	if (centerx < 0)
	{
		skipx = -centerx;
		centerx = 0;
	}
	if (centery < 0)
	{
		skipy = -centery;
		centery = 0;
	}
	// or the other side, we need to trim
	if (centerx + blockwidth > m_Image.width())
	{
		blockwidth = m_Image.width() - centerx;
	}
	if (centery + blockheight > m_Image.height())
	{
		blockheight = m_Image.height() - centery;
	}
	if ((blockwidth <= 0) || (skipx >= blockwidth))
	{
		return;
	}
	int imgoffset = centerx * 4 + centery * imgstride;

	// If the chunk is valid, use its data; otherwise use the empty placeholder:
	const short * src = m_EmptyChunkBiomes;
	if (region.get() != nullptr)
	{
		int relChunkX = a_ChunkX - regionX * 32;
		int relChunkZ = a_ChunkZ - regionZ * 32;
		Chunk & chunk = region->getRelChunk(relChunkX, relChunkZ);
		if (chunk.isValid())
		{
			src = chunk.getBiomes();
		}
	}

	// Scale-blit the image:
	for (int z = skipy; z < blockheight; z++, imgoffset += imgstride)
	{
		size_t srcoffset = static_cast<size_t>(std::floor((double)z / m_Zoom)) * 16;
		int imgxoffset = imgoffset;
		for (int x = skipx; x < blockwidth; x++)
		{
			short biome = src[srcoffset + static_cast<size_t>(std::floor((double)x / m_Zoom))];
			const uchar * color;
			if (biome < 0)
			{
				static const uchar emptyBiome1[] = { 0x44, 0x44, 0x44, 0xff };
				static const uchar emptyBiome2[] = { 0x88, 0x88, 0x88, 0xff };
				color = ((x & 8) ^ (z & 8)) ? emptyBiome1 : emptyBiome2;
			}
			else
			{
				if (biome * 4 >= ARRAYCOUNT(biomeToColor))
				{
					static const uchar errorImage[] = { 0xff, 0x00, 0x00, 0xff };
					color = errorImage;
				}
				else
				{
					color = biomeToColor + biome * 4;
				}
			}
			bits[imgxoffset]     = color[0];
			bits[imgxoffset + 1] = color[1];
			bits[imgxoffset + 2] = color[2];
			bits[imgxoffset + 3] = color[3];
			imgxoffset += 4;
		}  // for x
	}  // for z
}





void BiomeView::resizeEvent(QResizeEvent * a_Event)
{
	m_Image = QImage(a_Event->size(), QImage::Format_RGB32);
	redraw();
}





void BiomeView::paintEvent(QPaintEvent * a_Event)
{
	QPainter p(this);
	if (hasData())
	{
		p.drawImage(QPoint(0, 0), m_Image);
	}
	else
	{
		p.drawText(a_Event->rect(), Qt::AlignCenter, "No chunk source selected");
	}
	p.end();
}





void BiomeView::mousePressEvent(QMouseEvent * a_Event)
{
	m_LastX = a_Event->x();
	m_LastY = a_Event->y();
	m_IsMouseDragging = true;
}





void BiomeView::mouseMoveEvent(QMouseEvent * a_Event)
{
	// If there's no data displayed, bail out:
	if (!hasData())
	{
		return;
	}

	if (m_IsMouseDragging)
	{
		// The user is dragging the mouse, move the view around:
		m_X += (m_LastX - a_Event->x()) / m_Zoom;
		m_Z += (m_LastY - a_Event->y()) / m_Zoom;
		m_LastX = a_Event->x();
		m_LastY = a_Event->y();
		redraw();
		return;
	}

	// Update the status bar info text:
	int blockX = floor((a_Event->x() - width()  / 2) / m_Zoom + m_X);
	int blockZ = floor((a_Event->y() - height() / 2) / m_Zoom + m_Z);
	int regionX, regionZ;
	Region::blockToRegion(blockX, blockZ, regionX, regionZ);
	int relX = blockX - regionX * 512;
	int relZ = blockZ - regionZ * 512;
	auto region = m_Cache.fetch(regionX, regionZ);
	int biome = (region.get() != nullptr) ? region->getRelBiome(relX, relZ) : biInvalidBiome;
	emit hoverChanged(blockX, blockZ, biome);
}





void BiomeView::mouseReleaseEvent(QMouseEvent *)
{
	m_IsMouseDragging = false;
}





void BiomeView::wheelEvent(QWheelEvent * a_Event)
{
	m_MouseWheelDelta += a_Event->delta();
	while (m_MouseWheelDelta >= DELTA_STEP)
	{
		emit wheelUp();
		m_MouseWheelDelta -= DELTA_STEP;
	}
	while (m_MouseWheelDelta <= -DELTA_STEP)
	{
		emit wheelDown();
		m_MouseWheelDelta += DELTA_STEP;
	}
}





void BiomeView::keyPressEvent(QKeyEvent * a_Event)
{
	switch (a_Event->key())
	{
		case Qt::Key_Up:
		case Qt::Key_W:
		{
			m_Z -= 10.0 / m_Zoom;
			redraw();
			break;
		}

		case Qt::Key_Down:
		case Qt::Key_S:
		{
			m_Z += 10.0 / m_Zoom;
			redraw();
			break;
		}

		case Qt::Key_Left:
		case Qt::Key_A:
		{
			m_X -= 10.0 / m_Zoom;
			redraw();
			break;
		}

		case Qt::Key_Right:
		case Qt::Key_D:
		{
			m_X += 10.0 / m_Zoom;
			redraw();
			break;
		}

		case Qt::Key_PageUp:
		case Qt::Key_Q:
		{
			emit increaseZoom();
			break;
		}

		case Qt::Key_PageDown:
		case Qt::Key_E:
		{
			emit decreaseZoom();
			break;
		}
	}
}
