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

// XMLParser.h

// Interfaces to the CXMLParser class representing the base class for XML parsing

// To use, derive a class from this base and override its OnStartElement(), OnEndElement() and OnCharacters() functions





#pragma once

#include "expat/expat.h"





class CXMLParser
{
public:
	CXMLParser(void);
	virtual ~CXMLParser();
	
	// The actual parsing, may be called several times; the last time needs iIsFinal == true (-> flush)
	int Parse(const char * iData, size_t iLength, bool iIsFinal = false);

private:
	// LibExpat stuff:
	XML_Parser mParser;
	
	static void StartElementHandler(void * iContext, const XML_Char * iElement, const XML_Char ** iAttributes)
	{
		((CXMLParser *)iContext)->OnStartElement(iElement, iAttributes);
	}
	
	static void EndElementHandler  (void * iContext, const XML_Char * iElement)
	{
		((CXMLParser *)iContext)->OnEndElement(iElement);
	}
	
	static void CharacterDataHandler (void * iContext, const XML_Char * iData, int iLength)
	{
		((CXMLParser *)iContext)->OnCharacters(iData, iLength);
	}
	
protected:
	virtual void OnStartElement(const XML_Char * iElement, const XML_Char ** iAttributes) = 0;
	virtual void OnEndElement  (const XML_Char * iElement) = 0;
	virtual void OnCharacters  (const XML_Char * iCharacters, int iLength) = 0;
} ;





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// The following template has been modified from code available at
// http://www.codeproject.com/Articles/1847/C-Wrappers-for-the-Expat-XML-Parser
// It uses templates to remove the virtual function call penalty (both size and speed) for each callback

/* Usage:
1, Declare a subclass: 
	class CMyParser : public CExpatImpl<CMyParser>
2, Declare handlers that you want in that subclass:
	void CMyParser::OnEndElement(const XML_Char * iTagName);
3, Create an instance of your class:
	CMyParser Parser;
4, Call Create():
	Parser.Create(NULL, NULL);
4, Call Parse(), repeatedly:
	Parser.Parse(Buffer, Length);
*/

template <class _T>
class CExpatImpl
{

// @access Constructors and destructors
public:
	
	// @cmember General constructor

	CExpatImpl ()
	{
		m_p = NULL;
	}

	// @cmember Destructor

	~CExpatImpl ()
	{
		Destroy ();
	}

// @access Parser creation and deletion methods
public:

	// @cmember Create a parser

	bool Create (const XML_Char * pszEncoding = NULL, const XML_Char * pszSep = NULL)
	{
		// Destroy the old parser
		Destroy ();

		// If the encoding or seperator are empty, then NULL
		if (pszEncoding != NULL && pszEncoding [0] == 0)
		{
			pszEncoding = NULL;
		}
		if (pszSep != NULL && pszSep [0] == 0)
		{
			pszSep = NULL;
		}

		// Create the new parser
		m_p = XML_ParserCreate_MM (pszEncoding, NULL, pszSep);
		if (m_p == NULL)
		{
			return false;
		}

		// Invoke the post create routine
		_T * pThis = static_cast <_T *> (this);
		pThis ->OnPostCreate ();

		// Set the user data used in callbacks
		XML_SetUserData (m_p, (void *) this);
		return true;
	}

	// @cmember Destroy the parser

	void Destroy (void)
	{
		if (m_p != NULL)
		{
			XML_ParserFree (m_p);
		}
		m_p = NULL;
	}


	// @cmember Parse a block of data

	bool Parse (const char *pszBuffer, int nLength, bool fIsFinal = true)
	{
		assert (m_p != NULL);
		return XML_Parse (m_p, pszBuffer, nLength, fIsFinal) != 0;
	}

	// @cmember Parse internal buffer

	bool ParseBuffer (int nLength, bool fIsFinal = true)
	{
		assert (m_p != NULL);
		return XML_ParseBuffer (m_p, nLength, fIsFinal) != 0;
	}

	// @cmember Get the internal buffer

