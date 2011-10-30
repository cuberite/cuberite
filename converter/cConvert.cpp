// reading a complete binary file
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "zlib.h"
#include <time.h>
#include "cNBTData.cpp"

void quicksort(int*, int, int);
int partition(int*, int, int, int);
int median3(int*,int,int);
void swap(int &, int &);
double diffclock(clock_t, clock_t);

using namespace std;


int main () {

	char SourceFile[128];
        char OutputFile[128];
	//ifstream file ("region/r.0.0.mcr", ios::in|ios::binary|ios::ate);

	clock_t begin=clock(); //start execution timer

	//need to add code to search through region/ directory and load contents of filenames into array.
	//for each file in array do the following:
	FILE* f  = 0;
        FILE* wf = 0;
	#ifdef _WIN32
		sprintf_s(SourceFile, 128, "region/%s","r.0.0.mcr"); //replace hard coded file with file array variable
                sprintf_s(OutputFile, 128, "world/%s","X0_Z0.pak"); //parce x and z from file array variable and place into pak file format
		if( fopen_s(&wf, OutputFile, "wb" ) == 0 ) {} else { cout << "uhoh!" << endl; return 0; } //open new pak file for writing
	#else
		sprintf(SourceFile, "region/%s","r.0.0.mcr"); //same as above but for linux
                sprintf(OutputFile, "world/%s","X0_Z0.pak");
		if( (wf = fopen(OutputFile, "wb" )) != 0 ) {} else { cout << "uhoh!" << endl; return 0; }
	#endif


	if( (f = fopen(SourceFile, "rb" )) != 0 ) {	// no error

	//loop through notch's header
		int n;
		unsigned char byte1 = 0;
		unsigned char byte2 = 0;
		unsigned char byte3 = 0;
                unsigned char byte4 = 0;
                unsigned char byte5 = 0;
                unsigned char trash = 0;
		unsigned int  frloc = 0;
		int toffset = 0;
		int compdlength = 0;
		//string comp_data;
		string ucomp_date;
		int toffarr[1024];
		//unsigned char *BlockData;
                //unsigned char *comp_data;

		//cout << sizeof(byte1) << endl;
		//return 0;
		for( short i = 0; i < 1024 ; ++i ) {//loop through first 4096 bytes of data, 4 bytes at a time
			//Region files begin with an 8kiB header containing information about which chunks are present in the region file, when they were last updated, and where they can be found. The location in the region file of a chunk at (x, z) (in chunk coordinates) can be found at byte offset 4 * ((x mod 32) + (z mod 32) * 32) in its region file. Its timestamp can be found 4096 bytes later in the file. The remainder of the file consists of data for up to 1024 chunks, interspersed with an arbitrary amount of unused space. 
			//we are only using the first 4096 bytes. We don't need the timestamps right now.
			if( fread( &byte1, sizeof(byte1), 1, f) != 1 ) { cout << "ERROR 21hs READING FROM FILE " << SourceFile; fclose(f); return false; }
                        if( fread( &byte2, sizeof(byte2), 1, f) != 1 ) { cout << "ERROR ks93 READING FROM FILE " << SourceFile; fclose(f); return false; }
                        if( fread( &byte3, sizeof(byte3), 1, f) != 1 ) { cout << "ERROR 2s5f READING FROM FILE " << SourceFile; fclose(f); return false; }//first three bytes area big-endian representation of the chunk offsets in no particular order.
                        if( fread( &byte4, sizeof(byte4), 1, f) != 1 ) { cout << "ERROR dhj3 READING FROM FILE " << SourceFile; fclose(f); return false; }//we don't need to use this byte right now.
			toffset = 4096 * ((byte1*256*256) + (byte2*256) + byte3);//find the chunk offsets using the first three bytes of each long;
			toffarr[i] = toffset;//array of chunk offset locatiosn in the fle.
		}
		for ( short i = 0; i < 4096; i++ ) {//loop through next 4096 bytes of the header. 
			//keeping this code here in case we need it later. not using it right now.
			if( fread( &trash, sizeof(byte4), 1, f) != 1 ) { cout << "ERROR 2jkd READING FROM FILE " << SourceFile; fclose(f); return false; }
		}
		frloc = 8192; //current location of fread is at 4096+ 4096 since we read through and collected important info from the header.
                quicksort(toffarr, 0, 1023); //sort the array from smallest to larget offset locations so we only have to read through the file once.

                for ( short ia = 0; ia < 1024; ia++ ) {//a region file can hold a maximum of 1024 chunks (32*32)
			if (toffarr[ia] < 8192) { //offsets of less than 8192 are impossible. 0 means there is no chunk in a particular location.
				if (toffarr[ia] > 0) { cout << "ERROR 2s31 IN COLLECTED CHUNK OFFSETS " << toffarr[ia]; fclose(f); return false; } //values between 0 and 8192 should be impossible. 
				//This file does not contain the max 1024 chunks, skip until we get to the first
			} else { // found a chunk offset value
				//Chunk data begins with a (big-endian) four-byte length field which indicates the exact length of the remaining chunk data in bytes. The following byte indicates the compression scheme used for chunk data, and the remaining (length-1) bytes are the compressed chunk data. 
                        	if( fread( &byte1, sizeof(byte1), 1, f) != 1 ) { cout << "ERROR 2t32 READING FROM FILE " << SourceFile; fclose(f); return false; }
                        	if( fread( &byte2, sizeof(byte2), 1, f) != 1 ) { cout << "ERROR 2y51 READING FROM FILE " << SourceFile; fclose(f); return false; }
                        	if( fread( &byte3, sizeof(byte3), 1, f) != 1 ) { cout << "ERROR 3424 READING FROM FILE " << SourceFile; fclose(f); return false; }
				if( fread( &byte4, sizeof(byte4), 1, f) != 1 ) { cout << "ERROR sd22 READING FROM FILE " << SourceFile; fclose(f); return false; }
				compdlength = ((byte1*256*256*256) + (byte2*256*256) + (byte3*256) + byte4 - 0); //length of compressed chunk data
                                if( fread( &byte5, sizeof(byte5), 1, f) != 1 ) { cout << "ERROR 2341 READING FROM FILE " << SourceFile; fclose(f); return false; } //compression type, 1 = GZip (RFC1952) (unused in practice) , 2 = Zlib (RFC1950) 
                                //printf("byte1: %x \n", byte1);
				//printf("byte2: %x \n", byte2);
				//printf("byte3: %x \n", byte3);
				//printf("byte4: %x \n", byte4);

				frloc += 5; //moved ahead 5 bytes while reading data.
				//cout << compdlength << endl; return 1;
                                //unsigned char* comp_data = new unsigned char[ compdlength ];
                                //cout << "size of comp_data: " << compdlength << endl;
                                //cout << "size of comp_data2: " << sizeof(comp_data) << endl;

				//fread( comp_data, sizeof(unsigned char), compdlength, f);
                                //if( fread( &comp_data, sizeof(unsigned char), compdlength, f) != 1 ) { cout << "ERROR 1234 READING FROM FILE " << SourceFile; fclose(f); return false; } //actual compressed chunk data
				//cout << "frloc: " << frloc << endl;



				char temparr[compdlength]; //can't get fread to read more than one char at a time into a char array... so that's what I'll do.  :(    At least it works.
				if( fread( temparr, compdlength, 1, f) != 1 ) { cout << "ERROR rf22 READING FROM FILE " << SourceFile; fclose(f); return false; }
				frloc = frloc + compdlength;
				/*
				int re = 0;
				char tempbyte = 0;
				while (re < compdlength) { //loop through file and read contents into char array a byte at a time.
	                                if( fread( &tempbyte, sizeof(tempbyte), 1, f) != 1 ) { cout << "ERROR rf22 READING FROM FILE " << SourceFile; fclose(f); return false; }
					temparr[re] = tempbyte;
					re++;
					frloc++;
				}

				*/

				//if( fread( comp_data, compdlength, sizeof(unsigned char), f) != 1 ) { cout << "ERROR 1234 READING FROM FILE " << SourceFile <<endl; fclose(f); return false; } //actual compressed chunk data
				//frloc += compdlength;
				//cout << "frloc: " << frloc << endl;
				//return 1;
				//cout << deflateBound(&comp_data,compdlength) << endl;
				uLongf DestSize = 98576;// uncompressed chunks should never be larger than this
				//cout << "echo1: " << DestSize << endl;
				char* BlockData = new char[ DestSize ];
				//return 1;
				//cout << "size of comp_data1: " << sizeof(comp_data) << endl;
				//int errorcode = uncompress( (Bytef*)BlockData, &DestSize, (Bytef*)comp_data, compdlength );
                                int errorcode = uncompress( (Bytef*)BlockData, &DestSize, (Bytef*)temparr, compdlength ); //DestSize will update to the actual uncompressed data size after this opperation.
				//cout << "echo2: " << DestSize << endl;
				//cout << "echo3: " << errorcode << endl;
                                //cout << "size of Block data: " << sizeof(BlockData) << endl;
				//int errorcode = 1;
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

				//cout << "1" << endl;
				//cout << comp_data << endl;
				//return 0;


				//playing with FakeTruth's NBT parser. (unsuccessfully)
				//string BlockDataString(BlockData);
				//memcpy (BlockDataString,BlockData,strlen(BlockData)+1);
				//BlockDataString = BlockData;
				//cNBTCompound* NBTCompound = new cNBTCompound( 0, 0 );
				//cout << cNBTData(BlockData, DestSize)->cNBTCompound << endl;
				//cout << BlockDataString << endl;


				//testing of nbtparser.
				cNBTData* NBTData = new cNBTData::cNBTData(BlockData, (testr));
				//NBTData->m_bDecompressed = true;
				NBTData->ParseData();
				NBTData->PrintData();
				return 1;
                                fwrite( BlockData, DestSize, 1, wf ); //write contents of uncompressed block data to file to check to see if it's valid... It is! :D
				//fwrite( &temparr, compdlength, sizeof(unsigned char), wf );
				//cin >> n; //just to see screen output
				//delete [] comp_data;
				//return 0;
				delete [] BlockData;
				while ( (frloc < toffarr[ia+1]) && (ia<1023) ) { //loop through Notch's junk data until we get to another chunk offset possition to start the loop again
					if( fread( &trash, sizeof(byte4), 1, f) != 1 ) { cout << "ERROR 2nkd READING FROM FILE " << SourceFile; fclose(f); return false; }
					frloc ++;
				}

			}
		//if (ia == 30) { break; }
		}
                                //return 0;
/*
                for( short i = 0; i < 1024 ; ++i ) {
                        if( fread( &byte1, sizeof(byte1), 1, f) != 1 ) { cout << "ERROR READING FROM FILE " << SourceFile; fclose(f); return false; }
                        if( fread( &byte2, sizeof(byte2), 1, f) != 1 ) { cout << "ERROR READING FROM FILE " << SourceFile; fclose(f); return false; }
                        if( fread( &byte3, sizeof(byte3), 1, f) != 1 ) { cout << "ERROR READING FROM FILE " << SourceFile; fclose(f); return false; }
                        if( fread( &byte4, sizeof(byte4), 1, f) != 1 ) { cout << "ERROR READING FROM FILE " << SourceFile; fclose(f); return false; }

		}
		//printf("value: %x \n",trash);


*/
		for ( short i = 0; i < 1024; i++ ) {
			//cout << toffarr[i] << endl;
		}
		//cin >> n;

                for ( short i = 0; i < 24; i++ ) {
                    //    cout << toffarr[i] << endl;
                }


        fclose(wf); //close file.
	fclose(f); //close file.
	}

	clock_t end=clock();
	cout << "Time elapsed: " << double(diffclock(end,begin)) << " ms"<< endl;
	return 0;


}


