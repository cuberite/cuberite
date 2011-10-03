#pragma once

#include <string>

// tolua_begin
class cChatColor
{
public:
	static const std::string Color;
	static const std::string Delimiter;

	static const std::string Black;
	static const std::string Navy;
	static const std::string Green;
	static const std::string Blue;
	static const std::string Red;
	static const std::string Purple;
	static const std::string Gold;
	static const std::string LightGray;
	static const std::string Gray;
	static const std::string DarkPurple;
	static const std::string LightGreen;
	static const std::string LightBlue;
	static const std::string Rose;
	static const std::string LightPurple;
	static const std::string Yellow;
 	static const std::string White;

	static const std::string MakeColor( char a_Color );
};

// tolua_end