/**
 * Copyright 2014 MCServer Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#pragma once

class cEnchantments;
class cFastNBTWriter;
class cParsedNBT;

namespace EnchantmentSerializer
{

	/// Writes the enchantments into the specified NBT writer; begins with the LIST tag of the specified name ("ench" or "StoredEnchantments")
	void WriteToNBTCompound(cEnchantments const& a_Enchantments, cFastNBTWriter & a_Writer, const AString & a_ListTagName);
	
	/// Reads the enchantments from the specified NBT list tag (ench or StoredEnchantments)
	void ParseFromNBT(cEnchantments& a_Enchantments, const cParsedNBT & a_NBT, int a_EnchListTagIdx);
	
};
