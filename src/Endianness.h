
#pragma once

#undef  ntohll
#define ntohll(x) ((((uint64_t)ntohl((u_long)x)) << 32) + ntohl(x >> 32))





// Changes endianness
inline uint64_t HostToNetwork8(const void * a_Value)
{
	unsigned long long __HostToNetwork8;
	memcpy( &__HostToNetwork8, a_Value, sizeof( __HostToNetwork8));
	__HostToNetwork8 = (( ( (unsigned long long)htonl((u_long)__HostToNetwork8)) << 32) + htonl(__HostToNetwork8 >> 32));
	return __HostToNetwork8;
}





inline uint32_t HostToNetwork4(const void* a_Value)
{
	unsigned int __HostToNetwork4;
	memcpy( &__HostToNetwork4, a_Value, sizeof( __HostToNetwork4));
	__HostToNetwork4 = ntohl( __HostToNetwork4);
	return __HostToNetwork4;
}





inline double NetworkToHostDouble8(const void * a_Value)
{
	uint64_t buf = 0;
	memcpy(&buf, a_Value, 8);
	buf = ntohll(buf);
	double x;
	memcpy(&x, &buf, sizeof(double));
	return x;
}





inline int64_t NetworkToHostLong8(const void * a_Value)
{
	uint64_t buf;
	memcpy(&buf, a_Value, 8);
	buf = ntohll(buf);
	return *reinterpret_cast<int64_t *>(&buf);
}





inline float NetworkToHostFloat4(const void * a_Value)
{
	uint32_t buf;
	float x;
	memcpy(&buf, a_Value, 4);
	buf = ntohl(buf);
	memcpy(&x, &buf, sizeof(float));
	return x;
}




