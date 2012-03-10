
// LightingThread.cpp

// Implements the cLightingThread class representing the thread that processes requests for lighting

#include "Globals.h"
#include "LightingThread.h"





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cLightingThread:

cLightingThread::cLightingThread(void) :
	super("cLightingThread")
{
}





cLightingThread::~cLightingThread()
{
	Stop();
}





void cLightingThread::Stop(void)
{
	{
		cCSLock Lock(m_CS);
		for (cLightingBufferQueue::iterator itr = m_Queue.begin(); itr != m_Queue.end(); ++itr)
		{
			delete *itr;
		}  // for itr - m_Queue[]
	}
	m_ShouldTerminate = true;
	m_Event.Set();
	
	Wait();
}





void cLightingThread::QueueLighting(cWorld * a_World, int a_MinX, int a_MaxX, int a_MinY, int a_MaxY, int a_MinZ, int a_MaxZ)
{
	// TODO
}





void cLightingThread::Execute(void)
{
	// TODO
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cLightingThread::cLightingBuffer:

cLightingThread::cLightingBuffer::cLightingBuffer(int a_MinX, int a_MaxX, int a_MinY, int a_MaxY, int a_MinZ, int a_MaxZ) :
	m_MinX(a_MinX), 
	m_MaxX(a_MaxX),
	m_MinY(a_MinY),
	m_MaxY(a_MaxY),
	m_MinZ(a_MinZ),
	m_MaxZ(a_MaxZ)
{
	// TODO: initialize strides
}





void cLightingThread::cLightingBuffer::GetFromWorld(cWorld * a_World)
{
	// TODO: Set m_BlockData, m_SkyLight and m_BlockLight from the world's chunks
}





void cLightingThread::cLightingBuffer::SetToWorld  (cWorld * a_World)
{
	// TODO: Set world's chunks from m_BlockData, m_SkyLight and m_BlockLight
}





void cLightingThread::cLightingBuffer::Process(void)
{
	// TODO: Does the actual lighting on this buffer
}