	void *GetBuffer (int nLength)
	{
		assert (m_p != NULL);
		return XML_GetBuffer (m_p, nLength);
	}


protected:
	// Parser callback enable/disable methods:

	// @cmember Enable/Disable the start element handler

	void EnableStartElementHandler (bool fEnable = true)
	{
		assert (m_p != NULL);
		XML_SetStartElementHandler (m_p, fEnable ? StartElementHandler : NULL);
	}

	// @cmember Enable/Disable the end element handler

	void EnableEndElementHandler (bool fEnable = true)
	{
		assert (m_p != NULL);
		XML_SetEndElementHandler (m_p, fEnable ? EndElementHandler : NULL);
	}

	// @cmember Enable/Disable the element handlers

	void EnableElementHandler (bool fEnable = true)
	{
		assert (m_p != NULL);
		EnableStartElementHandler (fEnable);
		EnableEndElementHandler (fEnable);
	}

	// @cmember Enable/Disable the character data handler

	void EnableCharacterDataHandler (bool fEnable = true)
	{
		assert (m_p != NULL);
		XML_SetCharacterDataHandler (m_p, fEnable ? CharacterDataHandler : NULL);
	}

	// @cmember Enable/Disable the processing instruction handler

	void EnableProcessingInstructionHandler (bool fEnable = true)
	{
		assert (m_p != NULL);
		XML_SetProcessingInstructionHandler (m_p, fEnable ? ProcessingInstructionHandler : NULL);
	}

	// @cmember Enable/Disable the comment handler

	void EnableCommentHandler (bool fEnable = true)
	{
		assert (m_p != NULL);
		XML_SetCommentHandler (m_p, fEnable ? CommentHandler : NULL);
	}

	// @cmember Enable/Disable the start CDATA section handler

	void EnableStartCdataSectionHandler (bool fEnable = true)
	{
		assert (m_p != NULL);
		XML_SetStartCdataSectionHandler (m_p, fEnable ? StartCdataSectionHandler : NULL);
	}

	// @cmember Enable/Disable the end CDATA section handler

	void EnableEndCdataSectionHandler (bool fEnable = true)
	{
		assert (m_p != NULL);
		XML_SetEndCdataSectionHandler (m_p, fEnable ? EndCdataSectionHandler : NULL);
	}

	// @cmember Enable/Disable the CDATA section handlers

	void EnableCdataSectionHandler (bool fEnable = true)
	{
		assert (m_p != NULL);
		EnableStartCdataSectionHandler (fEnable);
		EnableEndCdataSectionHandler (fEnable);
	}

	// @cmember Enable/Disable default handler
	
	void EnableDefaultHandler (bool fEnable = true, bool fExpand = true)
	{
		assert (m_p != NULL);
		if (fExpand)
		{
			XML_SetDefaultHandlerExpand (m_p, fEnable ? DefaultHandler : NULL);
		}
		else
			XML_SetDefaultHandler (m_p, fEnable ? DefaultHandler : NULL);
	}
	
	// @cmember Enable/Disable external entity ref handler
	
	void EnableExternalEntityRefHandler (bool fEnable = true)
	{
		assert (m_p != NULL);
		XML_SetExternalEntityRefHandler (m_p, fEnable ? ExternalEntityRefHandler : NULL);
	}
	
	// @cmember Enable/Disable unknown encoding handler
	
	void EnableUnknownEncodingHandler (bool fEnable = true)
	{
		assert (m_p != NULL);
		XML_SetUnknownEncodingHandler (m_p, fEnable ? UnknownEncodingHandler : NULL);
	}
	
	// @cmember Enable/Disable start namespace handler
	
	void EnableStartNamespaceDeclHandler (bool fEnable = true)
	{
		assert (m_p != NULL);
		XML_SetStartNamespaceDeclHandler (m_p, fEnable ? StartNamespaceDeclHandler : NULL);
	}
	
	// @cmember Enable/Disable end namespace handler
	
