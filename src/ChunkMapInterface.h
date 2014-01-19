
typedef cItemCallback<cChestEntity>     cChestCallback;

class cChunkMapInterface
{
public:
	bool ForEachChestInChunk(int a_ChunkX, int a_ChunkZ, cChestCallback & a_Callback);
}
