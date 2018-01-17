
#pragma once

#undef  ntohll
#define ntohll(x) (((static_cast<UInt64>(ntohl(static_cast<UInt32>(x)))) << 32) + ntohl(x >> 32))





// Changes endianness
inline UInt64 HostToNetwork8(const void * a_Value)
{
	UInt64 buf;
	memcpy( &buf, a_Value, sizeof( buf));
	buf = (( ( static_cast<UInt64>(htonl(static_cast<UInt32>(buf)))) << 32) + htonl(buf >> 32));
	return buf;
}





inline UInt32 HostToNetwork4(const void * a_Value)
{
	UInt32 buf;
	memcpy( &buf, a_Value, sizeof( buf));
	buf = ntohl( buf);
	return buf;
}





inline double NetworkToHostDouble8(const void * a_Value)
{
	UInt64 buf = 0;
	memcpy(&buf, a_Value, 8);
	buf = ntohll(buf);
	double x;
	memcpy(&x, &buf, sizeof(double));
	return x;
}





inline Int64 NetworkToHostLong8(const void * a_Value)
{
	UInt64 buf;
	memcpy(&buf, a_Value, 8);
	buf = ntohll(buf);
	return *reinterpret_cast<Int64 *>(&buf);
}





inline UInt64 NetworkToHostULong8(const void * a_Value)
{
	UInt64 buf;
	memcpy(&buf, a_Value, 8);
	buf = ntohll(buf);
	return buf;
}





inline float NetworkToHostFloat4(const void * a_Value)
{
	UInt32 buf;
	float x;
	memcpy(&buf, a_Value, 4);
	buf = ntohl(buf);
	memcpy(&x, &buf, sizeof(float));
	return x;
}




