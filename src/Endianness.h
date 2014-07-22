
#pragma once

#define ntohll(x) ((((UInt64)ntohl((u_long)x)) << 32) + ntohl(x >> 32))





// Changes endianness
inline UInt64 HostToNetwork8(const void * a_Value)
{
	unsigned long long __HostToNetwork8;
	memcpy( &__HostToNetwork8, a_Value, sizeof( __HostToNetwork8));
	__HostToNetwork8 = (( ( (unsigned long long)htonl((u_long)__HostToNetwork8)) << 32) + htonl(__HostToNetwork8 >> 32));
	return __HostToNetwork8;
}





inline UInt32 HostToNetwork4(const void* a_Value)
{
	unsigned int __HostToNetwork4;
	memcpy( &__HostToNetwork4, a_Value, sizeof( __HostToNetwork4));
	__HostToNetwork4 = ntohl( __HostToNetwork4);
	return __HostToNetwork4;
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





inline float NetworkToHostFloat4(const void * a_Value)
{
	UInt32 buf;
	float x;
	memcpy(&buf, a_Value, 4);
	buf = ntohl(buf);
	memcpy(&x, &buf, sizeof(float));
	return x;
}




