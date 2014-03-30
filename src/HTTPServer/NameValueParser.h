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

// NameValueParser.h

// Declares the cNameValueParser class that parses strings in the "name=value;name2=value2" format into a stringmap





#pragma once





class cNameValueParser :
	public std::map<AString, AString>
{
public:
	/// Creates an empty parser
	cNameValueParser(bool a_AllowsKeyOnly = true);
	
	/// Creates an empty parser, then parses the data given. Doesn't call Finish(), so more data can be parsed later
	cNameValueParser(const char * a_Data, int a_Size, bool a_AllowsKeyOnly = true);
	
	/// Parses the data given
	void Parse(const char * a_Data, int a_Size);
	
	/// Notifies the parser that no more data will be coming. Returns true if the parser state is valid
	bool Finish(void);
	
	/// Returns true if the data parsed so far was valid
	bool IsValid(void) const { return (m_State != psInvalid); }
	
	/// Returns true if the parser expects no more data
	bool IsFinished(void) const { return ((m_State == psInvalid) || (m_State == psFinished)); }
	
protected:
	enum eState
	{
		psKeySpace,        ///< Parsing the space in front of the next key
		psKey,             ///< Currently adding more chars to the key in m_CurrentKey
		psEqualSpace,      ///< Space after m_CurrentKey
		psEqual,           ///< Just parsed the = sign after a name
		psValueInSQuotes,  ///< Just parsed a Single-quote sign after the Equal sign
		psValueInDQuotes,  ///< Just parsed a Double-quote sign after the Equal sign
		psValueRaw,        ///< Just parsed a raw value without a quote
		psAfterValue,      ///< Just finished parsing the value, waiting for semicolon or data end
		psInvalid,         ///< The parser has encountered an invalid input; further parsing is skipped
		psFinished,        ///< The parser has already been instructed to finish and doesn't expect any more data
	} ;
	
	/// The current state of the parser
	eState m_State;
	
	/// If true, the parser will accept keys without an equal sign and the value
	bool m_AllowsKeyOnly;
	
	/// Buffer for the current Key
	AString m_CurrentKey;
	
	/// Buffer for the current Value;
	AString m_CurrentValue;
	
	
} ;