	void EnableEndNamespaceDeclHandler (bool fEnable = true)
	{
		assert (m_p != NULL);
		XML_SetEndNamespaceDeclHandler (m_p, fEnable ? EndNamespaceDeclHandler : NULL);
	}

	// @cmember Enable/Disable namespace handlers

	void EnableNamespaceDeclHandler (bool fEnable = true)
	{
		EnableStartNamespaceDeclHandler (fEnable);
		EnableEndNamespaceDeclHandler (fEnable);
	}
	
	// @cmember Enable/Disable the XML declaration handler

	void EnableXmlDeclHandler (bool fEnable = true)
	{
		assert (m_p != NULL);
		XML_SetXmlDeclHandler (m_p, fEnable ? XmlDeclHandler : NULL);
	}

	// @cmember Enable/Disable the start DOCTYPE declaration handler

	void EnableStartDoctypeDeclHandler (bool fEnable = true)
	{
		assert (m_p != NULL);
		XML_SetStartDoctypeDeclHandler (m_p, fEnable ? StartDoctypeDeclHandler : NULL);
	}

	// @cmember Enable/Disable the end DOCTYPE declaration handler

	void EnableEndDoctypeDeclHandler (bool fEnable = true)
	{
		assert (m_p != NULL);
		XML_SetEndDoctypeDeclHandler (m_p, 
			fEnable ? EndDoctypeDeclHandler : NULL);
	}

	// @cmember Enable/Disable the DOCTYPE declaration handler

	void EnableDoctypeDeclHandler (bool fEnable = true)
	{
		assert (m_p != NULL);
		EnableStartDoctypeDeclHandler (fEnable);
		EnableEndDoctypeDeclHandler (fEnable);
	}

public:
	// Parser error reporting methods

	// @cmember Get last error

	enum XML_Error GetErrorCode () 
	{
		assert (m_p != NULL);
		return XML_GetErrorCode (m_p);
	}

	// @cmember Get the current byte index

	long GetCurrentByteIndex () 
	{
		assert (m_p != NULL);
		return XML_GetCurrentByteIndex (m_p);
	}

	// @cmember Get the current line number

	int GetCurrentLineNumber () 
	{
		assert (m_p != NULL);
		return XML_GetCurrentLineNumber (m_p);
	}

	// @cmember Get the current column number

	int GetCurrentColumnNumber () 
	{
		assert (m_p != NULL);
		return XML_GetCurrentColumnNumber (m_p);
	}

	// @cmember Get the current byte count

	int GetCurrentByteCount () 
	{
		assert (m_p != NULL);
		return XML_GetCurrentByteCount (m_p);
	}

	// @cmember Get the input context

	const char *GetInputContext (int *pnOffset, int *pnSize)
	{
		assert (m_p != NULL);
		return XML_GetInputContext (m_p, pnOffset, pnSize);
	}

	// @cmember Get last error string

	const XML_LChar *GetErrorString () 
	{
		return XML_ErrorString (GetErrorCode ());
	}

	// @cmember Return the version string

	static const XML_LChar *GetExpatVersion ()
	{
		return XML_ExpatVersion ();
	}

	// @cmember Get the version information

	static void GetExpatVersion (int *pnMajor, int *pnMinor, int *pnMicro)
	{
		XML_expat_version v = XML_ExpatVersionInfo ();
		if (pnMajor)
			*pnMajor = v .major;
		if (pnMinor)
			*pnMinor = v .minor;
		if (pnMicro)
			*pnMicro = v .micro;
	}

	// @cmember Get last error string

	static const XML_LChar *GetErrorString (enum XML_Error nError) 
	{
		return XML_ErrorString (nError);
	}


	// Public handler methods:
	// The template parameter should provide their own implementation for those handlers that they want

	// @cmember Start element handler

	void OnStartElement (const XML_Char *pszName, const XML_Char **papszAttrs)
	{
		return;
	}

	// @cmember End element handler

	void OnEndElement (const XML_Char *pszName)
	{
		return;
	}

	// @cmember Character data handler

