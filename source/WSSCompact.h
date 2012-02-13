
// WSSCompact.h

// Interfaces to the cWSSCompact class representing the "Compact" storage schema (PAK-files)





#pragma once
#ifndef WSSCOMPACT_H_INCLUDED
#define WSSCOMPACT_H_INCLUDED

#include "WorldStorage.h"





class cWSSCompact :
	public cWSSchema
{
public:
	cWSSCompact(cWorld * a_World) : cWSSchema(a_World) {}
	virtual ~cWSSCompact();
	
protected:

	struct sChunkHeader;
	typedef std::vector<sChunkHeader *> sChunkHeaders;
	
	/// Implements a cache for a single PAK file; implements lazy-write in order to be able to write multiple chunks fast
	class cPAKFile
	{
	public:
	
		cPAKFile(const AString & a_FileName, int a_LayerX, int a_LayerZ);
		~cPAKFile();

		bool SaveChunk(const cChunkPtr & a_Chunk);
		bool LoadChunk(const cChunkPtr & a_Chunk);
		
		int GetLayerX(void) const {return m_LayerX; }
		int GetLayerZ(void) const {return m_LayerZ; }
		
	protected:
	
		AString m_FileName;
		int     m_LayerX;
		int     m_LayerZ;
		
		sChunkHeaders m_ChunkHeaders;
		AString       m_DataContents;  // Data contents of the file, cached
		
		int           m_NumDirty;  // Number of chunks that were written into m_DataContents but not into the file
		
		bool LoadChunk(const cChunkPtr & a_Chunk, int a_Offset, sChunkHeader * a_Header);
		void EraseChunk(const cChunkPtr & a_Chunk);  // Erases the chunk data from m_DataContents and updates m_ChunkHeaders
		bool SaveChunkToData(const cChunkPtr & a_Chunk);  // Saves the chunk to m_DataContents, updates headers and m_NumDirty
		void SynchronizeFile(void);  // Writes m_DataContents along with the headers to file, resets m_NumDirty
	} ;
	
	typedef std::list<cPAKFile *> cPAKFiles;
	
	cPAKFiles m_PAKFiles;  // A MRU cache of PAK files
	
	/// Loads the correct PAK file either from cache or from disk, manages the m_PAKFiles cache
	cPAKFile * LoadPAKFile(const cChunkPtr & a_Chunk);
	
	// cWSSchema overrides:
	virtual bool LoadChunk(const cChunkPtr & a_Chunk) override;
	virtual bool SaveChunk(const cChunkPtr & a_Chunk) override;
	virtual const AString GetName(void) const override {return "compact"; }
} ;





#endif  // WSSCOMPACT_H_INCLUDED




