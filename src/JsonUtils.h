#pragma once

// fwd:
namespace Json
{
class Value;
}

namespace JsonUtils
{

AString WriteFastString(const Json::Value & a_Root);
AString WriteStyledString(const Json::Value & a_Root);

bool ParseString(const AString & a_JsonStr, Json::Value & a_Root, AString * a_ErrorMsg = nullptr);

/** Creates a Json string representing an object with the specified single value. */
extern AString SerializeSingleValueJsonObject(const AString & a_Key, const AString & a_Value);

}  // namespace JsonUtils