double diffclock(clock_t clock1,clock_t clock2)
{
	double diffticks=clock1-clock2;
	double diffms=(diffticks*10)/CLOCKS_PER_SEC;
	return diffms;
}

// Quicksort controller function, it partitions the different pieces of our array.
void quicksort(int *arIntegers, int left, int right)
{
/*    cout << "quicksort ([" << arIntegers[0] << ","
  			  << arIntegers[1] << ","
                          << arIntegers[2] << ","
                          << arIntegers[3] << ","
                          << arIntegers[4] << ","
                          << arIntegers[5] << ","
                          << arIntegers[6] << "],"
                          << left << ","
                          << right << ")\n";
*/
    if (right > left)
    {
         int pivotIndex = median3(arIntegers,left,right);
         int pivotNewIndex = partition(arIntegers, left, right, pivotIndex);

         // Recursive call to quicksort to sort each half.
         quicksort(arIntegers, left, pivotNewIndex-1);
         quicksort(arIntegers, pivotNewIndex+1, right);
    }
}

int median3(int *arIntegers,int left,int right)
{
 	int center = (left+right)/2;

   if(arIntegers[center] < arIntegers[left])
   	swap(arIntegers[left],arIntegers[center]);
   if(arIntegers[right] < arIntegers[left])
   	swap(arIntegers[left],arIntegers[right]);
   if(arIntegers[right] < arIntegers[center])
   	swap(arIntegers[center],arIntegers[right]);

   swap(arIntegers[center],arIntegers[right-1]);

   return center;
}

