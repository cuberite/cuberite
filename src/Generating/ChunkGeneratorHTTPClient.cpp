
#include "Globals.h"

//  For cChunkGenerator::CreateFromIniFile
#include "ComposableGenerator.h"
#include "Noise3DGenerator.h"

#include "ChunkGeneratorHTTPClient.h"
#include "../HTTP/UrlClient.h"
#include "../Generating/ChunkDesc.h"
#include "../WorldStorage/SchematicFileSerializer.h"
#include "../WorldStorage/WSSAnvil.h"
#include "../WorldStorage/FastNBT.h"
#include "../WorldStorage/NBTChunkSerializer.h"
#include "../StringCompression.h"
#include "../DeadlockDetect.h"
#include "../World.h"





static constexpr char DEFAULT_HOST[] = "localhost";
static constexpr char DEFAULT_PORT[] = "5555";





class ChunkGeneratorHTTPClientCallbacks:
	public cUrlClient::cCallbacks
{
public:
	ChunkGeneratorHTTPClientCallbacks(std::shared_ptr<cEvent> a_Event, std::shared_ptr<AString> a_Data):
		m_Event(std::move(a_Event)),
		m_Data(std::move(a_Data))
	{
		LOGD("Created a cCallbacks instance at %p", reinterpret_cast<void *>(this));
	}


	virtual ~ChunkGeneratorHTTPClientCallbacks() override
	{
		LOGD("Deleting the cCallbacks instance at %p", reinterpret_cast<void *>(this));
	}

	virtual void OnBodyData(const void * a_Data, size_t a_Size) override
	{
		m_Data->append((const char *)a_Data, a_Size);
	}

	virtual void OnBodyFinished() override
	{
		LOGD("Body finished.");
		m_Event->Set();
	}


	virtual void OnError(const AString & a_ErrorMsg) override
	{
		LOG("Error: %s", a_ErrorMsg.c_str());
		m_Event->Set();
	}

protected:
	std::shared_ptr<cEvent> m_Event;
	std::shared_ptr<AString> m_Data;
};





void ChunkGeneratorHTTPClient::SendIniFile(void)
{
	AString URL = fmt::format("http://{}:{}/{}/Initialize", m_Host, m_Port, m_Name);

	AString INI;
	m_IniFile.WriteString(INI);

	auto evtFinished = std::make_shared<cEvent>();
	auto Data = std::make_shared<AString>();
	auto callbacks = std::make_unique<ChunkGeneratorHTTPClientCallbacks>(evtFinished, Data);
	auto res = cUrlClient::Put(URL, std::move(callbacks), AStringMap(), std::move(INI), AStringMap());

	if (res.first)
	{
		evtFinished->Wait();
		LOGD("Response : %s", Data->c_str());

		m_IniFileSent = (*Data == AString("OK"));
	}
	else
	{
		LOG("Immediate error: %s", res.second.c_str());
		m_IniFileSent = false;
	}
}





void ChunkGeneratorHTTPClient::Initialize(cIniFile & a_IniFile)
{
	Super::Initialize(a_IniFile);

	/* Copy */
	m_IniFile = a_IniFile;

	/* Extract config */
	m_Host = m_IniFile.GetValue("ChunkGeneratorHTTPClient", "host", DEFAULT_HOST);
	m_Port = m_IniFile.GetValue("ChunkGeneratorHTTPClient", "port", DEFAULT_PORT);

	const AString dim = m_IniFile.GetValue("General", "Dimension", "Unknown");

	m_Name = m_IniFile.GetValue("ChunkGeneratorHTTPClient", "name", fmt::format("{}-{}", dim, m_Seed));

	/* Delete HTTP Client config from INI file before passing to the server */
	m_IniFile.DeleteKey("ChunkGeneratorHTTPClient");

	/* Send INI file to the server */
	SendIniFile();
}





void ChunkGeneratorHTTPClient::GenerateBiomes(cChunkCoords a_ChunkCoords, cChunkDef::BiomeMap & a_BiomeMap)
{
	if (!m_IniFileSent)
	{
		SendIniFile();
	}

	if (m_GenerateBiomesCache.find(a_ChunkCoords) == m_GenerateBiomesCache.end())
	{
		AString URL = fmt::format("http://{}:{}/{}/GenerateBiomes/{};{}",
			m_Host, m_Port, m_Name,
			a_ChunkCoords.m_ChunkX, a_ChunkCoords.m_ChunkZ);

		auto evtFinished = std::make_shared<cEvent>();
		auto Data = std::make_shared<AString>();
		auto callbacks = std::make_unique<ChunkGeneratorHTTPClientCallbacks>(evtFinished, Data);
		auto res = cUrlClient::Get(URL, std::move(callbacks));

		if (res.first)
		{
			evtFinished->Wait();
			LOGD("Response size : %lld", (long long)Data->size());

			AString UncrompressedData;
			UncompressStringGZIP(Data->data(), Data->size(), UncrompressedData);
			cParsedNBT NBT(UncrompressedData.data(), UncrompressedData.size());;
			NBTChunkSerializer::deserializeBiomeMap(m_GenerateBiomesCache[a_ChunkCoords], NBT);
		}
		else
		{
			LOGD("Immediate error: %s", res.second.c_str());
		}
	}

	std::memcpy(a_BiomeMap, m_GenerateBiomesCache[a_ChunkCoords], sizeof(a_BiomeMap));
}





void ChunkGeneratorHTTPClient::Generate(cChunkDesc & a_ChunkDesc)
{
	if (!m_IniFileSent)
	{
		SendIniFile();
	}

	AString URL = fmt::format("http://{}:{}/{}/Generate/{};{}",
		m_Host, m_Port, m_Name,
		a_ChunkDesc.GetChunkX(), a_ChunkDesc.GetChunkZ());

	auto evtFinished = std::make_shared<cEvent>();
	auto Data = std::make_shared<AString>();
	auto callbacks = std::make_unique<ChunkGeneratorHTTPClientCallbacks>(evtFinished, Data);
	auto res = cUrlClient::Get(URL, std::move(callbacks));

	if (res.first)
	{
		evtFinished->Wait();
		LOGD("Response size : %lld", (long long)Data->size());

		AString UncrompressedData;
		UncompressStringGZIP(Data->data(), Data->size(), UncrompressedData);
		cParsedNBT NBT(UncrompressedData.data(), UncrompressedData.size());
		static cDeadlockDetect DD;
		static cWorld DummyWorld(AString("Dummy"), AString("Dummy"), DD, AStringVector());
		static cWSSAnvil Anvil(&DummyWorld, 0);
		Anvil.LoadChunkDescFromNBT(a_ChunkDesc, NBT);
	}
	else
	{
		LOG("Immediate error: %s", res.second.c_str());
	}
}





std::unique_ptr<cChunkGenerator> cChunkGenerator::CreateFromIniFile(cIniFile & a_IniFile, bool a_EnableHTTP)
{
	std::unique_ptr<cChunkGenerator> res;

	if (a_EnableHTTP && (cIniFile::noID != a_IniFile.FindKey("ChunkGeneratorHTTPClient")))
	{
		res.reset(new ChunkGeneratorHTTPClient());
	}
	else
	{
		// Get the generator engine based on the INI file settings:
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
	}

	if (res == nullptr)
	{
		LOGERROR("Generator could not start, aborting the server");
		return nullptr;
	}

	res->Initialize(a_IniFile);
	return res;
}
