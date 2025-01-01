
// SchematicFileSerializer.cpp

// Implements the cSchematicFileSerializer class representing the interface to load and save cBlockArea to a .schematic file

#include "Globals.h"

#include "FastNBT.h"
#include "SchematicFileSerializer.h"
#include "../OSSupport/GZipFile.h"
#include "../Protocol/Palettes/Upgrade.h"





////////////////////////////////////////////////////////////////////////////////
// cSchematicFileSerializer:

void cSchematicFileSerializer::LoadFromSchematicFile(cBlockArea & a_BlockArea, const std::string & a_FileName)
{
	const auto Data = GZipFile::ReadRestOfFile(a_FileName);
	const cParsedNBT NBT(Data.GetView());

	if (!NBT.IsValid())
	{
		throw std::runtime_error(fmt::format("Cannot parse the NBT in the schematic file \"{}\".", a_FileName));
	}

	LoadFromSchematicNBT(a_BlockArea, NBT);
}





void cSchematicFileSerializer::LoadFromSchematicString(cBlockArea & a_BlockArea, const ContiguousByteBufferView a_SchematicData)
{
	const auto Extracted = Compression::Extractor().ExtractGZip(a_SchematicData);
	const cParsedNBT NBT(Extracted.GetView());

	if (!NBT.IsValid())
	{
		throw std::runtime_error("Cannot parse the NBT in the schematic data.");
	}

	LoadFromSchematicNBT(a_BlockArea, NBT);
}





void cSchematicFileSerializer::SaveToSchematicFile(const cBlockArea & a_BlockArea, const std::string & a_FileName)
{
	GZipFile::Write(a_FileName, SaveToSchematicNBT(a_BlockArea));
}





Compression::Result cSchematicFileSerializer::SaveToSchematicString(const cBlockArea & a_BlockArea)
{
	return Compression::Compressor().CompressGZip(SaveToSchematicNBT(a_BlockArea));
}





