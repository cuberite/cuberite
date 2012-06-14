/*
   stdHelpers.cpp

   Copyright (C) 2002-2004 René Nyffenegger

   This source code is provided 'as-is', without any express or implied
   warranty. In no event will the author be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this source code must not be misrepresented; you must not
      claim that you wrote the original source code. If you use this source code
      in a product, an acknowledgment in the product documentation would be
      appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
      misrepresented as being the original source code.

   3. This notice may not be removed or altered from any source distribution.

   René Nyffenegger rene.nyffenegger@adp-gmbh.ch
*/

/*
	Note on point 2:
		THIS IS NOT THE ORIGINAL SOURCE1!!1!!!~!!~`1ONE!!`1
*/

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "StdHelpers.h"
#include <cctype>

std::string ReplaceInStr(const std::string& in, const std::string& search_for, const std::string& replace_with) {
  std::string ret = in;

  std::string::size_type pos = ret.find(search_for);

  while (pos != std::string::npos) {
    ret = ret.replace(pos, search_for.size(), replace_with);
    pos = pos - search_for.size() + replace_with.size() + 1;
    pos = ret.find(search_for, pos);
  }

  return ret;
}

// std:toupper and std::tolower are overloaded. Well...
// http://gcc.gnu.org/ml/libstdc++/2002-11/msg00180.html
char toLower_ (char c) { return std::tolower(c); }
char toUpper_ (char c) { return std::toupper(c); }

void ToUpper(std::string& s) {
  std::transform(s.begin(), s.end(), s.begin(),toUpper_);
}

void ToLower(std::string& s) {
  std::transform(s.begin(), s.end(), s.begin(),toLower_);
}
