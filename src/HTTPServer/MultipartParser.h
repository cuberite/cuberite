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

// MultipartParser.h

// Declares the cMultipartParser class that parses messages in "multipart/*" encoding into the separate parts





#pragma once

#include "EnvelopeParser.h"





class cMultipartParser :
	protected cEnvelopeParser::cCallbacks
{
public:
	class cCallbacks
	{
	public:
		/// Called when a new part starts
		virtual void OnPartStart(void) = 0;
		
		/// Called when a complete header line is received for a part
		virtual void OnPartHeader(const AString & a_Key, const AString & a_Value) = 0;
		
		/// Called when body for a part is received
		virtual void OnPartData(const char * a_Data, int a_Size) = 0;
		
		/// Called when the current part ends
		virtual void OnPartEnd(void) = 0;
	} ;
	
	/// Creates the parser, expects to find the boundary in a_ContentType
	cMultipartParser(const AString & a_ContentType, cCallbacks & a_Callbacks);
	
	/// Parses more incoming data
	void Parse(const char * a_Data, int a_Size);
	
protected:
	/// The callbacks to call for various parsing events
	cCallbacks & m_Callbacks;
	
	/// True if the data parsed so far is valid; if false, further parsing is skipped
	bool m_IsValid;
	
	/// Parser for each part's envelope
	cEnvelopeParser m_EnvelopeParser;
	
	/// Buffer for the incoming data until it is parsed
	AString m_IncomingData;
	
	/// The boundary, excluding both the initial "--" and the terminating CRLF
	AString m_Boundary;
	
	/// Set to true if some data for the current part has already been signalized to m_Callbacks. Used for proper CRLF inserting.
	bool m_HasHadData;
	
	
	/// Parse one line of incoming data. The CRLF has already been stripped from a_Data / a_Size
	void ParseLine(const char * a_Data, int a_Size);
	
	/// Parse one line of incoming data in the headers section of a part. The CRLF has already been stripped from a_Data / a_Size
	void ParseHeaderLine(const char * a_Data, int a_Size);
	
	// cEnvelopeParser overrides:
	virtual void OnHeaderLine(const AString & a_Key, const AString & a_Value) override;
} ;




