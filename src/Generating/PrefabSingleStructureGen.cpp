
#include "Globals.h"
#include "PrefabSingleStructureGen.h"

////////////////////////////////////////////////////////////////////////////////
// cPrefabSinglePiece


cPrefabSinglePiece::cPrefabSinglePiece()
{
}






void cPrefabSinglePiece::ReadFromFile(AString a_Filename)
{
	// TODO
}


////////////////////////////////////////////////////////////////////////////////
// cPrefabSingleStructure

cPrefabSingleStructure::cPrefabSingleStructure
	(
		int a_Seed,
		int a_GridX, int a_GridZ,
		int a_OriginX, int a_OriginZ,
		cTerrainHeightGenPtr a_HeightGen
	) :
	Super(a_GridX, a_GridZ, a_OriginX, a_OriginZ),
	m_Seed(a_Seed),
	m_Noise(a_Seed),
	m_HeightGen(std::move(a_HeightGen))
{

}





void cPrefabSingleStructure::DrawIntoChunk(cChunkDesc &a_ChunkDesc)
{

}


////////////////////////////////////////////////////////////////////////////////
// cPrefabSingleStructureGen

cPrefabSingleStructureGen::cPrefabSingleStructureGen(
	int a_Seed,
	int a_GridSize,
	int a_MaxOffset,
	int a_MaxDepth,
	cBiomeGenPtr a_BiomeGen,
	cTerrainHeightGenPtr a_HeightGen,
	int a_SeaLevel,
	enum PrefabSingleStructure a_Type
) :
	// the maximum size is chosen at random ATM - working on making at dependent on a_Type
	Super(a_Seed, a_GridSize, a_GridSize, a_MaxOffset, a_MaxOffset, 50, 50, 100),
	m_RandNoise(a_Seed),
	m_MaxDepth(a_MaxDepth),
	m_BiomeGen(std::move(a_BiomeGen)),
	m_HeightGen(std::move(a_HeightGen)),
	m_Piece()
{
	// load prefab dependent on m_Type
	AString ToLoad;
	switch (m_Type)
	{
		case PrefabSingleStructure::DesertTemple: ToLoad = "DesertTemple"; break;
		case PrefabSingleStructure::DesertWell:   ToLoad = "DesertWell"; break;
		case PrefabSingleStructure::JungleTemple: ToLoad = "JungleTemple"; break;
		case PrefabSingleStructure::WitchHut:     ToLoad = "WitchHut"; break;
	}
	// auto prefab = std::make_shared<>()
	auto FileName = Printf("Prefabs%sSingleStructure%s%s.cubeset", cFile::GetPathSeparator().c_str(), cFile::GetPathSeparator().c_str(), ToLoad.c_str());
	m_Piece.ReadFromFile(FileName);

}





cGridStructGen::cStructurePtr cPrefabSingleStructureGen::CreateStructure(int a_GridX, int a_GridZ, int a_OriginX, int a_OriginZ)
{
	return nullptr;
}
