
#pragma once

#include "json/json.h"

class cItemGrid;


namespace BlockEntityTagSerializer
{
	/** Reads the block entity data into json object value **/
	void ParseFromJson(const Json::Value & a_Value, cItemGrid & a_Grid);

	/** Writes the block entity data into json object value **/
	void WriteToJson(Json::Value & a_Value, const cItem & a_Item, const cItemGrid & a_Grid);
};




