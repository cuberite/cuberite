
// SchematicFileSerializer.cpp

// Implements the cSchematicFileSerializer class representing the interface to load and save cBlockArea to a .schematic file

#include "Globals.h"

#include "OSSupport/GZipFile.h"
#include "FastNBT.h"
#include "SchematicFileSerializer.h"
#include "../StringCompression.h"





bool cSchematicFileSerializer::LoadFromSchematicFile(cBlockArea & a_BlockArea, const AString & a_FileName)
{
	// Un-GZip the contents:
	AString Contents;
	cGZipFile File;
	if (!File.Open(a_FileName, cGZipFile::fmRead))
	{
		LOG("Cannot open the schematic file \"%s\".", a_FileName.c_str());
		return false;
	}
	int NumBytesRead = File.ReadRestOfFile(Contents);
	if (NumBytesRead < 0)
	{
		LOG("Cannot read GZipped data in the schematic file \"%s\", error %d", a_FileName.c_str(), NumBytesRead);
		return false;
	}
	File.Close();
	
	// Parse the NBT:
	cParsedNBT NBT(Contents.data(), Contents.size());
	if (!NBT.IsValid())
	{
		LOG("Cannot parse the NBT in the schematic file \"%s\".", a_FileName.c_str());
		return false;
	}
	
	return LoadFromSchematicNBT(a_BlockArea, NBT);
}






bool cSchematicFileSerializer::LoadFromSchematicString(cBlockArea & a_BlockArea, const AString & a_SchematicData)
{
	// Uncompress the data:
	AString UngzippedData;
	if (UncompressStringGZIP(a_SchematicData.data(), a_SchematicData.size(), UngzippedData) != Z_OK)
	{
		LOG("%s: Cannot unGZip the schematic data.", __FUNCTION__);
		return false;
	}

	// Parse the NBT:
	cParsedNBT NBT(UngzippedData.data(), UngzippedData.size());
	if (!NBT.IsValid())
	{
		LOG("%s: Cannot parse the NBT in the schematic data.", __FUNCTION__);
		return false;
	}
	
	return LoadFromSchematicNBT(a_BlockArea, NBT);
}





bool cSchematicFileSerializer::SaveToSchematicFile(const cBlockArea & a_BlockArea, const AString & a_FileName)
{
	// Serialize into NBT data:
	AString NBT = SaveToSchematicNBT(a_BlockArea);
	if (NBT.empty())
	{
		LOG("%s: Cannot serialize the area into an NBT representation for file \"%s\".", __FUNCTION__, a_FileName.c_str());
		return false;
	}
	
	// Save to file
	cGZipFile File;
	if (!File.Open(a_FileName, cGZipFile::fmWrite))
	{
		LOG("%s: Cannot open file \"%s\" for writing.", __FUNCTION__, a_FileName.c_str());
		return false;
	}
	if (!File.Write(NBT))
	{
		LOG("%s: Cannot write data to file \"%s\".", __FUNCTION__, a_FileName.c_str());
		return false;
	}
	return true;
}






bool cSchematicFileSerializer::SaveToSchematicString(const cBlockArea & a_BlockArea, AString & a_Out)
{
	// Serialize into NBT data:
	AString NBT = SaveToSchematicNBT(a_BlockArea);
	if (NBT.empty())
	{
		LOG("%s: Cannot serialize the area into an NBT representation.", __FUNCTION__);
		return false;
	}
	
	// Gzip the data:
	int res = CompressStringGZIP(NBT.data(), NBT.size(), a_Out);
	if (res != Z_OK)
	{
		LOG("%s: Cannot Gzip the area data NBT representation: %d", __FUNCTION__, res);
		return false;
	}
	return true;
}





