
// MCADefrag.cpp

// Implements the main app entrypoint and the cMCADefrag class representing the entire app

#include "Globals.h"
#include "MCADefrag.h"
#include "MCLogger.h"





int main(int argc, char ** argv)
{
	new cMCLogger(Printf("Defrag_%08x.log", time(NULL)));
	cMCADefrag Defrag;
	if (!Defrag.Init(argc, argv))
	{
		return 1;
	}
	
	Defrag.Run();
	
	return 0;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cMCADefrag:

cMCADefrag::cMCADefrag(void) :
	m_NumThreads(1)
{
}





bool cMCADefrag::Init(int argc, char ** argv)
{
	// Nothing needed yet
	return true;
}





void cMCADefrag::Run(void)
{
	// Fill the queue with MCA files
	m_Queue = cFile::GetFolderContents(".");

	// Start the processing threads:
	for (int i = 0; i < m_NumThreads; i++)
	{
		StartThread();
	}
	
	// Wait for all the threads to finish:
	while (!m_Threads.empty())
	{
		m_Threads.front()->Wait();
		delete m_Threads.front();
		m_Threads.pop_front();
	}
}




void cMCADefrag::StartThread(void)
{
	cThread * Thread = new cThread(*this);
	m_Threads.push_back(Thread);
	Thread->Start();
}





AString cMCADefrag::GetNextFileName(void)
{
	cCSLock Lock(m_CS);
	if (m_Queue.empty())
	{
		return AString();
	}
	AString res = m_Queue.back();
	m_Queue.pop_back();
	return res;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cMCADefrag::cThread:

cMCADefrag::cThread::cThread(cMCADefrag & a_Parent) :
	super("MCADefrag thread"),
	m_Parent(a_Parent)
{
}





void cMCADefrag::cThread::Execute(void)
{
	for (;;)
	{
		AString FileName = m_Parent.GetNextFileName();
		if (FileName.empty())
		{
			return;
		}
		ProcessFile(FileName);
	}
}





void cMCADefrag::cThread::ProcessFile(const AString & a_FileName)
{
	// Filter out non-MCA files:
	if ((a_FileName.length() < 4) || (a_FileName.substr(a_FileName.length() - 4, 4) != ".mca"))
	{
		return;
	}
	LOGINFO("%s", a_FileName.c_str());
	
	// Open input and output files:
	AString OutFileName = a_FileName + ".new";
	cFile In, Out;
	if (!In.Open(a_FileName, cFile::fmRead))
	{
		LOGWARNING("Cannot open file %s for reading, skipping file.", a_FileName.c_str());
		return;
	}
	if (!Out.Open(OutFileName.c_str(), cFile::fmWrite))
	{
		LOGWARNING("Cannot open file %s for writing, skipping file.", OutFileName.c_str());
		return;
	}
	
	// Read the Locations and Timestamps from the input file:
	Byte Locations[4096];
	UInt32 Timestamps[1024];
	if (In.Read(Locations, sizeof(Locations)) != sizeof(Locations))
	{
		LOGWARNING("Cannot read Locations in file %s, skipping file.", a_FileName.c_str());
		return;
	}
	if (In.Read(Timestamps, sizeof(Timestamps)) != sizeof(Timestamps))
	{
		LOGWARNING("Cannot read Timestamps in file %s, skipping file.", a_FileName.c_str());
		return;
	}
	
	// Write dummy Locations to the Out file (will be overwritten once the correct ones are known)
	if (Out.Write(Locations, sizeof(Locations)) != sizeof(Locations))
	{
		LOGWARNING("Cannot write Locations to file %s, skipping file.", OutFileName.c_str());
		return;
	}
	m_CurrentSectorOut = 2;
	
	// Write a copy of the Timestamps into the Out file:
	if (Out.Write(Timestamps, sizeof(Timestamps)) != sizeof(Timestamps))
	{
		LOGWARNING("Cannot write Timestamps to file %s, skipping file.", OutFileName.c_str());
		return;
	}
	
	// Process each chunk:
	for (size_t i = 0; i < 1024; i++)
	{
		size_t idx = i * 4;
		if (
			(Locations[idx] == 0) &&
			(Locations[idx + 1] == 0) &&
			(Locations[idx + 2] == 0) &&
			(Locations[idx + 3] == 0)
		)
		{
			// Chunk not present
			continue;
		}
		if (!ReadChunk(In, Locations + idx))
		{
			LOGWARNING("Cannot read chunk #%d from file %s. Skipping file.", i, a_FileName.c_str());
			return;
		}
		if (!WriteChunk(Out, Locations + idx))
		{
			LOGWARNING("Cannot write chunk #%d to file %s. Skipping file.", i, OutFileName.c_str());
			return;
		}
	}
	
	// Write the new Locations into the MCA header:
	Out.Seek(0);
	if (Out.Write(Locations, sizeof(Locations)) != sizeof(Locations))
	{
		LOGWARNING("Cannot write updated Locations to file %s, skipping file.", OutFileName.c_str());
		return;
	}
	
	// Close the files, delete orig, rename new:
	In.Close();
	Out.Close();
	cFile::Delete(a_FileName);
	cFile::Rename(OutFileName, a_FileName);
}





bool cMCADefrag::cThread::ReadChunk(cFile & a_File, const Byte * a_LocationRaw)
{
	int SectorNum = (a_LocationRaw[0] << 16) | (a_LocationRaw[1] << 8) | a_LocationRaw[2];
	int SizeInSectors = a_LocationRaw[3] * (4 KiB);
	if (a_File.Seek(SectorNum * (4 KiB)) < 0)
	{
		LOGWARNING("Failed to seek to chunk data - file pos %llu (%d KiB, %.02f MiB)!", (Int64)SectorNum * (4 KiB), SectorNum * 4, ((double)SectorNum) / 256);
		return false;
	}
	
	// Read the exact size:
	Byte Buf[4];
	if (a_File.Read(Buf, 4) != 4)
	{
		LOGWARNING("Failed to read chunk data length");
		return false;
	}
	m_CompressedChunkDataSize = (Buf[0] << 24) | (Buf[1] << 16) | (Buf[2] << 8) | Buf[3];
	if (m_CompressedChunkDataSize > SizeInSectors)
	{
		LOGWARNING("Invalid chunk data - SizeInSectors (%d) smaller that RealSize (%d)", SizeInSectors, m_CompressedChunkDataSize);
		return false;
	}
	
	// Read the data:
	if (a_File.Read(m_CompressedChunkData, m_CompressedChunkDataSize) != m_CompressedChunkDataSize)
	{
		LOGWARNING("Failed to read chunk data!");
		return false;
	}
	
	// TODO: Uncompress the data if recompression is active
	
	return true;
}





bool cMCADefrag::cThread::WriteChunk(cFile & a_File, Byte * a_LocationRaw)
{
	// TODO: Recompress the data if recompression is active
	
	a_LocationRaw[0] = m_CurrentSectorOut >> 16;
	a_LocationRaw[1] = (m_CurrentSectorOut >> 8) & 0xff;
	a_LocationRaw[2] = m_CurrentSectorOut & 0xff;
	a_LocationRaw[3] = (m_CompressedChunkDataSize + (4 KiB) - 1) / (4 KiB);
	
	// Write the data length:
	Byte Buf[4];
	Buf[0] = m_CompressedChunkDataSize >> 24;
	Buf[1] = (m_CompressedChunkDataSize >> 16) & 0xff;
	Buf[2] = (m_CompressedChunkDataSize >> 8) & 0xff;
	Buf[3] = m_CompressedChunkDataSize & 0xff;
	if (a_File.Write(Buf, 4) != 4)
	{
		LOGWARNING("Failed to write chunk length!");
		return false;
	}
	
	// Write the data:
	if (a_File.Write(m_CompressedChunkData, m_CompressedChunkDataSize) != m_CompressedChunkDataSize)
	{
		LOGWARNING("Failed to write chunk data!");
		return false;
	}
	return true;
}




