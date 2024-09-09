#pragma once

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
		auto ret = m_String.substr(m_Cursor, m_String.find(' '));
		m_Cursor += static_cast<int>(ret.size());
		return ret;
	}
	int GetCursor() const { return m_Cursor; }

	void SetCursor(const int a_Cursor) { m_Cursor = a_Cursor; }

	bool IsDone() const { return static_cast<int>(m_String.size()) == m_Cursor; }
	AString GetString() const { return m_String; }
  private:
	int m_Cursor;
	AString m_String;
};
