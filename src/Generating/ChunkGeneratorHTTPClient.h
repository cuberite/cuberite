#pragma once

#include "ChunkGenerator.h"
#include "../IniFile.h"





class ChunkGeneratorHTTPClient:
	public cChunkGenerator
{
	using Super = cChunkGenerator;

public:

	virtual void Initialize(cIniFile & a_IniFile) override;
	virtual void GenerateBiomes(cChunkCoords a_ChunkCoords, cChunkDef::BiomeMap & a_BiomeMap) override;
	virtual void Generate(cChunkDesc & a_ChunkDesc) override;

protected:

	void SendIniFile(void);

protected:

	AString m_Host;
	AString m_Port;
	AString m_Name;

	cIniFile m_IniFile;

	bool m_IniFileSent = false;

	std::map<cChunkCoords, cChunkDef::BiomeMap> m_GenerateBiomesCache;
};




