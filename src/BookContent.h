
// BookContent.h

#pragma once

#include "WorldStorage/FastNBT.h"





// tolua_begin
class cBookContent
{
public:
	/** Creates a empty book */
	cBookContent() {}

	/** Set the author of the book */
	void SetAuthor(const AString & a_Author) { m_Author = a_Author; }

	/** Returns the author of the book */
	const AString GetAuthor(void) const { return m_Author; }

	/** Set the title of the book */
	void SetTitle(const AString & a_Title) { m_Title = a_Title; }

	/** Returns the title of the book */
	const AString GetTitle(void) const { return m_Title; }

	/** Add a page to the end of the book */
	void AddPage(const AString & a_Page) { m_Pages.emplace_back(a_Page); }

	/** Clears the whole book */
	void Clear();

	/** Returns true if the book has no author, no title and no pages */
	bool IsEmpty(void) const;

	// tolua_end

	/** Returns a AStringVector ref to the pages. Used in ManualBindings and for saving the book */
	const AStringVector & GetPages(void) const { return m_Pages; }

	/** Set the pages. Used in ManualBindings */
	void SetPages(const AStringVector & a_Pages) { m_Pages = a_Pages; }

	/** Read the book content from nbt. The boolean a_SaveAsJson is optional. If a player creates a book, the text should be in a json string */
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

};  // tolua_export
