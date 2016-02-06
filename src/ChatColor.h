
#pragma once





// tolua_begin
class cChatColor
{
public:
	static const char * Delimiter;

	/** @deprecated use ChatColor::Delimiter instead */
	static const char * Color;

	static const char * Black;
	static const char * Navy;
	static const char * Green;
	static const char * Blue;
	static const char * Red;
	static const char * Purple;
	static const char * Gold;
	static const char * LightGray;
	static const char * Gray;
	static const char * DarkPurple;
	static const char * LightGreen;
	static const char * LightBlue;
	static const char * Rose;
	static const char * LightPurple;
	static const char * Yellow;
	static const char * White;

	// Styles
	// source: http://wiki.vg/Chat
	static const char * Random;
	static const char * Bold;
	static const char * Strikethrough;
	static const char * Underlined;
	static const char * Italic;
	static const char * Plain;
};

// tolua_end
