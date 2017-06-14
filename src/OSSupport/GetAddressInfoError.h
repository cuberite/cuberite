#pragma once



/** Returns the readable form of a getaddressinfo type error code */
inline AString ErrorString(int a_ErrorCode)
{
	// Note gai_strerror is not threadsafe on windows
	#ifdef _WIN32
		AString ErrorStr(GAI_STRERROR_BUFFER_SIZE, '\0');

		int MsgLen = FormatMessageA(
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS |
			FORMAT_MESSAGE_MAX_WIDTH_MASK,
			nullptr,
			a_ErrorCode,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			&ErrorStr[0],
			GAI_STRERROR_BUFFER_SIZE,
			nullptr
		);

		ErrorStr.resize(MsgLen);
		return ErrorStr;
	#else
		return gai_strerror(a_ErrorCode);
	#endif
}

