
bool cSchematicFileSerializer::LoadFromSchematicFile(const AString & a_FileName)
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
	
	return LoadFromSchematicNBT(NBT);
}

bool cSchematicFileSerializer::SaveToSchematicFile(const AString & a_FileName)
{
	cFastNBTWriter Writer("Schematic");
	Writer.AddShort("Width", m_SizeX);
	Writer.AddShort("Height", m_SizeY);
	Writer.AddShort("Length", m_SizeZ);
	Writer.AddString("Materials", "Alpha");
	if (HasBlockTypes())
	{
		Writer.AddByteArray("Blocks", (const char *)m_BlockTypes, GetBlockCount());
	}
	else
	{
		AString Dummy(GetBlockCount(), 0);
		Writer.AddByteArray("Blocks", Dummy.data(), Dummy.size());
	}
	if (HasBlockMetas())
	{
		Writer.AddByteArray("Data", (const char *)m_BlockMetas, GetBlockCount());
	}
	else
	{
		AString Dummy(GetBlockCount(), 0);
		Writer.AddByteArray("Data", Dummy.data(), Dummy.size());
	}
	// TODO: Save entities and block entities
	Writer.BeginList("Entities", TAG_Compound);
	Writer.EndList();
	Writer.BeginList("TileEntities", TAG_Compound);
	Writer.EndList();
	Writer.Finish();
	
	// Save to file
	cGZipFile File;
	if (!File.Open(a_FileName, cGZipFile::fmWrite))
	{
		LOG("Cannot open file \"%s\" for writing.", a_FileName.c_str());
		return false;
	}
	if (!File.Write(Writer.GetResult()))
	{
		LOG("Cannot write data to file \"%s\".", a_FileName.c_str());
		return false;
	}
	return true;
}

bool cBlockArea::LoadFromSchematicNBT(cParsedNBT & a_NBT)
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
	
	Clear();
	SetSize(SizeX, SizeY, SizeZ, AreMetasPresent ? (baTypes | baMetas) : baTypes);
	
	// Copy the block types and metas:
	int NumBytes = m_SizeX * m_SizeY * m_SizeZ;
	if (a_NBT.GetDataLength(TBlockTypes) < NumBytes)
	{
		LOG("BlockTypes truncated in the schematic file (exp %d, got %d bytes). Loading partial.",
			NumBytes, a_NBT.GetDataLength(TBlockTypes)
		);
		NumBytes = a_NBT.GetDataLength(TBlockTypes);
	}
	memcpy(m_BlockTypes, a_NBT.GetData(TBlockTypes), NumBytes);
	
	if (AreMetasPresent)
	{
		int NumBytes = m_SizeX * m_SizeY * m_SizeZ;
		if (a_NBT.GetDataLength(TBlockMetas) < NumBytes)
		{
			LOG("BlockMetas truncated in the schematic file (exp %d, got %d bytes). Loading partial.",
				NumBytes, a_NBT.GetDataLength(TBlockMetas)
			);
			NumBytes = a_NBT.GetDataLength(TBlockMetas);
		}
		memcpy(m_BlockMetas, a_NBT.GetData(TBlockMetas), NumBytes);
	}
	
	return true;
}


