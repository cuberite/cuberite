//
// Created by MightyFilipns on 01 / 04 / 25.
//
#pragma once
#include "JsonUtils.h"
#include "json/value.h"

namespace DataComponents
{
	class cDataComponents
	{
	public:
		static void LoadFromJsonInternal(const Json::Value & a_FileRoot);
		static void LoadFromJson(const AString & a_FileName);
	};
}
