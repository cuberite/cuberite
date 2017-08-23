
// BookContent.h

#pragma once

#include "WorldStorage/FastNBT.h"





// tolua_begin
class cBookContent
{
public:
	/** Creates a empty book */
	cBookContent():
		m_IsSigned(false)
	{}

	/** Set the author of the book */
	void SetAuthor(const AString & a_Author) { m_Author = a_Author; }

	/** Returns the author of the book */
	const AString & GetAuthor(void) const { return m_Author; }

	/** Set the title of the book */
	void SetTitle(const AString & a_Title) { m_Title = a_Title; }

	/** Returns the title of the book */
	const AString & GetTitle(void) const { return m_Title; }

	/** Clears the whole book */
	void Clear();

	/** Returns true if the book has no author, no title and no pages */
	bool IsEmpty(void) const;

	// tolua_end

	/** Required in ManualBindings to save the page as simple string or json string */
	void SetIsSigned(bool a_IsSigned) { m_IsSigned = a_IsSigned; }

	/** Returns true if the book is signed */
	bool IsSigned(void) const { return m_IsSigned; }

	/** Add a page to the end of the book */
	void AddPage(const AString & a_Page) { m_Pages.emplace_back(a_Page); }

	/** Returns the page at the index */
	const AString & GetPage(size_t a_Index) { return m_Pages[a_Index]; }

	/** Changes the page at the index */
	void SetPage(size_t a_Index, const AString & a_Page) { m_Pages[a_Index] = a_Page; }

	/** Removes all pages */
	void ClearPages(void)
	{
		m_Pages.clear();
	}

	/** Returns a ref to the vector. Used in ManualBindings and for saving the book */
	const std::vector<AString> & GetPages(void) const { return m_Pages; }

	/** Read the book content from nbt. The boolean a_SaveAsJson is optional. If the book is signed, the text should be in a json string */
	static void ParseFromNBT(int TagTag, cBookContent & a_BookContent, const cParsedNBT & a_NBT, bool a_SaveAsJson = false);

	/** Write book content to nbt */
	static void WriteToNBTCompound(const cBookContent & a_BookContent, cFastNBTWriter & a_Writer);

private:
	/** Author of the book */
	AString m_Author;

	/** Title of the book */
	AString m_Title;

	/** Contains the pages */
	AStringVector m_Pages;

	/** If true the book is written */
	bool m_IsSigned;

};  // tolua_export
