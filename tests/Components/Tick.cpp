
#include "Globals.h"
#include "Mobs/Components/BurningComponent.h"

int main(int argc, char** argv)
{
	class cMockEntity 
	{
	public:
		double GetPosX () { return 0;}
		double GetPosY () { return 0;}
		double GetPosZ () { return 0;}
		
		int GetChunkX () { return 0; }
		int GetChunkZ () { return 0; }
		
		virtual bool IsOnFire () const { return false; }
		void StartBurning(int a_TicksLeftBurning) {}
	} Entity;
	class cMockChunk 
	{
		class cMockWorld 
		{
			public:
			void QueueLightChunk(int a_ChunkX, int a_ChunkZ, cChunkCoordCallback * a_Callback = NULL) {}
			virtual int GetTimeOfDay(void) const { return 0;}
			virtual bool IsWeatherWetAt(int a_BlockX, int a_BlockZ) { return true; }
		} mutable m_World;
	public:
		bool IsLightValid(void) { return true; }
		inline NIBBLETYPE GetSkyLight (int a_RelX, int a_RelY, int a_RelZ) const { return 0; }
		cMockWorld * GetWorld(void) const { return &m_World; }
		BLOCKTYPE GetBlock (int a_BlockX, int a_BlockY, int a_BlockZ) { return 0; }
	} chunk;
	cBurningComponent<cMockEntity, cMockChunk> component(Entity);
	
	component.Tick(1.0f/20.0f, chunk);
	
	
	return 0;
}
