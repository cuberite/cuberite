#pragma once
#include <optional>
#include "../Defines.h"
#include "CommandException.h"
#include "CommandExecutionContext.h"


class cCommandArgument
{
public:
	virtual ~cCommandArgument() = default;
	cCommandArgument() = default;
	virtual eCommandParserType GetParserType() const { return eCommandParserType::None; }
	virtual void WriteProperties(cPacketizer & a_Packet) const { return; }
	virtual void Parse(BasicStringReader & a_ToParse, cCommandExecutionContext & ctx, const AString & a_Name) { return; }
};





class cCommandFloatArgument final : public cCommandArgument
{
public:
	cCommandFloatArgument(float a_Min, float a_Max) : m_Min(a_Min), m_Max(a_Max) {}
	cCommandFloatArgument() = default;

	void WriteProperties(cPacketizer & a_Packet) const override
	{
		const Byte flags = static_cast<Byte>(m_Min.has_value()) | static_cast<Byte>(m_Max.has_value() << 1);
		a_Packet.WriteBEInt8(static_cast<Int8>(flags));
		if (m_Min.has_value())
		{
			a_Packet.WriteBEFloat(m_Min.value());
		}
		if (m_Max.has_value())
		{
			a_Packet.WriteBEFloat(m_Max.value());
		}
	}
	void Parse(BasicStringReader & a_ToParse, cCommandExecutionContext & ctx, const AString & a_Name) override
	{
		float value = 0;
		const auto str = a_ToParse.ReadStringUntilWhiteSpace();
		try
		{
			value = std::stof(str);
		}
		catch (std::invalid_argument & ex)
		{
			throw cCommandParseException("Failed to parse " + str + " as float. Error: "+ ex.what());
		}
		catch (std::out_of_range & ex)
		{
			throw cCommandParseException("Failed to parse " + str + " as float because its too large/small. Error: "+ ex.what());
		}
		if (m_Max.has_value() && value > m_Max.value())
		{
			throw cCommandParseException(str + "is too large. Max value is " + std::to_string(m_Max.value()));
		}
		if (m_Min.has_value() && value < m_Min.value())
		{
			throw cCommandParseException(str + "is too small. Max value is " + std::to_string(m_Min.value()));
		}
		ctx.AddValue(a_Name, value);
	}
	virtual eCommandParserType GetParserType() const override { return eCommandParserType::Float; }
	static float GetFloatFromCtx(cCommandExecutionContext a_Ctx, const AString & a_Name)
	{
		return std::any_cast<float>(a_Ctx.GetValue(a_Name));
	}

private:
	std::optional<float> m_Min;
	std::optional<float> m_Max;
};





class cCommandTimeArgument final : public cCommandArgument
{
public:
	cCommandTimeArgument(int a_Min) : m_Min(a_Min){}
	cCommandTimeArgument() = default;

	void WriteProperties(cPacketizer & a_Packet) const override
	{
		a_Packet.WriteBEInt32(m_Min.value_or(0));
	}
	void Parse(BasicStringReader & a_ToParse, cCommandExecutionContext & ctx, const AString & a_Name) override
	{
		float value = 0;
		auto str = a_ToParse.ReadStringUntilWhiteSpace();
		float multiplier = 1;
		try
		{
			const char unit = str.at(str.size() - 1);
			if (unit == 'd')
			{
				multiplier = 24000;
				str = str.substr(0, str.size() - 1);
			}
			if (unit == 's')
			{
				multiplier = 20;
				str = str.substr(0, str.size() - 1);
			}
			if (unit == 't')
			{
				str = str.substr(0, str.size() - 1);
			}
			value = std::stof(str);
		}
		catch (std::invalid_argument& ex)
		{
			throw cCommandParseException("Failed to parse " + str + " as float. Error: " + ex.what());
		}
		catch (std::out_of_range& ex)
		{
			throw cCommandParseException("Failed to parse " + str + " as float because its too large/small. Error: " + ex.what());
		}
		value *= multiplier;
		if (m_Min.has_value() && value < m_Min.value())
		{
			throw cCommandParseException(str + " is too small. Min value is " + std::to_string(m_Min.value()));
		}
		ctx.AddValue(a_Name, static_cast<int>(value));
	}
	virtual eCommandParserType GetParserType() const override { return eCommandParserType::Time; }
	static int GetTimeTicksFromCtx(cCommandExecutionContext a_Ctx, const AString & a_Name)
	{
		return std::any_cast<int>(a_Ctx.GetValue(a_Name));
	}

private:
	std::optional<int> m_Min;
};





class cCommandGameModeArgument final : public cCommandArgument
{
public:
	cCommandGameModeArgument() = default;

	void Parse(BasicStringReader & a_ToParse, cCommandExecutionContext & ctx, const AString & a_Name) override
	{
		eGameMode value = eGameMode_NotSet;
		auto str = a_ToParse.ReadStringUntilWhiteSpace();
		if (str == "survival")
		{
			value = eGameMode_Survival;
		}
		else if (str == "creative")
		{
			value = eGameMode_Creative;
		}
		else if (str == "spectator")
		{
			value = eGameMode_Spectator;
		}
		else if (str == "adventure")
		{
			value = eGameMode_Adventure;
		}
		else
		{
			throw cCommandParseException("Failed to parse " + str + " as Gamemode. Expected survival, creative, spectator or adventure");
		}
		ctx.AddValue(a_Name, value);
	}
	virtual eCommandParserType GetParserType() const override { return eCommandParserType::Gamemode; }
	static eGameMode GetGameModeFromCtx(cCommandExecutionContext a_Ctx, const AString & a_Name)
	{
		return std::any_cast<eGameMode>(a_Ctx.GetValue(a_Name));
	}
};
