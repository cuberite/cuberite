#pragma once
#include "CommandException.h"

class BasicStringReader
{
  public:
	BasicStringReader(const AString& a_String) :
		m_Cursor(0),
		m_String(a_String)
	{

	}

	AString ReadStringUntilWhiteSpace()
	{
		//TODO: check range before calling
		try
		{
			auto ret = m_String.substr(m_Cursor, m_String.find(' ',m_Cursor) - m_Cursor);
			m_Cursor += static_cast<int>(ret.size())+1;
			return ret;
		}
		catch (std::out_of_range ex)
		{
			return "";
		}
	}
	int GetCursor() const { return m_Cursor; }

	void SetCursor(const int a_Cursor) { m_Cursor = a_Cursor; }

	bool IsDone() const { return static_cast<int>(m_String.size()) == m_Cursor-1; }
	AString GetString() const { return m_String; }
  private:
	int m_Cursor;
	AString m_String;
};
