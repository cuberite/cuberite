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

// EnvelopeParser.h

// Declares the cEnvelopeParser class representing a parser for RFC-822 envelope headers, used both in HTTP and in MIME





#pragma once





class cEnvelopeParser
{
public:
	class cCallbacks
	{
	public:
		/// Called when a full header line is parsed
		virtual void OnHeaderLine(const AString & a_Key, const AString & a_Value) = 0;
	} ;
	
	
	cEnvelopeParser(cCallbacks & a_Callbacks);
	
	/** Parses the incoming data.
	Returns the number of bytes consumed from the input. The bytes not consumed are not part of the envelope header
	*/
	int Parse(const char * a_Data, int a_Size);
	
	/// Makes the parser forget everything parsed so far, so that it can be reused for parsing another datastream
	void Reset(void);
	
	/// Returns true if more input is expected for the envelope header
	bool IsInHeaders(void) const { return m_IsInHeaders; }
	
	/// Sets the IsInHeaders flag; used by cMultipartParser to simplify the parser initial conditions
	void SetIsInHeaders(bool a_IsInHeaders) { m_IsInHeaders = a_IsInHeaders; }
	
public:
	/// Callbacks to call for the various events
	cCallbacks & m_Callbacks;
	
	/// Set to true while the parser is still parsing the envelope headers. Once set to true, the parser will not consume any more data.
	bool m_IsInHeaders;
	
	/// Buffer for the incoming data until it is parsed
	AString m_IncomingData;
	
	/// Holds the last parsed key; used for line-wrapped values
	AString m_LastKey;
	
	/// Holds the last parsed value; used for line-wrapped values
	AString m_LastValue;


	/// Notifies the callback of the key/value stored in m_LastKey/m_LastValue, then erases them
	void NotifyLast(void);
	
	/// Parses one line of header data. Returns true if successful
	bool ParseLine(const char * a_Data, size_t a_Size);
} ;




