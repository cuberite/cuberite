#pragma once
#include "CommandException.h"

class BasicStringReader
{
public:
	BasicStringReader(const AString & a_String) :
		m_Cursor(0),
		m_String(a_String)
	{

	}

	AString ReadStringUntilWhiteSpace()
	{
		// TODO: check range before calling
		try
		{
			auto ret = m_String.substr(m_Cursor, m_String.find(' ', m_Cursor) - m_Cursor);
			m_Cursor += ret.size() + 1;
			return ret;
		}
		catch (std::out_of_range & ex)
		{
			LOGERROR(fmt::format(FMT_STRING("Command parsing failed. Reason: {}"), ex.what()));
			return "";
		}
	}
	UInt64 GetCursor() const { return m_Cursor; }

	void SetCursor(const UInt64 a_Cursor) { m_Cursor = a_Cursor; }

	bool IsDone() const { return m_String.size() == m_Cursor - 1; }
	AString GetString() const { return m_String; }
private:
	UInt64 m_Cursor;
	AString m_String;
};
