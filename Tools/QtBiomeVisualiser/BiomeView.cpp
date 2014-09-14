#include "Globals.h"
#include "BiomeView.h"
#include "Chunk.h"
#include <QPainter>
#include <QResizeEvent>





BiomeView::BiomeView(QWidget * parent) :
	super(parent),
	m_X(0),
	m_Z(0),
	m_Zoom(1)
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
	connect(&m_Cache, SIGNAL(chunkAvailable(int, int)), this, SLOT(chunkAvailable(int, int)));
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





void BiomeView::chunkAvailable(int a_ChunkX, int a_ChunkZ)
{
	drawChunk(a_ChunkX, a_ChunkZ);
	update();
}





void BiomeView::drawChunk(int a_ChunkX, int a_ChunkZ)
{
	if (!hasData())
	{
		return;
	}

	//fetch the chunk:
	ChunkPtr chunk = m_Cache.fetch(a_ChunkX, a_ChunkZ);

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
	// centerx,y now points to the top left corner of the center chunk
	// so now calculate our x,y in relation
	double chunksize = 16 * m_Zoom;
	centerx += (a_ChunkX - centerchunkx) * chunksize;
	centery += (a_ChunkZ - centerchunkz) * chunksize;

	int srcoffset = 0;
	uchar * bits = m_Image.bits();
	int imgstride = m_Image.bytesPerLine();

	int skipx = 0,skipy = 0;
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
	const uchar * src = m_EmptyChunkImage;
	if (chunk.get() != nullptr)
	{
		src = chunk->getImage();
	}

	// Blit or scale-blit the image:
	for (int z = skipy; z < blockheight; z++, imgoffset += imgstride)
	{
		srcoffset = floor((double)z / m_Zoom) * 16 * 4;
		if (m_Zoom == 1.0)
		{
			memcpy(bits + imgoffset, src + srcoffset + skipx * 4, (blockwidth - skipx) * 4);
		}
		else
		{
			int xofs = 0;
			for (int x = skipx; x < blockwidth; x++, xofs +=4)
			{
				memcpy(bits + imgoffset + xofs, src + srcoffset + (int)floor((double)x / m_Zoom) * 4, 4);
			}
		}
	}
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





void BiomeView::queueChunkRender(ChunkPtr a_Chunk)
{

}




