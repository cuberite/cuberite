
// MapSerializer.cpp


#include "Globals.h"
#include "MapSerializer.h"
#include "../StringCompression.h"
#include "zlib/zlib.h"
#include "FastNBT.h"

#include "../Map.h"
#include "../World.h"





cMapSerializer::cMapSerializer(const AString& a_WorldName, cMap * a_Map)
	: m_Map(a_Map)
{
	AString DataPath;
	Printf(DataPath, "%s/data", a_WorldName.c_str());

	Printf(m_Path, "%s/map_%i.dat", DataPath.c_str(), a_Map->GetID());

	cFile::CreateFolder(FILE_IO_PREFIX + DataPath);
}





bool cMapSerializer::Load(void)
{
	AString Data = cFile::ReadWholeFile(FILE_IO_PREFIX + m_Path);
	if (Data.empty())
	{
		return false;
	}

	AString Uncompressed;
	int res = UncompressStringGZIP(Data.data(), Data.size(), Uncompressed);

	if (res != Z_OK)
	{
		return false;
	}

	// Parse the NBT data:
	cParsedNBT NBT(Uncompressed.data(), Uncompressed.size());
	if (!NBT.IsValid())
	{
		// NBT Parsing failed
		return false;
	}

	return LoadMapFromNBT(NBT);
}





bool cMapSerializer::Save(void)
{
	cFastNBTWriter Writer;

	SaveMapToNBT(Writer);

	Writer.Finish();
	
	#ifdef _DEBUG
	cParsedNBT TestParse(Writer.GetResult().data(), Writer.GetResult().size());
	ASSERT(TestParse.IsValid());
	#endif  // _DEBUG

	cFile File;
	if (!File.Open(FILE_IO_PREFIX + m_Path, cFile::fmWrite))
	{
		return false;
	}

	AString Compressed;
	int res = CompressStringGZIP(Writer.GetResult().data(), Writer.GetResult().size(), Compressed);

	if (res != Z_OK)
	{
		return false;
	}

	File.Write(Compressed.data(), Compressed.size());
	File.Close();

	return true;
}





void cMapSerializer::SaveMapToNBT(cFastNBTWriter & a_Writer)
{
	a_Writer.BeginCompound("data");

	a_Writer.AddByte("scale", m_Map->GetScale());
	a_Writer.AddByte("dimension", (int) m_Map->GetDimension());

	a_Writer.AddShort("width",  m_Map->GetWidth());
	a_Writer.AddShort("height", m_Map->GetHeight());

	a_Writer.AddInt("xCenter", m_Map->GetCenterX());
	a_Writer.AddInt("zCenter", m_Map->GetCenterZ());

	// Potential bug - The internal representation may change
	const cMap::cColorList & Data = m_Map->GetData();
	a_Writer.AddByteArray("colors", (char *) Data.data(), Data.size());

	a_Writer.EndCompound();
}





bool cMapSerializer::LoadMapFromNBT(const cParsedNBT & a_NBT)
{
	int Data = a_NBT.FindChildByName(0, "data");
	if (Data < 0)
	{
		return false;
	}

	int CurrLine = a_NBT.FindChildByName(Data, "scale");
	if (CurrLine >= 0)
	{
		unsigned int Scale = a_NBT.GetByte(CurrLine);
		m_Map->m_Scale = Scale;
	}

	CurrLine = a_NBT.FindChildByName(Data, "dimension");
	if (CurrLine >= 0)
	{
		eDimension Dimension = (eDimension) a_NBT.GetByte(CurrLine);
		
		ASSERT(Dimension == m_Map->m_World->GetDimension());
	}

	CurrLine = a_NBT.FindChildByName(Data, "width");
	if (CurrLine >= 0)
	{
		unsigned int Width = a_NBT.GetShort(CurrLine);
		m_Map->m_Width = Width;
	}

	CurrLine = a_NBT.FindChildByName(Data, "height");
	if (CurrLine >= 0)
	{
		unsigned int Height = a_NBT.GetShort(CurrLine);
		m_Map->m_Height = Height;
	}

	CurrLine = a_NBT.FindChildByName(Data, "xCenter");
	if (CurrLine >= 0)
	{
		int CenterX = a_NBT.GetInt(CurrLine);
		m_Map->m_CenterX = CenterX;
	}

	CurrLine = a_NBT.FindChildByName(Data, "zCenter");
	if (CurrLine >= 0)
	{
		int CenterZ = a_NBT.GetInt(CurrLine);
		m_Map->m_CenterZ = CenterZ;
	}

	unsigned int NumPixels = m_Map->GetNumPixels();
	m_Map->m_Data.resize(NumPixels);

	// TODO xdot: Parse the byte array.

	return true;
}





cIDCountSerializer::cIDCountSerializer(const AString & a_WorldName) : m_MapCount(0)
{
	AString DataPath;
	Printf(DataPath, "%s/data", a_WorldName.c_str());

	Printf(m_Path, "%s/idcounts.dat", DataPath.c_str());

	cFile::CreateFolder(FILE_IO_PREFIX + DataPath);
}





bool cIDCountSerializer::Load(void)
{
	AString Data = cFile::ReadWholeFile(FILE_IO_PREFIX + m_Path);
	if (Data.empty())
	{
		return false;
	}

	// NOTE: idcounts.dat is not compressed (raw format)

	// Parse the NBT data:
	cParsedNBT NBT(Data.data(), Data.size());
	if (!NBT.IsValid())
	{
		// NBT Parsing failed
		return false;
	}

	int CurrLine = NBT.FindChildByName(0, "map");
	if (CurrLine >= 0)
	{
		m_MapCount = (int)NBT.GetShort(CurrLine);
	}

	return true;
}





bool cIDCountSerializer::Save(void)
{
	cFastNBTWriter Writer;

	Writer.AddShort("map", m_MapCount);

	Writer.Finish();
	
	#ifdef _DEBUG
	cParsedNBT TestParse(Writer.GetResult().data(), Writer.GetResult().size());
	ASSERT(TestParse.IsValid());
	#endif  // _DEBUG

	cFile File;
	if (!File.Open(FILE_IO_PREFIX + m_Path, cFile::fmWrite))
	{
		return false;
	}

	// NOTE: idcounts.dat is not compressed (raw format)

	File.Write(Writer.GetResult().data(), Writer.GetResult().size());
	File.Close();

	return true;
}








