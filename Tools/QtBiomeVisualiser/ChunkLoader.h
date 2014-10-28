#pragma once

#include <QObject>
#include <QRunnable>
#include <memory>




#if (!defined(_MSC_VER) && (__cplusplus < 201103L))
	// GCC in non-c++11 mode doesn't have the "override" keyword
	#define override
#endif





// fwd:
class Chunk;
typedef std::shared_ptr<Chunk> ChunkPtr;

class ChunkSource;
typedef std::shared_ptr<ChunkSource> ChunkSourcePtr;





class ChunkLoader :
	public QObject,
	public QRunnable
{
	Q_OBJECT

public:
	ChunkLoader(int a_ChunkX, int a_ChunkZ, ChunkPtr a_Chunk, ChunkSourcePtr a_ChunkSource);
	virtual ~ChunkLoader() {}

signals:
	void loaded(int a_ChunkX, int a_ChunkZ);

protected:
	virtual void run() override;

private:
	int m_ChunkX, m_ChunkZ;
	ChunkPtr m_Chunk;
	ChunkSourcePtr m_ChunkSource;
};




