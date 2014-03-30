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





class cLog
{	// tolua_export
private:
	FILE * m_File;
	static cLog * s_Log;

public:
	cLog(const AString & a_FileName);
	~cLog();
	void Log(const char * a_Format, va_list argList) FORMATSTRING(2, 0);
	void Log(const char * a_Format, ...) FORMATSTRING(2, 3);
	// tolua_begin
	void SimpleLog(const char * a_String);
	void OpenLog(const char * a_FileName);
	void CloseLog();
	void ClearLog();
	static cLog* GetInstance();
};
// tolua_end




