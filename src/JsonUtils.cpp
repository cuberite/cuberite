#include "Globals.h"

#include "JsonUtils.h"
#include "json/json.h"

#include <sstream>


namespace JsonUtils
{

AString WriteFastString(const Json::Value & a_Root)
{
	Json::StreamWriterBuilder Builder;
	Builder["commentStyle"] = "None";
	Builder["indentation"] = "";
	return Json::writeString(Builder, a_Root);
}





AString WriteStyledString(const Json::Value & a_Root)
{
	Json::StreamWriterBuilder Builder;
	return Json::writeString(Builder, a_Root);
}





bool ParseString(const AString & a_JsonStr, Json::Value & a_Root, AString * a_ErrorMsg)
{
	Json::CharReaderBuilder Builder;
	std::unique_ptr<Json::CharReader> Reader(Builder.newCharReader());

	const char * Doc = a_JsonStr.data();
	return Reader->parse(Doc, Doc + a_JsonStr.size(), &a_Root, a_ErrorMsg);
}

}  // namespace JsonUtils
