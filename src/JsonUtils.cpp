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
	std::unique_ptr<Json::StreamWriter> Writer(Builder.newStreamWriter());

	std::stringstream Stream;
	Writer->write(a_Root, &Stream);
	return Stream.str();
}





AString WriteStyledString(const Json::Value & a_Root)
{
	Json::StreamWriterBuilder Builder;
	std::unique_ptr<Json::StreamWriter> Writer(Builder.newStreamWriter());

	std::stringstream Stream;
	Writer->write(a_Root, &Stream);
	return Stream.str();
}





bool ParseString(const AString & a_JsonStr, Json::Value & a_Root, AString * a_ErrorMsg)
{
	Json::CharReaderBuilder Builder;
	std::unique_ptr<Json::CharReader> Reader(Builder.newCharReader());

	const char * Doc = a_JsonStr.data();
	return Reader->parse(Doc, Doc + a_JsonStr.size(), &a_Root, a_ErrorMsg);
}

}  // namespace JsonUtils
