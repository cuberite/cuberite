
#include "Globals.h"

class MockChest;
typedef cItemCallback<MockChest> cChestCallback;

AString ItemToFullString(const cItem & a_Item)
{
return "";
}

class cEntity
{
public:
	const Vector3d & GetPosition (void) const { return m_pos;}
	double GetWidth (void) const { return 0; }
	double GetHeight (void) const { return 0; }
	static const Vector3d m_pos;
};

const Vector3d cEntity::m_pos = Vector3d(0,0,0);

class cItem
{
public:
	cItem(BLOCKTYPE val) {}
};

void cBlockInfo::Initialize(cBlockInfoArray & a_Info) {}
cBlockInfo::~cBlockInfo () {}

#include "Blocks/ChunkInterface.h"

bool cChunkInterface::ForEachChunkInRect(int a_MinChunkX, int a_MaxChunkX, int a_MinChunkZ, int a_MaxChunkZ, cChunkDataCallback & a_Callback)
{
	return false;
}

bool cChunkInterface::WriteBlockArea(cBlockArea & a_Area, int a_MinBlockX, int a_MinBlockY, int a_MinBlockZ, int a_DataTypes)
{
	return false;
}

#include "Simulator/Simulator.inc"

#include "Simulator/IncrementalRedstoneSimulator.inc"

class MockWorld;


class MockHandler
{
public:
	static eBlockFace MetadataToDirection(NIBBLETYPE a_MetaData) { return BLOCK_FACE_NONE; }
	static eBlockFace MetaDataToDirection(NIBBLETYPE a_MetaData) { return BLOCK_FACE_NONE; }
	static eBlockFace BlockMetaDataToBlockFace(NIBBLETYPE a_MetaData) { return BLOCK_FACE_NONE; }
	static NIBBLETYPE IsOpen(cChunkInterface & a_ChunkInterface, int a_BlockX, int a_BlockY, int a_BlockZ) { return 0; }
	static void ExtendPiston(int a_BlockX, int a_BlockY, int a_BlockZ, MockWorld * a_World) {}
	static void RetractPiston(int a_BlockX, int a_BlockY, int a_BlockZ, MockWorld * a_World) {}
	static void SetOpen(cChunkInterface & a_ChunkInterface, int a_BlockX, int a_BlockY, int a_BlockZ, bool a_Open) {}

};

template<unsigned char val>
class MockHandlerFetcher
{
public:
	typedef MockHandler type;
};

class MockWorld 
{
public:
	bool IsChunkLighted(int a_ChunkX, int a_ChunkZ) { return false; }
	bool ForEachEntityInChunk(int a_ChunkX, int a_ChunkZ, cEntityCallback & a_Callback) { return false; }
	
	void QueueLightChunk(int a_ChunkX, int a_ChunkZ, cChunkCoordCallback * a_Callback = NULL) {}


	NIBBLETYPE GetBlockSkyLight (int a_BlockX, int a_BlockY, int a_BlockZ) { return 0; }
	
	cPlayer * FindClosestPlayer(const Vector3d & a_Pos, float a_SightLimit, bool a_CheckLineOfSight = true) { return NULL; }
	
	
	void WakeUpSimulators(int a_BlockX, int a_BlockY, int a_BlockZ) {}

	void SpawnPrimedTNT(double a_X, double a_Y, double a_Z, int a_FuseTimeInSec = 80, double a_InitialVelocityCoeff = 1) {}


	bool SetTrapdoorOpen(int a_BlockX, int a_BlockY, int a_BlockZ, bool a_Open) {return false; }
	
	
	cChunkMap * GetChunkMap (void) { return NULL; }

};


class MockChunk 
{
public:
	cRedstoneSimulatorChunkData * GetRedstoneSimulatorData() { return NULL; }
	void SetRedstoneSimulatorData(cRedstoneSimulatorChunkData * a_Data) {}
	bool IsRedstoneDirty() { return true; }
	void SetIsRedstoneDirty(bool a_Param) {}
	
	void GetBlockTypeMeta(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta) {}

	void SetBlock(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, bool a_SendToClients = true) {}
	void SetBlock( const Vector3i & a_RelBlockPos, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) {}

	int GetPosX(void) const { return 0; }
	int GetPosZ(void) const { return 0; }
	
	MockChunk * GetRelNeighborChunkAdjustCoords(int & a_RelX, int & a_RelZ) const { return NULL; }


	BLOCKTYPE GetBlock(int a_RelX, int a_RelY, int a_RelZ) const { return 0; }
	BLOCKTYPE GetBlock(const Vector3i & a_RelCoords) const { return 0; }
	
	NIBBLETYPE GetMeta(int a_RelX, int a_RelY, int a_RelZ) const { return 0; }
	void SetMeta(int a_RelX, int a_RelY, int a_RelZ, NIBBLETYPE a_Meta) {}


	bool UnboundedRelGetBlock(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta) const { return false; }

	bool UnboundedRelGetBlockType(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE & a_BlockType) const { return false; }
	
	MockChunk * GetNeighborChunk(int a_BlockX, int a_BlockZ) { return NULL; }

	MockChunk * GetRelNeighborChunk(int a_RelX, int a_RelZ) { return NULL; }

	bool IsValid(void) const { return false; }
	
	NIBBLETYPE GetTimeAlteredLight(NIBBLETYPE a_Skylight) const { return 0; }

	void BroadcastSoundParticleEffect(int a_EffectID, int a_SrcX, int a_SrcY, int a_SrcZ, int a_Data, const cClientHandle * a_Exclude = NULL) {}
	void BroadcastSoundEffect (const AString & a_SoundName, double a_X, double a_Y, double a_Z, float a_Volume, float a_Pitch, const cClientHandle * a_Exclude = NULL) {}

	bool DoWithRedstonePoweredEntityAt(int a_BlockX, int a_BlockY, int a_BlockZ, cRedstonePoweredCallback & a_Callback) { return false; }

	template <class T>
	bool DoWithChestAt(int a_BlockX, int a_BlockY, int a_BlockZ, T & a_Callback)
	{
		return false;
	}

};
class MockChest 
{
public:
	BLOCKTYPE GetBlockType(void) const { return 0; }
	int GetNumberOfPlayers(void) const { return 0; }
};

int main(int argc, char** argv)
{
	
	

	MockWorld World;

	cIncrementalRedstoneSimulator<MockChunk, MockWorld, MockHandlerFetcher, MockChest> Simulator(World);
	return 0;
}