bool cSchematicFileSerializer::LoadFromSchematicNBT(cBlockArea & a_BlockArea, cParsedNBT & a_NBT)
{
	int TMaterials = a_NBT.FindChildByName(a_NBT.GetRoot(), "Materials");
	if ((TMaterials > 0) && (a_NBT.GetType(TMaterials) == TAG_String))
	{
		AString Materials = a_NBT.GetString(TMaterials);
		if (Materials.compare("Alpha") != 0)
		{
			LOG("Materials tag is present and \"%s\" instead of \"Alpha\". Possibly a wrong-format schematic file.", Materials.c_str());
			return false;
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
		LOG("Dimensions are missing from the schematic file (%d, %d, %d), (%d, %d, %d)",
			TSizeX, TSizeY, TSizeZ,
			a_NBT.GetType(TSizeX), a_NBT.GetType(TSizeY), a_NBT.GetType(TSizeZ)
		);
		return false;
	}
	
	int SizeX = a_NBT.GetShort(TSizeX);
	int SizeY = a_NBT.GetShort(TSizeY);
	int SizeZ = a_NBT.GetShort(TSizeZ);
	if ((SizeX < 1) || (SizeY < 1) || (SizeZ < 1))
	{
		LOG("Dimensions are invalid in the schematic file: %d, %d, %d", SizeX, SizeY, SizeZ);
		return false;
	}
	
	int TBlockTypes = a_NBT.FindChildByName(a_NBT.GetRoot(), "Blocks");
	int TBlockMetas = a_NBT.FindChildByName(a_NBT.GetRoot(), "Data");
	if ((TBlockTypes < 0) || (a_NBT.GetType(TBlockTypes) != TAG_ByteArray))
	{
		LOG("BlockTypes are invalid in the schematic file: %d", TBlockTypes);
		return false;
	}
	bool AreMetasPresent = (TBlockMetas > 0) && (a_NBT.GetType(TBlockMetas) == TAG_ByteArray);
	
	a_BlockArea.Clear();
	a_BlockArea.SetSize(SizeX, SizeY, SizeZ, AreMetasPresent ? (cBlockArea::baTypes | cBlockArea::baMetas) : cBlockArea::baTypes);
	
	// Copy the block types and metas:
	int NumBytes = a_BlockArea.m_SizeX * a_BlockArea.m_SizeY * a_BlockArea.m_SizeZ;
	if (a_NBT.GetDataLength(TBlockTypes) < NumBytes)
	{
		LOG("BlockTypes truncated in the schematic file (exp %d, got %d bytes). Loading partial.",
			NumBytes, a_NBT.GetDataLength(TBlockTypes)
		);
		NumBytes = a_NBT.GetDataLength(TBlockTypes);
	}
	memcpy(a_BlockArea.m_BlockTypes, a_NBT.GetData(TBlockTypes), NumBytes);
	
	if (AreMetasPresent)
	{
		int NumBytes = a_BlockArea.m_SizeX * a_BlockArea.m_SizeY * a_BlockArea.m_SizeZ;
		if (a_NBT.GetDataLength(TBlockMetas) < NumBytes)
		{
			LOG("BlockMetas truncated in the schematic file (exp %d, got %d bytes). Loading partial.",
				NumBytes, a_NBT.GetDataLength(TBlockMetas)
			);
			NumBytes = a_NBT.GetDataLength(TBlockMetas);
		}
		memcpy(a_BlockArea.m_BlockMetas, a_NBT.GetData(TBlockMetas), NumBytes);
	}
	
	return true;
}





AString cSchematicFileSerializer::SaveToSchematicNBT(const cBlockArea & a_BlockArea)
{
	cFastNBTWriter Writer("Schematic");
	Writer.AddShort("Width",  a_BlockArea.m_SizeX);
	Writer.AddShort("Height", a_BlockArea.m_SizeY);
	Writer.AddShort("Length", a_BlockArea.m_SizeZ);
	Writer.AddString("Materials", "Alpha");
	if (a_BlockArea.HasBlockTypes())
	{
		Writer.AddByteArray("Blocks", (const char *)a_BlockArea.m_BlockTypes, a_BlockArea.GetBlockCount());
	}
	else
	{
		AString Dummy(a_BlockArea.GetBlockCount(), 0);
		Writer.AddByteArray("Blocks", Dummy.data(), Dummy.size());
	}
	if (a_BlockArea.HasBlockMetas())
	{
		Writer.AddByteArray("Data", (const char *)a_BlockArea.m_BlockMetas, a_BlockArea.GetBlockCount());
	}
	else
	{
		AString Dummy(a_BlockArea.GetBlockCount(), 0);
		Writer.AddByteArray("Data", Dummy.data(), Dummy.size());
	}
	
	// TODO: Save entities and block entities
	Writer.BeginList("Entities", TAG_Compound);
	Writer.EndList();
	Writer.BeginList("TileEntities", TAG_Compound);
	Writer.EndList();
	Writer.Finish();
	
	return Writer.GetResult();
}




