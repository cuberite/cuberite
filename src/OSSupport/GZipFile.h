/**
 * Copyright 2014 MCServer Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

// GZipFile.h

// Declares the cGZipFile class representing a RAII wrapper over zlib's GZip file routines





#pragma once

#include "zlib/zlib.h"





class cGZipFile
{
public:
	enum eMode
	{
		fmRead,      // Read-only. If the file doesn't exist, object will not be valid
		fmWrite,     // Write-only. If the file already exists, it will be overwritten
	} ;
	
	cGZipFile(void);
	~cGZipFile();
	
	/// Opens the file. Returns true if successful. Fails if a file has already been opened through this object.
	bool Open(const AString & a_FileName, eMode a_Mode);
	
	/// Closes the file, flushing all buffers. This object may be then reused for a different file and / or mode
	void Close(void);
	
	/// Reads the rest of the file and decompresses it into a_Contents. Returns the number of decompressed bytes, <0 for error
	int ReadRestOfFile(AString & a_Contents);
	
	/// Writes a_Contents into file, compressing it along the way. Returns true if successful. Multiple writes are supported.
	bool Write(const AString & a_Contents) { return Write(a_Contents.data(), (int)(a_Contents.size())); }
	
	bool Write(const char * a_Data, int a_Size);
	
protected:
	gzFile m_File;
	eMode  m_Mode;
} ;





