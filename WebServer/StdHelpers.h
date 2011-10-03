/* 
   stdHelpers.h

   Copyright (C) 2002-2005 René Nyffenegger

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

#ifndef STDHELPERS_H__
#define STDHELPERS_H__

#include <string>
#include <sstream>

std::string ReplaceInStr(const std::string& in, const std::string& search_for, const std::string& replace_with);

void ToUpper(std::string& s);
void ToLower(std::string& s);

template <class T>
T To(std::string const& s) {
  T ret;

  std::stringstream stream;
  stream << s;
  stream >> ret;

  return ret;
}

template<class T>
std::string StringFrom(T const& t) {
  std::string ret;

  std::stringstream stream;
  stream << t;
  stream >> ret;

  return ret;
}

#endif