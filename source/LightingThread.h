
// LightingThread.h

// Interfaces to the cLightingThread class representing the thread that processes requests for lighting
// Note that the world generators need direct access to the lighting methods so that they can light the generated chunk




#pragma once

#include "cIsThread.h"





// fwd:
class cWorld;





class cLightingThread :
	public cIsThread
{
	typedef cIsThread super;
	
public:

	class cLightingBuffer
	{
	public:
		cLightingBuffer(int m_MinX, int m_MaxX, int m_MinY, int m_MaxY, int m_MinZ, int m_MaxZ);

		/// Copies the world's existing chunks into m_BlockData, m_Skylight and m_BlockLight
		void GetFromWorld(cWorld * a_World);
		void SetToWorld  (cWorld * a_World);
		
		void Process(void);  // Does the actual lighting on this buffer
		
	protected:
	
		// Block coords:
		int m_MinX, m_MaxX;
		int m_MinY, m_MaxY;
		int m_MinZ, m_MaxZ;

		int m_StrideX;  // = OffsetOfBlock(x, y, z) - OffsetOfBlock(x + 1, y, z)
		int m_StrideZ;  // = OffsetOfBlock(x, y, z) - OffsetOfBlock(x,     y, z + 1)
		
		// These buffers actually store 1 block in each direction more than is specified in the coords
		// This way we can throw out a lot of conditions inside the processing cycles
		// And it allows us to light a chunk with regard to its surrounding chunks without much work
		// (So if m_MinX is 16 and m_MaxX is 32, the buffers actually contain data for X in range from 15 to 33 (18 items)
		char * m_BlockData;
		char * m_SkyLight;
		char * m_BlockLight;
	} ;
	
	cLightingThread(void);
	~cLightingThread();
	
	void Stop(void);
	
	void QueueLighting(cWorld * a_World, int a_MinX, int a_MaxX, int a_MinY, int a_MaxY, int a_MinZ, int a_MaxZ);  // queues the request

protected:

	typedef std::list<cLightingBuffer *> cLightingBufferQueue;
	
	cCriticalSection     m_CS;
	cLightingBufferQueue m_Queue;
	cEvent               m_Event;  // Set when queue is appended or to stop the thread
	
	virtual void Execute(void) override;
} ;




