/* 
   UrlHelper.h

   Copyright (C) 2002-2004 Ren� Nyffenegger

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

   Ren� Nyffenegger rene.nyffenegger@adp-gmbh.ch
*/

/*
	Note on point 2:
		THIS IS NOT THE ORIGINAL SOURCE1!!1!!!~!!~`1ONE!!`1
*/
#ifndef __URL_HELPER_H__
#define __URL_HELPER_H__

#include <string>
#include <map>

void SplitUrl             (std::string const& url, std::string& protocol, std::string& server, std::string& path);
bool RemoveProtocolFromUrl(std::string const& url, std::string& protocol, std::string& rest);

void SplitGetReq          (std::string et_req,     std::string& path,     std::map<std::string, std::string>& params);

#endif
