
// Color.h

// Declares a class to handle item color related code





#pragma once

// tolua_begin
class cColor
{
public:

	enum eColorLimits : unsigned int
	{
		COLOR_MIN = 0,
		COLOR_MAX = 255,
		COLOR_LIMIT = 256,
		COLOR_NONE = 0xFFFFFFFF,
	};
	cColor() { m_Color = static_cast<unsigned int>(COLOR_NONE);}
	cColor(unsigned char a_Red, unsigned char a_Green, unsigned char a_Blue) { SetColor(a_Red, a_Green, a_Blue); }

	/** Returns whether the color is a valid color */
	bool IsValid() const { return m_Color != COLOR_NONE; }


	/** Changes the color */
	void SetColor(unsigned char a_Red, unsigned char a_Green, unsigned char a_Blue);

	/** Alters the red value of the color */
	void SetRed(unsigned char a_Red);

	/** Alters the green value of the color */
	void SetGreen(unsigned char a_Green);

	/** Alters the blue value of the color */
	void SetBlue(unsigned char a_Blue);

	/** Returns the red value of the color */
	unsigned char GetRed() const;

	/** Returns the green value of the color */
	unsigned char GetGreen() const;

	/** Returns the blue value of the color */
	unsigned char GetBlue() const;

	/** Resets the color */
	void Clear() { m_Color = static_cast<unsigned int>(COLOR_NONE); }
	// tolua_end

	unsigned int m_Color;

};  // tolua_export