	void OnCharacterData (const XML_Char *pszData, int nLength)
	{
		return;
	}

	// @cmember Processing instruction handler

	void OnProcessingInstruction (const XML_Char *pszTarget, 
		const XML_Char *pszData)
	{
		return;
	}

	// @cmember Comment handler

	void OnComment (const XML_Char *pszData)
	{
		return;
	}

	// @cmember Start CDATA section handler

	void OnStartCdataSection ()
	{
		return;
	}

	// @cmember End CDATA section handler

	void OnEndCdataSection ()
	{
		return;
	}

	// @cmember Default handler
	
	void OnDefault (const XML_Char *pszData, int nLength)
	{
		return;
	}
	
	// @cmember External entity ref handler
	
	bool OnExternalEntityRef (const XML_Char *pszContext,
		const XML_Char *pszBase, const XML_Char *pszSystemID,
		const XML_Char *pszPublicID)
	{
		return false;
	}
	
	// @cmember Unknown encoding handler
	
	bool OnUnknownEncoding (const XML_Char *pszName, XML_Encoding *pInfo)
	{
		return false;
	}
	
	// @cmember Start namespace declaration handler
	
	void OnStartNamespaceDecl (const XML_Char *pszPrefix, 
		const XML_Char *pszURI)
	{
		return;
	}
	
	// @cmember End namespace declaration handler
	
	void OnEndNamespaceDecl (const XML_Char *pszPrefix)
	{
		return;
	}
	
	// @cmember XML declaration handler

	void OnXmlDecl (const XML_Char *pszVersion, const XML_Char *pszEncoding,
		bool fStandalone)
	{
		return;
	}

	// @cmember Start DOCTYPE declaration handler

	void OnStartDoctypeDecl (const XML_Char *pszDoctypeName, 
		const XML_Char *pszSysID, const XML_Char *pszPubID,
		bool fHasInternalSubset)
	{
		return;
	}

	// @cmember End DOCTYPE declaration handler

	void OnEndDoctypeDecl ()
	{
		return;
	}

// @access Protected methods
protected:

	// @cmember Handle any post creation

	void OnPostCreate ()
	{
	}

// @access Protected static methods
protected:

	// @cmember Start element handler wrapper

	static void __cdecl StartElementHandler (void *pUserData,
		const XML_Char *pszName, const XML_Char **papszAttrs)
	{
		_T *pThis = static_cast <_T *> ((CExpatImpl <_T> *) pUserData);
		pThis ->OnStartElement (pszName, papszAttrs);
	}

	// @cmember End element handler wrapper

	static void __cdecl EndElementHandler (void *pUserData,
		const XML_Char *pszName)
	{
		_T *pThis = static_cast <_T *> ((CExpatImpl <_T> *) pUserData);
		pThis ->OnEndElement (pszName);
	}

	// @cmember Character data handler wrapper

	static void __cdecl CharacterDataHandler (void *pUserData,
		const XML_Char *pszData, int nLength)
	{
		_T *pThis = static_cast <_T *> ((CExpatImpl <_T> *) pUserData);
		pThis ->OnCharacterData (pszData, nLength);
	}

	// @cmember Processing instruction handler wrapper

	static void __cdecl ProcessingInstructionHandler (void *pUserData,
		const XML_Char *pszTarget, const XML_Char *pszData)
	{
		_T *pThis = static_cast <_T *> ((CExpatImpl <_T> *) pUserData);
		pThis ->OnProcessingInstruction (pszTarget, pszData);
	}

	// @cmember Comment handler wrapper

	static void __cdecl CommentHandler (void *pUserData,
		const XML_Char *pszData)
	{
		_T *pThis = static_cast <_T *> ((CExpatImpl <_T> *) pUserData);
		pThis ->OnComment (pszData);
	}

	// @cmember Start CDATA section wrapper

	static void __cdecl StartCdataSectionHandler (void *pUserData)
	{
		_T *pThis = static_cast <_T *> ((CExpatImpl <_T> *) pUserData);
		pThis ->OnStartCdataSection ();
	}

