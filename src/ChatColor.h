
#pragma once





// tolua_begin
class cChatColor
{
public:
	static const char * m_Delimiter;

	/** @deprecated use ChatColor::Delimiter instead */
	static const char * m_Color;

	static const char * m_Black;
	static const char * m_Navy;
	static const char * m_Green;
	static const char * m_Blue;
	static const char * m_Red;
	static const char * m_Purple;
	static const char * m_Gold;
	static const char * m_LightGray;
	static const char * m_Gray;
	static const char * m_DarkPurple;
	static const char * m_LightGreen;
	static const char * m_LightBlue;
	static const char * m_Rose;
	static const char * m_LightPurple;
	static const char * m_Yellow;
	static const char * m_White;

	// Styles
	// source: http://wiki.vg/Chat
	static const char * m_Random;
	static const char * m_Bold;
	static const char * m_Strikethrough;
	static const char * m_Underlined;
	static const char * m_Italic;
	static const char * m_Plain;
};

// tolua_end
