#include "MemoryLeak.h"

#include "cDeNotch.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <stdio.h>
#include <ctype.h>
#include "zlib.h"
#include "cNBTData.h"
#include "cTimer.h"
#include "cQuicksort.h"
#include "cDeNotch.h"
#include "cMakeDir.h"

#ifdef _WIN32
#include "wdirent.h"
#else
#include <dirent.h>
#endif

#ifndef _WIN32
#define sprintf_s(target, size, ... ) printf(target, __VA_ARGS__ )
#endif

inline bool fOpenFile( FILE*& a_hFile, const char* a_FileName, const char* a_Mode )
{
#ifdef _WIN32
	return fopen_s(&a_hFile, a_FileName, a_Mode ) == 0;
#else
	return (a_hFile = fopen(a_FileName, a_Mode )) != 0;
#endif
}

cDeNotch::cDeNotch ( ) 
{

}

int cDeNotch:: Converter ( std::string mcrSource, std::string pakOutput ) 
{
	std::string OutDir = "world/";

	FILE* f  = 0;
	FILE* wf = 0;

	std::string SourceFileName = mcrSource;
	std::string OutFileName = OutDir + pakOutput;

	cMakeDir::MakeDir( OutDir.c_str() );
	if( !fOpenFile(wf, OutFileName.c_str(), "wb") )
	{ 
		printf("Could not open/create file %s\n", OutFileName.c_str() );
	}

	printf ("Now Converting %s to %s\n", SourceFileName.c_str(), OutFileName.c_str() );
	if( fOpenFile(f, SourceFileName.c_str(), "rb") )
	{
		char* t_FakeHeader = new char[1*1024*1024]; //1MB Temp FakeHeader array
		int t_FakeHeaderSz  = -1; //Size of data in array

		char* t_CompChunk  = new char[5*1024*1024]; //5MB Temp Compressed Chunk Data array
		int t_CompChunkSz   = -1; //Size of data in array

		char PakVersion   = 1;
		char ChunkVersion = 1;
		short NumChunks   = 0;

		unsigned char byte1 = 0;
		unsigned char byte2 = 0;
		unsigned char byte3 = 0;
		unsigned char byte4 = 0;
		unsigned char byte5 = 0;
		unsigned char trash = 0;
		unsigned int  frloc = 0;

		int toffset     = 0;
		int compdlength = 0;

		int toffarr[1024];

		//loop through notch's header
		for( short i = 0; i < 1024 ; ++i ) 
		{	//loop through first 4096 bytes of data, 4 bytes at a time
			//Region files begin with an 8kiB header containing information about which chunks are present in the region file, when they were last updated, and where they can be found. The location in the region file of a chunk at (x, z) (in chunk coordinates) can be found at byte offset 4 * ((x mod 32) + (z mod 32) * 32) in its region file. Its timestamp can be found 4096 bytes later in the file. The remainder of the file consists of data for up to 1024 chunks, interspersed with an arbitrary amount of unused space. 
			//we are only using the first 4096 bytes. We don't need the timestamps right now.
			if( fread( &byte1, sizeof(byte1), 1, f) != 1 ) { std::cout << "ERROR 21hs READING FROM FILE " << SourceFileName; fclose(f); return false; }
			if( fread( &byte2, sizeof(byte2), 1, f) != 1 ) { std::cout << "ERROR ks93 READING FROM FILE " << SourceFileName; fclose(f); return false; }
			if( fread( &byte3, sizeof(byte3), 1, f) != 1 ) { std::cout << "ERROR 2s5f READING FROM FILE " << SourceFileName; fclose(f); return false; }//first three bytes area big-endian representation of the chunk offsets in no particular order.
			if( fread( &byte4, sizeof(byte4), 1, f) != 1 ) { std::cout << "ERROR dhj3 READING FROM FILE " << SourceFileName; fclose(f); return false; }//we don't need to use this byte right now.
			toffset = 4096 * ((byte1*256*256) + (byte2*256) + byte3);//find the chunk offsets using the first three bytes of each long;
			toffarr[i] = toffset;//array of chunk offset locations in the file.
		}
		for ( short i = 0; i < 4096; i++ ) 
		{	//loop through next 4096 bytes of the header.
			//keeping this code here in case we need it later. not using it right now.
			if( fread( &trash, sizeof(byte4), 1, f) != 1 ) { std::cout << "ERROR 2jkd READING FROM FILE " << SourceFileName; fclose(f); return false; }
		}
		frloc = 8192; //current location of fread is at 4096+ 4096 since we read through and collected important info from the header.

		cQuicksort Quick;
		Quick.quicksort(toffarr, 0, 1023); //sort the array from smallest to largest offset locations so we only have to read through the file once.

		for ( short ia = 0; ia < 1024; ia++ ) 
		{	//a region file can hold a maximum of 1024 chunks (32*32)
			if (ia < 3500 ) 
			{	//only run chunk # 3
				if (toffarr[ia] < 8192) 
				{	//offsets of less than 8192 are impossible. 0 means there is no chunk in a particular location.
					if (toffarr[ia] > 0) { std::cout << "ERROR 2s31 IN COLLECTED CHUNK OFFSETS " << toffarr[ia]; fclose(f); return false; } //values between 0 and 8192 should be impossible. 
					//This file does not contain the max 1024 chunks, skip until we get to the first
				} 
				else 
				{	// found a chunk offset value
					//Chunk data begins with a (big-endian) four-byte length field which indicates the exact length of the remaining chunk data in bytes. The following byte indicates the compression scheme used for chunk data, and the remaining (length-1) bytes are the compressed chunk data. 
					//printf("Working on chunk %i :: %i\n", ia, toffarr[ia]);
					if( fread( &byte1, sizeof(byte1), 1, f) != 1 ) { std::cout << "ERROR 2t32 READING FROM FILE " << SourceFileName; fclose(f); return false; }
					if( fread( &byte2, sizeof(byte2), 1, f) != 1 ) { std::cout << "ERROR 2y51 READING FROM FILE " << SourceFileName; fclose(f); return false; }
					if( fread( &byte3, sizeof(byte3), 1, f) != 1 ) { std::cout << "ERROR 3424 READING FROM FILE " << SourceFileName; fclose(f); return false; }
					if( fread( &byte4, sizeof(byte4), 1, f) != 1 ) { std::cout << "ERROR sd22 READING FROM FILE " << SourceFileName; fclose(f); return false; }
					compdlength = ((byte1*256*256*256) + (byte2*256*256) + (byte3*256) + byte4 - 0); //length of compressed chunk data
					if( fread( &byte5, sizeof(byte5), 1, f) != 1 ) { std::cout << "ERROR 2341 READING FROM FILE " << SourceFileName; fclose(f); return false; } //compression type, 1 = GZip (RFC1952) (unused in practice) , 2 = Zlib (RFC1950) 

					frloc += 5; //moved ahead 5 bytes while reading data.

					char* compBlockData = new char[compdlength];
					if( fread( compBlockData, compdlength, 1, f) != 1 ) { std::cout << "ERROR rf22 READING FROM FILE " << SourceFileName; fclose(f); return false; }
					frloc = frloc + compdlength;

					uLongf DestSize = 128576;// uncompressed chunks should never be larger than this

					char* BlockData = new char[ DestSize ];

					int errorcode = uncompress( (Bytef*)BlockData, &DestSize, (Bytef*)compBlockData, compdlength ); //DestSize will update to the actual uncompressed data size after this opperation.
					int testr = (int)DestSize; //testing something, can't remember what.
					if( errorcode != Z_OK ){
						printf("ERROR: Decompressing chunk data! %i", errorcode );
						switch( errorcode )
						{
							case Z_MEM_ERROR:
								printf("Not enough memory");
								break;
							case Z_BUF_ERROR:
								printf("Not enough room in output buffer");
								break;
							case Z_DATA_ERROR:
								printf("Input data corrupted or incomplete");
								break;
							default:
								break;
						};
					}

					NumChunks++;
					cNBTData* NBTData = new cNBTData(BlockData, (int)DestSize);
					NBTData->ParseData();
					//NBTData->PrintData();
					NBTData->OpenCompound("");
					NBTData->OpenCompound("Level"); // You need to open the right compounds before you can access the data in it

					//NBT Data for blocks should look something like this:
					//==== STRUCTURED NBT DATA ====
					// COMPOUND ( )
					//     COMPOUND
					//         COMPOUND (Level)
					//            LIST (Entities)
					//            LIST (TileEntities)
					//            INTEGER LastUpdate (0)
					//            INTEGER xPos (0)
					//            INTEGER zPos (0)
					//            BYTE TerrainPopulated (1)
					//            BYTE ARRAY BlockLight (length: 16384)
					//            BYTE ARRAY Blocks (length: 32768)
					//            BYTE ARRAY Data (length: 16384)
					//            BYTE ARRAY HeightMap (length: 256)
        			//		      BYTE ARRAY SkyLight (length: 16384)
					//=============================

					int UncompressedChunkSz = (32768+16384+16384+16384);
					char* UncompressedChunk = new char[ UncompressedChunkSz ];
					uLongf CompressedSize   = compressBound( UncompressedChunkSz );
					char* CompressedChunk   = new char[ CompressedSize ];
					int UnChunkArrLoc       = 0;
					int xPos                = NBTData->GetInteger("xPos");
					int zPos                = NBTData->GetInteger("zPos");
					//printf("Chunk [%i, %i]\n", xPos, zPos );

					memcpy( t_FakeHeader + t_FakeHeaderSz + 1, &xPos, sizeof(int) );t_FakeHeaderSz += sizeof(int);
					memcpy( t_FakeHeader + t_FakeHeaderSz + 1, &zPos, sizeof(int) );t_FakeHeaderSz += sizeof(int);


					//TODO: insert json code and add it to chunk data

					memcpy( UncompressedChunk + UnChunkArrLoc, NBTData->GetByteArray("Blocks"), 32768 );UnChunkArrLoc += 32768;
					memcpy( UncompressedChunk + UnChunkArrLoc, NBTData->GetByteArray("Data"), 16384 );UnChunkArrLoc += 16384;
					memcpy( UncompressedChunk + UnChunkArrLoc, NBTData->GetByteArray("BlockLight"), 16384 );UnChunkArrLoc += 16384;
					memcpy( UncompressedChunk + UnChunkArrLoc, NBTData->GetByteArray("SkyLight"), 16384 );UnChunkArrLoc += 16384;

					errorcode = compress2( (Bytef*)CompressedChunk, &CompressedSize, (const Bytef*)UncompressedChunk, UncompressedChunkSz, Z_DEFAULT_COMPRESSION);
					if( errorcode != Z_OK )
					{
						printf("Error compressing data (%i)", errorcode );
						break;
					}

					memcpy( t_FakeHeader + t_FakeHeaderSz + 1, &CompressedSize, sizeof(int) );t_FakeHeaderSz += sizeof(int);
					memcpy( t_FakeHeader + t_FakeHeaderSz + 1, &UncompressedChunkSz, sizeof(int) );t_FakeHeaderSz += sizeof(int);
					memcpy( t_CompChunk + t_CompChunkSz + 1, CompressedChunk, CompressedSize );t_CompChunkSz += CompressedSize;

					NBTData->CloseCompound();// Close the compounds after you're done
					NBTData->CloseCompound();

					delete [] UncompressedChunk;
					delete [] CompressedChunk;
					delete [] compBlockData;
					delete [] BlockData;

					while ( (frloc < toffarr[ia+1]) && (ia<1023) )
					{	//loop through Notch's junk data until we get to another chunk offset possition to start the loop again
						if( fread( &trash, sizeof(byte4), 1, f) != 1 ) { std::cout << "ERROR 2nkd READING FROM FILE " << SourceFileName; fclose(f); return false; }
						frloc ++;
					}


					delete NBTData;
				}
			} //only run chunk # 3
		}

		fwrite( &PakVersion, sizeof(PakVersion), 1, wf );
		fwrite( &ChunkVersion, sizeof(ChunkVersion), 1, wf );
		fwrite( &NumChunks, sizeof(NumChunks), 1, wf );
		fwrite( t_FakeHeader, t_FakeHeaderSz+1, 1, wf );
		fwrite( t_CompChunk, t_CompChunkSz+1, 1, wf );
		delete [] t_FakeHeader;
		delete [] t_CompChunk;

		fclose(wf); //close file.
		fclose(f); //close file.
	}
	return true;
};
