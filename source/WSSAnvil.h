
// WSSAnvil.h

// Interfaces to the cWSSAnvil class representing the Anvil world storage scheme




#pragma once

#include "WorldStorage.h"





enum
{
	// The MCA header is 8 KiB
	MCA_HEADER_SIZE = 8192,
} ;





// fwd: "NBT.h"
class cNBTTag;





class cWSSAnvil :
	public cWSSchema
{
	typedef cWSSchema super;
	
public:

	cWSSAnvil(cWorld * a_World) : super(a_World) {}
	virtual ~cWSSAnvil();
	
protected:

	class cMCAFile
	{
	public:
	
		cMCAFile(const AString & a_FileName, int a_RegionX, int a_RegionZ);
		
		bool GetChunkData(const cChunkCoords & a_Chunk, AString & a_Data);
		
		int             GetRegionX (void) const {return m_RegionX; }
		int             GetRegionZ (void) const {return m_RegionZ; }
		const AString & GetFileName(void) const {return m_FileName; }
		
	protected:
	
		int     m_RegionX;
		int     m_RegionZ;
		cFile   m_File;
		AString m_FileName;
		
		// The header, copied from the file so we don't have to seek to it all the time
		// First 1024 entries are chunk locations - the 3 + 1 byte sector-offset and sector-count
		// The next 1024 entries are chunk timestamps - unused in MCS
		unsigned m_Header[MCA_HEADER_SIZE / sizeof(unsigned)];
	} ;
	typedef std::list<cMCAFile *> cMCAFiles;
	
	cCriticalSection m_CS;
	cMCAFiles        m_Files;  // a MRU cache of MCA files

	/// Gets chunk data from the correct file; locks CS as needed
	bool GetChunkData(const cChunkCoords & a_Chunk, AString & a_Data);

	/// Loads the chunk from the data (no locking needed)
	bool LoadChunkFromData(const cChunkCoords & a_Chunk, const AString & a_Data);
	
	/// Loads the chunk from NBT data (no locking needed)
	bool LoadChunkFromNBT(const cChunkCoords & a_Chunk, cNBTTag & a_NBT);
	
	/// Gets the correct MCA file either from cache or from disk, manages the m_MCAFiles cache; assumes m_CS is locked
	cMCAFile * LoadMCAFile(const cChunkCoords & a_Chunk);
		
	// cWSSchema overrides:
	virtual bool LoadChunk(const cChunkCoords & a_Chunk) override;
	virtual bool SaveChunk(const cChunkCoords & a_Chunk) override;
	virtual const AString GetName(void) const override {return "anvil"; }
} ;




