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

}
