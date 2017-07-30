
// BookContent.cpp

#include "Globals.h"
#include "BookContent.h"
#include "json/json.h"





void cBookContent::Clear()
{
	m_Author.clear();
	m_Title.clear();
	m_Pages.clear();
}





bool cBookContent::IsEmpty(void) const
{
	return (
		m_Author.empty() &&
		m_Title.empty() &&
		m_Pages.empty()
	);
}





void cBookContent::ParseFromNBT(int TagTag, cBookContent & a_BookContent, const cParsedNBT & a_NBT, bool a_SaveAsJson)
{
	int AuthorTag = a_NBT.FindChildByName(TagTag, "author");
	if ((AuthorTag > 0) && (a_NBT.GetType(AuthorTag) == TAG_String))
	{
		a_BookContent.SetAuthor(a_NBT.GetString(AuthorTag));
	}

	int TitleTag = a_NBT.FindChildByName(TagTag, "title");
	if ((TitleTag > 0) && (a_NBT.GetType(TitleTag) == TAG_String))
	{
		a_BookContent.SetTitle(a_NBT.GetString(TitleTag));
	}

	int PagesTag = a_NBT.FindChildByName(TagTag, "pages");
	if ((PagesTag > 0) && (a_NBT.GetType(PagesTag) == TAG_List))
	{
		for (int PageTag = a_NBT.GetFirstChild(PagesTag); PageTag >= 0; PageTag = a_NBT.GetNextSibling(PageTag))
		{
			if (a_NBT.GetType(PageTag) == TAG_String)
			{
				if (a_SaveAsJson)
				{
					Json::Value Page;
					Page["text"] = a_NBT.GetString(PageTag);
					a_BookContent.AddPage(Page.toStyledString());
				}
				else
				{
					a_BookContent.AddPage(a_NBT.GetString(PageTag));
				}
			}
		}
	}
}





void cBookContent::WriteToNBTCompound(const cBookContent & a_BookContent, cFastNBTWriter & a_Writer)
{
	if (a_BookContent.IsEmpty())
	{
		return;
	}

	a_Writer.AddString("author", a_BookContent.GetAuthor());
	a_Writer.AddString("title", a_BookContent.GetTitle());
	a_Writer.BeginList("pages", TAG_String);
	for (const auto & Page : a_BookContent.GetPages())
	{
		a_Writer.AddString("", Page);
	}
	a_Writer.EndList();
}
