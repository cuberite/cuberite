
#include "Globals.h"

#include "ChunkGenerator.h"
#include "ChunkDesc.h"
#include "ComposableGenerator.h"
#include "Noise3DGenerator.h"
#include "../IniFile.h"
#include "../FastRandom.h"





void cChunkGenerator::Initialize(cIniFile & a_IniFile)
{
	// Get the seed; create a new one and log it if not found in the INI file:
	if (a_IniFile.HasValue("Seed", "Seed"))
	{
		m_Seed = a_IniFile.GetValueI("Seed", "Seed");
	}
	else
	{
		m_Seed = GetRandomProvider().RandInt();
		LOGINFO("Chosen a new random seed for world: %d", m_Seed);
		a_IniFile.SetValueI("Seed", "Seed", m_Seed);
	}

	m_Dimension = StringToDimension(a_IniFile.GetValue("General", "Dimension", "Overworld"));
}





std::unique_ptr<cChunkGenerator> cChunkGenerator::CreateFromIniFile(cIniFile & a_IniFile)
{
	// Get the generator engine based on the INI file settings:
	std::unique_ptr<cChunkGenerator> res;
	AString GeneratorName = a_IniFile.GetValueSet("Generator", "Generator", "Composable");
	if (NoCaseCompare(GeneratorName, "Noise3D") == 0)
	{
		res.reset(new cNoise3DGenerator());
	}
	else
	{
		if (NoCaseCompare(GeneratorName, "composable") != 0)
		{
			LOGWARN("[Generator]::Generator value \"%s\" not recognized, using \"Composable\".", GeneratorName.c_str());
		}
		res.reset(new cComposableGenerator());
	}

	if (res == nullptr)
	{
		LOGERROR("Generator could not start, aborting the server");
		return nullptr;
	}

	res->Initialize(a_IniFile);
	return res;
}





EMCSBiome cChunkGenerator::GetBiomeAt(int a_BlockX, int a_BlockZ)
{
	cChunkDef::BiomeMap Biomes;
	int Y = 0;
	int ChunkX, ChunkZ;
	cChunkDef::AbsoluteToRelative(a_BlockX, Y, a_BlockZ, ChunkX, ChunkZ);
	GenerateBiomes({ChunkX, ChunkZ}, Biomes);
	return cChunkDef::GetBiome(Biomes, a_BlockX, a_BlockZ);
}