// This function takes an array (or one half an array) and sorts it.
// It then returns a new pivot index number back to quicksort.

int partition(int *arIntegers, int left, int right, int pivot)
{
/*     cout << "partition ("<< arIntegers[0] << ","
    			  << arIntegers[1] << ","
                          << arIntegers[2] << ","
                          << arIntegers[3] << ","
                          << arIntegers[4] << ","
                          << arIntegers[5] << ","
                          << arIntegers[6] << "],"
                          << left << ","
                          << right << ")\n";
*/
     int pivotValue = arIntegers[pivot];

     // Swap it out all the way to the end of the array
     // So we know where it always is.
     swap(arIntegers[pivot], arIntegers[right]);
     int storeIndex = left;

     // Move through the array from start to finish comparing each to our
     // pivot value (not index, the value that was located at the pivot index)
     for (int i = left; i < right; i++)
     {
         if (arIntegers[i] <= pivotValue)
         {
             swap(arIntegers[i], arIntegers[storeIndex]);
             storeIndex++;
         }
     }
     swap(arIntegers[storeIndex], arIntegers[right]);
     return storeIndex;
}

// Simple swap function for our in place swapping.
void swap(int &val1, int &val2)
{
    int temp = val1;
    val1 = val2;
    val2 = temp;
}
