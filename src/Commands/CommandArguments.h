#pragma once
#include <optional>
#include "CommandException.h"
#include "CommandExecutionContext.h"


class cCommandArgument
{
	public:

	enum class eParserType : std::int8_t
	{
		None = -1,
		Bool = 0,
		Float,
		Double,
		Integer,
		Long,
		String,
		Entity,
		GameProfile,
		BlockPos,
		ColumnPos,
		Vector3,
		Vector2,
		BlockState,
		BlockPredicate,
		ItemStack,
		ItemPredicate,
		Color,
		Component,
		Style,
		Message,
		Nbt,
		NbtTag,
		NbtPath,
		Objective,
		ObjectiveCriteria,
		Operation,
		Particle,
		Angle,
		Rotation,
		ScoreboardSlot,
		ScoreHolder,
		Swizzle,
		Team,
		ItemSlot,
		ItemSlots,
		ResourceLocation,
		Function,
		EntityAnchor,
		IntRange,
		FloatRange,
		Dimension,
		GameMode,
		Time,
		ResourceOrTag,
		ResourceOrTagKey,
		Resource,
		ResourceKey,
		TemplateMirror,
		TemplateRotation,
		HeightMap,
		UUID
		// TODO: actually implement these parsers
	};
  public:
	cCommandArgument() = default;
	virtual void WriteParserID(cPacketizer& a_Packet) const { return; }
	virtual void WriteProperties(cPacketizer& a_Packet) const { return; }
	virtual void Parse(BasicStringReader& a_ToParse, cCommandExecutionContext& ctx, const AString& a_Name) { return; }
};
class cCommandFloatArgument final : public cCommandArgument
{

  public:
	cCommandFloatArgument(float a_Min, float a_Max) : m_Min(a_Min), m_Max(a_Max){}
	cCommandFloatArgument() = default;

	void WriteProperties(cPacketizer& a_Packet) const override
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
	void Parse(BasicStringReader& a_ToParse, cCommandExecutionContext& ctx, const AString& a_Name) override
	{
		float value = 0;
		const auto str = a_ToParse.ReadStringUntilWhiteSpace();
		try
		{
			 value = std::stof(str);
		}
		catch (std::invalid_argument& ex)
		{
			throw cCommandParseException("Failed to parse " + str + " as float. Error: "+ ex.what());
		}
		catch (std::out_of_range& ex)
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
	virtual void WriteParserID(cPacketizer& a_Packet) const override
	{
		a_Packet.WriteVarInt32(static_cast<UInt32>(eParserType::Float));
	}
	static float GetFloatFromCtx(cCommandExecutionContext a_Ctx, const AString& a_Name)
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

	void WriteProperties(cPacketizer& a_Packet) const override
	{
		a_Packet.WriteBEInt32(m_Min.value_or(0));
	}
	void Parse(BasicStringReader& a_ToParse, cCommandExecutionContext& ctx, const AString& a_Name) override
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
				str = str.substr(0,str.size() - 1);
			}
			if (unit == 's')
			{
				multiplier = 20;
				str = str.substr(0,str.size() - 1);
			}
			if (unit == 't')
			{
				str = str.substr(0,str.size() - 1);
			}
			 value = std::stof(str);
		}
		catch (std::invalid_argument& ex)
		{
			throw cCommandParseException("Failed to parse " + str + " as float. Error: "+ ex.what());
		}
		catch (std::out_of_range& ex)
		{
			throw cCommandParseException("Failed to parse " + str + " as float because its too large/small. Error: "+ ex.what());
		}
		value *= multiplier;
		if (m_Min.has_value() && value < m_Min.value())
		{
			throw cCommandParseException(str + " is too small. Min value is " + std::to_string(m_Min.value()));
		}
		ctx.AddValue(a_Name, static_cast<int>(value));
	}
	virtual void WriteParserID(cPacketizer& a_Packet) const override
	{
		a_Packet.WriteVarInt32(static_cast<UInt32>(eParserType::Time));
	}
	static int GetTimeTicksFromCtx(cCommandExecutionContext a_Ctx, const AString& a_Name)
	{
		return std::any_cast<int>(a_Ctx.GetValue(a_Name));
	}

private:
	std::optional<int> m_Min;
};