	// @cmember End CDATA section wrapper

	static void __cdecl EndCdataSectionHandler (void *pUserData)
	{
		_T *pThis = static_cast <_T *> ((CExpatImpl <_T> *) pUserData);
		pThis ->OnEndCdataSection ();
	}

	// @cmember Default wrapper
	
	static void __cdecl DefaultHandler (void *pUserData, 
		const XML_Char *pszData, int nLength)
	{
		_T *pThis = static_cast <_T *> ((CExpatImpl <_T> *) pUserData);
		pThis ->OnDefault (pszData, nLength);
	}
	
	// @cmember External entity ref wrapper
	
	static int __cdecl ExternalEntityRefHandler (void *pUserData, 
		const XML_Char *pszContext, const XML_Char *pszBase, 
		const XML_Char *pszSystemID, const XML_Char *pszPublicID)
	{
		_T *pThis = static_cast <_T *> ((CExpatImpl <_T> *) pUserData);
		return pThis ->OnExternalEntityRef (pszContext, 
			pszBase, pszSystemID, pszPublicID) ? 1 : 0;
	}
	
	// @cmember Unknown encoding wrapper
	
	static int __cdecl UnknownEncodingHandler (void * pUserData, const XML_Char * pszName, XML_Encoding * pInfo)
	{
		_T *pThis = static_cast <_T *> ((CExpatImpl <_T> *) pUserData);
		return pThis ->OnUnknownEncoding (pszName, pInfo) ? 1 : 0;
	}
	
	// @cmember Start namespace decl wrapper
	
	static void __cdecl StartNamespaceDeclHandler (void * pUserData, const XML_Char * pszPrefix, const XML_Char * pszURI)
	{
		_T *pThis = static_cast <_T *> ((CExpatImpl <_T> *) pUserData);
		pThis ->OnStartNamespaceDecl (pszPrefix, pszURI);
	}
	
	// @cmember End namespace decl wrapper
	
	static void __cdecl EndNamespaceDeclHandler (void * pUserData, const XML_Char * pszPrefix)
	{
		_T *pThis = static_cast <_T *> ((CExpatImpl <_T> *) pUserData);
		pThis ->OnEndNamespaceDecl (pszPrefix);
	}
	
	// @cmember XML declaration wrapper

	static void __cdecl XmlDeclHandler (void *pUserData, const XML_Char *pszVersion, const XML_Char *pszEncoding, int nStandalone)
	{
		_T *pThis = static_cast <_T *> ((CExpatImpl <_T> *) pUserData);
		pThis ->OnXmlDecl (pszVersion, pszEncoding, nStandalone != 0);
	}

	// @cmember Start Doctype declaration wrapper

	static void __cdecl StartDoctypeDeclHandler (
		void *pUserData, const XML_Char *pszDoctypeName, const XML_Char *pszSysID, 
		const XML_Char *pszPubID, int nHasInternalSubset
	)
	{
		_T *pThis = static_cast <_T *> ((CExpatImpl <_T> *) pUserData);
		pThis ->OnStartDoctypeDecl (pszDoctypeName, pszSysID, 
			pszPubID, nHasInternalSubset != 0);
	}

	// @cmember End Doctype declaration wrapper

	static void __cdecl EndDoctypeDeclHandler (void *pUserData)
	{
		_T *pThis = static_cast <_T *> ((CExpatImpl <_T> *) pUserData);
		pThis ->OnEndDoctypeDecl ();
	}
	
	
protected:

	XML_Parser m_p;
	
	/// Returns the value of the specified attribute, if found; NULL otherwise
	static const XML_Char * FindAttr(const XML_Char ** iAttrs, const XML_Char * iAttrToFind)
	{
		for (const XML_Char ** Attr = iAttrs; *Attr != NULL; Attr += 2)
		{
			if (strcmp(*Attr, iAttrToFind) == 0)
			{
				return *(Attr + 1);
			}
		}  // for Attr - iAttrs[]
		return NULL;
	}
} ;