void cSchematicFileSerializer::LoadFromSchematicNBT(cBlockArea & a_BlockArea, const cParsedNBT & a_NBT)
{
	int TMaterials = a_NBT.FindChildByName(a_NBT.GetRoot(), "Materials");
	if ((TMaterials > 0) && (a_NBT.GetType(TMaterials) == TAG_String))
	{
		AString Materials = a_NBT.GetString(TMaterials);
		if (Materials.compare("Alpha") != 0)
		{
			throw std::runtime_error(fmt::format("Materials tag is present and \"{}\" instead of \"Alpha\". Possibly a wrong-format schematic file.", Materials));
		}
	}
	int TSizeX = a_NBT.FindChildByName(a_NBT.GetRoot(), "Width");
	int TSizeY = a_NBT.FindChildByName(a_NBT.GetRoot(), "Height");
	int TSizeZ = a_NBT.FindChildByName(a_NBT.GetRoot(), "Length");
	if (
		(TSizeX < 0) || (TSizeY < 0) || (TSizeZ < 0) ||
		(a_NBT.GetType(TSizeX) != TAG_Short) ||
		(a_NBT.GetType(TSizeY) != TAG_Short) ||
		(a_NBT.GetType(TSizeZ) != TAG_Short)
	)
	{
		throw std::runtime_error(fmt::format(
			"Dimensions are missing from the schematic file ({}, {}, {}), ({}, {}, {})",
			TSizeX, TSizeY, TSizeZ,
			(TSizeX >= 0) ? a_NBT.GetType(TSizeX) : -1,
			(TSizeY >= 0) ? a_NBT.GetType(TSizeY) : -1,
			(TSizeZ >= 0) ? a_NBT.GetType(TSizeZ) : -1
		));
	}

	int SizeX = a_NBT.GetShort(TSizeX);
	int SizeY = a_NBT.GetShort(TSizeY);
	int SizeZ = a_NBT.GetShort(TSizeZ);
	if ((SizeX < 1) || (SizeX > 65535) || (SizeY < 1) || (SizeY > 65535) || (SizeZ < 1) || (SizeZ > 65535))
	{
		throw std::runtime_error(fmt::format("Dimensions are invalid in the schematic file: {}, {}, {}", SizeX, SizeY, SizeZ));
	}

	int TBlockTypes = a_NBT.FindChildByName(a_NBT.GetRoot(), "Blocks");
	int TBlockMetas = a_NBT.FindChildByName(a_NBT.GetRoot(), "Data");
	if ((TBlockTypes < 0) || (a_NBT.GetType(TBlockTypes) != TAG_ByteArray))
	{
		throw std::runtime_error(fmt::format("BlockTypes are invalid in the schematic file: {}", TBlockTypes));
	}

	a_BlockArea.Clear();
	a_BlockArea.SetSize(SizeX, SizeY, SizeZ, cBlockArea::baBlocks);

	int TOffsetX = a_NBT.FindChildByName(a_NBT.GetRoot(), "WEOffsetX");
	int TOffsetY = a_NBT.FindChildByName(a_NBT.GetRoot(), "WEOffsetY");
	int TOffsetZ = a_NBT.FindChildByName(a_NBT.GetRoot(), "WEOffsetZ");

	if (
		(TOffsetX < 0) || (TOffsetY < 0) || (TOffsetZ < 0) ||
		(a_NBT.GetType(TOffsetX) != TAG_Int) ||
		(a_NBT.GetType(TOffsetY) != TAG_Int) ||
		(a_NBT.GetType(TOffsetZ) != TAG_Int)
	)
	{
		// Not every schematic file has an offset, so we shoudn't give a warn message.
		a_BlockArea.SetWEOffset(0, 0, 0);
	}
	else
	{
		a_BlockArea.SetWEOffset(a_NBT.GetInt(TOffsetX), a_NBT.GetInt(TOffsetY), a_NBT.GetInt(TOffsetZ));
	}

	// Copy the block types and metas:
	size_t NumBytes = a_BlockArea.GetBlockCount();
	if (a_NBT.GetDataLength(TBlockTypes) < NumBytes)
	{
		LOG("BlockTypes truncated in the schematic file (exp %u, got %u bytes). Loading partial.",
			static_cast<unsigned>(NumBytes), static_cast<unsigned>(a_NBT.GetDataLength(TBlockTypes))
		);
		NumBytes = a_NBT.GetDataLength(TBlockTypes);
	}

	auto BlockTypes = a_NBT.GetData(TBlockTypes);
	auto BlockMetas = a_NBT.GetData(TBlockMetas);

	auto Blocks = std::make_unique<BlockState[]>(NumBytes);

	for (size_t I = 0; I < NumBytes; I ++)
	{
		Blocks[I] = PaletteUpgrade::FromBlock(static_cast<unsigned char>(BlockTypes[I]), static_cast<unsigned char>(BlockMetas[I]));
	}

	memcpy(a_BlockArea.GetBlocks(), Blocks.get(), NumBytes);
}





ContiguousByteBuffer cSchematicFileSerializer::SaveToSchematicNBT(const cBlockArea & a_BlockArea)
{
	cFastNBTWriter Writer;
	Writer.AddShort("Width",  static_cast<Int16>(a_BlockArea.m_Size.x));
	Writer.AddShort("Height", static_cast<Int16>(a_BlockArea.m_Size.y));
	Writer.AddShort("Length", static_cast<Int16>(a_BlockArea.m_Size.z));
	Writer.AddString("Materials", "Alpha");
	if (a_BlockArea.HasBlocks())
	{
		auto Blocks = a_BlockArea.GetBlocks();

		std::string BlockData;
		std::string MetaData;

		for (size_t I = 0; I < a_BlockArea.GetBlockCount(); I++)
		{
			auto NumericBlock = PaletteUpgrade::ToBlock(Blocks[I]);
			BlockData += static_cast<char>(NumericBlock.first);
			MetaData += static_cast<char>(NumericBlock.second);
		}

		Writer.AddByteArray("Blocks", BlockData);
		Writer.AddByteArray("Data", MetaData);
	}
	else
	{
		AString Dummy(a_BlockArea.GetBlockCount(), 0);
		Writer.AddByteArray("Blocks", Dummy);
		Writer.AddByteArray("Data", Dummy);
	}

	Writer.AddInt("WEOffsetX", a_BlockArea.m_WEOffset.x);
	Writer.AddInt("WEOffsetY", a_BlockArea.m_WEOffset.y);
	Writer.AddInt("WEOffsetZ", a_BlockArea.m_WEOffset.z);

	// TODO: Save entities and block entities
	Writer.BeginList("Entities", TAG_Compound);
	Writer.EndList();
	Writer.BeginList("TileEntities", TAG_Compound);
	Writer.EndList();
	Writer.Finish();

	return ContiguousByteBuffer(Writer.GetResult());
}
